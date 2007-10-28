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
#ifndef _ACTIVEMQ_NETWORK_SSLSOCKETFACTORY_H_
#define _ACTIVEMQ_NETWORK_SSLSOCKETFACTORY_H_

#include <activemq/network/TcpSocketFactory.h>

#include <activemq/concurrent/Mutex.h>

namespace activemq {
namespace network {

    /**
     * Socket Factory implementation for use in Creating SSL Sockets
     *
     * @see <code>SSLSocket</code>
     */
    class SSLSocketFactory : public TcpSocketFactory {
    #ifdef AMQ_HAVE_OPENSSL	
    
        /**
         * If we should manage locks.
         */
        bool manageLocks;
    
        /**
         * Locks for OpenSSL and callback to manage them.
         */
        static concurrent::Mutex *locks;
        static void locking_cb( int mode, int n, const char* file, int line );
    
    #endif /* AMQ_HAVE_OPENSSL */ 	
    
    public:
        SSLSocketFactory();
        virtual ~SSLSocketFactory();
    
        virtual TcpSocket* createTcpSocket(const util::Properties &properties)
                throw (SocketException );
    };

}}

#endif /*_ACTIVEMQ_NETWORK_TCPSOCKETFACTORY_H_*/
