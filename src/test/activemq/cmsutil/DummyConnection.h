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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYCONNECTION_H_
#define ACTIVEMQ_CMSUTIL_DUMMYCONNECTION_H_

#include <cms/Connection.h>
#include <activemq/cmsutil/DummySession.h>

namespace activemq {
namespace cmsutil {

    class MessageContext;

    class DummyConnection : public cms::Connection {
    private:
        
        cms::ExceptionListener* listener;
        std::string clientId;
        MessageContext* messageContext;
        
    public:

        DummyConnection(MessageContext* messageContext ) {
            this->messageContext = messageContext;
        }
        virtual ~DummyConnection() {}

        virtual void close() throw( cms::CMSException ) {            
        }
        
        virtual void start() throw( cms::CMSException ) {            
        }
        
        virtual void stop() throw( cms::CMSException ) {            
        }

        virtual cms::Session* createSession() throw ( cms::CMSException ) {
            return new DummySession(messageContext);
        }
                
        virtual cms::Session* createSession( cms::Session::AcknowledgeMode ackMode ) 
            throw ( cms::CMSException ) {
            
            DummySession* s = new DummySession(messageContext);
            s->setAcknowledgeMode(ackMode);
            return s;
        }

        virtual std::string getClientID() const {
            return clientId;
        }
        
        virtual void setClientID( const std::string& id ) {
            this->clientId = id;
        }

        virtual cms::ExceptionListener* getExceptionListener() const {
            return listener;
        }

        virtual void setExceptionListener( cms::ExceptionListener* listener ) {
            this->listener = listener;
        }
    };
    
}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYCONNECTION_H_*/
