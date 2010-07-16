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

    protected:

        mutable std::string hostname;
        mutable bool reached;
        mutable decaf::lang::ArrayPointer<unsigned char> addressBytes;

    protected:

        InetAddress();
        InetAddress( const unsigned char* ipAddress, int numBytes );
        InetAddress( const std::string& hostname, const unsigned char* ipAddress, int numBytes );

    public:

        virtual ~InetAddress();

        /**
         * Returns the Raw IP address in Network byte order.  The returned address is a copy
         * of the bytes contained in this InetAddress.
         *
         * @returns and ArrayPointer containing the raw bytes of the network address.
         */
        virtual decaf::lang::ArrayPointer<unsigned char> getAddress() const;

        /**
         * Returns a textual representation of the IP Address.
         *
         * @returns the string form of the IP Address.
         */
        virtual std::string getHostAddress() const;

        /**
         * Get the host name associated with this InetAddress instance.
         *
         * If a host name was set upon construction then that value is returned, otherwise a
         * reverse name lookup with be performed to attempt to get the host name associated
         * with the set IP Address.  If the host name cannot be resolved the textual representation
         * of the IP Address is returned instead.
         *
         * @returns the name of the host associated with this set IP Address.
         */
        virtual std::string getHostName() const;

        /**
         * Returns a string representation of the InetAddress in the form 'hostname / ipaddress'
         *
         * If the hostname is not resolved than it appears as empty.
         *
         * @returns string value of this InetAddress.
         */
        virtual std::string toString() const;

        /**
         * Returns a newly allocated copy of this InetAddress.  The caller owns the resulting
         * copy and must delete it.
         *
         * @return a new InetAddress instance that is a copy of this one, caller owns.
         */
        virtual InetAddress* clone() const;

    public:  // Address Property Tests, override in the subclasses for correct results

        /**
         * Check if this InetAddress is a valid wildcard address.
         *
         * @return true if the address is a wildcard address.
         */
        virtual bool isAnyLocalAddress() const {
            return false;
        }

        /**
         * Check if this InetAddress is a valid loopback address.
         *
         * @return true if the address is a loopback address.
         */
        virtual bool isLoopbackAddress() const {
            return false;
        }

        /**
         * Check if this InetAddress is a valid Multicast address.
         *
         * @return true if the address is a Multicast address.
         */
        virtual bool isMulticastAddress() const {
            return false;
        }

        /**
         * Check if this InetAddress is a valid link local address.
         *
         * @return true if the address is a link local address.
         */
        virtual bool isLinkLocalAddress() const {
            return false;
        }

        /**
         * Check if this InetAddress is a valid site local address.
         *
         * @return true if the address is a site local address.
         */
        virtual bool isSiteLocalAddress() const {
            return false;
        }

        /**
         * Check if this InetAddress is Multicast and has Global scope.
         *
         * @return true if the address is Multicast and has Global scope.
         */
        virtual bool isMCGlobal() const {
            return false;
        }

        /**
         * Check if this InetAddress is Multicast and has Node Local scope.
         *
         * @return true if the address is Multicast and has Node Local scope.
         */
        virtual bool isMCNodeLocal() const {
            return false;
        }

        /**
         * Check if this InetAddress is Multicast and has Link Local scope.
         *
         * @return true if the address is Multicast and has Link Local scope.
         */
        virtual bool isMCLinkLocal() const {
            return false;
        }

        /**
         * Check if this InetAddress is Multicast and has Site Local scope.
         *
         * @return true if the address is Multicast and has Site Local scope.
         */
        virtual bool isMCSiteLocal() const {
            return false;
        }

        /**
         * Check if this InetAddress is Multicast and has Organization scope.
         *
         * @return true if the address is Multicast and has Organization scope.
         */
        virtual bool isMCOrgLocal() const {
            return false;
        }

    public:  // Static Create methods.

        /**
         * Given a raw IP Address in byte array form, create and return a new InetAddress instance.
         *
         * An IPV4 address must be only four bytes in length and an IPV6 address must be 16 bytes in length.
         *
         * @return a copy of an InetAddress that represents the given byte array address.
         *
         * @throws UnknownHostException if the address array length is invalid.
         */
        static InetAddress getByAddress( const unsigned char* bytes, int numBytes );

        /**
         * Given a host name and IPAddress return a new InetAddress.  There is no name service checking or
         * address validation done on the provided host name.  The host name can either be machine name or
         * the text based representation of the IP Address.
         *
         * An IPV4 address must be only four bytes in length and an IPV6 address must be 16 bytes in length.
         *
         * @return a copy of an InetAddress that represents the given byte array address.
         *
         * @throws UnknownHostException if the address array length is invalid.
         */
        static InetAddress getByAddress( const std::string& hostname, const unsigned char* bytes, int numBytes );

        /**
         * Gets an InetAddress that is the local host address.  If the localhost value cannot
         * be resolved than the InetAddress for Loopback is returned.
         *
         * @returns a new InetAddress object that contains the local host address.
         *
         * @throws UnknownHostException if the address for local host is not found.
         */
        static InetAddress getLocalHost();

    protected:

        /**
         * Converts the bytes in an address array to an int starting from the value start
         * treating the start value as the high order byte.
         *
         * @param bytes
         *      The array of bytes to convert to an int.
         * @param start
         *      The index in the array to treat as the high order byte.
         *
         * @return an unsigned int that represents the address value.
         */
        static unsigned int bytesToInt( const unsigned char* bytes, int start );

        static InetAddress getAnyAddress();

        static InetAddress getLoopbackAddress();

    };

}}

#endif /* _DECAF_NET_INETADDRESS_H_ */
