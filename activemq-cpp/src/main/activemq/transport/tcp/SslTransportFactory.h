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

#ifndef _ACTIVEMQ_TRANSPORT_TCP_SSLTRANSPORTFACTORY_H_
#define _ACTIVEMQ_TRANSPORT_TCP_SSLTRANSPORTFACTORY_H_

#include <activemq/util/Config.h>

#include <activemq/transport/tcp/TcpTransportFactory.h>

namespace activemq {
namespace transport {
namespace tcp {

    using decaf::lang::Pointer;

    class SslTransportFactory : public TcpTransportFactory {
    public:

        virtual ~SslTransportFactory();

    protected:

        virtual Pointer<Transport> doCreateComposite( const decaf::net::URI& location,
                                                      const Pointer<wireformat::WireFormat>& wireFormat,
                                                      const decaf::util::Properties& properties );

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_TCP_SSLTRANSPORTFACTORY_H_ */
