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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBLOBMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBLOBMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQMessage.h>

namespace activemq {
namespace connector {
namespace openwire {
namespace commands {

    class ActiveMQBlobMessage : public ActiveMQMessage {

    public:

        const static unsigned char ID_ACTIVEMQBLOBMESSAGE = 29;

    public:

        ActiveMQBlobMessage();
        virtual ~ActiveMQBlobMessage() {}

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ActiveMQBlobMessage* cloneDataStructure() const {
            ActiveMQBlobMessage* message = new ActiveMQBlobMessage();
            message->copyDataStructure( this );
            return message;
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQMessage::copyDataStructure( src );
        }

    public:  // ActiveMQMessage

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const {
            return false;
        }

    };

}}}}

#endif /* _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBLOBMESSAGE_H_ */
