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

#ifndef _DECAF_NET_URL_H_
#define _DECAF_NET_URL_H_

#include <decaf/util/Config.h>
#include <decaf/lang/String.h>

#include <decaf/io/InputStream.h>
#include <string>

namespace decaf {
namespace net {

    class URI;
    class URLImpl;
    class URLStreamHandler;
    class URLStreamHandlerFactory;
    class URLConnection;
    class Proxy;

    /**
     * Class URL represents a Uniform Resource Locator, a pointer to a "resource"
     * on the World Wide Web. A resource can be something as simple as a file
     * or a directory, or it can be a reference to a more complicated object,
     * such as a query to a database or to a search engine. More information on
     * the types of URLs and their formats can be found at:
     *
     *  http://www.ksc.nasa.gov/facts/internet/url-primer.html
     *
     * In general, a URL can be broken into several parts. The previous example
     * of a URL indicates that the protocol to use is http (HyperText Transfer
     * Protocol) and that the information resides on a host machine named
     * www.ksc.nasa.gov. The information on that host machine is named
     * /facts/internet/url-primer.html. The exact meaning of this name on the
     * host machine is both protocol dependent and host dependent. The information
     * normally resides in a file, but it could be generated on the fly. This
     * component of the URL is called the path component.
     *
     * A URL can optionally specify a "port", which is the port number to which
     * the TCP connection is made on the remote host machine. If the port is not
     * specified, the default port for the protocol is used instead. For example,
     * the default port for http is 80. An alternative port could be specified as:
     *
     *  http://www.ksc.nasa.gov:80/facts/internet/url-primer.html
     *
     * The syntax of URL is defined by RFC 2396: Uniform Resource Identifiers (URI):
     * Generic Syntax, amended by RFC 2732: Format for Literal IPv6 Addresses in URLs.
     * The Literal IPv6 address format also supports scope_ids. The syntax and usage
     * of scope_ids is described here.
     *
     * A URL may have appended to it a "fragment", also known as a "ref" or a
     * "reference". The fragment is indicated by the sharp sign character "#" followed
     * by more characters. For example,
     *
     *  http://www.apache.org/cms/index.html#chapter1
     *
     * This fragment is not technically part of the URL. Rather, it indicates that
     * after the specified resource is retrieved, the application is specifically
     * interested in that part of the document that has the tag chapter1 attached to
     * it. The meaning of a tag is resource specific.
     *
     * An application can also specify a "relative URL", which contains only enough
     * information to reach the resource relative to another URL. Relative URLs
     * are frequently used within HTML pages. For example, if the contents of the URL:
     *
     *  http://www.apache.org/cms/index.html
     *
     * contained within it the relative URL:
     *
     *  FAQ.html
     *
     * it would be a shorthand for:
     *
     *  http://www.apache.org/cms/FAQ.html
     *
     * The relative URL need not specify all the components of a URL. If the protocol,
     * host name, or port number is missing, the value is inherited from the fully
     * specified URL. The file component must be specified. The optional fragment is
     * not inherited.
     *
     * The URL class does not itself encode or decode any URL components according
     * to the escaping mechanism defined in RFC2396. It is the responsibility of the
     * caller to encode any fields, which need to be escaped prior to calling URL,
     * and also to decode any escaped fields, that are returned from URL. Furthermore,
     * because URL has no knowledge of URL escaping, it does not recognise equivalence
     * between the encoded or decoded form of the same URL. For example, the two URLs:
     *
     *  http://foo.com/hello world/ and http://foo.com/hello%20world
     *
     * would be considered not equal to each other.
     *
     * Note, the URI class does perform escaping of its component fields in certain
     * circumstances. The recommended way to manage the encoding and decoding of URLs
     * is to use URI, and to convert between these two classes using toURI() and
     * URI.toURL().
     *
     * The URLEncoder and URLDecoder classes can also be used, but only for HTML form
     * encoding, which is not the same as the encoding scheme defined in RFC2396.
     *
     * @since 1.0
     */
    class DECAF_API URL {
    private:

        URLImpl* impl;

    public:

        /**
         * Creates a URL object from the String representation.
         *
         * This constructor is equivalent to a call to the two-argument constructor with
         * a empty first argument.
         *
         * @param url
         *      the String to parse as a URL.
         *
         * @throws MalformedURLException If the string specifies an unknown protocol.
         */
        URL(const decaf::lang::String& url);

        /**
         * Creates a URL by parsing the given spec within a specified context. The new URL is
         * created from the given context URL and the spec argument as described in RFC2396
         * "Uniform Resource Identifiers : Generic * Syntax" :
         *
         *     <scheme>://<authority><path>?<query>#<fragment>
         *
         * The reference is parsed into the scheme, authority, path, query and fragment parts.
         * If the path component is empty and the scheme, authority, and query components are
         * undefined, then the new URL is a reference to the current document. Otherwise, the
         * fragment and query parts present in the spec are used in the new URL.
         *
         * If the scheme component is defined in the given spec and does not match the scheme
         * of the context, then the new URL is created as an absolute URL based on the spec alone.
         * Otherwise the scheme component is inherited from the context URL.
         *
         * If the authority component is present in the spec then the spec is treated as absolute
         * and the spec authority and path will replace the context authority and path. If the
         * authority component is absent in the spec then the authority of the new URL will be
         * inherited from the context.
         *
         * If the spec's path component begins with a slash character "/" then the path is treated
         * as absolute and the spec path replaces the context path.
         *
         * Otherwise, the path is treated as a relative path and is appended to the context path,
         * as described in RFC2396. Also, in this case, the path is canonicalized through the
         * removal of directory changes made by occurrences of ".." and ".".
         *
         * For a more detailed description of URL parsing, refer to RFC2396.
         *
         * @param context
         *      the URL which is used as the context.
         * @param spec
         *      the URL string representation which has to be parsed.
         *
         * @throws MalformedURLException
         *      if the given string spec could not be parsed as a URL or an invalid
         *      protocol has been found.
         */
        URL(const URL& context, const decaf::lang::String& spec);

        /**
         * Creates a URL object from the specified protocol, host, port number, file, and
         * handler. Specifying a port number of -1 indicates that the URL should use the
         * default port for the protocol. Specifying a handler of null indicates that the
         * URL should use a default stream handler for the protocol, as outlined for:
         *
         *    URL(const String&, const String&, int, const String&)
         *
         * If a URLStreamHandler instance is provided then this class will take ownership
         * of the object and delete at a later time.
         *
         * @param protocol
         *      the name of the protocol to use.
         * @param host
         *      the name of the host.
         * @param port
         *      the port number on the host.
         * @param file
         *      the file on the host
         * @param handler
         *      the stream handler for the URL.
         *
         * @throws MalformedURLException if an unknown protocol is specified.
         */
        URL(const decaf::lang::String& protocol, const decaf::lang::String& host, int port,
            const decaf::lang::String& file, URLStreamHandler* handler);

        /**
         * Creates a URL from the specified protocol name, host name, and file name. The default
         * port for the specified protocol is used.
         *
         * This method is equivalent to calling the four-argument constructor with the arguments
         * being protocol, host, -1, and file. No validation of the inputs is performed by this
         * constructor.
         *
         * @param protocol
         *      the name of the protocol to use.
         * @param host
         *      the name of the host.
         * @param file
         *      the file on the host
         *
         * @throws MalformedURLException if an unknown protocol is specified.
         */
        URL(const decaf::lang::String& protocol, const decaf::lang::String& host, const decaf::lang::String& file);

        /**
         * Creates a new URL instance using the given arguments. The URL uses the
         * specified port instead of the default port for the given protocol.
         *
         * @param protocol
         *      the name of the protocol to use.
         * @param host
         *      the name of the host.
         * @param port
         *      the specific port number of the URL. Value of -1 represents the
         *      default port of the protocol.
         * @param file
         *      the name of the resource.
         *
         * @throws MalformedURLException
         *      if the combination of all arguments do not represent a valid
         *      URL or the protocol is invalid.
         */
        URL(const decaf::lang::String& protocol, const decaf::lang::String& host, int port, const decaf::lang::String& file);

        /**
         * Creates a URL by parsing the given spec with the specified handler within a
         * specified context. If the handler is NULL, the parsing occurs as with the two
         * argument constructor.
         *
         * If a stream handler instance is passed then this object takes ownership of it
         * and will destroy the resources when no longer needed.
         *
         * @param context
         *      the URL which is used as the context.
         * @param spec
         *      the URL string representation which has to be parsed.
         * @param handler
         *      the stream handler for the URL.
         *
         * @throws MalformedURLException if an unknown protocol is specified.
         */
        URL(const URL& context, const decaf::lang::String& spec, URLStreamHandler* handler);

        virtual ~URL();

    public:

        /**
         * Compares this URL for equality with another URL.
         *
         * Two URL objects are equal if they have the same protocol, reference equivalent
         * hosts, have the same port number on the host, and the same file and fragment
         * of the file.
         *
         * Two hosts are considered equivalent if both host names can be resolved into the
         * same IP addresses; else if either host name can't be resolved, the host names
         * must be equal without regard to case; or both host names equal to empty string.
         *
         * Since hosts comparison requires name resolution, this operation is a blocking operation.
         *
         * @return true if this URL is considered equal to the given URL instance.
         */
        bool equals(const URL& other) const;

        /**
         * Gets the authority part of this URL.
         *
         * @return the authority part of this URL.
         */
        decaf::lang::String getAuthority() const;

        /**
         * Gets the default port number of the protocol associated with this URL. If the URL
         * scheme or the URLStreamHandler for the URL do not define a default port number, then
         * -1 is returned.
         *
         * @return the default port for the given scheme.
         */
        int getDefaultPort() const;

        /**
         * Gets the file name of this URL. The returned file portion will be the same as getPath(),
         * plus the concatenation of the value of getQuery(), if any. If there is no query portion,
         * this method and getPath() will return identical results.
         *
         * @return the file name associated with this URL.
         */
        decaf::lang::String getFile() const;

        /**
         * Gets the host name of this URL, if applicable. The format of the host conforms to
         * RFC 2732, i.e. for a literal IPv6 address, this method will return the IPv6 address
         * enclosed in square brackets ('[' and ']').
         *
         * @return the host name for this URL.
         */
        decaf::lang::String getHost() const;

        /**
         * Gets the path part of this URL.
         *
         * @return the path part of this URL.
         */
        decaf::lang::String getPath() const;

        /**
         * Gets the user Info part of this URL.
         *
         * @return the user info part of this URL.
         */
        decaf::lang::String getUserInfo() const;

        /**
         * Gets the port of this URL.
         *
         * @return the port of this URL or -1 if not set.
         */
        int getPort() const;

        /**
         * Gets the protocol of this URL.
         *
         * @return the path part of this URL.
         */
        decaf::lang::String getProtocol() const;

        /**
         * Gets the query part of this URL.
         *
         * @return the query part of this URL or empty string if not set.
         */
        decaf::lang::String getQuery() const;

        /**
         * Gets the anchor or "reference" portion of this URL.
         *
         * @return the anchor or "reference" portion of this URL.
         */
        decaf::lang::String getRef() const;

        /**
         * Creates an integer hash code for this URL which is used in hash based collections.
         *
         * The hash code is based upon all the URL components relevant for URL comparison which
         * means that the host resolution may cause this operation to block.
         *
         * @return the integer has code for this URL.
         */
        int hashCode() const;

        /**
         * Returns a URLConnection object that represents a connection to the remote object
         * referred to by the URL.
         *
         * A new connection is opened every time by calling the openConnection method of the
         * protocol handler for this URL.
         *
         * If for the URL's protocol (such as HTTP), there exists a public, specialized
         * URLConnection subclass belonging to one of the following packages or one of their
         * subpackages: java.lang, java.io, java.util, java.net, the connection returned will
         * be of that subclass. For example, for HTTP an HttpURLConnection will be returned,
         * and for JAR a JarURLConnection will be returned.
         *
         * @return a new URLConnection instance for this URL.
         *
         * @throws IOException if an error occurs while opening the connection.
         */
        URLConnection* openConnection();

        /**
         * Same basic functionality as openConnection() is provided here, except that the connection
         * will be made through the specified proxy; Protocol handlers that do not support proxing
         * will ignore the proxy parameter and make a normal connection.
         *
         * @param proxy
         *      The proxy instance to use to make the connection.
         *
         * @return a new URLConnection instance for this URL.
         *
         * @throws IOException if an error occurs while opening the connection.
         * @throws IllegalArgumentException if proxy is null, or proxy has the wrong type.
         * @throws UnsupportedOperationException if this method is not supported.
         */
        URLConnection* openConnection(const Proxy* proxy);

        /**
         * Shortcut method to opens a connection to this URL and fetch an InputStream
         * for reading from that connection.
         *
         * @return an InputStream that reads from this URL's location.
         *
         * @throws IOException if an error occurs.
         */
        decaf::io::InputStream* openStream();

        /**
         * Compares this URL to the other ignoring the fragment portion to determine if both
         * reference the same remote object.
         *
         * @return true if both URL's reference the same external object.
         */
        bool sameFile(const URL& other) const;

        /**
         * Constructs a string representation of this URL, by calling the toExternalForm
         * method of the stream protocol handler for this object.
         *
         * @return the string representation of this URL.
         */
        decaf::lang::String toExternalForm() const;

        /**
         * Calls toExternalForm to create a string representation of this URL.
         *
         * @return the string representation of this URL.
         */
        std::string toString() const;

        /**
         * Returns a URI instance that is the equivalent of this URL.
         *
         * @return the URI that is the equivalent of this URL.
         */
        URI toURI() const;

    public:

        /**
         * Sets an application's URLStreamHandlerFactory. This method can be called at most once.
         *
         * The URLStreamHandlerFactory instance is used to construct a stream protocol handler
         * from a protocol name.  The provided factory becomes the property of this runtime and
         * will be deleted at shutdown.
         *
         * @param factory
         *      the desired factory.
         *
         * @throws Exception if there is already a set factory.
         */
        static void setURLStreamHandlerFactory(URLStreamHandlerFactory* factory);

    private:

        /**
         * Sets the fields of the URL. This is not a public method so that only URLStreamHandlers
         * can modify URL fields. URLs are otherwise constant.
         *
         * @param protocol
         *      the name of the protocol to use.
         * @param host
         *      the name of the host.
         * @param port
         *      the specific port number of the URL. Value of -1 represents the
         *      default port of the protocol.
         * @param file
         *      the file value.
         * @param ref
         *      the internal reference in the URL
         */
        void set(const decaf::lang::String& protocol, const decaf::lang::String& host, int port,
                 const decaf::lang::String& file, const decaf::lang::String& ref);

        /**
         * Sets the fields of the URL. This is not a public method so that only URLStreamHandlers
         * can modify URL fields. URLs are otherwise constant.
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
         */
        void set(const decaf::lang::String& protocol, const decaf::lang::String& host, int port,
                 const decaf::lang::String& authority, const decaf::lang::String& userInfo,
                 const decaf::lang::String& path, const decaf::lang::String& query,
                 const decaf::lang::String& ref);

        /**
         * Returns the URLStreamHandler configured for this URL, used to validate that
         * the operations performed by a protocol handler are only done on its own URL.
         */
        URLStreamHandler* getURLStreamHandler() const;

    private:

        void initialize(const URL* context, const decaf::lang::String& theSpec, URLStreamHandler* handler);
        void initialize(const decaf::lang::String& protocol, const decaf::lang::String& host, int port,
                        const decaf::lang::String& file, URLStreamHandler* handler);


        friend class URLStreamHandler;

    };

}}

#endif /*_DECAF_NET_URL_H_*/
