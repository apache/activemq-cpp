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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYCONNECTIONFACTORY_H_
#define ACTIVEMQ_CMSUTIL_DUMMYCONNECTIONFACTORY_H_

#include <cms/ConnectionFactory.h>
#include <activemq/cmsutil/DummyConnection.h>
#include <activemq/cmsutil/MessageContext.h>

namespace activemq {
namespace cmsutil {

    class DummyConnectionFactory : public cms::ConnectionFactory {
    private:
        
        MessageContext messageContext;
        
    public:
        
        virtual ~DummyConnectionFactory() {}

        virtual cms::Connection* createConnection() throw ( cms::CMSException ) {
            
            return new DummyConnection(&messageContext);
        }

        virtual cms::Connection* createConnection( const std::string& username,
                                                   const std::string& password )
            throw ( cms::CMSException ) {
            
            return new DummyConnection(&messageContext);
        }

        virtual cms::Connection* createConnection( const std::string& username,
                                                   const std::string& password,
                                                   const std::string& clientId )
            throw ( cms::CMSException ) {
            
            DummyConnection* c = new DummyConnection(&messageContext);
            c->setClientID(clientId);
            
            return c;
        }
        
        MessageContext* getMessageContext() {
            return &messageContext;
        }
        
    };
    
}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYCONNECTIONFACTORY_H_*/
