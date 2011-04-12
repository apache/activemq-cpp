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

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMATFACTORY_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMATFACTORY_H_

#include <activemq/util/Config.h>
#include <activemq/wireformat/WireFormatFactory.h>
#include <activemq/wireformat/stomp/StompWireFormat.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace wireformat {
namespace stomp {

    using decaf::lang::Pointer;

    /**
     * Factory used to create the Stomp Wire Format instance.
     */
    class AMQCPP_API StompWireFormatFactory : public WireFormatFactory {
    public:

        StompWireFormatFactory() {}
        virtual ~StompWireFormatFactory() {}

        /**
         * Creates a new WireFormat Object passing it a set of
         * properties from which it can obtain any optional settings
         * @param properties - the Properties for this WireFormat
         */
        virtual Pointer<WireFormat> createWireFormat( const decaf::util::Properties& properties );

    };

}}}

#endif /* _ACTIVEMQ_WIREFORMAT_STOMP_STOMPWIREFORMATFACTORY_H_ */
