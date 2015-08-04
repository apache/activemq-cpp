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

#include <decaf/net/HttpURLConnection.h>

#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/net/ProtocolException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
bool HttpURLConnection::followRedirects = true;
const int HttpURLConnection::DEFAULT_CHUNK_LENGTH = 1024;

const int HttpURLConnection::HTTP_ACCEPTED = 202;
const int HttpURLConnection::HTTP_BAD_GATEWAY = 502;
const int HttpURLConnection::HTTP_BAD_METHOD = 405;
const int HttpURLConnection::HTTP_BAD_REQUEST = 400;
const int HttpURLConnection::HTTP_CLIENT_TIMEOUT = 408;
const int HttpURLConnection::HTTP_CONFLICT = 409;
const int HttpURLConnection::HTTP_CREATED = 201;
const int HttpURLConnection::HTTP_ENTITY_TOO_LARGE = 413;
const int HttpURLConnection::HTTP_FORBIDDEN = 403;
const int HttpURLConnection::HTTP_GATEWAY_TIMEOUT = 504;
const int HttpURLConnection::HTTP_GONE = 410;
const int HttpURLConnection::HTTP_INTERNAL_ERROR = 500;
const int HttpURLConnection::HTTP_LENGTH_REQUIRED = 411;
const int HttpURLConnection::HTTP_MOVED_PERM = 301;
const int HttpURLConnection::HTTP_MOVED_TEMP = 302;
const int HttpURLConnection::HTTP_MULT_CHOICE = 300;
const int HttpURLConnection::HTTP_NO_CONTENT = 204;
const int HttpURLConnection::HTTP_NOT_ACCEPTABLE = 406;
const int HttpURLConnection::HTTP_NOT_AUTHORITATIVE = 203;
const int HttpURLConnection::HTTP_NOT_FOUND = 404;
const int HttpURLConnection::HTTP_NOT_IMPLEMENTED = 501;
const int HttpURLConnection::HTTP_NOT_MODIFIED = 304;
const int HttpURLConnection::HTTP_OK = 200;
const int HttpURLConnection::HTTP_PARTIAL = 206;
const int HttpURLConnection::HTTP_PAYMENT_REQUIRED = 402;
const int HttpURLConnection::HTTP_PRECON_FAILED = 412;
const int HttpURLConnection::HTTP_PROXY_AUTH = 407;
const int HttpURLConnection::HTTP_REQ_TOO_LONG = 414;
const int HttpURLConnection::HTTP_RESET = 205;
const int HttpURLConnection::HTTP_SEE_OTHER = 303;
const int HttpURLConnection::HTTP_USE_PROXY = 305;
const int HttpURLConnection::HTTP_UNAUTHORIZED = 401;
const int HttpURLConnection::HTTP_UNSUPPORTED_TYPE = 415;
const int HttpURLConnection::HTTP_UNAVAILABLE = 503;
const int HttpURLConnection::HTTP_VERSION = 505;

////////////////////////////////////////////////////////////////////////////////
HttpURLConnection::HttpURLConnection(const URL& url) : URLConnection(url),
                                                       method("GET"),
                                                       responseCode(-1),
                                                       responseMessage(),
                                                       instanceFollowRedirects(true),
                                                       chunkLength(-1),
                                                       fixedContentLength(-1) {
}

////////////////////////////////////////////////////////////////////////////////
HttpURLConnection::~HttpURLConnection() {
}

////////////////////////////////////////////////////////////////////////////////
int HttpURLConnection::getResponseCode() {
    // Call getInputStream() first since getHeaderField() doesn't return exceptions
    getInputStream();
    std::string response = getHeaderField(0);
    if (response == "") {
        return -1;
    }

    // TODO response = response.trim();
    std::size_t mark = response.find_first_of(" ") + 1;
    if (mark == std::string::npos) {
        return -1;
    }

    std::size_t last = mark + 3;
    if (last > response.length()) {
        last = response.length();
    }

    responseCode = Integer::parseInt(response.substr(mark, mark + last));
    if (last + 1 <= response.length()) {
        responseMessage = response.substr(last + 1);
    }
    return responseCode;
}

////////////////////////////////////////////////////////////////////////////////
std::string HttpURLConnection::getResponseMessage() {
    if (responseMessage != "") {
        return responseMessage;
    }
    getResponseCode();
    return responseMessage;
}

////////////////////////////////////////////////////////////////////////////////
void HttpURLConnection::setRequestMethod(const std::string& method DECAF_UNUSED) {
    if (connected) {
        throw ProtocolException(__FILE__, __LINE__, "Connection already established");
    }

    // TODO
//    for (int i = 0; i < methodTokens.length; i++) {
//        if (methodTokens[i].equals(method)) {
//            // if there is a supported method that matches the desired
//            // method, then set the current method and return
//            this.method = methodTokens[i];
//            return;
//        }
//    }

    // if none matches, then throw ProtocolException
    throw new ProtocolException();
}

////////////////////////////////////////////////////////////////////////////////
void HttpURLConnection::setFixedLengthStreamingMode(int contentLength) {
    if (connected) {
        throw IllegalStateException(__FILE__, __LINE__, "Already connected");
    }

    if (0 < chunkLength) {
        throw IllegalStateException(__FILE__, __LINE__, "different mode already set");
    }

    if (0 > contentLength) {
        throw IllegalArgumentException(__FILE__, __LINE__, "scale value < than zero");
    }

    fixedContentLength = contentLength;
}

////////////////////////////////////////////////////////////////////////////////
void HttpURLConnection::setChunkedStreamingMode(int chunklen) {
    if (connected) {
        throw IllegalStateException(__FILE__, __LINE__, "Already connected");
    }

    if (0 <= fixedContentLength) {
        throw IllegalStateException(__FILE__, __LINE__, "different mode already set");
    }

    if (0 >= chunklen) {
        chunkLength = DEFAULT_CHUNK_LENGTH;
    } else {
        chunkLength = chunklen;
    }
}
