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
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <cms/ConnectionFactory.h>
#include <cms/DeliveryMode.h>
#include <string>

namespace activemq {
namespace cmsutil {

    // Forward declarations.
    class SessionCallback;
    
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
    
    private:
        
        /**
         * Session callback that executes a producer callback.
         */
        class ProducerSessionCallback : public SessionCallback {
        private:
            
            ProducerCallback* action;
            
        public:
            
            ProducerSessionCallback(PoducerCallback* action){
                this->action = action;
            }
            
            virtual ~ProducerSessionCallback() {}
            
            virtual void doInCms(cms::Session* session) throw (cms::CMSException);
        };
                
        static const int NUM_SESSION_POOLS = (int)cms::Session::SESSION_TRANSACTED + 1;
    
        cms::Connection* connection;
        
        SessionPool*[NUM_SESSION_POOLS] sessionPools;
        
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
    
        virtual void setDefaultDestination(cms::Destination* defaultDestination) {
            this->defaultDestination = defaultDestination;
        }
    
        virtual const cms::Destination* getDefaultDestination() const {
            return this->defaultDestination;
        }
    
        virtual void setDefaultDestinationName(const std::string& defaultDestinationName) {
            this->defaultDestinationName = defaultDestinationName;
        }
    
        virtual const std::string getDefaultDestinationName() const {
            return this->defaultDestinationName;
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
            this->deliveryMode = (deliveryPersistent ? cms::DeliveryMode.PERSISTENT : cms::DeliveryMode.NON_PERSISTENT);
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
            this.deliveryMode = deliveryMode;
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
        virtual public void setPriority(int priority) {
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
    };

}}

#endif /*ACTIVEMQ_CMSUTIL_CMSTEMPLATE_H_*/
