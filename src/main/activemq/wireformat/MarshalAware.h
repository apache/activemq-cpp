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

#ifndef _ACTIVEMQ_WIREFORMAT_MARSHALAWARE_H_
#define _ACTIVEMQ_WIREFORMAT_MARSHALAWARE_H_

#include <vector>
#include <decaf/io/IOException.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace wireformat{

    class WireFormat;

    class AMQCPP_API MarshalAware {
    public:

        virtual ~MarshalAware() {}

        /**
         * Determine if the class implementing this interface is really
         * wanting to be told about marshaling.  Normally if you didn't
         * want to be marshal aware you just wouldn't implement this interface
         * but since this is C++ and we don't have true interfaces we need
         * a flat inheritance hierarchy, so we always implement this.
         *
         * @returns true if this class cares about marshaling.
         */
        virtual bool isMarshalAware() const = 0;

        /**
         * Called before marshaling is started to prepare the object to be
         * marshaled.
         *
         * @param wireFormat
         *      The wireformat object to control marshaling
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void beforeMarshal( WireFormat* wireFormat ) = 0;

        /**
         * Called after marshaling is started to cleanup the object being
         * marshaled.
         *
         * @param wireFormat
         *      The wireformat object to control marshaling
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void afterMarshal( WireFormat* wireFormat ) = 0;

        /**
         * Called before unmarshaling is started to prepare the object to be
         * unmarshaled.
         *
         * @param wireFormat
         *      The wireformat object to control marshaling
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void beforeUnmarshal( WireFormat* wireFormat ) = 0;

        /**
         * Called after unmarshaling is started to cleanup the object being
         * unmarshaled.
         *
         * @param wireFormat
         *      The wireformat object to control marshaling
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void afterUnmarshal( WireFormat* wireFormat ) = 0;

        /**
         * Called to set the data to this object that will contain the objects
         * marshaled form.
         * @param wireFormat - the wireformat object to control unmarshaling
         * @param data - vector of object binary data
         *
         * @param wireFormat
         *      The wireformat object to control marshaling
         * @param data
         *      A vector of bytes that contains the object in marshaled form.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void setMarshaledForm( WireFormat* wireFormat, const std::vector<char>& data ) = 0;

        /**
         * Called to get the data to this object that will contain the objects
         * marshaled form.
         *
         * @param wireFormat
         *      The wireformat object to control unmarshaling
         *
         * @return buffer that holds the objects data.
         */
        virtual std::vector<unsigned char> getMarshaledForm( WireFormat* wireFormat ) = 0;

    };

}}

#endif /*_ACTIVEMQ_WIREFORMAT_MARSHALAWARE_H_*/
