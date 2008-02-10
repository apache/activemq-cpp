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

#ifndef ACTIVEMQ_CMSUTIL_MESSAGECONTEXT_H_
#define ACTIVEMQ_CMSUTIL_MESSAGECONTEXT_H_

#include <cms/Destination.h>
#include <cms/Message.h>

namespace activemq {
namespace cmsutil {

    class MessageContext {
        
    public:
            
        class SendListener {
        public:
            
            virtual ~SendListener(){}
            
            virtual void onSend(const cms::Destination* destination,
                cms::Message* message, int deliveryMode, int priority, 
                long long timeToLive) throw (cms::CMSException)= 0;
            
            virtual cms::Message* doReceive(const cms::Destination* dest, 
                    const std::string& selector, 
                    bool noLocal, 
                    long long timeout) throw (cms::CMSException) = 0;
        };
        
    private:
        
        SendListener* listener;
        
    public:
        
        MessageContext() {
            listener = NULL;
        }
        virtual ~MessageContext(){}
            
        void setSendListener(SendListener* listener) {
            this->listener = listener;
        }
        
        void send(const cms::Destination* destination,
            cms::Message* message, int deliveryMode, int priority, 
            long long timeToLive) throw (cms::CMSException){
            
            if( listener != NULL ) {
                listener->onSend(destination, message, deliveryMode, priority, timeToLive);
            }
        }
        
        cms::Message* receive(const cms::Destination* dest, 
                const std::string& selector, 
                bool noLocal, 
                long long timeout) throw (cms::CMSException){
            
            if( listener != NULL ) {
                return listener->doReceive(dest, selector, noLocal, timeout);
            }
            
            return NULL;
        }
    };
}}

#endif /*ACTIVEMQ_CMSUTIL_MESSAGECONTEXT_H_*/
