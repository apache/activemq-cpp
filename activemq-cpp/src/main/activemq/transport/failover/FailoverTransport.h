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

#ifndef _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORT_H_
#define _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORT_H_

#include <activemq/util/Config.h>

#include <activemq/commands/Command.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/threads/TaskRunner.h>
#include <activemq/threads/CompositeTaskRunner.h>
#include <activemq/state/ConnectionStateTracker.h>
#include <activemq/transport/CompositeTransport.h>
#include <activemq/transport/failover/BackupTransportPool.h>
#include <activemq/transport/failover/CloseTransportsTask.h>
#include <activemq/transport/failover/FailoverTransportListener.h>
#include <activemq/transport/failover/URIPool.h>
#include <activemq/wireformat/WireFormat.h>

#include <decaf/util/LinkedList.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicReference.h>
#include <decaf/net/URI.h>
#include <decaf/io/IOException.h>

namespace activemq {
namespace transport {
namespace failover {

    using namespace decaf::lang;
    using decaf::net::URI;
    using namespace decaf::util;
    using namespace activemq::threads;
    using activemq::commands::Command;
    using activemq::commands::Response;

    class AMQCPP_API FailoverTransport : public CompositeTransport,
                                         public activemq::threads::CompositeTask {
    private:

        friend class FailoverTransportListener;

        bool closed;
        bool connected;
        bool started;

        long long timeout;
        long long initialReconnectDelay;
        long long maxReconnectDelay;
        long long backOffMultiplier;
        bool useExponentialBackOff;
        bool initialized;
        int maxReconnectAttempts;
        int startupMaxReconnectAttempts;
        int connectFailures;
        long long reconnectDelay;
        bool trackMessages;
        bool trackTransactionProducers;
        int maxCacheSize;
        bool connectionInterruptProcessingComplete;
        bool firstConnection;
        bool updateURIsSupported;
        bool reconnectSupported;

        mutable decaf::util::concurrent::Mutex reconnectMutex;
        mutable decaf::util::concurrent::Mutex sleepMutex;
        mutable decaf::util::concurrent::Mutex listenerMutex;

        state::ConnectionStateTracker stateTracker;
        decaf::util::StlMap<int, Pointer<Command> > requestMap;

        Pointer<URIPool> uris;
        decaf::util::LinkedList<URI> updated;
        Pointer<URI> connectedTransportURI;
        Pointer<Transport> connectedTransport;
        Pointer<Exception> connectionFailure;
        Pointer<BackupTransportPool> backups;
        Pointer<CloseTransportsTask> closeTask;
        Pointer<CompositeTaskRunner> taskRunner;
        Pointer<TransportListener> disposedListener;
        Pointer<TransportListener> myTransportListener;
        TransportListener* transportListener;

    private:

        FailoverTransport( const FailoverTransport& );
        FailoverTransport& operator= ( const FailoverTransport& );

    public:

        FailoverTransport();

        virtual ~FailoverTransport();

        /**
         * Indicates that the Transport needs to reconnect to another URI in its
         * list.
         *
         * @param rebalance
         *      Indicates if the current connection should be broken and reconnected.
         */
        void reconnect( bool rebalance );

        /**
         * Adds a New URI to the List of URIs this transport can Connect to.
         * @param uri
         *        A String version of a URI to add to the URIs to failover to.
         */
        void add( const std::string& uri );

    public: // CompositeTransport methods

        virtual void addURI( bool rebalance, const List<URI>& uris );

        virtual void removeURI( bool rebalance, const List<URI>& uris );

    public: // Transport Members

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual void oneway( const Pointer<Command>& command );

        virtual Pointer<Response> request( const Pointer<Command>& command );

        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout );

        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat AMQCPP_UNUSED ) {}

        virtual void setTransportListener( TransportListener* listener );

        virtual TransportListener* getTransportListener() const;

        virtual bool isFaultTolerant() const {
            return true;
        }

        virtual bool isConnected() const;

        virtual bool isClosed() const;

        bool isInitialized() const;

        void setInitialized( bool value );

        virtual Transport* narrow( const std::type_info& typeId );

        virtual std::string getRemoteAddress() const;

        virtual void reconnect( const decaf::net::URI& uri );

        virtual void updateURIs( bool rebalance, const decaf::util::List<decaf::net::URI>& uris );

    public:  // CompositeTask Methods.

        /**
         * @returns true if there is a need for the iterate method to be called by this
         *          classes task runner.
         */
        virtual bool isPending() const;

        /**
         * Performs the actual Reconnect operation for the FailoverTransport, when a
         * connection is made this method returns false to indicate it doesn't need to
         * run again, otherwise it returns true to indicate its still trying to connect.
         *
         * @return false to indicate a connection, true to indicate it needs to try again.
         */
        virtual bool iterate();

    public: // FailoverTransport Property Getters / Setters

        long long getTimeout() const;

        void setTimeout( long long value );

        long long getInitialReconnectDelay() const;

        void setInitialReconnectDelay( long long value );

        long long getMaxReconnectDelay() const;

        void setMaxReconnectDelay( long long value );

        long long getBackOffMultiplier() const;

        void setBackOffMultiplier( long long value );

        bool isUseExponentialBackOff() const;

        void setUseExponentialBackOff( bool value );

        bool isRandomize() const;

        void setRandomize( bool value );

        int getMaxReconnectAttempts() const;

        void setMaxReconnectAttempts( int value );

        int getStartupMaxReconnectAttempts() const;

        void setStartupMaxReconnectAttempts( int value );

        long long getReconnectDelay() const;

        void setReconnectDelay( long long value );

        bool isBackup() const;

        void setBackup( bool value );

        int getBackupPoolSize() const;

        void setBackupPoolSize( int value );

        bool isTrackMessages() const;

        void setTrackMessages( bool value );

        bool isTrackTransactionProducers() const;

        void setTrackTransactionProducers( bool value );

        int getMaxCacheSize() const;

        void setMaxCacheSize( int value );

        bool isReconnectSupported() const;

        void setReconnectSupported( bool value );

        bool isUpdateURIsSupported() const;

        void setUpdateURIsSupported( bool value );

        void setConnectionInterruptProcessingComplete( const Pointer<commands::ConnectionId>& connectionId );

    protected:

        /**
         * Given a Transport restore the state of the Client's connection to the Broker
         * using the data accumulated in the State Tracker.
         *
         * @param transport
         *        The new Transport connected to the Broker.
         *
         * @throw IOException if an errors occurs while restoring the old state.
         */
        void restoreTransport( const Pointer<Transport>& transport );

        /**
         * Called when this class' TransportListener is notified of a Failure.
         * @param error - The CMS Exception that was thrown.
         * @throw Exception if an error occurs.
         */
        void handleTransportFailure( const decaf::lang::Exception& error );

        /**
         * Called when the Broker sends a ConnectionControl command which could
         * signal that this Client needs to reconnect in order to rebalance the
         * connections on a Broker or the set of Known brokers has changed.
         *
         * @param control
         *      The ConnectionControl command sent from the Broker.
         */
        void handleConnectionControl( const Pointer<Command>& control );

    private:

        /**
         * Looks up the correct Factory and create a new Composite version of the
         * Transport requested.
         *
         * @param uri - The URI to connect to
         *
         * @throw IOException if an I/O error occurs while creating the new Transport.
         */
        Pointer<Transport> createTransport( const URI& location ) const;

        void processNewTransports( bool rebalance, std::string newTransports );

        void processResponse(const Pointer<Response>& response);

    };

}}}

#endif /* _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORT_H_ */
