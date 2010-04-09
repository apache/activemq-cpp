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

#ifndef _DECAF_NET_INETADDRESS_H_
#define _DECAF_NET_INETADDRESS_H_

#include <decaf/util/Config.h>

#include <decaf/lang/ArrayPointer.h>

namespace decaf {
namespace net {

    /**
     * Represents an IP address.
     *
     * @since 1.0
     */
    class DECAF_API InetAddress {
    protected:

        static const unsigned char loopbackBytes[4];
        static const unsigned char anyBytes[4];

        static const InetAddress ANY;
        static const InetAddress LOOPBACK;

    private:

        std::string hostname;
        bool reached;
        decaf::lang::ArrayPointer<unsigned char> addressBytes;

    protected:

        int family;

    protected:

        InetAddress();
        InetAddress( unsigned char* ipAddress, int numBytes );
        InetAddress( const std::string& hostname, unsigned char* ipAddress, int numBytes );

    private:

        InetAddress( const InetAddress& );
        InetAddress& operator= ( const InetAddress& );

    public:

        virtual ~InetAddress();

        /**
         * Returns the Raw IP address in Network byte order.  The returned address is a copy
         * of the bytes contained in this InetAddress.
         *
         * @returns and ArrayPointer containing the raw bytes of the network address.
         */
        decaf::lang::ArrayPointer<unsigned char> getAddress() const;

        /**
         * Returns a textual representation of the IP Address.
         *
         * @returns the string form of the IP Adrress.
         */
        std::string getHostAddress() const;

    };

}}

#endif /* _DECAF_NET_INETADDRESS_H_ */
