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

#include "OpenSSLSocket.h"

#ifdef HAVE_OPENSSL
    #include <openssl/ssl.h>
    #include <openssl/tls1.h>
    #include <openssl/x509.h>
    #include <openssl/x509v3.h>
    #include <openssl/bio.h>
#endif

#include <decaf/net/SocketImpl.h>
#include <decaf/io/IOException.h>
#include <decaf/net/SocketException.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/internal/util/StringUtils.h>
#include <decaf/internal/net/SocketFileDescriptor.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLParameters.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketException.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketInputStream.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketOutputStream.h>
#include <decaf/util/concurrent/Mutex.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::util::concurrent;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    class SocketData {
    public:

        bool handshakeStarted;
        bool handshakeCompleted;
        std::string commonName;

        Mutex handshakeLock;

    public:

        SocketData() : handshakeStarted(false),
                       handshakeCompleted(false),
                       commonName(),
                       handshakeLock() {
        }

        ~SocketData() {}

#ifdef HAVE_OPENSSL
        static int verifyCallback(int verified, X509_STORE_CTX* store DECAF_UNUSED) {
            if (!verified) {
                // Trap debug info here about why the Certificate failed to validate.
            }

            return verified;
        }
#endif

    };

}}}}}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket(OpenSSLParameters* parameters) :
    SSLSocket(), data(new SocketData()), parameters(parameters), input(NULL), output(NULL) {

    if (parameters == NULL) {
        throw NullPointerException(__FILE__, __LINE__,
            "The OpenSSL Parameters object instance passed was NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket(OpenSSLParameters* parameters, const InetAddress* address, int port) :
    SSLSocket(address, port), data(new SocketData()), parameters(parameters), input(NULL), output(NULL) {

    if (parameters == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The OpenSSL Parameters object instance passed was NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket(OpenSSLParameters* parameters, const InetAddress* address, int port, const InetAddress* localAddress, int localPort) :
    SSLSocket(address, port, localAddress, localPort), data(new SocketData()), parameters(parameters), input(NULL), output(NULL) {

    if (parameters == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The OpenSSL Parameters object instance passed was NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket(OpenSSLParameters* parameters, const std::string& host, int port) :
    SSLSocket(host, port), data(new SocketData()), parameters(parameters), input(NULL), output(NULL) {

    if (parameters == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The OpenSSL Parameters object instance passed was NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::OpenSSLSocket(OpenSSLParameters* parameters, const std::string& host, int port, const InetAddress* localAddress, int localPort) :
    SSLSocket(host, port, localAddress, localPort), data(new SocketData()), parameters(parameters), input(NULL), output(NULL) {

    if (parameters == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The OpenSSL Parameters object instance passed was NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocket::~OpenSSLSocket() {
    try {

        SSLSocket::close();

#ifdef HAVE_OPENSSL
        if (this->parameters->getSSL()) {
            SSL_set_shutdown(this->parameters->getSSL(), SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
            SSL_shutdown(this->parameters->getSSL());
        }
#endif

        delete data;
        delete parameters;
        delete input;
        delete output;
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::connect(const std::string& host, int port, int timeout) {

    try {

#ifdef HAVE_OPENSSL

        // Perform the actual Socket connection work
        SSLSocket::connect(host, port, timeout);

        // If we actually connected then we can connect the Socket to an OpenSSL
        // BIO filter so that we can use it in OpenSSL APIs.
        if (isConnected()) {

            BIO* bio = BIO_new(BIO_s_socket());
            if (!bio) {
                throw SocketException(__FILE__, __LINE__, "Failed to create SSL IO Bindings");
            }

            const SocketFileDescriptor* fd = dynamic_cast<const SocketFileDescriptor*>(this->impl->getFileDescriptor());

            if (fd == NULL) {
                throw SocketException(__FILE__, __LINE__, "Invalid File Descriptor returned from Socket");
            }

            BIO_set_fd(bio, (int )fd->getValue(), BIO_NOCLOSE);
            SSL_set_bio(this->parameters->getSSL(), bio, bio);

            // Later when startHandshake is called we will check for this common name
            // in the provided certificate
            this->data->commonName = host;
        }
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::close() {

    try {

        if (isClosed()) {
            return;
        }

        SSLSocket::close();

        if (this->input != NULL) {
            this->input->close();
        }
        if (this->output != NULL) {
            this->output->close();
        }
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
decaf::io::InputStream* OpenSSLSocket::getInputStream() {

    checkClosed();

    try {
        if (this->input == NULL) {
            this->input = new OpenSSLSocketInputStream(this);
        }

        return this->input;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
decaf::io::OutputStream* OpenSSLSocket::getOutputStream() {

    checkClosed();

    try {
        if (this->output == NULL) {
            this->output = new OpenSSLSocketOutputStream(this);
        }

        return this->output;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::shutdownInput() {
    throw SocketException(__FILE__, __LINE__, "Not supported for SSL Sockets");
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::shutdownOutput() {
    throw SocketException(__FILE__, __LINE__, "Not supported for SSL Sockets");
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setOOBInline(bool value DECAF_UNUSED) {
    throw SocketException(__FILE__, __LINE__, "Not supported for SSL Sockets");
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::sendUrgentData(int data DECAF_UNUSED) {
    throw SocketException(__FILE__, __LINE__, "Not supported for SSL Sockets");
}

////////////////////////////////////////////////////////////////////////////////
decaf::net::ssl::SSLParameters OpenSSLSocket::getSSLParameters() const {

    SSLParameters params(this->getEnabledCipherSuites(), this->getEnabledProtocols());

    params.setServerNames(this->parameters->getServerNames());
    params.setNeedClientAuth(this->parameters->getNeedClientAuth());
    params.setWantClientAuth(this->parameters->getWantClientAuth());

    return params;
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setSSLParameters(const decaf::net::ssl::SSLParameters& value) {
    this->parameters->setEnabledCipherSuites(value.getCipherSuites());
    this->parameters->setEnabledProtocols(value.getProtocols());
    this->parameters->setServerNames(value.getServerNames());
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getSupportedCipherSuites() const {
    return this->parameters->getSupportedCipherSuites();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getSupportedProtocols() const {
    return this->parameters->getSupportedProtocols();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getEnabledCipherSuites() const {
    return this->parameters->getEnabledCipherSuites();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setEnabledCipherSuites(const std::vector<std::string>& suites) {
    this->parameters->setEnabledCipherSuites(suites);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLSocket::getEnabledProtocols() const {
    return this->parameters->getEnabledProtocols();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setEnabledProtocols(const std::vector<std::string>& protocols) {
    this->parameters->setEnabledProtocols(protocols);
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::startHandshake() {

    if (!this->isConnected()) {
        throw IOException(__FILE__, __LINE__, "Socket is not connected.");
    }

    if (this->isClosed()) {
        throw IOException(__FILE__, __LINE__, "Socket already closed.");
    }

    try {

#ifdef HAVE_OPENSSL
        synchronized( &(this->data->handshakeLock ) ) {

            if (this->data->handshakeStarted) {
                return;
            }

            this->data->handshakeStarted = true;

            bool peerVerifyDisabled = Boolean::parseBoolean(System::getProperty("decaf.net.ssl.disablePeerVerification", "false"));

            if (this->parameters->getUseClientMode()) {

                // Since we are a client we want to enforce peer verification, we set a
                // callback so we can collect data on why a verify failed for debugging.
                if (!peerVerifyDisabled) {
                    SSL_set_verify(this->parameters->getSSL(), SSL_VERIFY_PEER, SocketData::verifyCallback);
                } else {
                    SSL_set_verify(this->parameters->getSSL(), SSL_VERIFY_NONE, NULL);
                }

                std::vector<std::string> serverNames = this->parameters->getServerNames();
                if (!serverNames.empty()) {
                    std::string serverName = serverNames.at(0);
                    SSL_set_tlsext_host_name(this->parameters->getSSL(), serverName.c_str());
                }

                int result = SSL_connect(this->parameters->getSSL());

                // Checks the error status, when things go right we still perform a deeper
                // check on the provided certificate to ensure that it matches the host name
                // that we connected to, this prevents someone from using any certificate
                // signed by a signing authority that we trust.
                switch (SSL_get_error(this->parameters->getSSL(), result)) {
                case SSL_ERROR_NONE:
                    if (!peerVerifyDisabled) {
                        verifyServerCert(this->data->commonName);
                    }
                    break;
                case SSL_ERROR_SSL:
                case SSL_ERROR_ZERO_RETURN:
                case SSL_ERROR_SYSCALL:
                    SSLSocket::close();
                    throw OpenSSLSocketException(__FILE__, __LINE__);
                }

            } else { // We are in Server Mode.

                int mode = SSL_VERIFY_NONE;

                if (!peerVerifyDisabled) {

                    if (this->parameters->getWantClientAuth()) {
                        mode = SSL_VERIFY_PEER;
                    }

                    if (this->parameters->getNeedClientAuth()) {
                        mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
                    }
                }

                // Since we are a server we want to enforce peer verification, we set a
                // callback so we can collect data on why a verify failed for debugging.
                SSL_set_verify(this->parameters->getSSL(), mode, SocketData::verifyCallback);

                int result = SSL_accept(this->parameters->getSSL());

                if (result != SSL_ERROR_NONE) {
                    SSLSocket::close();
                    throw OpenSSLSocketException(__FILE__, __LINE__);
                }
            }

            this->data->handshakeCompleted = true;
        }
#else
        throw IOException( __FILE__, __LINE__, "SSL Not Supported." );
#endif
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setUseClientMode(bool value) {

    synchronized( &( this->data->handshakeLock ) ) {
        if (this->data->handshakeStarted) {
            throw IllegalArgumentException(__FILE__, __LINE__,
                "Handshake has already been started cannot change mode.");
        }

        this->parameters->setUseClientMode(value);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLSocket::getUseClientMode() const {
    return this->parameters->getUseClientMode();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setNeedClientAuth(bool value) {
    this->parameters->setNeedClientAuth(value);
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLSocket::getNeedClientAuth() const {
    return this->parameters->getNeedClientAuth();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::setWantClientAuth(bool value) {
    this->parameters->setWantClientAuth(value);
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLSocket::getWantClientAuth() const {
    return this->parameters->getWantClientAuth();
}

////////////////////////////////////////////////////////////////////////////////
int OpenSSLSocket::read(unsigned char* buffer, int size, int offset, int length) {

    try {
        if (this->isClosed()) {
            throw IOException(__FILE__, __LINE__, "The Stream has been closed");
        }

        if (this->isInputShutdown() == true) {
            return -1;
        }

        if (length == 0) {
            return 0;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Buffer passed is Null");
        }

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__,
                "size parameter out of Bounds: %d.", size);
        }

        if (offset > size || offset < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__,
                "offset parameter out of Bounds: %d.", offset);
        }

        if (length < 0 || length > size - offset) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__,
                "length parameter out of Bounds: %d.", length);
        }

#ifdef HAVE_OPENSSL

        if (!this->data->handshakeCompleted) {
            this->startHandshake();
        }

        // Read data from the socket.
        int result = SSL_read(this->parameters->getSSL(), buffer + offset, length);

        switch (SSL_get_error(this->parameters->getSSL(), result)) {
        case SSL_ERROR_NONE:
            return result;
        case SSL_ERROR_ZERO_RETURN:
            if (!isClosed()) {
                this->shutdownInput();
                return -1;
            }
        default:
            throw OpenSSLSocketException(__FILE__, __LINE__);
        }
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::write(const unsigned char* buffer, int size, int offset, int length) {

    try {

        if (length == 0) {
            return;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__,
                "TcpSocketOutputStream::write - passed buffer is null");
        }

        if (isClosed()) {
            throw IOException(__FILE__, __LINE__,
                "TcpSocketOutputStream::write - This Stream has been closed.");
        }

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__,
                "size parameter out of Bounds: %d.", size);
        }

        if (offset > size || offset < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__,
                "offset parameter out of Bounds: %d.", offset);
        }

        if (length < 0 || length > size - offset) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__,
                "length parameter out of Bounds: %d.", length);
        }

#ifdef HAVE_OPENSSL

        if (!this->data->handshakeCompleted) {
            this->startHandshake();
        }

        int remaining = length;

        while (remaining > 0 && !isClosed()) {

            int written = SSL_write(this->parameters->getSSL(), buffer + offset, remaining);

            switch (SSL_get_error(this->parameters->getSSL(), written)) {
            case SSL_ERROR_NONE:
                offset += written;
                remaining -= written;
                break;
            case SSL_ERROR_ZERO_RETURN:
                throw SocketException(__FILE__, __LINE__, "The connection was broken unexpectedly.");
            default:
                throw OpenSSLSocketException(__FILE__, __LINE__);
            }
        }
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int OpenSSLSocket::available() {

    try {

#ifdef HAVE_OPENSSL
        if (!isClosed()) {
            return SSL_pending(this->parameters->getSSL());
        }
#else
        throw SocketException( __FILE__, __LINE__, "Not Supported" );
#endif

        return -1;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocket::verifyServerCert(const std::string& serverName) {

#ifdef HAVE_OPENSSL
    X509* cert = SSL_get_peer_certificate(this->parameters->getSSL());

    if (cert == NULL) {
        this->close();
        throw OpenSSLSocketException(__FILE__, __LINE__, "No server certificate for verify for host: %s", serverName.c_str());
    }

    class Finalizer {
    private:

        Finalizer(const Finalizer&);
        Finalizer& operator=(const Finalizer&);

    private:

        X509* cert;

    public:

        Finalizer(X509* cert) : cert(cert) {}
        ~Finalizer() {
            if (cert != NULL) {
                X509_free(cert);
            }
        }
    };

    // Store the Certificate to be cleaned up when the method returns
    Finalizer final(cert);

    // We check the extensions first since newer x509v3 Certificates are recommended
    // to store the FQDN in the dsnName field of the subjectAltName extension.  If we
    // don't find it there then we can check the commonName field which is where older
    // Certificates placed the FQDN.
    int extensions = X509_get_ext_count(cert);

    for (int ix = 0; ix < extensions; ix++) {

        X509_EXTENSION* extension = X509_get_ext(cert, ix);
        const char* extensionName = OBJ_nid2sn(OBJ_obj2nid(X509_EXTENSION_get_object(extension)));

        if (StringUtils::compare("subjectAltName", extensionName) == 0) {

            X509V3_EXT_METHOD* method = (X509V3_EXT_METHOD*) X509V3_EXT_get(extension);
            if (method == NULL) {
                break;
            }

            const unsigned char* data = extension->value->data;
            STACK_OF(CONF_VALUE)* confValue = method->i2v(method,
                    method->it ?
                            ASN1_item_d2i(NULL, &data, extension->value->length, ASN1_ITEM_ptr(method->it)) :
                            method->d2i(NULL, &data, extension->value->length), NULL);

            CONF_VALUE* value = NULL;

            for (int iy = 0; iy < sk_CONF_VALUE_num( confValue ); iy++) {
                value = sk_CONF_VALUE_value( confValue, iy );
                if ((StringUtils::compare(value->name, "DNS") == 0) && StringUtils::compare(value->value, serverName.c_str()) == 0) {

                    // Found it.
                    return;
                }
            }
        }
    }

    X509_NAME* subject = X509_get_subject_name(cert);
    X509_NAME_ENTRY *entry;
    int lastpos = -1;

    if (subject != NULL) {
       for (;;) {
           lastpos = X509_NAME_get_index_by_NID(subject, NID_commonName, lastpos);
           if (lastpos == -1) {
               break;
           }
           entry = X509_NAME_get_entry(subject, lastpos);
           const char * entryText = (const char *) ASN1_STRING_data(X509_NAME_ENTRY_get_data(entry));
            if (StringUtils::compare(entryText , serverName.c_str()) == 0) {
                return;
            }
       }
    }

    // We got here so no match to serverName in the Certificate
    throw OpenSSLSocketException(__FILE__, __LINE__,
        "Server Certificate Name doesn't match the URI Host Name value.");
#endif
}
