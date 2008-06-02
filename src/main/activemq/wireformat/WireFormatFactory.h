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

#ifndef _ACTIVEMQ_WIREFORMAT_WIREFORMATFACTORY_H_
#define _ACTIVEMQ_WIREFORMAT_WIREFORMATFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/wireformat/WireFormat.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

namespace activemq{
namespace wireformat{

    /**
     * The WireFormatFactory is the interface that all WireFormatFactory
     * classes must extend.  The Factory creates a WireFormat Object based on
     * the properties that are set in the passed <code>Properties</code>
     * object.
     */
    class AMQCPP_API WireFormatFactory {
    public:

        virtual ~WireFormatFactory() {}

        /**
         * Creates a new WireFormat Object passing it a set of
         * properties from which it can obtain any optional settings
         * @param properties - the Properties for this WireFormat
         */
        virtual WireFormat* createWireFormat( const decaf::util::Properties& properties )
            throw ( decaf::lang::exceptions::IllegalStateException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_WIREFORMAT_WIREFORMATFACTORY_H_*/
