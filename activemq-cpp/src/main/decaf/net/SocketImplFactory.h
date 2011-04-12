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

#ifndef _DECAF_NET_SOCKETIMPLFACTORY_H_
#define _DECAF_NET_SOCKETIMPLFACTORY_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace net {

    class SocketImpl;

    /**
     * Factory class interface for a Factory that creates ScoketImpl objects.  These
     * factories can be used to create various types of Sockets, e.g. Streaming, Multicast,
     * SSL, or platform specific variations of these types.
     *
     * @see decaf::net::Socket
     * @see decaf::net::ServerSocket
     *
     * @since 1.0
     */
    class DECAF_API SocketImplFactory {
    public:

        virtual ~SocketImplFactory() {}

        /**
         * Creates a new SokcetImpl instance and returns it, the caller then owns the
         * instance and must delete it when finished with the SocketImpl.
         *
         * @return new SocketImpl instance that is owned by the caller.
         */
        virtual SocketImpl* createSocketImpl() = 0;

    };

}}

#endif /* _DECAF_NET_SOCKETIMPLFACTORY_H_ */
