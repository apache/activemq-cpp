/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_DATASTREAMMARSHALLER_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_DATASTREAMMARSHALLER_H_

#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/IOException.h>
#include <activemq/commands/DataStructure.h>
#include <activemq/wireformat/openwire/utils/BooleanStream.h>
#include <activemq/wireformat/openwire/OpenWireFormat.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace marshal{

    /**
     * Base class for all classes that marshal commands for Openwire
     */
    class AMQCPP_API DataStreamMarshaller {
    public:

        virtual ~DataStreamMarshaller() {}

        /**
         * Gets the DataStructureType that this class marshals/unmarshals
         * @returns byte Id of this classes DataStructureType
         */
        virtual unsigned char getDataStructureType() const = 0;

        /**
         * Creates a new instance of the class that this class is a marshaling
         * director for.
         * @returns newly allocated Command
         */
        virtual commands::DataStructure* createObject() const = 0;

        /**
         * Tight Marhsal to the given stream
         *
         * @param format
         *      The OpenwireFormat properties
         * @param command
         *      The object to Marshal
         * @param bs
         *      The boolean stream to marshal to.
         *
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshal1( OpenWireFormat* format,
                                   commands::DataStructure* command,
                                   utils::BooleanStream* bs ) = 0;

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Marshal
         * @param ds - the DataOutputStream to Marshal to
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshal2( OpenWireFormat* format,
                                    commands::DataStructure* command,
                                    decaf::io::DataOutputStream* ds,
                                    utils::BooleanStream* bs ) = 0;

        /**
         * Tight Un-marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Un-Marshal
         * @param dis - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @throws IOException if an error occurs.
         */
        virtual void tightUnmarshal( OpenWireFormat* format,
                                     commands::DataStructure* command,
                                     decaf::io::DataInputStream* dis,
                                     utils::BooleanStream* bs ) = 0;

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Marshal
         * @param ds - DataOutputStream to marshal to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshal( OpenWireFormat* format,
                                   commands::DataStructure* command,
                                   decaf::io::DataOutputStream* ds ) = 0;

        /**
         * Loose Un-marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Un-Marshal
         * @param dis - the DataInputStream to Un-Marshal from
         * @throws IOException if an error occurs.
         */
        virtual void looseUnmarshal( OpenWireFormat* format,
                                     commands::DataStructure* command,
                                     decaf::io::DataInputStream* dis ) = 0;

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_DATASTREAMMARSHALLER_H_*/
