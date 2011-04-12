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

#ifndef ACTIVEMQ_CMSUTIL_POOLEDSESSION_H_
#define ACTIVEMQ_CMSUTIL_POOLEDSESSION_H_

#include <cms/Session.h>
#include <decaf/util/StlMap.h>
#include <activemq/cmsutil/CachedProducer.h>
#include <activemq/cmsutil/CachedConsumer.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    // Forward declarations.
    class SessionPool;

    /**
     * A pooled session object that wraps around a delegate session.  Calls
     * to close this session only result in giving the session back to the pool.
     */
    class AMQCPP_API PooledSession : public cms::Session {
    private:

        SessionPool* pool;

        cms::Session* session;

        decaf::util::StlMap<std::string, CachedProducer*> producerCache;

        decaf::util::StlMap<std::string, CachedConsumer*> consumerCache;

    private:

        PooledSession( const PooledSession& );
        PooledSession& operator= ( const PooledSession& );

    public:

        PooledSession( SessionPool* pool, cms::Session* session );

        /**
         * Does nothing
         */
        virtual ~PooledSession() throw();

        /**
         * Returns a non-constant reference to the internal session object.
         *
         * @return the session object.
         */
        virtual cms::Session* getSession() {
            return session;
        }

        /**
         * Returns a constant reference to the internal session object.
         *
         * @return the session object.
         */
        virtual const cms::Session* getSession() const {
            return session;
        }

        /**
         * Returns this session back to the pool, but does not close
         * or destroy the internal session object.
         *
         * @throws CMSException if an error occurs while performing this operation.
         */
        virtual void close();

        virtual void commit() {
            session->commit();
        }

        virtual void rollback() {
            session->rollback();
        }

        virtual void recover() {
            session->recover();
        }

        virtual cms::MessageConsumer* createConsumer( const cms::Destination* destination ) {
            return session->createConsumer(destination);
        }

        virtual cms::MessageConsumer* createConsumer( const cms::Destination* destination,
                                                      const std::string& selector ) {
            return session->createConsumer(destination, selector);
        }

        virtual cms::MessageConsumer* createConsumer( const cms::Destination* destination,
                                                      const std::string& selector,
                                                      bool noLocal ) {
            return session->createConsumer(destination, selector, noLocal);
        }

        virtual cms::MessageConsumer* createDurableConsumer( const cms::Topic* destination,
                                                             const std::string& name,
                                                             const std::string& selector,
                                                             bool noLocal = false ) {
            return session->createDurableConsumer(destination, name, selector, noLocal);
        }

        /**
         * First checks the internal consumer cache and creates on if none exist
         * for the given destination, selector, noLocal.  If created, the consumer is
         * added to the pool's lifecycle manager.
         *
         * @param destination
         *          the destination to receive on
         * @param selector
         *          the selector to use
         * @param noLocal
         *          whether or not to receive messages from the same connection
         *
         * @return the consumer resource
         *
         * @throws cms::CMSException if something goes wrong.
         */
        virtual cms::MessageConsumer* createCachedConsumer( const cms::Destination* destination,
                                                            const std::string& selector,
                                                            bool noLocal );

        virtual cms::MessageProducer* createProducer( const cms::Destination* destination ) {
            return session->createProducer(destination);
        }

        /**
         * First checks the internal producer cache and creates one if none exist
         * for the given destination.  If created, the producer is added to the
         * pool's lifecycle manager.
         *
         * @param destination
         *          the destination to send on
         *
         * @return the producer resource
         *
         * @throws cms::CMSException if something goes wrong.
         */
        virtual cms::MessageProducer* createCachedProducer( const cms::Destination* destination );

        virtual cms::QueueBrowser* createBrowser( const cms::Queue* queue );

        virtual cms::QueueBrowser* createBrowser( const cms::Queue* queue, const std::string& selector );

        virtual cms::Queue* createQueue( const std::string& queueName ) {
            return session->createQueue( queueName );
        }

        virtual cms::Topic* createTopic( const std::string& topicName ) {
            return session->createTopic( topicName );
        }

        virtual cms::TemporaryQueue* createTemporaryQueue() {
            return session->createTemporaryQueue();
        }

        virtual cms::TemporaryTopic* createTemporaryTopic() {
            return session->createTemporaryTopic();
        }

        virtual cms::Message* createMessage() {
            return session->createMessage();
        }

        virtual cms::BytesMessage* createBytesMessage() {
            return session->createBytesMessage();
        }

        virtual cms::BytesMessage* createBytesMessage( const unsigned char* bytes, int bytesSize ) {
            return session->createBytesMessage( bytes, bytesSize );
        }

        virtual cms::StreamMessage* createStreamMessage() {
            return session->createStreamMessage();
        }

        virtual cms::TextMessage* createTextMessage() {
            return session->createTextMessage();
        }

        virtual cms::TextMessage* createTextMessage( const std::string& text ) {
            return session->createTextMessage( text );
        }

        virtual cms::MapMessage* createMapMessage() {
            return session->createMapMessage();
        }

        virtual cms::Session::AcknowledgeMode getAcknowledgeMode() const {
            return session->getAcknowledgeMode();
        }

        virtual bool isTransacted() const {
            return session->isTransacted();
        }

        virtual void unsubscribe( const std::string& name ) {
            session->unsubscribe( name );
        }

    private:

        std::string getUniqueDestName( const cms::Destination* dest );

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_POOLEDSESSION_H_*/
