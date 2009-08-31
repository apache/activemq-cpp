/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "InactivityMonitor.h"

#include "ReadChecker.h"
#include "WriteChecker.h"

#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/KeepAliveInfo.h>
#include <decaf/lang/Math.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::transport;
using namespace activemq::transport::inactivity;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace transport{
namespace inactivity{

    class AsyncException : decaf::lang::Runnable {
    private:

        InactivityMonitor* parent;

    public:

        AsyncException( InactivityMonitor* parent ) : parent( parent ) {
        }

        virtual void run() {
            IOException ex (
                __FILE__, __LINE__,
                ( std::string( "Channel was inactive for too long: " ) + parent->next->getRemoteAddress() ).c_str() );
            parent->onException( ex );
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace transport{
namespace inactivity{

    class AsyncWriteKeepAlive : decaf::lang::Runnable {
    private:

        InactivityMonitor* parent;

    public:

        AsyncWriteKeepAlive( InactivityMonitor* parent ) : parent( parent ) {
        }

        virtual void run() {
            if( parent->monitorStarted.get() ) {
                try {
                    Pointer<KeepAliveInfo> info( new KeepAliveInfo() );
                    info->setResponseRequired( parent->keepAliveResponseRequired );
                    parent->oneway( info );
                } catch( IOException e ) {
                    parent->onException( e );
                }
            }
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::InactivityMonitor( const Pointer<Transport>& next, const Pointer<WireFormat>& wireFormat )
:   TransportFilter( next ),
    wireFormat( wireFormat ),
    monitorStarted( false ),
    commandSent( false ),
    commandReceived( true),
    failed( false ),
    inRead( false ),
    inWrite( false ),
    readCheckTime( 0 ),
    writeCheckTime( 0 ),
    initialDelayTime( 0 ),
    keepAliveResponseRequired( false ) {

}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::~InactivityMonitor() {
    try{
        this->stopMonitorThreads();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::close() throw( decaf::io::IOException ) {
    try{
        stopMonitorThreads();
        TransportFilter::close();
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::onException( const decaf::lang::Exception& ex ) {

    if( failed.compareAndSet( false, true ) ) {
        stopMonitorThreads();
        TransportFilter::onException( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::onCommand( const Pointer<Command>& command ) {

    TransportFilter::onCommand( command );

    commandReceived.set( true );
    inRead.set( true );

    try {

        if( command->isWireFormatInfo() ) {
            synchronized( &monitor ) {

                remoteWireFormatInfo = command.dynamicCast<WireFormatInfo>();
                try {
                    startMonitorThreads();
                } catch( IOException& e ) {
                    onException( e );
                }
            }
        }

        TransportFilter::onCommand( command );

        inRead.set( false );

    } catch( Exception& ex ) {
        inRead.set( false );
        ex.setMark( __FILE__, __LINE__ );
        throw ex;
    }
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::oneway( const Pointer<Command>& command )
    throw( decaf::io::IOException, decaf::lang::exceptions::UnsupportedOperationException ) {

    try{
        // Disable inactivity monitoring while processing a command.  Synchronize this
        // method - its not synchronized
        // further down the transport stack and gets called by more
        // than one thread  by this class
        synchronized( &inWriteMutes ) {
            this->inWrite.set( true );
            try {

                if( failed.get() ) {
                    throw IOException(
                        __FILE__, __LINE__,
                        ( std::string( "Channel was inactive for too long: " ) + next->getRemoteAddress() ).c_str() );
                }

                if( command->isWireFormatInfo() ) {
                    synchronized( &monitor ) {
                        localWireFormatInfo = command.dynamicCast<WireFormatInfo>();
                        startMonitorThreads();
                    }
                }

                this->next->oneway( command );

                this->commandSent.set( true );
                this->inWrite.set( false );

            } catch( Exception& ex ) {
                this->commandSent.set( true );
                this->inWrite.set( false );

                ex.setMark( __FILE__, __LINE__ );
                throw ex;
            }
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
bool InactivityMonitor::allowReadCheck( long long elapsed ) {
    return elapsed > (readCheckTime * 9 / 10);
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::readCheck() {

    if( inRead.get() || wireFormat->inReceive() ) {
        return;
    }

    if( !commandReceived.get() ) {
//        ASYNC_TASKS.execute( new Runnable() {
//            public void run() {
//                onException(new InactivityIOException("Channel was inactive for too long: "+next.getRemoteAddress()));
//            };
//
//        });
    }

    commandReceived.set(false);
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::writeCheck() {

    if( inWrite.get() ) {
        return;
    }

    if (!commandSent.get()) {

//        ASYNC_TASKS.execute( new Runnable() {
//            public void run() {
//                if (monitorStarted.get()) {
//                    try {
//
//                        KeepAliveInfo info = new KeepAliveInfo();
//                        info.setResponseRequired(keepAliveResponseRequired);
//                        oneway(info);
//                    } catch (IOException e) {
//                        onException(e);
//                    }
//                }
//            };
//        });
    }

    commandSent.set(false);
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::startMonitorThreads() {

    synchronized( &monitor ) {

        if( monitorStarted.get() ) {
            return;
        }
        if( localWireFormatInfo == NULL ) {
            return;
        }
        if( remoteWireFormatInfo == NULL ) {
            return;
        }

        readCheckTime = Math::min( localWireFormatInfo->getMaxInactivityDuration(),
                                   remoteWireFormatInfo->getMaxInactivityDuration() );
        initialDelayTime = Math::min( localWireFormatInfo->getMaxInactivityDurationInitalDelay(),
                                      remoteWireFormatInfo->getMaxInactivityDurationInitalDelay() );
        if( readCheckTime > 0 ) {

            monitorStarted.set( true );
            writeCheckerTask.reset( new WriteChecker( this ) );
            readCheckerTask.reset( new ReadChecker( this ) );
            writeCheckTime = readCheckTime > 3 ? readCheckTime / 3 : readCheckTime;

            this->writeCheckTimer.scheduleAtFixedRate( writeCheckerTask, initialDelayTime, writeCheckTime );
            this->readCheckTimer.scheduleAtFixedRate( readCheckerTask, initialDelayTime, readCheckTime );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::stopMonitorThreads() {

    synchronized( &monitor ) {

        if( monitorStarted.compareAndSet( true, false ) ) {

            readCheckerTask->cancel();
            writeCheckerTask->cancel();

            readCheckTimer.purge();
            readCheckTimer.cancel();
            writeCheckTimer.purge();
            writeCheckTimer.cancel();
        }
    }
}
