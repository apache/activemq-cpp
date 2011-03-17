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

#include "ActiveMQConnection.h"

#include <cms/Session.h>

#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/policies/DefaultPrefetchPolicy.h>
#include <activemq/core/policies/DefaultRedeliveryPolicy.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/exceptions/BrokerException.h>
#include <activemq/exceptions/ConnectionFailedException.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/IdGenerator.h>
#include <activemq/transport/failover/FailoverTransport.h>

#include <decaf/lang/Math.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/UUID.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <activemq/commands/Command.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/BrokerInfo.h>
#include <activemq/commands/BrokerError.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/KeepAliveInfo.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/commands/ProducerAck.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::policies;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::threads;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class ConnectionConfig {
    private:

        ConnectionConfig( const ConnectionConfig& );
        ConnectionConfig& operator= ( const ConnectionConfig& );

    public:

        typedef decaf::util::StlMap< Pointer<commands::ConsumerId>,
                                     Dispatcher*,
                                     commands::ConsumerId::COMPARATOR > DispatcherMap;

        typedef decaf::util::StlMap< Pointer<commands::ProducerId>,
                                     ActiveMQProducer*,
                                     commands::ProducerId::COMPARATOR > ProducerMap;

    public:

        static util::IdGenerator CONNECTION_ID_GENERATOR;

        Pointer<decaf::util::Properties> properties;
        Pointer<transport::Transport> transport;
        Pointer<util::IdGenerator> clientIdGenerator;
        Pointer<Scheduler> scheduler;

        util::LongSequenceGenerator sessionIds;
        util::LongSequenceGenerator tempDestinationIds;
        util::LongSequenceGenerator localTransactionIds;

        std::string brokerURL;

        bool clientIDSet;
        bool isConnectionInfoSentToBroker;
        bool userSpecifiedClientID;

        decaf::util::concurrent::Mutex ensureConnectionInfoSentMutex;
        decaf::util::concurrent::Mutex mutex;

        bool dispatchAsync;
        bool alwaysSyncSend;
        bool useAsyncSend;
        bool messagePrioritySupported;
        bool useCompression;
        int compressionLevel;
        unsigned int sendTimeout;
        unsigned int closeTimeout;
        unsigned int producerWindowSize;

        std::auto_ptr<PrefetchPolicy> defaultPrefetchPolicy;
        std::auto_ptr<RedeliveryPolicy> defaultRedeliveryPolicy;

        cms::ExceptionListener* exceptionListener;

        Pointer<commands::ConnectionInfo> connectionInfo;
        Pointer<commands::BrokerInfo> brokerInfo;
        Pointer<commands::WireFormatInfo> brokerWireFormatInfo;
        Pointer<CountDownLatch> transportInterruptionProcessingComplete;
        Pointer<CountDownLatch> brokerInfoReceived;

        Pointer<Exception> firstFailureError;

        DispatcherMap dispatchers;
        ProducerMap activeProducers;

        decaf::util::concurrent::CopyOnWriteArrayList<ActiveMQSession*> activeSessions;
        decaf::util::concurrent::CopyOnWriteArrayList<transport::TransportListener*> transportListeners;

        ConnectionConfig() : properties(),
                             transport(),
                             clientIdGenerator(),
                             scheduler(),
                             sessionIds(),
                             tempDestinationIds(),
                             localTransactionIds(),
                             brokerURL(""),
                             clientIDSet( false ),
                             isConnectionInfoSentToBroker( false ),
                             userSpecifiedClientID( false ),
                             ensureConnectionInfoSentMutex(),
                             mutex(),
                             dispatchAsync( true ),
                             alwaysSyncSend( false ),
                             useAsyncSend( false ),
                             messagePrioritySupported( true ),
                             useCompression( false ),
                             compressionLevel( -1 ),
                             sendTimeout( 0 ),
                             closeTimeout( 15000 ),
                             producerWindowSize( 0 ),
                             defaultPrefetchPolicy( NULL ),
                             defaultRedeliveryPolicy( NULL ),
                             exceptionListener( NULL ),
                             connectionInfo(),
                             brokerInfo(),
                             brokerWireFormatInfo(),
                             transportInterruptionProcessingComplete(),
                             brokerInfoReceived(),
                             firstFailureError(),
                             dispatchers(),
                             activeProducers(),
                             activeSessions(),
                             transportListeners() {

            this->defaultPrefetchPolicy.reset( new DefaultPrefetchPolicy() );
            this->defaultRedeliveryPolicy.reset( new DefaultRedeliveryPolicy() );
            this->clientIdGenerator.reset(new util::IdGenerator );
            this->connectionInfo.reset( new ConnectionInfo() );
            this->brokerInfoReceived.reset( new CountDownLatch(1) );

            // Generate a connectionId
            std::string uniqueId = CONNECTION_ID_GENERATOR.generateId();
            decaf::lang::Pointer<ConnectionId> connectionId( new ConnectionId() );
            connectionId->setValue(uniqueId);
            this->connectionInfo->setConnectionId( connectionId );
            this->scheduler.reset(new Scheduler(std::string("ActiveMQConnection[")+uniqueId+"] Scheduler"));
            this->scheduler->start();
        }

        void waitForBrokerInfo() {
            this->brokerInfoReceived->await();
        }
    };

    // Static init.
    util::IdGenerator ConnectionConfig::CONNECTION_ID_GENERATOR;
}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::ActiveMQConnection( const Pointer<transport::Transport>& transport,
                                        const Pointer<decaf::util::Properties>& properties ) :
    config( NULL ),
    connectionMetaData( new ActiveMQConnectionMetaData() ),
    started(false),
    closed(false),
    closing(false),
    transportFailed(false) {

    Pointer<ConnectionConfig> configuration( new ConnectionConfig );

    // Register for messages and exceptions from the connector.
    transport->setTransportListener( this );

    // Set the initial state of the ConnectionInfo
    configuration->connectionInfo->setManageable( false );
    configuration->connectionInfo->setFaultTolerant( transport->isFaultTolerant() );

    // Store of the transport and properties, the Connection now owns them.
    configuration->properties = properties;
    configuration->transport = transport;

    this->config = configuration.release();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::~ActiveMQConnection() throw() {
    try {

        try{
            this->close();
        } catch(...) {}

        // This must happen even if exceptions occur in the Close attempt.
        delete this->config;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addDispatcher(
    const decaf::lang::Pointer<ConsumerId>& consumer, Dispatcher* dispatcher ) {

    try{
        // Add the consumer to the map.
        synchronized( &this->config->dispatchers ) {
            this->config->dispatchers.put( consumer, dispatcher );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeDispatcher( const decaf::lang::Pointer<ConsumerId>& consumer ) {

    try{
        // Remove the consumer from the map.
        synchronized( &this->config->dispatchers ) {
            this->config->dispatchers.remove( consumer );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession() {
    try {
        return createSession( Session::AUTO_ACKNOWLEDGE );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession( cms::Session::AcknowledgeMode ackMode ) {

    try {

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        // Create the session instance.
        ActiveMQSession* session = new ActiveMQSession(
            this, getNextSessionId(), ackMode, *this->config->properties );

        return session;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
Pointer<SessionId> ActiveMQConnection::getNextSessionId() {

    decaf::lang::Pointer<SessionId> sessionId( new SessionId() );
    sessionId->setConnectionId( this->config->connectionInfo->getConnectionId()->getValue() );
    sessionId->setValue( this->config->sessionIds.getNextSequenceId() );

    return sessionId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addSession( ActiveMQSession* session ) {

    try {

        // Remove this session from the set of active sessions.
        this->config->activeSessions.add( session );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeSession( ActiveMQSession* session ) {

    try {

        // Remove this session from the set of active sessions.
        this->config->activeSessions.remove( session );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addProducer( ActiveMQProducer* producer ) {

    try {

        // Add this producer from the set of active consumer.
        synchronized( &this->config->activeProducers ) {
            this->config->activeProducers.put( producer->getProducerInfo()->getProducerId(), producer );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeProducer( const decaf::lang::Pointer<ProducerId>& producerId ) {

    try {

        // Remove this producer from the set of active consumer.
        synchronized( &this->config->activeProducers ) {
            this->config->activeProducers.remove( producerId );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getClientID() const {

    if( this->isClosed() ) {
        return "";
    }

    return this->config->connectionInfo->getClientId();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setClientID( const std::string& clientID ) {

    if( this->closed.get() ) {
        throw cms::IllegalStateException( "Connection is already closed", NULL );
    }

    if( this->config->clientIDSet ) {
        throw cms::IllegalStateException( "Client ID is already set", NULL );
    }

    if( this->config->isConnectionInfoSentToBroker ) {
        throw cms::IllegalStateException( "Cannot set client Id on a Connection already in use.", NULL );
    }

    if( clientID.empty() ) {
        throw cms::InvalidClientIdException( "Client ID cannot be an empty string", NULL );
    }

    this->config->connectionInfo->setClientId( clientID );
    this->config->userSpecifiedClientID = true;
    ensureConnectionInfoSent();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setDefaultClientId( const std::string& clientId ) {
    this->setClientID( clientId );
    this->config->userSpecifiedClientID = true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::close() {

    try {

        if( this->isClosed() ) {
            return;
        }

        // If we are running lets stop first.
        if( !this->transportFailed.get() ) {
            this->stop();
        }

        // Indicates we are on the way out to suppress any exceptions getting
        // passed on from the transport as it goes down.
        this->closing.set( true );

        if(this->config->scheduler != NULL) {
            try {
                this->config->scheduler->stop();
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        // Get the complete list of active sessions.
        std::auto_ptr< Iterator<ActiveMQSession*> > iter( this->config->activeSessions.iterator() );

        long long lastDeliveredSequenceId = 0;

        // Dispose of all the Session resources we know are still open.
        while( iter->hasNext() ) {
            ActiveMQSession* session = iter->next();
            try{
                session->dispose();

                lastDeliveredSequenceId =
                    Math::max( lastDeliveredSequenceId, session->getLastDeliveredSequenceId() );
            } catch( cms::CMSException& ex ){
                /* Absorb */
            }
        }

        // Now inform the Broker we are shutting down.
        this->disconnect( lastDeliveredSequenceId );

        // Once current deliveries are done this stops the delivery
        // of any new messages.
        this->started.set( false );
        this->closed.set( true );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::cleanup() {

    try{

        // Get the complete list of active sessions.
        std::auto_ptr< Iterator<ActiveMQSession*> > iter( this->config->activeSessions.iterator() );

        // Dispose of all the Session resources we know are still open.
        while( iter->hasNext() ) {
            ActiveMQSession* session = iter->next();
            try{
                session->dispose();
            } catch( cms::CMSException& ex ){
                /* Absorb */
            }
        }

        if( this->config->isConnectionInfoSentToBroker ) {
            if( !transportFailed.get() && !closing.get() ) {
                this->syncRequest( this->config->connectionInfo->createRemoveCommand() );
            }
            this->config->isConnectionInfoSentToBroker = false;
        }

        if( this->config->userSpecifiedClientID ) {
            this->config->connectionInfo->setClientId("");
            this->config->userSpecifiedClientID = false;
        }

        this->config->clientIDSet = false;
        this->started.set( false );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::start() {

    try{

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        // This starts or restarts the delivery of all incoming messages
        // messages delivered while this connection is stopped are dropped
        // and not acknowledged.
        if( this->started.compareAndSet( false, true ) ) {

            // Start all the sessions.
            std::auto_ptr< Iterator<ActiveMQSession*> > iter( this->config->activeSessions.iterator() );
            while( iter->hasNext() ) {
                iter->next()->start();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::stop() {

    try {

        checkClosedOrFailed();

        // Once current deliveries are done this stops the delivery of any
        // new messages.
        if( this->started.compareAndSet( true, false ) ) {
            synchronized( &this->config->activeSessions ) {
                std::auto_ptr< Iterator<ActiveMQSession*> > iter( this->config->activeSessions.iterator() );

                while( iter->hasNext() ){
                    iter->next()->stop();
                }
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disconnect( long long lastDeliveredSequenceId ) {

    try{

        // Clear the listener, we don't care about async errors at this point.
        this->config->transport->setTransportListener( NULL );

        if( this->config->isConnectionInfoSentToBroker ) {

            // Remove our ConnectionId from the Broker
            Pointer<RemoveInfo> command( this->config->connectionInfo->createRemoveCommand() );
            command->setLastDeliveredSequenceId( lastDeliveredSequenceId );
            this->syncRequest( command, this->config->closeTimeout );

            // Send the disconnect command to the broker.
            Pointer<ShutdownInfo> shutdown( new ShutdownInfo() );
            oneway( shutdown );
        }

        // Allow the Support class to shutdown its resources, including the Transport.
        bool hasException = false;
        exceptions::ActiveMQException e;

        if( this->config->transport != NULL ){

            try{
                this->config->transport->close();
            }catch( exceptions::ActiveMQException& ex ){
                if( !hasException ){
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__ );
                    e = ex;
                }
            }

            try{
                this->config->transport.reset( NULL );
            }catch( exceptions::ActiveMQException& ex ){
                if( !hasException ){
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__ );
                    e = ex;
                }
            }
        }

        // If we encountered an exception - throw the first one we encountered.
        // This will preserve the stack trace for logging purposes.
        if( hasException ){
            throw e;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::sendPullRequest( const ConsumerInfo* consumer, long long timeout ) {

    try {

         if( consumer->getPrefetchSize() == 0 ) {

             Pointer<MessagePull> messagePull( new MessagePull() );
             messagePull->setConsumerId( consumer->getConsumerId() );
             messagePull->setDestination( consumer->getDestination() );
             messagePull->setTimeout( timeout );

             this->oneway( messagePull );
         }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination( const ActiveMQDestination* destination ) {

    try{

        if( destination == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Destination passed was NULL" );
        }

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        Pointer<DestinationInfo> command( new DestinationInfo() );

        command->setConnectionId( this->config->connectionInfo->getConnectionId() );
        command->setOperationType( ActiveMQConstants::DESTINATION_REMOVE_OPERATION );
        command->setDestination( Pointer<ActiveMQDestination>( destination->cloneDataStructure() ) );

        // Send the message to the broker.
        syncRequest( command );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( decaf::lang::exceptions::IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination( const cms::Destination* destination ) {

    try{

        if( destination == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Destination passed was NULL" );
        }

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( destination );

        this->destroyDestination( amqDestination );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( decaf::lang::exceptions::IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onCommand( const Pointer<Command>& command ) {

    try{

        if( command->isMessageDispatch() ) {

            Pointer<MessageDispatch> dispatch = command.dynamicCast<MessageDispatch>();

            // Check first to see if we are recovering.
            waitForTransportInterruptionProcessingToComplete();

            // Look up the dispatcher.
            Dispatcher* dispatcher = NULL;
            synchronized( &this->config->dispatchers ) {

                dispatcher = this->config->dispatchers.get( dispatch->getConsumerId() );

                // If we have no registered dispatcher, the consumer was probably
                // just closed.
                if( dispatcher != NULL ) {

                    Pointer<commands::Message> message = dispatch->getMessage();

                    // Message == NULL to signal the end of a Queue Browse.
                    if( message != NULL ) {
                        message->setReadOnlyBody( true );
                        message->setReadOnlyProperties( true );
                        message->setRedeliveryCounter( dispatch->getRedeliveryCounter() );
                    }

                    dispatcher->dispatch( dispatch );
                }
            }

        } else if( command->isProducerAck() ) {

            ProducerAck* producerAck = dynamic_cast<ProducerAck*>( command.get() );

            // Get the consumer info object for this consumer.
            ActiveMQProducer* producer = NULL;
            synchronized( &this->config->activeProducers ) {
                producer = this->config->activeProducers.get( producerAck->getProducerId() );
                if( producer != NULL ){
                    producer->onProducerAck( *producerAck );
                }
            }

        } else if( command->isWireFormatInfo() ) {
            this->config->brokerWireFormatInfo =
                command.dynamicCast<WireFormatInfo>();
        } else if( command->isBrokerInfo() ) {
            this->config->brokerInfo =
                command.dynamicCast<BrokerInfo>();
            this->config->brokerInfoReceived->countDown();
        } else if( command->isShutdownInfo() ) {

            try {
                if( !this->isClosed() ) {
                    fire( ActiveMQException(
                        __FILE__, __LINE__,
                        "ActiveMQConnection::onCommand - "
                        "Broker closed this connection."));
                }
            } catch( ... ) { /* do nothing */ }

        } else {
            //LOGDECAF_WARN( logger, "Received an unknown command" );
        }

        Pointer< Iterator<TransportListener*> > iter( this->config->transportListeners.iterator() );

        while( iter->hasNext() ) {
            try{
                iter->next()->onCommand( command );
            } catch(...) {}
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onException( const decaf::lang::Exception& ex ) {

    try {

        // We're disconnected - the asynchronous error is expected.
        if( this->isClosed() || this->closing.get() ) {
            return;
        }

        onAsyncException(ex);

        // TODO This part should be done in a separate Thread.

        // Mark this Connection as having a Failed transport.
        this->transportFailed.set( true );
        if( this->config->firstFailureError == NULL ) {
            this->config->firstFailureError.reset( ex.clone() );
        }

        this->config->brokerInfoReceived->countDown();

        // TODO - Until this fires in another thread we can't dipose of
        //        the transport here since it could result in this method
        //        being called again recursively.
        try{
            // this->config->transport->stop();
        } catch(...) {
        }

        // Clean up the Connection resources.
        cleanup();

        Pointer< Iterator<TransportListener*> > iter( this->config->transportListeners.iterator() );

        while( iter->hasNext() ) {
            try{
                iter->next()->onException( ex );
            } catch(...) {}
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onAsyncException( const decaf::lang::Exception& ex ) {

    if( !this->isClosed() || !this->closing.get() ) {

        if( this->config->exceptionListener != NULL ) {

            // Inform the user of the error.
            // TODO - This should be run by another Thread, i.e. Executor
            fire( exceptions::ActiveMQException( ex ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::transportInterrupted() {

    this->config->transportInterruptionProcessingComplete.reset(
        new CountDownLatch( (int)this->config->dispatchers.size() ) );

    std::auto_ptr< Iterator<ActiveMQSession*> > sessions( this->config->activeSessions.iterator() );

    while( sessions->hasNext() ) {
        sessions->next()->clearMessagesInProgress();
    }

    Pointer< Iterator<TransportListener*> > listeners( this->config->transportListeners.iterator() );

    while( listeners->hasNext() ) {
        try{
            listeners->next()->transportInterrupted();
        } catch(...) {}
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::transportResumed() {

    Pointer< Iterator<TransportListener*> > iter( this->config->transportListeners.iterator() );

    while( iter->hasNext() ) {
        try{
            iter->next()->transportResumed();
        } catch(...) {}
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::oneway( Pointer<Command> command ) {

    try {
        checkClosedOrFailed();
        this->config->transport->oneway( command );
    }
    AMQ_CATCH_EXCEPTION_CONVERT( IOException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::exceptions::UnsupportedOperationException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> ActiveMQConnection::syncRequest( Pointer<Command> command, unsigned int timeout ) {

    try {

        checkClosedOrFailed();

        Pointer<Response> response;

        if( timeout == 0 ) {
            response = this->config->transport->request( command );
        } else {
            response = this->config->transport->request( command, timeout );
        }

        commands::ExceptionResponse* exceptionResponse =
            dynamic_cast<ExceptionResponse*>( response.get() );

        if( exceptionResponse != NULL ) {

            // Create an exception to hold the error information.
            BrokerException exception( __FILE__, __LINE__, exceptionResponse->getException().get() );

            // Throw the exception.
            throw exception;
        }

        return response;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( IOException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::exceptions::UnsupportedOperationException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::checkClosed() const {
    if( this->isClosed() ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConnection::enforceConnected - Connection has already been closed!" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::checkClosedOrFailed() const {

    checkClosed();
    if( this->transportFailed.get() == true ) {
        throw ConnectionFailedException( *this->config->firstFailureError );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::ensureConnectionInfoSent() {

    try{

        // Can we skip sending the ConnectionInfo packet, cheap test
        if( this->config->isConnectionInfoSentToBroker || closed.get() ) {
            return;
        }

        synchronized( &( this->config->ensureConnectionInfoSentMutex ) ) {

            // Can we skip sending the ConnectionInfo packet??
            if( this->config->isConnectionInfoSentToBroker || closed.get() ) {
                return;
            }

            // check for a user specified Id
            if( !this->config->userSpecifiedClientID ) {
                this->config->connectionInfo->setClientId( this->config->clientIdGenerator->generateId() );
            }

            // Now we ping the broker and see if we get an ack / nack
            syncRequest( this->config->connectionInfo );

            this->config->isConnectionInfoSentToBroker = true;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::fire( const ActiveMQException& ex ) {
    if( this->config->exceptionListener != NULL ) {
        try {
            this->config->exceptionListener->onException( ex.convertToCMSException() );
        }
        catch(...){}
    }
}

////////////////////////////////////////////////////////////////////////////////
const ConnectionInfo& ActiveMQConnection::getConnectionInfo() const {
    checkClosed();
    return *this->config->connectionInfo;
}

////////////////////////////////////////////////////////////////////////////////
const ConnectionId& ActiveMQConnection::getConnectionId() const {
    checkClosed();
    return *( this->config->connectionInfo->getConnectionId() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addTransportListener( TransportListener* transportListener ) {

    if( transportListener == NULL ) {
        return;
    }

    // Add this listener from the set of active TransportListeners
    this->config->transportListeners.add( transportListener );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeTransportListener( TransportListener* transportListener ) {

    if( transportListener == NULL ) {
        return;
    }

    // Remove this listener from the set of active TransportListeners
    this->config->transportListeners.remove( transportListener );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::waitForTransportInterruptionProcessingToComplete() {

    Pointer<CountDownLatch> cdl = this->config->transportInterruptionProcessingComplete;
    if( cdl != NULL ) {

        while( !closed.get() && !transportFailed.get() && cdl->getCount() > 0 ) {
            cdl->await( 10, TimeUnit::SECONDS );
        }

        signalInterruptionProcessingComplete();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setTransportInterruptionProcessingComplete() {

    Pointer<CountDownLatch> cdl = this->config->transportInterruptionProcessingComplete;
    if( cdl != NULL ) {
        cdl->countDown();

        try {
            signalInterruptionProcessingComplete();
        } catch( InterruptedException& ignored ) {}
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::signalInterruptionProcessingComplete() {

    Pointer<CountDownLatch> cdl = this->config->transportInterruptionProcessingComplete;

    if( cdl->getCount() == 0 ) {

        this->config->transportInterruptionProcessingComplete.reset( NULL );
        FailoverTransport* failoverTransport =
            dynamic_cast<FailoverTransport*>( this->config->transport->narrow( typeid( FailoverTransport ) ) );

        if( failoverTransport != NULL ) {
            failoverTransport->setConnectionInterruptProcessingComplete(
                this->config->connectionInfo->getConnectionId() );

            //if( LOG.isDebugEnabled() ) {
            //    LOG.debug("transportInterruptionProcessingComplete for: " + this.getConnectionInfo().getConnectionId());
            //}
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUsername( const std::string& username ) {
    this->config->connectionInfo->setUserName( username );
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnection::getUsername() const {
    return this->config->connectionInfo->getUserName();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setPassword( const std::string& password ){
    this->config->connectionInfo->setPassword( password );
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnection::getPassword() const {
    return this->config->connectionInfo->getPassword();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setBrokerURL( const std::string& brokerURL ){
    this->config->brokerURL = brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnection::getBrokerURL() const {
    return this->config->brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setExceptionListener( cms::ExceptionListener* listener ) {
    this->config->exceptionListener = listener;
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQConnection::getExceptionListener() const {
    return this->config->exceptionListener;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setPrefetchPolicy( PrefetchPolicy* policy ) {
    this->config->defaultPrefetchPolicy.reset( policy );
}

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy* ActiveMQConnection::getPrefetchPolicy() const {
    return this->config->defaultPrefetchPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setRedeliveryPolicy( RedeliveryPolicy* policy ) {
    this->config->defaultRedeliveryPolicy.reset( policy );
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConnection::getRedeliveryPolicy() const {
    return this->config->defaultRedeliveryPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isDispatchAsync() const {
    return this->config->dispatchAsync;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setDispatchAsync( bool value ) {
    this->config->dispatchAsync = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isAlwaysSyncSend() const {
    return this->config->alwaysSyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setAlwaysSyncSend( bool value ) {
    this->config->alwaysSyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isUseAsyncSend() const {
    return this->config->useAsyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUseAsyncSend( bool value ) {
    this->config->useAsyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isUseCompression() const {
    return this->config->useCompression;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUseCompression( bool value ) {
    this->config->useCompression = value;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnection::getCompressionLevel() const {
    return this->config->compressionLevel;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setCompressionLevel( int value ) {

    if( value < 0 ) {
        this->config->compressionLevel = -1;
    }

    this->config->compressionLevel = Math::min( value, 9 );
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnection::getSendTimeout() const {
    return this->config->sendTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setSendTimeout( unsigned int timeout ) {
    this->config->sendTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnection::getCloseTimeout() const {
    return this->config->closeTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setCloseTimeout( unsigned int timeout ) {
    this->config->closeTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnection::getProducerWindowSize() const {
    return this->config->producerWindowSize;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setProducerWindowSize( unsigned int windowSize ) {
    this->config->producerWindowSize = windowSize;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getNextTempDestinationId() {
    return this->config->tempDestinationIds.getNextSequenceId();
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getNextLocalTransactionId() {
    return this->config->localTransactionIds.getNextSequenceId();
}

////////////////////////////////////////////////////////////////////////////////
transport::Transport& ActiveMQConnection::getTransport() const {
    return *( this->config->transport );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Scheduler> ActiveMQConnection::getScheduler() const {
    return this->config->scheduler;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isMessagePrioritySupported() const {
    return this->config->messagePrioritySupported;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setMessagePrioritySupported( bool value ) {
    this->config->messagePrioritySupported = value;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Exception* ActiveMQConnection::getFirstFailureError() const {
    return this->config->firstFailureError.get();
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getResourceManagerId() const {
    try {
        this->config->waitForBrokerInfo();

        if( this->config->brokerInfo == NULL ) {
            throw CMSException("Connection failed before Broker info was received.");
        }

        return this->config->brokerInfo->getBrokerId()->getValue();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
const decaf::util::Properties& ActiveMQConnection::getProperties() const {
    return *( this->config->properties );
}
