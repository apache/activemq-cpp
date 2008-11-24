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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_

#include <activemq/connector/openwire/commands/DataStructure.h>
#include <activemq/util/Config.h>

#include <string>
#include <sstream>

namespace activemq{
namespace connector{
namespace openwire{

    // Forward Declare for now
    class OpenWireFormat;

namespace commands{

    class AMQCPP_API BaseDataStructure : public DataStructure {
    public:

        virtual ~BaseDataStructure() {}

        /**
         * Determine if this object is aware of marshaling and should have
         * its before and after marshaling methods called.  Defaults to false.
         * @returns true if aware of marshaling
         */
        virtual bool isMarshalAware() const {
            return false;
        }

        /**
         * Perform any processing needed before an marshal
         * @param wireformat - the OpenWireFormat object in use.
         */
        virtual void beforeMarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED)
            throw ( decaf::io::IOException ) {}

        /**
         * Perform any processing needed after an unmarshal
         * @param wireformat - the OpenWireFormat object in use.
         */
        virtual void afterMarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED)
            throw ( decaf::io::IOException ) {}

        /**
         * Perform any processing needed before an unmarshal
         * @param wireformat - the OpenWireFormat object in use.
         */
        virtual void beforeUnmarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED)
            throw ( decaf::io::IOException ) {}

        /**
         * Perform any processing needed after an unmarshal
         * @param wireformat - the OpenWireFormat object in use.
         */
        virtual void afterUnmarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED)
            throw ( decaf::io::IOException ) {}

        /**
         * Called to set the data to this object that will contain the objects
         * marshaled form.
         * @param wireFormat - the wireformat object to control unmarshaling
         * @param data - vector of object binary data
         */
        virtual void setMarshaledForm( OpenWireFormat* wireFormat AMQCPP_UNUSED,
                                        const std::vector<char>& data AMQCPP_UNUSED) {}

        /**
         * Called to get the data to this object that will contain the objects
         * marshaled form.
         * @param wireFormat - the wireformat object to control unmarshaling
         * @return buffer that holds the objects data.
         */
        virtual std::vector<unsigned char> getMarshaledForm( OpenWireFormat* wireFormat AMQCPP_UNUSED) {
            return std::vector<unsigned char>();
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src AMQCPP_UNUSED ) {
            // Nothing to do here
        }

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const {
            return std::string("No Data for Class BaseDataStructure\n");
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value AMQCPP_UNUSED ) const {
            return true;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_*/
