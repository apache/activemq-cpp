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
#include <activemq/threads/TaskRunner.h>
#include <activemq/threads/CompositeTaskRunner.h>
#include <activemq/state/ConnectionStateTracker.h>
#include <activemq/transport/CompositeTransport.h>
#include <activemq/transport/failover/BackupTransport.h>
#include <activemq/transport/failover/ReconnectTask.h>
#include <activemq/transport/failover/CloseTransportsTask.h>
#include <activemq/transport/failover/FailoverTransportListener.h>
#include <activemq/wireformat/WireFormat.h>

#include <decaf/util/StlList.h>
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

    class AMQCPP_API FailoverTransport : public CompositeTransport {
    private:

        friend class FailoverTransportListener;
        friend class ReconnectTask;

        bool closed;
        bool connected;
        bool started;

        decaf::util::StlList<URI> uris;

        long long timeout;
        long long initialReconnectDelay;
        long long maxReconnectDelay;
        long long backOffMultiplier;
        bool useExponentialBackOff;
        bool randomize;
        bool initialized;
        int maxReconnectAttempts;
        int connectFailures;
        long long reconnectDelay;
        bool backup;
        int backupPoolSize;
        bool trackMessages;
        int maxCacheSize;

        decaf::util::StlList< Pointer<BackupTransport> > backups;

        state::ConnectionStateTracker stateTracker;
        decaf::util::concurrent::Mutex reconnectMutex;
        decaf::util::concurrent::Mutex backupMutex;
        decaf::util::concurrent::Mutex sleepMutex;
        decaf::util::concurrent::Mutex listenerMutex;
        decaf::util::StlMap<int, Pointer<Command> > requestMap;

        Pointer<URI> connectedTransportURI;
        Pointer<URI> failedConnectTransportURI;
        Pointer<Transport> connectedTransport;
        Pointer<Exception> connectionFailure;
        Pointer<ReconnectTask> reconnectTask;
        Pointer<CloseTransportsTask> closeTask;
        Pointer<TaskRunner> taskRunner;
        Pointer<CompositeTaskRunner> compositeTaskRunner;
        Pointer<TransportListener> disposedListener;
        Pointer<TransportListener> myTransportListener;
        TransportListener* transportListener;

    public:

        FailoverTransport();
        virtual ~FailoverTransport();

        /**
         * Indicates that the Transport needs to reconnect to another URI in its
         * list.
         */
        void reconnect();

        /**
         * Adds a New URI to the List of URIs this transport can Connect to.
         * @param uri
         *        A String version of a URI to add to the URIs to failover to.
         */
        void add( const std::string& uri );

    public: // CompositeTransport methods

        /**
         * Add a URI to the list of URI's that will represent the set of Transports
         * that this Transport is a composite of.
         *
         * @param uris
         *        The new URIs to add to the set this composite maintains.
         */
        virtual void addURI( const List<URI>& uris );

        /**
         * Remove a URI from the set of URI's that represents the set of Transports
         * that this Transport is composed of, removing a URI for which the composite
         * has created a connected Transport should result in that Transport being
         * disposed of.
         *
         * @param uris
         *        The new URIs to remove to the set this composite maintains.
         */
        virtual void removeURI( const List<URI>& uris );

    public: // Transport Members

        /**
         * Starts this transport object and creates the thread for
         * polling on the input stream for commands.  If this object
         * has been closed, throws an exception.  Before calling start,
         * the caller must set the IO streams and the reader and writer
         * objects.
         * @throws CMSException if an error occurs or if this transport
         * has already been closed.
         */
        virtual void start() throw( cms::CMSException );

        /**
         * Stops the polling thread and closes the streams.  This can
         * be called explicitly, but is also called in the destructor. Once
         * this object has been closed, it cannot be restarted.
         * @throws CMSException if errors occur.
         */
        virtual void close() throw( cms::CMSException );

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws IOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( const Pointer<Command>& command )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Sends the given command to the broker and then waits for the response.
         * @param command the command to be sent.
         * @return the response from the broker.
         * @throws IOException if an exception occurs during the read of the
         * command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Sends the given command to the broker and then waits for the response.
         * @param command - The command to be sent.
         * @param timeout - The time to wait for this response.
         * @return the response from the broker.
         * @throws IOException if an exception occurs during the read of the
         * command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * Sets the WireFormat instance to use.
         * @param WireFormat the object used to encode / decode commands.
         */
        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat AMQCPP_UNUSED ) {
        }

        /**
         * Sets the observer of asynchronous events from this transport.
         * @param listener the listener of transport events.
         */
        virtual void setTransportListener( TransportListener* listener );

        /**
         * Is this Transport fault tolerant, meaning that it will reconnect to
         * a broker on disconnect.
         *
         * @returns true if the Transport is fault tolerant.
         */
        virtual bool isFaultTolerant() const {
            return true;
        }

        /**
         * Is the Transport Connected to its Broker.
         *
         * @returns true if a connection has been made.
         */
        virtual bool isConnected() const {
            return this->connected;
        }

        /**
         * Has the Transport been shutdown and no longer usable.
         *
         * @returns true if the Transport
         */
        virtual bool isClosed() const {
            return this->closed;
        }

        /**
         * Returns true if the Transport has been initialized by a BrokerInfo command.
         * @return true if the Transport has been initialized by a BrokerInfo command.
         */
        bool isInitialized() const {
            return this->initialized;
        }

        /**
         * Sets the initialized state of this Transport to true.
         * @param value - true if this Transport has been initialized.
         */
        void setInitialized( bool value ) {
            this->initialized = value;
        }

        /**
         * Narrows down a Chain of Transports to a specific Transport to allow a
         * higher level transport to skip intermediate Transports in certain
         * circumstances.
         *
         * @param typeId - The type_info of the Object we are searching for.
         *
         * @return the requested Object. or NULL if its not in this chain.
         */
        virtual Transport* narrow( const std::type_info& typeId ) {
            if( typeid( *this ) == typeId ) {
                return this;
            }

            if( this->connectedTransport != NULL ) {
                return this->connectedTransport->narrow( typeId );
            }

            return NULL;
        }

        /**
         * @return the remote address for this connection
         */
        virtual std::string getRemoteAddress() const;

        /**
         * reconnect to another location
         * @param uri
         * @throws IOException on failure of if not supported
         */
        virtual void reconnect( const decaf::net::URI& uri )
            throw( decaf::io::IOException );

    public: // FailoverTransport Property Getters / Setters

        long long getTimeout() const {
            return this->timeout;
        }

        void setTimeout( long long value ) {
            this->timeout = value;
        }

        long long getInitialReconnectDelay() const {
            return this->initialReconnectDelay;
        }

        void setInitialReconnectDelay( long long value ) {
            this->initialReconnectDelay = value;
        }

        long long getMaxReconnectDelay() const {
            return this->maxReconnectDelay;
        }

        void setMaxReconnectDelay( long long value ) {
            this->maxReconnectDelay = value;
        }

        long long getBackOffMultiplier() const {
            return this->backOffMultiplier;
        }

        void setBackOffMultiplier( long long value ) {
            this->backOffMultiplier = value;
        }

        bool isUseExponentialBackOff() const {
            return this->useExponentialBackOff;
        }

        void setUseExponentialBackOff( bool value ) {
            this->useExponentialBackOff = value;
        }

        bool isRandomize() const {
            return this->randomize;
        }

        void setRandomize( bool value ) {
            this->randomize = value;
        }

        int getMaxReconnectAttempts() const {
            return this->maxReconnectAttempts;
        }

        void setMaxReconnectAttempts( int value ) {
            this->maxReconnectAttempts = value;
        }

        long long getReconnectDelay() const {
            return this->reconnectDelay;
        }

        void setReconnectDelay( long long value ) {
            this->reconnectDelay = value;
        }

        bool isBackup() const {
            return this->backup;
        }

        void setBackup( bool value ) {
            this->backup = value;
        }

        int getBackupPoolSize() const {
            return this->backupPoolSize;
        }

        void setBackupPoolSize( int value ) {
            this->backupPoolSize = value;
        }

        bool isTrackMessages() const {
            return this->trackMessages;
        }

        void setTrackMessages( bool value ) {
            this->trackMessages = value;
        }

        int getMaxCacheSize() const {
            return this->maxCacheSize;
        }

        void setMaxCacheSize( int value ) {
            this->maxCacheSize = value;
        }

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
        void restoreTransport( const Pointer<Transport>& transport )
            throw( decaf::io::IOException );

        /**
         * Called when this class' TransportListener is notified of a Failure.
         * @param error - The CMS Exception that was thrown.
         * @throw Exception if an error occurs.
         */
        void handleTransportFailure( const decaf::lang::Exception& error )
            throw( decaf::lang::Exception );

    private:

        /**
         * Returns a set of URIs that this Transport is to connect to, applying a
         * random swapping from the class stored list of URIs if the randomize flag
         * is enabled, otherwise just return the original list.
         *
         * @returns a Set of URI object that this Transport iterates over to connect.
         */
        decaf::util::StlList<URI> getConnectList() const;

        /**
         * @return Returns true if the command is one sent when a connection
         * is being closed.
         */
        bool isShutdownCommand( const Pointer<Command>& command ) const;

        /**
         * Performs the actual Reconnect operation.
         */
        bool doReconnect();

        /**
         * Builds a set of Backup Transports for fast Failover.
         */
        bool buildBackups();

        /**
         * Looks up the correct Factory and create a new Composite version of the
         * Transport requested.
         *
         * @param uri - The URI to connect to
         */
        Pointer<Transport> createTransport( const URI& location ) const
            throw ( decaf::io::IOException );

    };

}}}

#endif /* _ACTIVE_TRANSPORT_FAILOVER_FAILOVERTRANSPORT_H_ */
