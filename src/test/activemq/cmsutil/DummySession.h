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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYSESSION_H_
#define ACTIVEMQ_CMSUTIL_DUMMYSESSION_H_

#include <cms/Session.h>
#include <activemq/cmsutil/DummyProducer.h>
#include <activemq/cmsutil/DummyConsumer.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTopic.h>

namespace activemq {
namespace cmsutil {

    class MessageContext;

    class DummySession : public cms::Session {

    private:

        cms::Session::AcknowledgeMode mode;
        MessageContext* messageContext;

    public:

        DummySession(MessageContext* messageContext) {
            this->mode = cms::Session::AUTO_ACKNOWLEDGE;
            this->messageContext = messageContext;
        }

        virtual ~DummySession() throw() {}

        virtual void close() {}

        virtual void start() {}

        virtual void stop() {}

        virtual void commit() {}

        virtual void rollback() {}

        virtual void recover() {}

        virtual cms::MessageConsumer* createConsumer(
            const cms::Destination* destination )
                throw ( cms::CMSException ) {

            return new DummyConsumer(messageContext, destination, "", false);
        }

        virtual cms::MessageConsumer* createConsumer(
            const cms::Destination* destination,
            const std::string& selector )
                throw ( cms::CMSException ) {
            return new DummyConsumer(messageContext, destination, selector, false);
        }

        virtual cms::MessageConsumer* createConsumer(
            const cms::Destination* destination,
            const std::string& selector,
            bool noLocal )
                throw ( cms::CMSException ) {
            return new DummyConsumer(messageContext, destination, selector, noLocal);
        }

        virtual cms::MessageConsumer* createDurableConsumer(
            const cms::Topic* destination,
            const std::string& name,
            const std::string& selector,
            bool noLocal = false )
                throw ( cms::CMSException ) { return NULL; }

        virtual cms::MessageProducer* createProducer( const cms::Destination* destination )
            throw ( cms::CMSException ) { return new DummyProducer(messageContext, destination); }

        virtual cms::QueueBrowser* createBrowser( const cms::Queue* queue )
            throw( cms::CMSException ) { return NULL; }

        virtual cms::QueueBrowser* createBrowser( const cms::Queue* queue, const std::string& selector )
            throw( cms::CMSException ) { return NULL; }

        virtual cms::Queue* createQueue( const std::string& queueName )
            throw ( cms::CMSException ) {
            return new activemq::commands::ActiveMQQueue( queueName );
        }

        virtual cms::Topic* createTopic( const std::string& topicName )
            throw ( cms::CMSException ) {
            return new activemq::commands::ActiveMQTopic( topicName );
        }

        virtual cms::TemporaryQueue* createTemporaryQueue()
            throw ( cms::CMSException ) { return NULL; }

        virtual cms::TemporaryTopic* createTemporaryTopic()
            throw ( cms::CMSException ){ return NULL; }

        virtual cms::Message* createMessage()
            throw ( cms::CMSException ){ return NULL; }

        virtual cms::BytesMessage* createBytesMessage()
            throw ( cms::CMSException){ return NULL; }

        virtual cms::BytesMessage* createBytesMessage(
            const unsigned char* bytes, int bytesSize ) throw ( cms::CMSException){
            return NULL;
        }

        virtual cms::StreamMessage* createStreamMessage()
            throw ( cms::CMSException ){ return NULL; }

        virtual cms::TextMessage* createTextMessage()
            throw ( cms::CMSException ){ return NULL; }

        virtual cms::TextMessage* createTextMessage( const std::string& text )
            throw ( cms::CMSException ){ return NULL; }

        virtual cms::MapMessage* createMapMessage()
            throw ( cms::CMSException ){ return NULL; }

        virtual cms::Session::AcknowledgeMode getAcknowledgeMode() const throw ( cms::CMSException ) {
            return mode;
        }

        virtual void setAcknowledgeMode(cms::Session::AcknowledgeMode mode) {
            this->mode = mode;
        }

        virtual bool isTransacted() const throw ( cms::CMSException ){
            return mode==cms::Session::SESSION_TRANSACTED;
        }

        virtual void unsubscribe( const std::string& name )
            throw ( cms::CMSException ){}

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYSESSION_H_*/
