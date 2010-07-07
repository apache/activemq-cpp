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

#ifndef _DECAF_NET_INET4ADDRESS_H_
#define _DECAF_NET_INET4ADDRESS_H_

#include <decaf/util/Config.h>

#include <decaf/net/InetAddress.h>

namespace decaf {
namespace net {

    class DECAF_API Inet4Address : public InetAddress {
    private:

        friend class InetAddress;

    protected:

        Inet4Address();
        Inet4Address( const unsigned char* ipAddress, int numBytes );
        Inet4Address( const std::string& hostname, const unsigned char* ipAddress, int numBytes );

    public:

        virtual ~Inet4Address();

    public:

        virtual InetAddress* clone() const;

        /**
         * Check if this InetAddress is a valid wildcard address.
         *
         * @return true if the address is a wildcard address.
         */
        virtual bool isAnyLocalAddress() const;

        /**
         * Check if this InetAddress is a valid loopback address.
         *
         * @return true if the address is a loopback address.
         */
        virtual bool isLoopbackAddress() const;

        /**
         * Check if this InetAddress is a valid Multicast address.
         *
         * @return true if the address is a Multicast address.
         */
        virtual bool isMulticastAddress() const;

        /**
         * Check if this InetAddress is a valid link local address.
         *
         * @return true if the address is a link local address.
         */
        virtual bool isLinkLocalAddress() const;

        /**
         * Check if this InetAddress is a valid site local address.
         *
         * @return true if the address is a site local address.
         */
        virtual bool isSiteLocalAddress() const;

        /**
         * Check if this InetAddress is Multicast and has Global scope.
         *
         * @return true if the address is Multicast and has Global scope.
         */
        virtual bool isMCGlobal() const;

        /**
         * Check if this InetAddress is Multicast and has Node Local scope.
         *
         * @return true if the address is Multicast and has Node Local scope.
         */
        virtual bool isMCNodeLocal() const;

        /**
         * Check if this InetAddress is Multicast and has Link Local scope.
         *
         * @return true if the address is Multicast and has Link Local scope.
         */
        virtual bool isMCLinkLocal() const;

        /**
         * Check if this InetAddress is Multicast and has Site Local scope.
         *
         * @return true if the address is Multicast and has Site Local scope.
         */
        virtual bool isMCSiteLocal() const;

        /**
         * Check if this InetAddress is Multicast and has Organization scope.
         *
         * @return true if the address is Multicast and has Organization scope.
         */
        virtual bool isMCOrgLocal() const;

    };

}}

#endif /* _DECAF_NET_INET4ADDRESS_H_ */
