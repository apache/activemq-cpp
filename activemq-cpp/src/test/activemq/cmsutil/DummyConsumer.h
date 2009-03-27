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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYCONSUMER_H_
#define ACTIVEMQ_CMSUTIL_DUMMYCONSUMER_H_

#include <cms/MessageConsumer.h>
#include <activemq/cmsutil/MessageContext.h>

namespace activemq {
namespace cmsutil {

    class DummyConsumer : public cms::MessageConsumer {  
    private:
        std::string selector;
        cms::MessageListener* listener;
        MessageContext* messageContext;
        const cms::Destination* dest;
        bool noLocal;
        
    public:

        DummyConsumer(MessageContext* messageContext, const cms::Destination* dest, const std::string& selector, bool noLocal) {
            this->messageContext = messageContext;
            this->selector = selector;
            this->noLocal = noLocal;
            this->dest = dest;
            this->listener = NULL;
        }
        virtual ~DummyConsumer() {}
              
        virtual void close() throw( cms::CMSException ){}

        virtual cms::Message* receive() throw ( cms::CMSException ) {
            return messageContext->receive(dest, selector, noLocal, 0);
        }

        virtual cms::Message* receive( int millisecs ) throw ( cms::CMSException ) {
            return messageContext->receive(dest, selector, noLocal, millisecs);
        }

        virtual cms::Message* receiveNoWait() throw ( cms::CMSException ) {
            return messageContext->receive(dest, selector, noLocal, -1);
        }

        virtual void setMessageListener( cms::MessageListener* listener ) {
            this->listener = listener;
        }

        virtual cms::MessageListener* getMessageListener() const {
            return listener;
        }

        virtual std::string getMessageSelector() const 
            throw ( cms::CMSException ) {
            return selector;
        }
        
    };
    
}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYCONSUMER_H_*/
