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

#ifndef _DECAF_NET_SOCKETOPTIONS_H_
#define _DECAF_NET_SOCKETOPTIONS_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace net {

    /**
     *
     * @since 1.0
     */
    class DECAF_API SocketOptions {
    public:

        /**
         * Disable Nagle's algorithm for this connection. Written data to the network is not
         * buffered pending acknowledgment of previously written data.  Valid for TCP sockets.
         */
        static const int SOCKET_OPTION_TCP_NODELAY;

        /**
         * Fetch the local address binding of a socket (this option cannot be "set" only "gotten",
         * since sockets are bound at creation time, and so the locally bound address cannot be
         * changed). The default local address of a socket is INADDR_ANY, meaning any local address
         * on a multi-homed host. A multi-homed host can use this option to accept connections to
         * only one of its addresses (in the case of a ServerSocket or DatagramSocket), or to
         * specify its return address to the peer (for a Socket or DatagramSocket). The parameter
         * of this option is an InetAddress.
         */
        static const int SOCKET_OPTION_BINDADDR;

        /**
         * Sets SO_REUSEADDR for a socket. This is used only for MulticastSockets in decaf, and it is
         * set by default for MulticastSockets.
         */
        static const int SOCKET_OPTION_REUSEADDR;

        /**
         * Sets SO_BROADCAST for a socket. This option enables and disables the ability of the process
         * to send broadcast messages. It is supported for only datagram sockets and only on networks
         * that support the concept of a broadcast message (e.g. Ethernet, token ring, etc.), and it
         * is set by default for DatagramSockets.
         */
        static const int SOCKET_OPTION_BROADCAST;

        /**
         * Set which outgoing interface on which to send multicast packets. Useful on hosts with
         * multiple network interfaces, where applications want to use other than the system default.
         * Takes/returns an InetAddress.
         *
         * Valid for Multicast: DatagramSocketImpl.
         */
        static const int SOCKET_OPTION_IP_MULTICAST_IF;

        /**
         * Same as above. This option is introduced so that the behaviour with IP_MULTICAST_IF will
         * be kept the same as before, while this new option can support setting outgoing interfaces
         * with either IPv4 and IPv6 addresses.
         */
        static const int SOCKET_OPTION_IP_MULTICAST_IF2;

        /**
         * This option enables or disables local loopback of multicast datagrams. This option is enabled
         * by default for Multicast Sockets.
         */
        static const int SOCKET_OPTION_IP_MULTICAST_LOOP;

        /**
         * This option sets the type-of-service or traffic class field in the IP header for a TCP or
         * UDP socket.
         */
        static const int SOCKET_OPTION_IP_TOS;

        /**
         * Specify a linger-on-close timeout. This option disables/enables immediate return from a
         * close() of a TCP Socket. Enabling this option with a non-zero Integer timeout means that
         * a close() will block pending the transmission and acknowledgment of all data written to the
         * peer, at which point the socket is closed gracefully. Upon reaching the linger timeout, the
         * socket is closed forcefully, with a TCP RST. Enabling the option with a timeout of zero
         * does a forceful close immediately. If the specified timeout value exceeds 65,535 it will
         * be reduced to 65,535.
         *
         * Valid only for TCP: SocketImpl
         */
        static const int SOCKET_OPTION_LINGER;

        /**
         * Set a timeout on blocking Socket operations.  The option must be set prior to entering a
         * blocking operation to take effect.
         */
        static const int SOCKET_OPTION_TIMEOUT;

        /**
         * Set a hint the size of the underlying buffers used by the platform for outgoing network
         * I/O. When used in set, this is a suggestion to the kernel from the application about the
         * size of buffers to use for the data to be sent over the socket. When used in get, this
         * must return the size of the buffer actually used by the platform when sending out data
         * on this socket. Valid for all sockets: SocketImpl, DatagramSocketImpl
         */
        static const int SOCKET_OPTION_SNDBUF;

        /**
         * Set a hint the size of the underlying buffers used by the platform for incoming network
         * I/O. When used in set, this is a suggestion to the kernel from the application about the
         * size of buffers to use for the data to be received over the socket. When used in get,
         * this must return the size of the buffer actually used by the platform when receiving in
         * data on this socket. Valid for all sockets: SocketImpl, DatagramSocketImpl.
         */
        static const int SOCKET_OPTION_RCVBUF;

        /**
         * When the keepalive option is set for a TCP socket and no data has been exchanged across
         * the socket in either direction for 2 hours (NOTE: the actual value is implementation
         * dependent), TCP automatically sends a keepalive probe to the peer. This probe is a TCP
         * segment to which the peer must respond. One of three responses is expected: 1. The peer
         * responds with the expected ACK. The application is not notified (since everything is OK).
         * TCP will send another probe following another 2 hours of inactivity. 2. The peer responds
         * with an RST, which tells the local TCP that the peer host has crashed and rebooted. The
         * socket is closed. 3. There is no response from the peer. The socket is closed. The
         * purpose of this option is to detect if the peer host crashes.
         *
         * Valid only for TCP socket: SocketImpl
         */
        static const int SOCKET_OPTION_KEEPALIVE;

        /**
         * When the OOBINLINE option is set, any TCP urgent data received on the socket will be
         * received through the socket input stream. When the option is disabled (which is the default)
         * urgent data is silently discarded.
         */
        static const int SOCKET_OPTION_OOBINLINE;

    public:

        virtual ~SocketOptions();

    };

}}

#endif /* _DECAF_NET_SOCKETOPTIONS_H_ */
