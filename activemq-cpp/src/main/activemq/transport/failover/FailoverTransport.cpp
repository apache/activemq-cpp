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
#include <decaf/util/StringTokenizer.h>
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
    this->startupMaxReconnectAttempts = 0;
    this->connectFailures = 0;
    this->reconnectDelay = this->initialReconnectDelay;
    this->trackMessages = false;
    this->trackTransactionProducers = true;
    this->maxCacheSize = 128 * 1024;

    this->started = false;
    this->closed = false;
    this->connected = false;
    this->connectionInterruptProcessingComplete = false;
    this->firstConnection = true;

    this->updateURIsSupported = true;
    this->reconnectSupported = true;

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

        reconnect( false );
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::addURI( bool rebalance, const List<URI>& uris ) {

    std::auto_ptr< Iterator<URI> > iter( uris.iterator() );

    while( iter->hasNext() ) {
        this->uris->addURI( iter->next() );
    }

    reconnect( rebalance );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::removeURI( bool rebalance, const List<URI>& uris ) {

    std::auto_ptr< Iterator<URI> > iter( uris.iterator() );

    while( iter->hasNext() ) {
        this->uris->removeURI( iter->next() );
    }

    reconnect( rebalance );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect( const decaf::net::URI& uri ) {

    try {

        this->uris->addURI( uri );

        reconnect( true );
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
void FailoverTransport::oneway( const Pointer<Command>& command ) {

    Pointer<Exception> error;

    try {

        synchronized( &reconnectMutex ) {

            if( isShutdownCommand( command ) && connectedTransport == NULL ) {

                if( command->isShutdownInfo() ) {
                    // Skipping send of ShutdownInfo command when not connected.
                    return;
                }

                if( command->isRemoveInfo() || command->isMessageAck() ) {
                    // Simulate response to RemoveInfo command or Ack as they will be stale.
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
                                "Failover timeout of %d ms reached.", timeout ) );
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
Pointer<Response> FailoverTransport::request( const Pointer<Command>& command AMQCPP_UNUSED ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FailoverTransport::request( const Pointer<Command>& command AMQCPP_UNUSED,
                                                unsigned int timeout AMQCPP_UNUSED ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::start() {

    try{

        synchronized( &reconnectMutex ) {

            if( this->started ) {
                return;
            }

            started = true;

            stateTracker.setMaxCacheSize( this->getMaxCacheSize() );
            stateTracker.setTrackMessages( this->isTrackMessages() );
            stateTracker.setTrackTransactionProducers( this->isTrackTransactionProducers() );

            if( connectedTransport != NULL ) {
                stateTracker.restore( connectedTransport );
            } else {
                reconnect( false );
            }
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::stop() {

    try{
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::close() {

    try{

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
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect( bool rebalance ) {

    Pointer<Transport> transport;

    synchronized( &reconnectMutex  ) {
        if( started ) {

            if( rebalance ) {

                transport.swap( this->connectedTransport );

                if( transport != NULL ) {

                    if( this->disposedListener != NULL ) {
                        transport->setTransportListener( disposedListener.get() );
                    }

                    // Hand off to the close task so it gets done in a different thread.
                    closeTask->add( transport );
                }
            }

            taskRunner->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::restoreTransport( const Pointer<Transport>& transport ) {

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
void FailoverTransport::handleTransportFailure( const decaf::lang::Exception& error AMQCPP_UNUSED ) {

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

        synchronized( &reconnectMutex ) {

            initialized = false;
            uris->addURI( *connectedTransportURI );
            connectedTransportURI.reset( NULL );
            connected = false;

            // Place the State Tracker into a reconnection state.
            this->stateTracker.transportInterrupted();

            // Notify before we attempt to reconnect so that the consumers have a chance
            // to cleanup their state.
            if( transportListener != NULL ) {
                transportListener->transportInterrupted();
            }

            if( started ) {
                taskRunner->wakeup();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::handleConnectionControl( const Pointer<Command>& control ) {

    try {

        Pointer<ConnectionControl> ctrlCommand = control.dynamicCast<ConnectionControl>();

        std::string reconnectStr = ctrlCommand->getReconnectTo();
        if( !reconnectStr.empty() ) {

            std::remove(reconnectStr.begin(), reconnectStr.end(), ' ');

            if( reconnectStr.length() > 0 ) {
                try {
                    if( isReconnectSupported() ) {
                        reconnect( URI( reconnectStr ) );
                    }
                } catch( Exception e ) {
                }
            }
        }

        processNewTransports( ctrlCommand->isRebalanceConnection(), ctrlCommand->getConnectedBrokers() );
    }
    AMQ_CATCH_RETHROW( Exception )
    AMQ_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::processNewTransports( bool rebalance, std::string newTransports ) {

    if( !newTransports.empty() ) {

        std::remove( newTransports.begin(), newTransports.end(), ' ' );

        if( newTransports.length() > 0 && isUpdateURIsSupported() ) {

            StlList<URI> list;
            StringTokenizer tokenizer( newTransports, "," );

            while( tokenizer.hasMoreTokens() ) {
                std::string str = tokenizer.nextToken();
                try {
                    URI uri( str );
                    list.add( uri );
                } catch( Exception e ) {
                    //LOG.error( "Failed to parse broker address: " + str, e );
                }
            }

            if( !list.isEmpty() ) {
                try {
                    updateURIs( rebalance, list );
                } catch( IOException e ) {
                    //LOG.error( "Failed to update transport URI's from: " + newTransports, e );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::updateURIs( bool rebalance, const decaf::util::List<decaf::net::URI>& updatedURIs ) {

    if( isUpdateURIsSupported() ) {

        StlList<URI> copy( this->updated );
        StlList<URI> add;

        if( !updatedURIs.isEmpty() ) {

            StlSet<URI> set;

            for( int i = 0; i < updatedURIs.size(); i++ ) {
                set.add( updatedURIs.get(i) );
            }

            Pointer< Iterator<URI> > setIter( set.iterator() );
            while( setIter->hasNext() ) {
                URI value = setIter->next();
                if( copy.remove( value ) == false ) {
                    add.add( value );
                }
            }

            synchronized( &reconnectMutex ) {

                this->updated.clear();
                Pointer< Iterator<URI> > listIter1( add.iterator() );
                while( listIter1->hasNext() ) {
                    this->updated.add( listIter1->next() );
                }

                Pointer< Iterator<URI> > listIter2( copy.iterator() );
                while( listIter2->hasNext() ) {
                    this->uris->removeURI( listIter2->next() );
                }

                this->addURI( rebalance, add );
            }
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

                        if( transport != NULL ) {
                            if( this->disposedListener != NULL ) {
                                transport->setTransportListener( disposedListener.get() );
                            }

                            // Hand off to the close task so it gets done in a different thread
                            // this prevents a deadlock from occurring if the Transport happens
                            // to call back through our onException method or locks in some other
                            // way.
                            closeTask->add( transport );
                            taskRunner->wakeup();
                            transport.reset( NULL );
                        }

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

                    transport = createTransport( uri );
                    transport->setTransportListener( myTransportListener.get() );
                    transport->start();

                    if( started ) {
                        restoreTransport( transport );
                    }

                } catch( Exception& e ) {
                    e.setMark( __FILE__, __LINE__ );

                    if( transport != NULL ) {
                        if( this->disposedListener != NULL ) {
                            transport->setTransportListener( disposedListener.get() );
                        }

                        try{
                            transport->stop();
                        } catch(...) {}

                        // Hand off to the close task so it gets done in a different thread
                        // this prevents a deadlock from occurring if the Transport happens
                        // to call back through our onException method or locks in some other
                        // way.
                        closeTask->add( transport );
                        taskRunner->wakeup();
                        transport.reset( NULL );
                    }

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

                if( firstConnection ) {
                    firstConnection = false;
                }

                return false;
            }
        }

        int reconnectAttempts = 0;
        if( firstConnection ) {
            if( startupMaxReconnectAttempts != 0 ) {
                reconnectAttempts = startupMaxReconnectAttempts;
            }
        }
        if( reconnectAttempts == 0 ) {
            reconnectAttempts = maxReconnectAttempts;
        }

        if( reconnectAttempts > 0 && ++connectFailures >= reconnectAttempts ) {
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
            //std::cout << "Failover: Trying again in "
            //          << reconnectDelay << "Milliseconds." << std::endl;
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
Pointer<Transport> FailoverTransport::createTransport( const URI& location ) const {

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

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setConnectionInterruptProcessingComplete( const Pointer<commands::ConnectionId>& connectionId ) {

    synchronized( &reconnectMutex ) {
        stateTracker.connectionInterruptProcessingComplete( this, connectionId );
    }
}
