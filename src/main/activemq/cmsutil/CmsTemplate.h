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

#ifndef ACTIVEMQ_CMSUTIL_CMSTEMPLATE_H_
#define ACTIVEMQ_CMSUTIL_CMSTEMPLATE_H_

#include <activemq/cmsutil/CmsDestinationAccessor.h>
#include <activemq/cmsutil/SessionCallback.h>
#include <activemq/cmsutil/SessionPool.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <cms/ConnectionFactory.h>
#include <cms/DeliveryMode.h>
#include <string>

namespace activemq {
namespace cmsutil {

    // Forward declarations.
    class ProducerCallback;
    class MessageCreator;
    
    /**
     * <code>CmsTemplate</code> simplifies performing synchronous CMS 
     * operations.  This class is intended to be for CMS what Spring's 
     * <code>JmsTemplate</code> is for JMS.  Provided with a CMS 
     * <code>ConnectionFactory</code>, creates and manages all other 
     * CMS resources internally.
     * <p>
     * Before using <code>CmsTemplate</code> the user must first set
     * the destination (either by name or by setting the destiation 
     * object directly) and then call <code>init</code> to initialize
     * the object for use.
     * <p>
     * <code>CmsTemplate</code> allows the user to get access to a CMS
     * <code>Session</code> through a user-defined <code>SessionCallback</code>.
     * Similarly, if the user wants direct access to a CMS
     * <code>MessageProducer</code>, it can provide a 
     * <code>ProducerCallback</code>.  As a convenience, the user can bypass
     * having to provide callbacks altogether for sending messages, by calling
     * one of the <code>send</code> methods.
     * 
     * @see SessionCallback
     * @see ProducerCallback
     * @see MessageCreator
     */
    class CmsTemplate : public CmsDestinationAccessor
    {
    public:
    
        /**
         * Timeout value indicating that a receive operation should
         * check if a message is immediately available without blocking.
         */
        static const long long RECEIVE_TIMEOUT_NO_WAIT = -1;
    
        /**
         * Timeout value indicating a blocking receive without timeout.
         */
        static const long long RECEIVE_TIMEOUT_INDEFINITE_WAIT = 0;
    
        /**
         * Default message priority.
         */
        static const int DEFAULT_PRIORITY = 4;
    
        /**
         * My default, messages should live forever.
         */
        static const long long DEFAULT_TIME_TO_LIVE = 0;                
    
    public:
        
        /**
         * Session callback that executes a producer callback.
         */
        class ProducerSessionCallback;
        friend class ProducerSessionCallback;
        class ProducerSessionCallback : public SessionCallback {
        private:
            
            ProducerCallback* action;
            CmsTemplate* parent;
            
        public:
            
            ProducerSessionCallback(ProducerCallback* action,
                    CmsTemplate* parent){
                this->action = action;
                this->parent = parent;
            }
            
            virtual ~ProducerSessionCallback() {}
            
            virtual void doInCms(cms::Session* session) throw (cms::CMSException);
        };
        
        /**
         * Session callback that sends to the given destination.
         */
        class Sender;
        friend class Sender;
        class Sender : public SessionCallback {
        private:
            
            cms::Destination* dest;
            MessageCreator* messageCreator;
            CmsTemplate* parent;
            
        public:
            
            Sender(cms::Destination* dest, MessageCreator* messageCreator,
                    CmsTemplate* parent) {
                this->dest = dest;
                this->messageCreator = messageCreator;
                this->parent = parent;
            }
            
            virtual ~Sender() {}
            
            virtual void doInCms(cms::Session* session) throw (cms::CMSException) {
                parent->doSend(session, dest, messageCreator);
            }
        };
        
        /**
         * Session callback that sends a message to a named destination.
         */
        class ResolveSender;
        friend class ResolveSender;
        class ResolveSender : public SessionCallback {
        private:
                    
            std::string destinationName;
            MessageCreator* messageCreator;
            CmsTemplate* parent;
            
        public:
            
            ResolveSender(const std::string& destinationName, 
                    MessageCreator* messageCreator,
                    CmsTemplate* parent ) {
                this->destinationName = destinationName;
                this->messageCreator = messageCreator;
                this->parent = parent;
            }
            
            virtual ~ResolveSender() {}
            
            virtual void doInCms(cms::Session* session) throw (cms::CMSException);
        };
        
    private:
                
        static const int NUM_SESSION_POOLS = (int)cms::Session::SESSION_TRANSACTED + 1;
    
        cms::Connection* connection;
        
        SessionPool* sessionPools[NUM_SESSION_POOLS];
        
        cms::Destination* defaultDestination;
    
        std::string defaultDestinationName;
    
        bool messageIdEnabled;
    
        bool messageTimestampEnabled;
    
        bool noLocal;
    
        long long receiveTimeout;
    
        bool explicitQosEnabled;
    
        int deliveryMode;
    
        int priority;
    
        long long timeToLive;
    
    public:
    
        CmsTemplate();
        CmsTemplate(cms::ConnectionFactory* connectionFactory);
    
        virtual ~CmsTemplate();
    
        /**
         * Initializes this object and prepares it for use.  This should be called
         * before any other methds are called.
         */
        virtual void init()
        throw (cms::CMSException, decaf::lang::exceptions::IllegalStateException);
    
        /**
         * Sets the destination object to be used by default for send/receive operations.
         * If no default destination is provided, the <code>defaultDestinationName</code>
         * property is used to resolve this default destination for send/receive 
         * operations.
         * 
         * @param defaultDestination
         *          the default destination
         */
        virtual void setDefaultDestination(cms::Destination* defaultDestination) {
            this->defaultDestination = defaultDestination;
        }
    
        /**
         * Retrieves the default destination to be used for send/receive operations.
         * @return the default destination. Const version of this method.
         */
        virtual const cms::Destination* getDefaultDestination() const {
            return this->defaultDestination;
        }
        
        /**
         * Retrieves the default destination to be used for send/receive operations.
         * @return the default destination. Non-const version of this method.
         */
        virtual cms::Destination* getDefaultDestination() {
            return this->defaultDestination;
        }
    
        /**
         * Sets the name of the default destination to be used from send/receive operations.
         * Calling this method will set the <code>defaultDestination</code> property to NULL.
         * The destination type (topic/queue) is determined by the
         * <code>pubSubDomain</code> property.
         * 
         * @param defaultDestinationName
         *          the name of the destination for send/receive to by default.
         */
        virtual void setDefaultDestinationName(const std::string& defaultDestinationName) {
            if( defaultDestinationName != this->defaultDestinationName ) {
                this->defaultDestination = NULL;
                this->defaultDestinationName = defaultDestinationName;
            }
        }
    
        /**
         * Gets the name of the default destination to be used for send/receive operations.
         * The destination type (topic/queue) is determined by the
         * <code>pubSubDomain</code> property.
         * 
         * @return the default name of the destination for send/receive operations.
         */
        virtual const std::string getDefaultDestinationName() const {
            return this->defaultDestinationName;
        }
        
        /**
         * Indicates whether the default destination is a topic (true) or a queue (false).
         * Calling this method will set the <code>defaultDestination</code> property to NULL.
         * 
         * @param pubSubDomain
         *          indicates whether to use pub-sub messaging (topics).
         */
        virtual void setPubSubDomain( bool pubSubDomain ) {
            if( pubSubDomain != isPubSubDomain() ) {
                this->defaultDestination = NULL;
                CmsDestinationAccessor::setPubSubDomain(pubSubDomain);                
            }
        }
    
        virtual void setMessageIdEnabled(bool messageIdEnabled) {
            this->messageIdEnabled = messageIdEnabled;
        }
    
        virtual bool isMessageIdEnabled() const {
            return this->messageIdEnabled;
        }
    
        virtual void setMessageTimestampEnabled(bool messageTimestampEnabled) {
            this->messageTimestampEnabled = messageTimestampEnabled;
        }
    
        virtual bool isMessageTimestampEnabled() const {
            return this->messageTimestampEnabled;
        }
    
        virtual void setNoLocal(bool noLocal) {
            this->noLocal = noLocal;
        }
    
        virtual bool isNoLocal() const {
            return this->noLocal;
        }
    
        virtual void setReceiveTimeout(long long receiveTimeout) {
            this->receiveTimeout = receiveTimeout;
        }
    
        virtual long long getReceiveTimeout() const {
            return this->receiveTimeout;
        }
    
        /**
         * Set if the QOS values (deliveryMode, priority, timeToLive)
         * should be used for sending a message.
         * 
         * @see #setDeliveryMode
         * @see #setPriority
         * @see #setTimeToLive
         */
        virtual void setExplicitQosEnabled(bool explicitQosEnabled) {
            this->explicitQosEnabled = explicitQosEnabled;
        }
    
        /**
         * If "true", then the values of deliveryMode, priority, and timeToLive
         * will be used when sending a message. Otherwise, the default values,
         * that may be set administratively, will be used.
         * 
         * @return true if overriding default values of QOS parameters
         * (deliveryMode, priority, and timeToLive)
         * 
         * @see #setDeliveryMode
         * @see #setPriority
         * @see #setTimeToLive
         */
        virtual bool isExplicitQosEnabled() const {
            return this->explicitQosEnabled;
        }
    
        /**
         * Set whether message delivery should be persistent or non-persistent,
         * specified as boolean value ("true" or "false"). This will set the delivery
         * mode accordingly, to either "PERSISTENT" or "NON_PERSISTENT".
         * <p>Default it "true" aka delivery mode "PERSISTENT".
         * 
         * @see #setDeliveryMode(int)
         */
        virtual void setDeliveryPersistent(bool deliveryPersistent) {
            this->deliveryMode = (deliveryPersistent ? cms::DeliveryMode::PERSISTENT : cms::DeliveryMode::NON_PERSISTENT);
        }
    
        /**
         * Set the delivery mode to use when sending a message.
         * Default is the Message default: "PERSISTENT".
         * <p>Since a default value may be defined administratively,
         * this is only used when "isExplicitQosEnabled" equals "true".
         * @param deliveryMode the delivery mode to use
         * @see #isExplicitQosEnabled
         */
        virtual void setDeliveryMode(int deliveryMode) {
            this->deliveryMode = deliveryMode;
        }
    
        /**
         * Return the delivery mode to use when sending a message.
         */
        virtual int getDeliveryMode() const {
            return this->deliveryMode;
        }
    
        /**
         * Set the priority of a message when sending.
         * <p>Since a default value may be defined administratively,
         * this is only used when "isExplicitQosEnabled" equals "true".
         * 
         * @see #isExplicitQosEnabled
         */
        virtual void setPriority(int priority) {
            this->priority = priority;
        }
    
        /**
         * Return the priority of a message when sending.
         */
        virtual int getPriority() const {
            return this->priority;
        }
    
        /**
         * Set the time-to-live of the message when sending.
         * <p>Since a default value may be defined administratively,
         * this is only used when "isExplicitQosEnabled" equals "true".
         * @param timeToLive the message's lifetime (in milliseconds)
         * 
         * @see #isExplicitQosEnabled
         */
        virtual void setTimeToLive(long long timeToLive) {
            this->timeToLive = timeToLive;
        }
    
        /**
         * Return the time-to-live of the message when sending.
         */
        virtual long long getTimeToLive() const {
            return this->timeToLive;
        }
        
        /**
         * Executes the given action within a CMS Session.
         * @param action
         *          the action to perform within a CMS Session
         * @throws cms::CMSException thrown if an error occurs.
         */
        virtual void execute(SessionCallback* action) throw (cms::CMSException);
        
        /**
         * Executes the given action and provides it with a CMS Session and 
         * producer
         * 
         * @param action
         *          the action to perform
         * @throws cms::CMSException thrown if an error occurs.
         */
        virtual void execute(ProducerCallback* action) throw (cms::CMSException);
    
        /**
         * Convenience method for sending a message to the default destination.
         * 
         * @param messageCreator
         *          Responsible for creating the message to be sent
         * @throws cms::CMSException thrown if an error occurs.
         * @throws decaf::lang::exceptions::IllegalStateException thrown if the
         *          default destination has not been specified.
         */
        virtual void send(MessageCreator* messageCreator) 
        throw (cms::CMSException, decaf::lang::exceptions::IllegalStateException);

        /**
         * Convenience method for sending a message to the specified destination.
         * 
         * @param dest
         *          The destination to send to
         * @param messageCreator
         *          Responsible for creating the message to be sent
         * @throws cms::CMSException thrown if an error occurs.
         * @throws decaf::lang::exceptions::IllegalStateException thrown if the
         *          default destination has not been specified.
         */
        virtual void send(cms::Destination* dest, 
                MessageCreator* messageCreator)
        throw (cms::CMSException, decaf::lang::exceptions::IllegalStateException);
        
        /**
         * Convenience method for sending a message to the specified destination.
         * 
         * @param destinationName
         *          The name of the destination to send to.
         * @param messageCreator
         *          Responsible for creating the message to be sent
         * @throws cms::CMSException thrown if an error occurs.
         * @throws decaf::lang::exceptions::IllegalStateException thrown if the
         *          default destination has not been specified.
         */
        virtual void send(const std::string& destinationName, 
                MessageCreator* messageCreator)
        throw (cms::CMSException, decaf::lang::exceptions::IllegalStateException);
        
    private:
    
        /**
         * Initializes all members to their defaults.
         */
        void initDefaults();
        
        /**
         * Creates the session pools objects.
         */
        void createSessionPools();
        
        /**
         * Destroys the session pool objects.
         */
        void destroySessionPools();
    
        /**
         * Checks that the default destination is valid, if not throws
         * an exception.
         * @throws decaf::lang::exceptions::IllegalStateException thrown
         * if the default destination is invalid.
         */
        void checkDefaultDestination()
        throw (decaf::lang::exceptions::IllegalStateException);
    
        /**
         * Gets the connection, creating it if it doesn't already exist.
         * @return the connection
         * @throws cms::CMSException if any of the CMS methods throw.
         */
        cms::Connection* getConnection() throw (cms::CMSException);
        
        /**
         * Creates a session initialized with the proper values.
         * 
         * @return the session
         * @throws cms::CMSException if any of the CMS methods throw.
         */
        PooledSession* takeSession() throw (cms::CMSException);
    
        /**
         * Closes, but does not destroy the pooled session resource.
         * @aaram session
         *          a pooled session resource
         * @throws cms::CMSException thrown if the CMS methods throw.
         */
        void returnSession( PooledSession*& session ) throw (cms::CMSException);
    
        /**
         * Allocates a producer initialized with the proper values.
         * 
         * @param session
         *          The session from which to create a producer
         * @param dest
         *          The destination for which to create the producer.  If
         *          this is NULL, the default will be used.
         * @return the producer
         * @throws cms::CMSException thrown by the CMS API
         */
        cms::MessageProducer* createProducer(cms::Session* session,
                cms::Destination* dest) throw (cms::CMSException);
        
        /**
         * Closes and destroys a producer resource
         * @aaram producer
         *          a producer to destroy
         * @throws cms::CMSException thrown if the CMS methods throw.
         */
        void destroyProducer( cms::MessageProducer*& producer ) throw (cms::CMSException);
    
        /**
         * Allocates a consumer initialized with the proper values.
         * 
         * @param session
         *          The session from which to create a consumer
         * @param dest
         *          The destination for which to create the consumer
         * @param messageSelector
         *          The message selector for the consumer.
         * @return the new consumer
         * @throws cms::CMSException if the CMS methods throw.
         */
        cms::MessageConsumer* createConsumer(cms::Session* session,
                cms::Destination* dest, const std::string& messageSelector)
        throw (cms::CMSException);
        
        /**
         * Closes and destroys a consumer resource
         * @aaram consumer
         *          a consumer to destroy
         * @throws cms::CMSException thrown if the CMS methods throw.
         */
        void destroyConsumer( cms::MessageConsumer*& consumer ) throw (cms::CMSException);
        
        /**
         * Destroys the given message
         * @param message   
         *          the message to destroy
         */
        void destroyMessage( cms::Message*& message );
        
        /**
         * Sends a message to a destination.
         * @param session
         *          the session to be used.
         * @param dest
         *          the destination to send the message on.
         * @param messageCreator
         *          creates the message to be sent
         * @throws cms::CMSException thrown if the CMS API throws.
         */
        void doSend(cms::Session* session, cms::Destination* dest, 
                MessageCreator* messageCreator) throw (cms::CMSException);
        
        /**
         * Resolves the default destination and returns it.
         * @param session
         *          the parent session.
         * @return the default destination
         * @throws cms::CMSException if an error occurs
         * @throws decaf::lang::exceptions::IllegalStateException thrown if
         *          no default destination was provided.
         */
        cms::Destination* resolveDefaultDestination(cms::Session* session)
        throw (cms::CMSException, decaf::lang::exceptions::IllegalStateException);
    };

}}

#endif /*ACTIVEMQ_CMSUTIL_CMSTEMPLATE_H_*/
