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
#ifndef _ACTIVEMQ_NETWORK_SOCKETFACTORY_H_
#define _ACTIVEMQ_NETWORK_SOCKETFACTORY_H_

#include <activemq/network/SocketException.h>
#include <activemq/util/Properties.h>

namespace activemq {
namespace network {

class Socket;

    /**
     * Socket Factory for use in Creating Sockets
    
     * @see <code>Socket</code>
     */
    class SocketFactory {
    public:
    
        virtual ~SocketFactory() {}
    
        /**
         * Creates and returns a Socket dervied Object based on the values
         * defined in the Properties Object that is passed in.
         * @param the URI for the Socket Connection.
         * @param properties a IProperties pointer.
         * @throws SocketException.
         */
        virtual Socket* createSocket(const std::string& uri,
                const util::Properties& properties) throw (SocketException ) = 0;
    
    };

}}

#endif /*_ACTIVEMQ_NETWORK_SOCKETFACTORY_H_*/
