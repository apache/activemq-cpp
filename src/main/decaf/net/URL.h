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
#include <string>

namespace decaf{
namespace net{

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
     */
    class DECAF_API URL {
    public:

        URL();
        URL( const std::string& url );
        virtual ~URL() {}

    };

}}

#endif /*_DECAF_NET_URL_H_*/
