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

#ifndef ACTIVEMQ_CORE_DISPATCHDATA_H_
#define ACTIVEMQ_CORE_DISPATCHDATA_H_

#include <stdlib.h>
#include <activemq/util/Config.h>

namespace activemq {
    
    namespace connector {
        class ConsumerInfo;
    }
    
namespace core {

    class ActiveMQMessage;
    
    /**
     * Contains information about dispatching to a particular consumer.
     */
    class AMQCPP_API DispatchData {
    private:
    
        connector::ConsumerInfo* consumer;
        ActiveMQMessage* message;
        
    public:
    
        DispatchData(){
            consumer = NULL;
            message = NULL;
        }
        
        DispatchData( connector::ConsumerInfo* consumer, ActiveMQMessage* message ) {
            this->consumer = consumer;
            this->message = message;
        }
        
        DispatchData( const DispatchData& d ) {
            (*this) = d;
        }
        
        DispatchData& operator =( const DispatchData& d ) {
            this->consumer = d.consumer;
            this->message = d.message;
            return *this;
        }
        
        connector::ConsumerInfo* getConsumer() {
            return consumer;
        }
        
        ActiveMQMessage* getMessage() {
            return message;
        }
          
    };    
}}

#endif /*ACTIVEMQ_CORE_DISPATCHDATA_H_*/
