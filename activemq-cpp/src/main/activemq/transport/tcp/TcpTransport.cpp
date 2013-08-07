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

#include "TcpTransport.h"

#include <activemq/transport/IOTransport.h>
#include <activemq/transport/TransportFactory.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/net/SocketFactory.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::transport;
using namespace activemq::transport::tcp;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

namespace activemq {
namespace transport {
namespace tcp {

    class TcpTransportImpl {
    private:

        TcpTransportImpl(const TcpTransportImpl&);
        TcpTransportImpl& operator= (const TcpTransportImpl&);

    public:

        int connectTimeout;

        std::auto_ptr<decaf::net::Socket> socket;
        std::auto_ptr<decaf::io::DataInputStream> dataInputStream;
        std::auto_ptr<decaf::io::DataOutputStream> dataOutputStream;

        const decaf::net::URI& location;

        int outputBufferSize;
        int inputBufferSize;

        bool trace;

        int soLinger;
        bool soKeepAlive;
        int soReceiveBufferSize;
        int soSendBufferSize;
        bool tcpNoDelay;

        TcpTransportImpl(const decaf::net::URI& location) :
            connectTimeout(0),
            socket(),
            dataInputStream(),
            dataOutputStream(),
            location(location),
            outputBufferSize(8192),
            inputBufferSize(8192),
            trace(false),
            soLinger(-1),
            soKeepAlive(false),
            soReceiveBufferSize(-1),
            soSendBufferSize(-1),
            tcpNoDelay(true) {
        }
    };
}}}

////////////////////////////////////////////////////////////////////////////////
TcpTransport::TcpTransport(const Pointer<Transport> next, const decaf::net::URI& location) :
    TransportFilter(next), impl(new TcpTransportImpl(location)) {
}

////////////////////////////////////////////////////////////////////////////////
TcpTransport::~TcpTransport() {
    try {
        close();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::beforeNextIsStarted() {
    try {
        connect();
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::afterNextIsStopped() {
    try {
        // The IOTransport is now stopped, so we can safely closed the socket
        // and no asynchronous exceptions should be triggered.
        if (impl->socket.get() != NULL) {
            impl->socket->close();
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::doClose() {
    try {
        if (impl->socket.get() != NULL) {
            impl->socket->close();
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::connect() {

    try {

        impl->socket.reset(this->createSocket());

        // Set all Socket Options from the URI options.
        this->configureSocket(impl->socket.get());

        URI uri = this->impl->location;

        // Ensure something is actually passed in for the URI
        if (uri.getAuthority() == "") {
            throw SocketException(__FILE__, __LINE__,
                "Connection URI was not provided or is invalid: %s", uri.toString().c_str());
        }

        // Connect the socket.
        string host = uri.getHost();
        int port = uri.getPort();

        impl->socket->connect(host, port, impl->connectTimeout);

        // Cast it to an IO transport so we can wire up the socket
        // input and output streams.
        IOTransport* ioTransport = dynamic_cast<IOTransport*>(next.get());
        if (ioTransport == NULL) {
            throw ActiveMQException(__FILE__, __LINE__, "TcpTransport::TcpTransport - "
                    "transport must be of type IOTransport");
        }

        // Get the read buffer size.
        int inputBufferSize = this->impl->inputBufferSize;

        // Get the write buffer size.
        int outputBufferSize = this->impl->outputBufferSize;

        // We don't own these ever, socket object owns.
        InputStream* socketIStream = impl->socket->getInputStream();
        OutputStream* sokcetOStream = impl->socket->getOutputStream();

        Pointer<InputStream> inputStream;
        Pointer<OutputStream> outputStream;

        // If tcp tracing was enabled, wrap the input / output streams with logging streams
        if (this->impl->trace) {
            // Wrap with logging stream, we don't own the wrapped streams
            inputStream.reset(new LoggingInputStream(socketIStream));
            outputStream.reset(new LoggingOutputStream(sokcetOStream));

            // Now wrap with the Buffered streams, we own the source streams
            inputStream.reset(new BufferedInputStream(inputStream.release(), inputBufferSize, true));
            outputStream.reset(new BufferedOutputStream(outputStream.release(), outputBufferSize, true));
        } else {
            // Wrap with the Buffered streams, we don't own the source streams
            inputStream.reset(new BufferedInputStream(socketIStream, inputBufferSize));
            outputStream.reset(new BufferedOutputStream(sokcetOStream, outputBufferSize));
        }

        // Now wrap the Buffered Streams with DataInput based streams.  We own
        // the Source streams, all the streams in the chain that we own are
        // destroyed when these are.
        this->impl->dataInputStream.reset(new DataInputStream(inputStream.release(), true));
        this->impl->dataOutputStream.reset(new DataOutputStream(outputStream.release(), true));

        // Give the IOTransport the streams.
        ioTransport->setInputStream(impl->dataInputStream.get());
        ioTransport->setOutputStream(impl->dataOutputStream.get());
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Socket* TcpTransport::createSocket() {

    try {
        SocketFactory* factory = SocketFactory::getDefault();
        return factory->createSocket();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::configureSocket(Socket* socket) {

    try {

        int soLinger = this->impl->soLinger;
        int soReceiveBufferSize = this->impl->soReceiveBufferSize;
        int soSendBufferSize = this->impl->soSendBufferSize;

        // Set the socket options.
        socket->setKeepAlive(this->impl->soKeepAlive);
        socket->setTcpNoDelay(this->impl->tcpNoDelay);

        if (soLinger > 0) {
            socket->setSoLinger(true, soLinger);
        }

        if (soReceiveBufferSize > 0) {
            socket->setReceiveBufferSize(soReceiveBufferSize);
        }

        if (soSendBufferSize > 0) {
            socket->setSendBufferSize(soSendBufferSize);
        }
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_RETHROW(SocketException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, SocketException)
    DECAF_CATCHALL_THROW(SocketException)
}

////////////////////////////////////////////////////////////////////////////////
bool TcpTransport::isConnected() const {
    if (this->impl->socket.get() != NULL) {
        return this->impl->socket->isConnected();
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setConnectTimeout(int soConnectTimeout) {
    this->impl->connectTimeout = soConnectTimeout;
}

////////////////////////////////////////////////////////////////////////////////
int TcpTransport::getConnectTimeout() const {
    return this->impl->connectTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setOutputBufferSize(int outputBufferSize) {
    this->impl->outputBufferSize = outputBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
int TcpTransport::getOutputBufferSize() const {
    return this->impl->outputBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setInputBufferSize(int inputBufferSize) {
    this->impl->inputBufferSize = inputBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
int TcpTransport::getInputBufferSize() const {
    return this->impl->inputBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setTrace(bool trace) {
    this->impl->trace = trace;
}

////////////////////////////////////////////////////////////////////////////////
bool TcpTransport::isTrace() const {
    return this->impl->trace;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setLinger(int soLinger) {
    this->impl->soLinger = soLinger;
}

////////////////////////////////////////////////////////////////////////////////
int TcpTransport::getLinger() const {
    return this->impl->soLinger;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setKeepAlive(bool soKeepAlive) {
    this->impl->soKeepAlive = soKeepAlive;
}

////////////////////////////////////////////////////////////////////////////////
bool TcpTransport::isKeepAlive() const {
    return this->impl->soKeepAlive;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setReceiveBufferSize(int soReceiveBufferSize) {
    this->impl->soReceiveBufferSize = soReceiveBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
int TcpTransport::getReceiveBufferSize() const {
    return this->impl->soReceiveBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setSendBufferSize(int soSendBufferSize) {
    this->impl->soSendBufferSize = soSendBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
int TcpTransport::getSendBufferSize() const {
    return this->impl->soSendBufferSize;
}

////////////////////////////////////////////////////////////////////////////////
void TcpTransport::setTcpNoDelay(bool tcpNoDelay) {
    this->impl->tcpNoDelay = tcpNoDelay;
}

////////////////////////////////////////////////////////////////////////////////
bool TcpTransport::isTcpNoDelay() const {
    return this->impl->tcpNoDelay;
}

////////////////////////////////////////////////////////////////////////////////
decaf::net::URI TcpTransport::getLocation() const {
    return this->impl->location;
}
