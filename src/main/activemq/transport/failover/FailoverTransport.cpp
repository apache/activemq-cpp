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

#include "FailoverTransport.h"

#include <activemq/commands/ConnectionControl.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/transport/TransportRegistry.h>
#include <decaf/util/Random.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
FailoverTransport::FailoverTransport( const Pointer<wireformat::WireFormat>& wireFormat ) {

    this->wireFormat = wireFormat;
    this->timeout = -1;
    this->initialReconnectDelay = 10;
    this->maxReconnectDelay = 1000 * 30;
    this->backOffMultiplier = 2;
    this->useExponentialBackOff = true;
    this->randomize = true;
    this->initialized = false;
    this->maxReconnectAttempts = 0;
    this->connectFailures = 0;
    this->reconnectDelay = this->initialReconnectDelay;
    this->backup = false;
    this->backupPoolSize = 1;
    this->trackMessages = false;
    this->maxCacheSize = 128 * 1024;

    this->started = false;
    this->closed = false;
    this->connected = false;

    this->stateTracker.setTrackTransactions( true );
    this->myTransportListener.reset( new FailoverTransportListener( this ) );
    this->reconnectTask.reset( new ReconnectTask( this ) );
    this->taskRunner.reset( new TaskRunner( reconnectTask.get() ) );
}

////////////////////////////////////////////////////////////////////////////////
FailoverTransport::~FailoverTransport() {
    try{
        close();
    }
    AMQ_CATCH_NOTHROW( Exception )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isShutdownCommand( const Pointer<Command>& command ) const {

    if( command != NULL ) {

        if( command->isShutdownInfo() ) {
            return true;
        }

        try{
            Pointer<RemoveInfo> remove =
                command.dynamicCast<RemoveInfo, Pointer<RemoveInfo>::CounterType >();

            return true;
        } AMQ_CATCHALL_NOTHROW()
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::add( const std::string& uri ) {

    try {
        URI newUri( uri );
        if( !uris.contains( newUri ) ) {
            uris.add( newUri );
        }

        reconnect();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::addURI( const List<URI>& uris ) {

    synchronized( &this->uris ) {
        std::auto_ptr< Iterator<URI> > iter( uris.iterator() );

        while( iter->hasNext() ) {
            this->uris.add( iter->next() );
        }
    }

    reconnect();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::removeURI( const List<URI>& uris ) {

    synchronized( &this->uris ) {
        std::auto_ptr< Iterator<URI> > iter( uris.iterator() );

        while( iter->hasNext() ) {
            this->uris.remove( iter->next() );
        }
    }

    reconnect();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect( const decaf::net::URI& uri )
    throw( decaf::io::IOException ) {

    try {

        if( !uris.contains( uri ) ) {
            uris.add( uri );
        }

        reconnect();
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
StlList<URI> FailoverTransport::getConnectList() const {

    StlList<URI> result( uris );
    bool removed = false;

    if( failedConnectTransportURI != NULL ) {
        removed = result.remove( *failedConnectTransportURI );
    }

    if( randomize ) {
        // Randomly, reorder the list by random swapping
        Random rand;
        rand.setSeed( decaf::lang::System::currentTimeMillis() );

        for( std::size_t i = 0; i < result.size(); i++ ) {
            int p = rand.nextInt( result.size() );
            URI temp = result.get( p );
            result.set( p, result.get( i ) );
            result.set( i, temp );
        }
    }

    if( removed ) {
        result.add( *failedConnectTransportURI );
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setTransportListener( TransportListener* listener ) {
    synchronized( &listenerMutex ) {
        this->transportListener = listener;
        listenerMutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string FailoverTransport::getRemoteAddress() const {
    if( connectedTransport != NULL ) {
        return connectedTransport->getRemoteAddress();
    }
    return "";
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::oneway( const Pointer<Command>& command )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    Pointer<Exception> error;

    try {

        synchronized( &reconnectMutex ) {

            if( isShutdownCommand( command ) && connectedTransport == NULL ) {

                if( command->isShutdownInfo() ) {
                    // Skipping send of ShutdownInfo command when not connected.
                    return;
                }

                if( command->isRemoveInfo() ) {
                    // Simulate response to RemoveInfo command
                    Pointer<Response> response( new Response() );
                    response->setCorrelationId( command->getCommandId() );
                    myTransportListener->onCommand( response );
                    return;
                }
            }

            // Keep trying until the message is sent.
            for( int i = 0; !closed; i++ ) {
                try {

                    // Wait for transport to be connected.
                    Pointer<Transport> transport = connectedTransport;
                    long long start = System::currentTimeMillis();
                    bool timedout = false;

                    while( transport == NULL && !closed && connectionFailure == NULL ) {
                        long long end = System::currentTimeMillis();
                        if( timeout > 0 && ( end - start > timeout ) ) {
                            timedout = true;
                            break;
                        }

                        reconnectMutex.wait( 100 );
                        transport = connectedTransport;
                    }

                    if( transport == NULL ) {
                        // Previous loop may have exited due to us being disposed.
                        if( closed ) {
                            error.reset( new IOException(
                                __FILE__, __LINE__, "Transport disposed.") );
                        } else if( connectionFailure != NULL ) {
                            error = connectionFailure;
                        } else if( timedout == true ) {
                            error.reset( new IOException(
                                __FILE__, __LINE__,
                                "Failover timeout of %d ms reached.", timedout ) );
                        } else {
                            error.reset( new IOException(
                                __FILE__, __LINE__, "Unexpected failure.") );
                        }

                        break;
                    }

                    // If it was a request and it was not being tracked by
                    // the state tracker,
                    // then hold it in the requestMap so that we can replay
                    // it later.
                    Pointer<Tracked> tracked = stateTracker.track( command );
                    synchronized( &requestMap ) {
                        if( tracked != NULL && tracked->isWaitingForResponse() ) {
                            requestMap.put( command->getCommandId(), tracked );
                        } else if( tracked == NULL && command->isResponseRequired() ) {
                            requestMap.put( command->getCommandId(), command );
                        }
                    }

                    // Send the message.
                    try {
                        transport->oneway( command );
                        stateTracker.trackBack( command );
                    } catch( IOException& e ) {

                        // If the command was not tracked.. we will retry in
                        // this method
                        if( tracked == NULL ) {

                            // since we will retry in this method.. take it out of the
                            // request map so that it is not sent 2 times on recovery
                            if( command->isResponseRequired() ) {
                                requestMap.remove( command->getCommandId() );
                            }

                            // Rethrow the exception so it will handled by
                            // the outer catch
                            throw e;
                        }
                    }

                    return;
                } catch( IOException& e ) {
                    handleTransportFailure( e );
                }
            }
        }
    }
    AMQ_CATCH_NOTHROW( Exception )
    AMQ_CATCHALL_NOTHROW()

    if( !closed ) {
        if( error != NULL ) {
            throw IOException( *error );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FailoverTransport::request( const Pointer<Command>& command AMQCPP_UNUSED )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FailoverTransport::request( const Pointer<Command>& command AMQCPP_UNUSED,
                                                unsigned int timeout AMQCPP_UNUSED )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::start() throw( cms::CMSException ) {

    try{

        synchronized( &reconnectMutex ) {

            if( this->started ) {
                return;
            }

            started = true;

            stateTracker.setMaxCacheSize( this->getMaxCacheSize() );
            stateTracker.setTrackMessages( this->isTrackMessages() );

            if( connectedTransport != NULL ) {
                stateTracker.restore( connectedTransport );
            } else {
                reconnect();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::close() throw( cms::CMSException ) {

    Pointer<Transport> transportToStop;

    synchronized( &reconnectMutex ) {
        if (!started) {
            return;
        }

        started = false;
        closed = true;
        connected = false;

        std::auto_ptr< Iterator< Pointer<BackupTransport> > > iter( backups.iterator() );
        while( iter->hasNext() ) {
            iter->next()->setClosed( true );
        }

        backups.clear();

        if( connectedTransport != NULL ) {
            transportToStop = connectedTransport;
            connectedTransport.reset( NULL );
        }

        reconnectMutex.notifyAll();
    }

    synchronized( &sleepMutex ) {
        sleepMutex.notifyAll();
    }

    taskRunner->shutdown( 500 );

    if( transportToStop != NULL ) {
        transportToStop->close();
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect() {

    synchronized( &reconnectMutex  ) {
        if( started ) {
            taskRunner->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::restoreTransport( const Pointer<Transport>& transport )
    throw( IOException ) {

    try{

        transport->start();

        //send information to the broker - informing it we are an ft client
        Pointer<ConnectionControl> cc( new ConnectionControl() );
        cc->setFaultTolerant( true );
        transport->oneway( cc );

        stateTracker.restore( transport );
        std::vector< Pointer<Command> > commands;
        synchronized( &requestMap ) {
            commands = requestMap.values();
        }

        std::vector< Pointer<Command> >::const_iterator iter = commands.begin();
        for( ; iter != commands.end(); ++iter ) {
            transport->oneway( *iter );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::handleTransportFailure( const decaf::lang::Exception& error AMQCPP_UNUSED )
    throw( decaf::lang::Exception ) {

    Pointer<Transport> transport;
    connectedTransport.swap( transport );

    if( transport != NULL ) {

        if( this->disposedListener != NULL ) {
            transport->setTransportListener( disposedListener.get() );
        }
        transport->close();

        synchronized( &reconnectMutex ) {
            bool reconnectOk = started;

            initialized = false;
            failedConnectTransportURI = connectedTransportURI;
            connectedTransportURI.reset( NULL );
            connected = false;
            if( reconnectOk ) {
                taskRunner->wakeup();
            }
        }

        if( transportListener != NULL ) {
            transportListener->transportInterrupted();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::doReconnect() {

    Pointer<Exception> failure;

    synchronized( &reconnectMutex ) {

        if( closed || connectionFailure != NULL ) {
            reconnectMutex.notifyAll();
        }

        if( connectedTransport != NULL || closed || connectionFailure != NULL ) {
            return false;
        } else {
            StlList<URI> connectList = getConnectList();
            if( connectList.isEmpty() ) {
                failure.reset( new IOException(
                    __FILE__, __LINE__, "No uris available to connect to." ) );
            } else {

                if( !useExponentialBackOff ) {
                    reconnectDelay = initialReconnectDelay;
                }

                synchronized( &backupMutex ) {

                    if( backup && !backups.isEmpty() ) {

                        Pointer<BackupTransport> backup = backups.remove( 0 );
                        Pointer<Transport> transport = backup->getTransport();
                        URI uri = backup->getUri();
                        transport->setTransportListener( myTransportListener.get() );

                        try {

                            if( started ) {
                                restoreTransport( transport );
                            }

                            reconnectDelay = initialReconnectDelay;
                            failedConnectTransportURI.reset( NULL );
                            connectedTransportURI.reset( new URI( uri ) );
                            connectedTransport = transport;
                            reconnectMutex.notifyAll();
                            connectFailures = 0;

                            return false;
                        }
                        AMQ_CATCH_NOTHROW( Exception )
                        AMQ_CATCHALL_NOTHROW()
                    }
                }

                std::auto_ptr< Iterator<URI> > iter( connectList.iterator() );

                while( iter->hasNext() && connectedTransport == NULL && !closed ) {

                    URI uri = iter->next();
                    try {

                        Pointer<Transport> transport = createTransport( uri );
                        transport->setTransportListener( myTransportListener.get() );
                        transport->start();

                        if( started ) {
                            restoreTransport( transport );
                        }

                        reconnectDelay = initialReconnectDelay;
                        connectedTransportURI.reset( new URI( uri ) );
                        connectedTransport = transport;
                        reconnectMutex.notifyAll();
                        connectFailures = 0;

                        // Make sure on initial startup, that the transportListener
                        // has been initialized for this instance.
                        synchronized( &listenerMutex ) {
                            if( transportListener == NULL ) {
                                // if it isn't set after 2secs - it
                                // probably never will be
                                listenerMutex.wait( 2000 );
                            }
                        }

                        if( transportListener != NULL ) {
                            transportListener->transportResumed();
                        }

                        connected = true;
                        return false;
                    } catch( Exception& e ) {
                        failure.reset( e.clone() );
                    }
                }
            }
        }

        if( maxReconnectAttempts > 0 && ++connectFailures >= maxReconnectAttempts ) {
            connectionFailure = failure;

            // Make sure on initial startup, that the transportListener has been initialized
            // for this instance.
            synchronized( &listenerMutex ) {
                if( transportListener == NULL ) {
                    listenerMutex.wait( 2000 );
                }
            }

            if( transportListener != NULL ) {

                Pointer<IOException> ioException;
                try{
                    ioException = connectionFailure.dynamicCast<
                        IOException, Pointer<IOException>::CounterType >();
                }
                AMQ_CATCH_NOTHROW( ClassCastException )

                if( ioException != NULL ) {
                    transportListener->onException( *connectionFailure );
                } else {
                    transportListener->onException( IOException( *connectionFailure ) );
                }
            }

            reconnectMutex.notifyAll();
            return false;
        }
    }

    if( !closed ) {

        synchronized( &sleepMutex ) {
            sleepMutex.wait( reconnectDelay );
        }

        if( useExponentialBackOff ) {
            // Exponential increment of reconnect delay.
            reconnectDelay *= backOffMultiplier;
            if( reconnectDelay > maxReconnectDelay ) {
                reconnectDelay = maxReconnectDelay;
            }
        }
    }

    return !closed;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::buildBackups() {

    synchronized( &backupMutex ) {

        if( !closed && backup && (int)backups.size() < backupPoolSize ) {

            StlList<URI> connectList = getConnectList();

            //removed closed backups
            StlList< Pointer<BackupTransport> > disposedList;
            std::auto_ptr< Iterator<Pointer<BackupTransport> > > iter( backups.iterator() );
            while( iter->hasNext() ) {
                Pointer<BackupTransport> backup = iter->next();
                if( backup->isClosed() ) {
                    disposedList.add( backup );
                }
            }

            backups.removeAll( disposedList );
            disposedList.clear();

            std::auto_ptr< Iterator<URI> > uriIter( connectList.iterator() );

            while( uriIter->hasNext() && (int)backups.size() < backupPoolSize ) {
                URI uri = uriIter->next();
                if( connectedTransportURI != NULL && !connectedTransportURI->equals( uri ) ) {
                    try {
                        Pointer<BackupTransport> backup( new BackupTransport( this ) );
                        backup->setUri( uri );

                        if( !backups.contains( backup ) ) {
                            Pointer<Transport> transport = createTransport( uri );
                            transport->setTransportListener( backup.get() );
                            transport->start();
                            backup->setTransport( transport );
                            backups.add( backup );
                        }
                    }
                    AMQ_CATCH_NOTHROW( Exception )
                    AMQ_CATCHALL_NOTHROW()
                }
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> FailoverTransport::createTransport( const URI& location ) const
    throw ( decaf::io::IOException ) {

    try{

        TransportFactory* factory =
            TransportRegistry::getInstance().findFactory( location.getScheme() );

        if( factory == NULL ) {
            throw new IOException(
                __FILE__, __LINE__, "Invalid URI specified, no valid Factory Found.");
        }

        Pointer<Transport> transport( factory->createComposite( location ) );

        return transport;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
