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

#include <decaf/lang/String.h>
#include <decaf/net/InetAddress.h>
#include <decaf/net/URL.h>

namespace decaf {
namespace net {

    class URLConnection;
    class Proxy;

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

        /**
         * Establishes a new connection to the resource specified by the given URL.
         * The version uses a Proxy to establish the connection if proxying is supported
         * by the URLConnection implementation in use, otherwise the proxy is ignored.
         *
         * @param url
         *      the URL to the resource where a connection has to be opened.
         * @param proxy
         *      the Proxy through which the connection should be made.
         *
         * @return pointer to the opened URLConnection to the specified resource.
         *
         * @throws IOException if an I/O error occurs during opening the connection.
         * @throws UnsupportedOperationException if the protocol in use doesn't support this.
         */
        virtual URLConnection* openConnection(const URL& url, const Proxy* proxy);

        /**
         * Provides the default equals calculation. May be overridden by handlers for
         * other protocols that have different requirements for equals().
         *
         * @param source
         *      The URL class that is invoking this comparison.
         * @param other
         *      The URL that is being compared to source.
         *
         * @return true if the two URL instances are considered equal.
         */
        virtual bool equals(const URL& source, const URL& other) const;

        /**
         * Provides the default hashCode calculation. May be overridden by handlers for
         * other protocols that have different algorithms for computing a hash code..
         *
         * @param url
         *      The URL class that is invoking this hash code computation.
         */
        virtual int hashCode(const URL& url) const;

        /**
         * Compares the host components of two URLs.
         *
         * @param source
         *      The URL class that is invoking this comparison.
         * @param other
         *      The URL that is being compared to source.
         *
         * @return true if the two URL instances are considered to point to the same host.
         */
        virtual bool hostsEqual(const URL& source, const URL& other) const;

        /**
         * Compare two URLs to see whether they refer to the same file, i.e., having
         * the same protocol, host, port, and path.
         *
         * @param source
         *      The URL class that is invoking this comparison.
         * @param other
         *      The URL that is being compared to source.
         *
         * @return true if the two URL instances are considered to be the same file.
         */
        virtual bool sameFile(const URL& source, const URL& other) const;

        /**
         * Converts the URL into a string representation.
         *
         * @param url
         *      The URL to construct the string from.
         *
         * @return string form of the provided URL.
         */
        virtual decaf::lang::String toExternalForm(const URL& url) const;

        /**
         * Attempts to get the IP address of the host.  Based on DNS errors or other conditions
         * it's possible this method will return an empty string.
         *
         * @param url
         *      The URL to construct the string from.
         *
         * @return the INetAddress object that the host points to.
         */
        virtual InetAddress getHostAddress(const URL& url) const;

        /**
         * Returns the default port for a URL parsed by this handler. This method should
         * be overidden by handlers with knowledge of the default port numbers as the default
         * implementation simply returns -1.
         *
         * @return the default port number for this URL's protocol.
         */
        virtual int getDefaultPort() const;

        /**
         * Parses the clear text URL in spec into a URL object. URL strings
         * generally have the following format:
         *
         * <p>
         *    http://www.company.com/java/file1.java#reference
         * <p>
         *
         * The string is parsed in HTTP format. If the protocol has a different URL
         * format this method must be overridden.
         *
         * @param url
         *      the URL to fill in the parsed clear text URL parts.
         * @param spec
         *      the URL string that is to be parsed.
         * @param start
         *      the string position from where to begin parsing.
         * @param limit
         *      the string position to stop parsing.
         */
        virtual void parseURL(URL& url, const decaf::lang::String& spec, int start, int limit);

        /**
         * Sets the fields of the URL  This method is only intended to be used by subclasses of
         * URLStreamHandler so it is kept protected.
         *
         * @param protocol
         *      the name of the protocol to use.
         * @param host
         *      the name of the host.
         * @param port
         *      the specific port number of the URL. Value of -1 represents the
         *      default port of the protocol.
         * @param authority
         *      the authority value.
         * @param userInfo
         *      the user info value.
         * @param path
         *      the path value.
         * @param query
         *      the query value.
         * @param ref
         *      the internal reference in the URL
         *
         * @throws SecurityException if the protocol handler of the URL is not this instance.
         */
        void setURL(URL& url,
                    const decaf::lang::String& protocol, const decaf::lang::String& host, int port,
                    const decaf::lang::String& authority, const decaf::lang::String& userInfo,
                    const decaf::lang::String& path, const decaf::lang::String& query,
                    const decaf::lang::String& ref);

    private:

        friend class URL;

    };

}}

#endif /* _DECAF_NET_URLSTREAMHANDLER_H_ */
