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

#ifndef _DECAF_NET_DATAGRAMPACKET_H_
#define _DECAF_NET_DATAGRAMPACKET_H_

#include <decaf/util/Config.h>

#include <decaf/net/Inet4Address.h>
#include <decaf/net/SocketAddress.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace net {

    /**
     * Class that represents a single datagram packet.
     *
     * Datagrams are sent in packets from machine to machine and can each be routed differently
     * and can arrive in any order.  Delivery of a packet is not guaranteed.
     *
     * @since 1.0
     */
    class DatagramPacket {
    private:

        unsigned char* buffer;
        int size;
        int offset;
        int length;

        InetAddress* address;
        int port;

        mutable decaf::util::concurrent::Mutex mutex;

    private:

        DatagramPacket( const DatagramPacket& );
        DatagramPacket& operator= ( const DatagramPacket& );

    public:

        /**
         * Creates a new DatagramPacket for use in receiving a packet of the given length.
         *
         * @param bytes
         *      The array of bytes to hold the incoming datagram buffer.
         * @param size
         *      The size of the supplied byte array.
         * @param length
         *      The number of byte to read starting at the supplied offset.
         *
         * @throws NullPointerException if the pointer to the buffer is NULL.
         * @throws IndexOutOfBoundsException if the number of bytes to read exceeds the buffer size.
         */
        DatagramPacket( unsigned char* bytes, int size, int length );

        /**
         * Creates a new DatagramPacket for use in receiving a packet of the given length
         * starting at the specified offset into the buffer.
         *
         * @param bytes
         *      The array of bytes to hold the incoming datagram buffer.
         * @param size
         *      The size of the supplied byte array.
         * @param offset
         *      The position in the array to start writing to.
         * @param length
         *      The number of byte to read starting at the supplied offset.
         *
         * @throws NullPointerException if the pointer to the buffer is NULL.
         * @throws IndexOutOfBoundsException if the number of bytes to copy exceeds the buffer size.
         */
        DatagramPacket( unsigned char* bytes, int size, int offset, int length );

        /**
         * Creates a new DatagramPacket for use in sending a packet of the given length
         * starting at the specified offset into the buffer to the specified host on the
         * specified port.
         *
         * @param bytes
         *      The array of bytes to hold the outgoing datagram buffer.
         * @param size
         *      The size of the supplied byte array.
         * @param offset
         *      The position in the array to start writing to.
         * @param length
         *      The number of byte to read starting at the supplied offset.
         * @param address
         *      The Address to send the packet to
         * @param port
         *      The port on the destination that is to receive this packet.
         *
         * @throws NullPointerException if the pointer to the buffer is NULL.
         * @throws IndexOutOfBoundsException if the number of bytes to copy exceeds the buffer size.
         */
        DatagramPacket( unsigned char* bytes, int size, int offset, int length, const InetAddress& address, int port );

        /**
         * Creates a new DatagramPacket for use in sending a packet of the given length
         * to the specified host on the specified port.
         *
         * @param bytes
         *      The array of bytes to hold the outgoing datagram buffer.
         * @param size
         *      The size of the supplied byte array.
         * @param length
         *      The number of byte to read starting at the supplied offset.
         * @param address
         *      The Address to send the packet to
         * @param port
         *      The port on the destination that is to receive this packet.
         *
         * @throws NullPointerException if the pointer to the buffer is NULL.
         * @throws IndexOutOfBoundsException if the number of bytes to copy exceeds the buffer size.
         */
        DatagramPacket( unsigned char* bytes, int size, int length, const InetAddress& address, int port );

        /**
         * Creates a new DatagramPacket for use in sending a packet of the given length
         * into the buffer to the specified socket address.
         *
         * @param bytes
         *      The array of bytes to hold the outgoing datagram buffer.
         * @param size
         *      The size of the supplied byte array.
         * @param length
         *      The number of byte to read starting at the supplied offset.
         * @param address
         *      The Address to send the packet to
         *
         * @throws NullPointerException if the pointer to the buffer is NULL.
         * @throws IndexOutOfBoundsException if the number of bytes to copy exceeds the buffer size.
         */
        DatagramPacket( unsigned char* bytes, int size, int length, const SocketAddress& address );

        /**
         * Creates a new DatagramPacket for use in sending a packet of the given length
         * starting at the specified offset into the buffer to the specified socket address.
         *
         * @param bytes
         *      The array of bytes to hold the outgoing datagram buffer.
         * @param size
         *      The size of the supplied byte array.
         * @param offset
         *      The position in the array to start writing to.
         * @param length
         *      The number of byte to read starting at the supplied offset.
         * @param address
         *      The Address to send the packet to
         *
         * @throws NullPointerException if the pointer to the buffer is NULL.
         * @throws IndexOutOfBoundsException if the number of bytes to copy exceeds the buffer size.
         */
        DatagramPacket( unsigned char* bytes, int size, int offset, int length, const SocketAddress& address );

        virtual ~DatagramPacket();

    public:

        /**
         * @return the IP address that this datagram packet is being sent to or was received from.
         */
        InetAddress* getAddress() const;

        /**
         * Sets the IP address of the machine to which this datagram is being sent.
         *
         * @param address
         *      The IP address.
         */
        void setAddress( const InetAddress& address );

        /**
         * Gets the SocketAddress (usually IP address + port number) of the remote host that this
         * packet is being sent to or is coming from.
         *
         * @returns the SocketAddress for this datagram packet.
         */
        SocketAddress* getSocketAddress() const;

        /**
         * Sets the SocketAddress (usually IP address + port number) of the remote host to which
         * this datagram is being sent.
         *
         * @param address
         *      The SocketAddress (IP + port) for this datagram packet.
         *
         * @throws IllegalArgumentException if the subclass of address is not supported by this Socket.
         */
        void setSocketAddress( const SocketAddress& address );

        /**
         * @return the port number that this datagram packet is being sent to or was received from.
         */
        int getPort() const;

        /**
         * Sets the port number on the remote host to which this datagram is being sent.
         *
         * @param port
         *      The port on the remote host.
         *
         * @throws IllegalArgumentException if the port value is not in the range [0..65535].
         */
        void setPort( int port );

        /**
         * @return the offset of the data to be sent or the offset of the data received.
         */
        int getOffset() const;

        /**
         * Sets the offset into the data buffer where data to sent is to be read from or where
         * the data that is received should start writing to.
         *
         * @param offset
         *      The buffer offset value.
         *
         * @throws IllegalArgumentException if the offset value is greater than the buffer size.
         */
        void setOffset( int offset );

        /**
         * @return the length of the data to be sent or the length of the data received.
         */
        int getLength() const;

        /**
         * Set the length for this packet. The length of the packet is the number of bytes from the
         * packet's data buffer that will be sent, or the number of bytes of the packet's data buffer
         * that will be used for receiving data. The length must be lesser or equal to the offset
         * plus the length of the packet's buffer.
         *
         * @param length
         *      The length value to set for this packet.
         *
         * @throws IllegalArgumentException if the value is negative or exceeds the data buffers length.
         */
        void setLength( int length );

        /**
         * @return the data buffer. The data received or the data to be sent starts from the offset
         *         in the buffer, and continues for length bytes.
         */
        unsigned char* getData() const;

        /**
         * @return the size of the buffer used in this datagram packet.
         */
        int getSize() const;

        /**
         * Set the data buffer for this packet. With the offset of this DatagramPacket set to 0, and
         * the length set to the size value specified.
         *
         * @param buffer
         *      The new data buffer to use for this datagram packet.
         * @param size
         *      The size of the buffer.
         *
         * @throws NullPointerException if the buffer pointer is NULL.
         */
        void setData( unsigned char* buffer, int size );

        /**
         * Set the data buffer for this packet. With the offset of this DatagramPacket set to 0, and
         * the length set to the size value specified.
         *
         * @param buffer
         *      The new data buffer to use for this datagram packet.
         * @param size
         *      The size of the buffer.
         * @param offset
         *      The position in the buffer to read from or write to.
         * @param length
         *      The number of bytes that will be read into the buffer or sent from the buffer.
         *
         * @throws NullPointerException if the buffer pointer is NULL.
         */
        void setData( unsigned char* buffer, int size, int offset, int length );

    };

}}

#endif /* _DECAF_NET_DATAGRAMPACKET_H_ */
