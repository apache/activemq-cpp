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
#include <activemq/threads/DedicatedTaskRunner.h>
#include <activemq/threads/CompositeTaskRunner.h>
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
FailoverTransport::FailoverTransport() {

    this->timeout = -1;
    this->initialReconnectDelay = 10;
    this->maxReconnectDelay = 1000 * 30;
    this->backOffMultiplier = 2;
    this->useExponentialBackOff = true;
    this->initialized = false;
    this->maxReconnectAttempts = 0;
    this->connectFailures = 0;
    this->reconnectDelay = this->initialReconnectDelay;
    this->trackMessages = false;
    this->maxCacheSize = 128 * 1024;

    this->started = false;
    this->closed = false;
    this->connected = false;

    this->transportListener = NULL;
    this->uris.reset( new URIPool() );
    this->stateTracker.setTrackTransactions( true );
    this->myTransportListener.reset( new FailoverTransportListener( this ) );
    this->closeTask.reset( new CloseTransportsTask() );
    this->taskRunner.reset( new CompositeTaskRunner() );
    this->backups.reset( new BackupTransportPool( taskRunner, closeTask, uris ) );

    this->taskRunner->addTask( this );
    this->taskRunner->addTask( this->closeTask.get() );
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

        if( command->isShutdownInfo() || command->isRemoveInfo() ) {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::add( const std::string& uri ) {

    try {
        uris->addURI( URI( uri ) );

        reconnect();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::addURI( const List<URI>& uris ) {

    std::auto_ptr< Iterator<URI> > iter( uris.iterator() );

    while( iter->hasNext() ) {
        this->uris->addURI( iter->next() );
    }

    reconnect();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::removeURI( const List<URI>& uris ) {

    std::auto_ptr< Iterator<URI> > iter( uris.iterator() );

    while( iter->hasNext() ) {
        this->uris->removeURI( iter->next() );
    }

    reconnect();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect( const decaf::net::URI& uri )
    throw( decaf::io::IOException ) {

    try {

        this->uris->addURI( uri );

        reconnect();
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setTransportListener( TransportListener* listener ) {
    synchronized( &listenerMutex ) {
        this->transportListener = listener;
        listenerMutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
TransportListener* FailoverTransport::getTransportListener() const {
    synchronized( &listenerMutex ) {
        return this->transportListener;
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
std::string FailoverTransport::getRemoteAddress() const {
    synchronized( &reconnectMutex ) {
        if( connectedTransport != NULL ) {
            return connectedTransport->getRemoteAddress();
        }
    }
    return "";
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::oneway( const Pointer<Command>& command )
    throw( IOException, decaf::lang::exceptions::UnsupportedOperationException ) {

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
                    stateTracker.track( command );
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

                    // If it was a request and it was not being tracked by the state
                    // tracker, then hold it in the requestMap so that we can replay
                    // it later.
                    Pointer<Tracked> tracked;
                    try{
                        tracked = stateTracker.track( command );
                        synchronized( &requestMap ) {
                            if( tracked != NULL && tracked->isWaitingForResponse() ) {
                                requestMap.put( command->getCommandId(), tracked );
                            } else if( tracked == NULL && command->isResponseRequired() ) {
                                requestMap.put( command->getCommandId(), command );
                            }
                        }
                    } catch( Exception& ex ) {
                        ex.setMark( __FILE__, __LINE__ );
                        error.reset( ex.clone() );
                        break;
                    }

                    // Send the message.
                    try {
                        transport->oneway( command );
                        stateTracker.trackBack( command );
                    } catch( IOException& e ) {

                        e.setMark( __FILE__, __LINE__ );

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
                    e.setMark( __FILE__, __LINE__ );
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
    throw( IOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FailoverTransport::request( const Pointer<Command>& command AMQCPP_UNUSED,
                                                unsigned int timeout AMQCPP_UNUSED )
    throw( IOException,
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

        backups->setEnabled( false );
        requestMap.clear();

        if( connectedTransport != NULL ) {
            transportToStop.swap( connectedTransport );
        }

        reconnectMutex.notifyAll();
    }

    synchronized( &sleepMutex ) {
        sleepMutex.notifyAll();
    }

    taskRunner->shutdown( 2000 );

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
    synchronized( &reconnectMutex ) {
        connectedTransport.swap( transport );
    }

    if( transport != NULL ) {

        if( this->disposedListener != NULL ) {
            transport->setTransportListener( disposedListener.get() );
        }

        // Hand off to the close task so it gets done in a different thread.
        closeTask->add( transport );
        taskRunner->wakeup();

        synchronized( &reconnectMutex ) {

            initialized = false;
            uris->addURI( *connectedTransportURI );
            connectedTransportURI.reset( NULL );
            connected = false;

            if( started ) {
                taskRunner->wakeup();
            }
        }

        if( transportListener != NULL ) {
            transportListener->transportInterrupted();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isPending() const {
    bool result = false;

    synchronized( &reconnectMutex ) {
        if( this->connectedTransport == NULL && !closed && started ) {
            result = true;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::iterate() {

    Pointer<Exception> failure;

    synchronized( &reconnectMutex ) {

        if( closed || connectionFailure != NULL ) {
            reconnectMutex.notifyAll();
        }

        if( connectedTransport != NULL || closed || connectionFailure != NULL ) {
            return false;
        } else {

            StlList<URI> failures;
            Pointer<Transport> transport;
            URI uri;

            if( !useExponentialBackOff ) {
                reconnectDelay = initialReconnectDelay;
            }

            if( backups->isEnabled() ) {

                Pointer<BackupTransport> backupTransport = backups->getBackup();

                if( backupTransport != NULL ) {

                    transport = backupTransport->getTransport();
                    uri = backupTransport->getUri();
                    transport->setTransportListener( myTransportListener.get() );

                    try {

                        if( started ) {
                            restoreTransport( transport );
                        }

                    } catch( Exception& e ) {
                        transport.reset( NULL );
                        this->uris->addURI( uri );
                    }
                }
            }

            while( transport == NULL && !closed ) {

                try{
                    uri = uris->getURI();
                } catch( NoSuchElementException& ex ) {
                    break;
                }

                try {

                    std::cout << "FailoverTransport: Attempting Connection to "
                              << uri.toString() << std::endl;

                    transport = createTransport( uri );
                    transport->setTransportListener( myTransportListener.get() );
                    transport->start();

                    if( started ) {
                        restoreTransport( transport );
                    }

                } catch( Exception& e ) {
                    e.setMark( __FILE__, __LINE__ );
                    transport.reset( NULL );
                    failures.add( uri );
                    failure.reset( e.clone() );
                }
            }

            // Return the failures to the pool, we will try again on the next iteration.
            this->uris->addURIs( failures );

            if( transport != NULL ) {
                reconnectDelay = initialReconnectDelay;
                connectedTransportURI.reset( new URI( uri ) );
                connectedTransport = transport;
                reconnectMutex.notifyAll();
                connectFailures = 0;
                connected = true;

                // Make sure on initial startup, that the transportListener
                // has been initialized for this instance.
                synchronized( &listenerMutex ) {
                    if( transportListener == NULL ) {
                        // if it isn't set after 2secs - it probably never will be
                        listenerMutex.wait( 2000 );
                    }
                }

                if( transportListener != NULL ) {
                    transportListener->transportResumed();
                }

                return false;
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
                    ioException = connectionFailure.dynamicCast<IOException>();
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
            sleepMutex.wait( (unsigned int)reconnectDelay );
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
