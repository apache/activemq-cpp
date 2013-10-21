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

#ifndef _DECAF_NET_URLSTREAMHANDLER_H_
#define _DECAF_NET_URLSTREAMHANDLER_H_

#include <decaf/util/Config.h>

#include <decaf/net/URL.h>

namespace decaf {
namespace net {

    class URLConnection;

    /**
     * The abstract class URLStreamHandler is the common superclass for all stream protocol
     * handlers. A stream protocol handler knows how to make a connection for a particular
     * protocol type, such as http, ftp, or https etc.
     *
     * In most cases, an instance of a URLStreamHandler subclass is not created directly by
     * an application. Rather, the first time a protocol name is encountered when constructing
     * a URL, the appropriate stream protocol handler is automatically created from a registered
     * stream handler factory.
     *
     * @since 1.0
     */
    class DECAF_API URLStreamHandler {
    private:

        URLStreamHandler(const URLStreamHandler&);
        URLStreamHandler& operator= (const URLStreamHandler&);

    public:

        virtual ~URLStreamHandler();

    protected:

        /**
         * Establishes a new connection to the resource specified by the given URL.
         * Since different protocols also have unique ways of connecting, it must be
         * overridden by the subclass.
         *
         * @param url
         *      the URL to the resource where a connection has to be opened.
         *
         * @return pointer to the opened URLConnection to the specified resource.
         *
         * @throws IOException if an I/O error occurs during opening the connection.
         */
        virtual URLConnection* openConnection(const URL& url) = 0;

    };

}}

#endif /* _DECAF_NET_URLSTREAMHANDLER_H_ */
