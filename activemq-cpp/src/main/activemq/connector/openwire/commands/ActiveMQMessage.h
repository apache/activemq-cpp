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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMESSAGE_H_

#include <activemq/connector/openwire/commands/Message.h>
#include <activemq/core/ActiveMQMessage.h>
#include <activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/util/Date.h>
#include <activemq/util/PrimitiveMap.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQMessage : 
        public Message, 
        public core::ActiveMQMessage
    {
    public:

        static const unsigned char ID_ACTIVEMQMESSAGE = 23; 

    public:

        ActiveMQMessage();
        virtual ~ActiveMQMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQMessage::copyDataStructure( src );
        }

        /**
         * Indicates that this command is aware of Marshalling, and needs
         * to have its Marshalling methods invoked.
         * @returns boolean indicating desire to be in marshalling stages
         */
        virtual bool IsMarshallAware() {
            return true;
        }
        
        /**
         * Handles the marshalling of the objects properties into the 
         * internal byte array before the object is marshalled to the
         * wire
         * @param wireFormat - the wireformatting controller
         */
        virtual void beforeMarshall( OpenWireFormat* wireFormat );

    public:   // core::ActiveMQMessage
    
        /**
         * Sets the Acknowledgement Handler that this Message will use
         * when the Acknowledge method is called.
         * @param handler ActiveMQAckHandler to call
         */
        virtual void setAckHandler( core::ActiveMQAckHandler* handler ) {
            this->ackHandler = handler;
        }
        
        /**
         * Gets the Acknowledgement Handler that this Message will use
         * when the Acknowledge method is called.
         * @returns handler ActiveMQAckHandler to call or NULL if not set
         */
        virtual core::ActiveMQAckHandler* getAckHandler() const {
            return this->ackHandler;
        }

        /**
         * Gets the number of times this message has been redelivered.
         * @return redelivery count
         */
        virtual int getRedeliveryCount(void) const {
            return redeliveryCount;
        }
        
        /**
         * Sets the count of the number of times this message has been 
         * redelivered
         * @param count the redelivery count
         */
        virtual void setRedeliveryCount( int count ) {
            this->redeliveryCount = count;
        }

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const {
            return false;
        }
      
    private:
   
        core::ActiveMQAckHandler* ackHandler;
        int redeliveryCount;
        util::PrimitiveMap properties;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMESSAGE_H_*/
