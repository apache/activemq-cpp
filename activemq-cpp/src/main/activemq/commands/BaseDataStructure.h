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

#ifndef _ACTIVEMQ_COMMANDS_BASEDATASTRUCTURE_H_
#define _ACTIVEMQ_COMMANDS_BASEDATASTRUCTURE_H_

#include <activemq/util/Config.h>
#include <activemq/commands/DataStructure.h>

#include <string>
#include <sstream>

namespace activemq{
namespace wireformat{
    class WireFormat;
}
namespace commands{

    class AMQCPP_API BaseDataStructure : public DataStructure {
    public:

        virtual ~BaseDataStructure() {}

        virtual bool isMarshalAware() const {
            return false;
        }

        virtual void beforeMarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED ) {}

        virtual void afterMarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED ) {}

        virtual void beforeUnmarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED ) {}

        virtual void afterUnmarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED ) {}

        virtual void setMarshaledForm( wireformat::WireFormat* wireFormat AMQCPP_UNUSED,
                                       const std::vector<char>& data AMQCPP_UNUSED ) {}

        virtual std::vector<unsigned char> getMarshaledForm(
            wireformat::WireFormat* wireFormat AMQCPP_UNUSED ) {

            return std::vector<unsigned char>();
        }

        virtual void copyDataStructure( const DataStructure* src AMQCPP_UNUSED ) {
            // Nothing to do here
        }

        virtual std::string toString() const {
            return std::string( "No Data for Class BaseDataStructure\n" );
        }

        virtual bool equals( const DataStructure* value AMQCPP_UNUSED ) const {
            return true;
        }

   };

}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_*/
