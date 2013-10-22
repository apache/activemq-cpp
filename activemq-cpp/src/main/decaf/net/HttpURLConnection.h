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

#ifndef _DECAF_NET_HTTPURLCONNECTION_H_
#define _DECAF_NET_HTTPURLCONNECTION_H_

#include <decaf/util/Config.h>

#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/net/URLConnection.h>

namespace decaf {
namespace net {

    /**
     * This abstract subclass of URLConnection defines methods for managing HTTP connection
     * according to the description given by RFC 2068.
     *
     * @see ContentHandler
     * @see URL
     * @see URLConnection
     * @see URLStreamHandler
     *
     * @since 1.0
     */
    class DECAF_API HttpURLConnection : public URLConnection {
    protected:

        /**
         * The HTTP request method of this HttpURLConnection.  The default value is "GET".
         */
        std::string method;

        /**
         * The status code of the response obtained from the HTTP request. The
         * default value is {@code -1}.
         * <p>
         * <li>1xx: Informational</li>
         * <li>2xx: Success</li>
         * <li>3xx: Relocation/Redirection</li>
         * <li>4xx: Client Error</li>
         * <li>5xx: Server Error</li>
         */
        int responseCode;

        /**
         * The HTTP response message which corresponds to the response code.
         */
        std::string responseMessage;

        /**
         * Flag to define whether the protocol will automatically follow redirects
         * or not. The default value is {@code true}.
         */
        bool instanceFollowRedirects;

        /**
         * If the HTTP chunked encoding is enabled this parameter defines the
         * chunk-length. Default value is {@code -1} that means the chunked encoding
         * mode is disabled.
         */
        int chunkLength;

        /**
         * If using HTTP fixed-length streaming mode this parameter defines the
         * fixed length of content. Default value is {@code -1} that means the
         * fixed-length streaming mode is disabled.
         */
        int fixedContentLength;

    public:

        /**
         * Numeric status code, 202: Accepted
         */
        static const int HTTP_ACCEPTED;

        /**
         * Numeric status code, 502: Bad Gateway
         */
        static const int HTTP_BAD_GATEWAY;

        /**
         * Numeric status code, 405: Bad Method
         */
        static const int HTTP_BAD_METHOD;

        /**
         * Numeric status code, 400: Bad Request
         */
        static const int HTTP_BAD_REQUEST;

        /**
         * Numeric status code, 408: Client Timeout
         */
        static const int HTTP_CLIENT_TIMEOUT;

        /**
         * Numeric status code, 409: Conflict
         */
        static const int HTTP_CONFLICT;

        /**
         * Numeric status code, 201: Created
         */
        static const int HTTP_CREATED;

        /**
         * Numeric status code, 413: Entity too large
         */
        static const int HTTP_ENTITY_TOO_LARGE;

        /**
         * Numeric status code, 403: Forbidden
         */
        static const int HTTP_FORBIDDEN;

        /**
         * Numeric status code, 504: Gateway timeout
         */
        static const int HTTP_GATEWAY_TIMEOUT;

        /**
         * Numeric status code, 410: Gone
         */
        static const int HTTP_GONE;

        /**
         * Numeric status code, 500: Internal error
         */
        static const int HTTP_INTERNAL_ERROR;

        /**
         * Numeric status code, 411: Length required
         */
        static const int HTTP_LENGTH_REQUIRED;

        /**
         * Numeric status code, 301 Moved permanently
         */
        static const int HTTP_MOVED_PERM;

        /**
         * Numeric status code, 302: Moved temporarily
         */
        static const int HTTP_MOVED_TEMP;

        /**
         * Numeric status code, 300: Multiple choices
         */
        static const int HTTP_MULT_CHOICE;

        /**
         * Numeric status code, 204: No content
         */
        static const int HTTP_NO_CONTENT;

        /**
         * Numeric status code, 406: Not acceptable
         */
        static const int HTTP_NOT_ACCEPTABLE;

        /**
         * Numeric status code, 203: Not authoritative
         */
        static const int HTTP_NOT_AUTHORITATIVE;

        /**
         * Numeric status code, 404: Not found
         */
        static const int HTTP_NOT_FOUND;

        /**
         * Numeric status code, 501: Not implemented
         */
        static const int HTTP_NOT_IMPLEMENTED;

        /**
         * Numeric status code, 304: Not modified
         */
        static const int HTTP_NOT_MODIFIED;

        /**
         * Numeric status code, 200: OK
         */
        static const int HTTP_OK;

        /**
         * Numeric status code, 206: Partial
         */
        static const int HTTP_PARTIAL;

        /**
         * Numeric status code, 402: Payment required
         */
        static const int HTTP_PAYMENT_REQUIRED;

        /**
         * Numeric status code, 412: Precondition failed
         */
        static const int HTTP_PRECON_FAILED;

        /**
         * Numeric status code, 407: Proxy authentication required
         */
        static const int HTTP_PROXY_AUTH;

        /**
         * Numeric status code, 414: Request too long
         */
        static const int HTTP_REQ_TOO_LONG;

        /**
         * Numeric status code, 205: Reset
         */
        static const int HTTP_RESET;

        /**
         * Numeric status code, 303: See other
         */
        static const int HTTP_SEE_OTHER;

        /**
         * Numeric status code, 305: Use proxy
         */
        static const int HTTP_USE_PROXY;

        /**
         * Numeric status code, 401: Unauthorized
         */
        static const int HTTP_UNAUTHORIZED;

        /**
         * Numeric status code, 415: Unsupported type
         */
        static const int HTTP_UNSUPPORTED_TYPE;

        /**
         * Numeric status code, 503: Unavailable
         */
        static const int HTTP_UNAVAILABLE;

        /**
         * Numeric status code, 505: Version not supported
         */
        static const int HTTP_VERSION;

    private:

        static bool followRedirects;

        static const int DEFAULT_CHUNK_LENGTH;

    private:

        HttpURLConnection(const HttpURLConnection&);
        HttpURLConnection& operator= (const HttpURLConnection&);

    protected:

        HttpURLConnection(const URL& url);

    public:

        virtual ~HttpURLConnection();

        /**
         * Closes the connection to the HTTP server.
         *
         * @see URLConnection#connect()
         * @see URLConnection#connected
         */
        virtual void disconnect() = 0;

        /**
         * Returns an input stream from the server in the case of an error such as
         * the requested file has not been found on the remote server. This stream
         * can be used to read the data the server will send back.
         *
         * @return the error input stream returned by the server.
         */
        virtual decaf::io::InputStream* getErrorStream() const {
            return NULL;
        }

        /**
         * Returns the request method which will be used to make the request to the
         * remote HTTP server. All possible methods of this HTTP implementation is
         * listed in the class definition.
         *
         * @return the request method string.
         */
        std::string getRequestMethod() const {
            return method;
        }

        /**
         * Returns the response code returned by the remote HTTP server.
         *
         * @return the response code, -1 if no valid response code.
         *
         * @throws IOException if there is an IO error during the retrieval.
         */
        int getResponseCode();

        /**
         * Returns the response message returned by the remote HTTP server.
         *
         * @return the response message. empty string if no such response exists.
         *
         * @throws IOException if there is an error during the retrieval.
         */
        std::string getResponseMessage();

        /**
         * Sets the request command which will be sent to the remote HTTP server.
         * This method can only be called before the connection is made.
         *
         * @param method
         *      the string representing the method to be used.
         *
         * @throws ProtocolException
         *      if this is called after connected, or the method is not
         *      supported by this HTTP implementation.
         */
        void setRequestMethod(const std::string& method);

        /**
         * Returns whether this connection follows redirects.
         *
         * @return if this connection follows redirects, false otherwise.
         */
        bool getInstanceFollowRedirects() const {
            return instanceFollowRedirects;
        }

        /**
         * Sets whether this connection follows redirects.
         *
         * @param followRedirects
         *      if this connection will follows redirects, false otherwise.
         */
        void setInstanceFollowRedirects(bool followRedirects) {
            instanceFollowRedirects = followRedirects;
        }

        /**
         * If the length of a HTTP request body is known ahead, sets fixed length to
         * enable streaming without buffering. Sets after connection will cause an
         * exception.
         *
         * @param contentLength
         *      the fixed length of the HTTP request body.
         *
         * @throws IllegalStateException if already connected or an other mode already set.
         * @throws IllegalArgumentException if contentLength is less than zero.
        */
        void setFixedLengthStreamingMode(int contentLength);

        /**
         * If the length of a HTTP request body is NOT known ahead, enable chunked
         * transfer encoding to enable streaming with buffering. Notice that not all
         * http servers support this mode. Sets after connection will cause an
         * exception.
         *
         * @param chunklen
         *      the length of a chunk.
         *
         * @throws IllegalStateException if already connected or an other mode already set.
         */
        void setChunkedStreamingMode(int chunklen);

    public:

        /**
         * Returns the value of followRedirects which indicates if this connection follows
         * a different URL redirected by the server. It is enabled by default.
         *
         * @return the value of the flag.
         */
        static bool getFollowRedirects() {
            return followRedirects;
        }

        /**
         * Sets the flag of whether this connection will follow redirects returned
         * by the remote server.
         *
         * @param follow
         *      the value to enable or disable this option.
         */
        static void setFollowRedirects(bool follow) {
            followRedirects = follow;
        }

    };

}}

#endif /* _DECAF_NET_HTTPURLCONNECTION_H_ */
