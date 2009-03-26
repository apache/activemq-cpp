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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_OPENWIRESTRINGSUPPORT_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_OPENWIRESTRINGSUPPORT_H_

#include <string>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace utils{

    class AMQCPP_API OpenwireStringSupport {
    protected:

        OpenwireStringSupport() {}
        virtual ~OpenwireStringSupport() {}

    public:

        /**
         * Static method used for reading a string that uses the Openwire format
         * from a DataInputStream, this can throw an IOException for the same
         * reason as a DataInputStream.readUTF might, as well as if the string
         * that is received doesn't conform to the supported character encoding.
         * @param dataIn - DataInputStream to read from
         * @returns A string that has been read
         * @throws IOException on Error.
         */
        static std::string readString( decaf::io::DataInputStream& dataIn )
            throw ( decaf::io::IOException );

        /**
         * Static method used for writing a string that uses the Openwire format
         * from a DataOutputStream, this can throw an IOException for the same
         * reason as a DataOutputStream.writeUTF might.
         * @param dataOut - DataOutputStream to write to
         * @param str - A pointer to a string that should be written, NULL needs
         *              to be an option here as its written as -1.
         * @throws IOException on Error.
         */
        static void writeString( decaf::io::DataOutputStream& dataOut,
                                 const std::string* str )
            throw ( decaf::io::IOException );

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_OPENWIRESTRINGSUPPORT_H_*/
