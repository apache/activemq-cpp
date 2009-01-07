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

#ifndef _ACTIVEMQ_WIREFORMAT_WIREFORMATNEGOTIATOR_H_
#define _ACTIVEMQ_WIREFORMAT_WIREFORMATNEGOTIATOR_H_

#include <activemq/util/Config.h>
#include <activemq/transport/TransportFilter.h>

namespace activemq {
namespace wireformat {

    /**
     * Defines a WireFormatNegotiator which allows a WireFormat to
     */
    class AMQCPP_API WireFormatNegotiator : public activemq::transport::TransportFilter {
    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         * @param own - true if this filter owns the next and should delete it
         */
        WireFormatNegotiator( Transport* next, const bool own = true ) :
            transport::TransportFilter( next, own ) {}

        virtual ~WireFormatNegotiator() {}

    };

}}

#endif /* _ACTIVEMQ_WIREFORMAT_WIREFORMATNEGOTIATOR_H_ */
