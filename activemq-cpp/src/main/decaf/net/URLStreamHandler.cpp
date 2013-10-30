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

#include <decaf/net/URLStreamHandler.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/SecurityException.h>
#include <decaf/lang/exceptions/StringIndexOutOfBoundsException.h>
#include <decaf/util/HashCode.h>
#include <decaf/internal/util/StringUtils.h>
#include <decaf/net/UnknownHostException.h>
#include <decaf/lang/Integer.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * File based URL instance with an empty host value are always considered
     * to have a host value of "localhost".
     */
    String getHost(const URL& url) {
        String host = url.getHost();
        if (url.getProtocol().equals("file") && host.isEmpty()) {
            return "localhost";
        }
        return host;
    }

    /**
     * Canonicalize the path, i.e. remove ".." and "." occurrences.
     *
     * @param path the path to be canonicalized
     *
     * @return the canonicalized path
     */
    String canonicalizePath(const String& original) {
        String path = original;
        int dirIndex;

        while ((dirIndex = path.indexOf("/./")) >= 0) {
            path = path.substring(0, dirIndex + 1) + path.substring(dirIndex + 3);
        }

        if (path.endsWith("/.")) {
            path = path.substring(0, path.length() - 1);
        }

        while ((dirIndex = path.indexOf("/../")) >= 0) {
            if (dirIndex != 0) {
                path = path.substring(0, path.lastIndexOf('/', dirIndex - 1)) + path.substring(dirIndex + 3);
            } else {
                path = path.substring(dirIndex + 3);
            }
        }

        if (path.endsWith("/..") && path.length() > 3) {
            path = path.substring(0, path.lastIndexOf('/', path.length() - 4) + 1);
        }
        return path;
    }
}

////////////////////////////////////////////////////////////////////////////////
URLStreamHandler::~URLStreamHandler() {}

////////////////////////////////////////////////////////////////////////////////
URLConnection* URLStreamHandler::openConnection(const URL& url DECAF_UNUSED, const Proxy* proxy DECAF_UNUSED) {
    throw UnsupportedOperationException(__FILE__, __LINE__, "method has not been implemented yet");
}

////////////////////////////////////////////////////////////////////////////////
bool URLStreamHandler::equals(const URL& source, const URL& other) const {
    if (!sameFile(source, other)) {
        return false;
    }

    String s1 = source.getRef();
    String s2 = other.getRef();

    if (s1.isEmpty() || s1 != s2) {
        return false;
    }

    s1 = source.getQuery();
    s2 = other.getQuery();

    return s1.isEmpty() && s1 != s2;
}

////////////////////////////////////////////////////////////////////////////////
int URLStreamHandler::hashCode(const URL& url) const {
    return hashCode(toExternalForm(url));
}

////////////////////////////////////////////////////////////////////////////////
bool URLStreamHandler::sameFile(const URL& source, const URL& other) const {

    String s1 = source.getProtocol();
    String s2 = other.getProtocol();

    if (s1.isEmpty() || s1 != s2) {
        return false;
    }

    s1 = source.getFile();
    s2 = other.getFile();
    if (s1.isEmpty() || s1 != s2) {
        return false;
    }

    if (!hostsEqual(source, other)) {
        return false;
    }

    int p1 = source.getPort();
    if (p1 == -1) {
        p1 = getDefaultPort();
    }

    int p2 = other.getPort();
    if (p2 == -1) {
        p2 = getDefaultPort();
    }

    return p1 == p2;
}

////////////////////////////////////////////////////////////////////////////////
bool URLStreamHandler::hostsEqual(const URL& source, const URL& other) const {
    // TODO need a NULL address.
//    // Compare by addresses if known.
//    InetAddress address1 = getHostAddress(source);
//    InetAddress address2 = getHostAddress(other);
//    if (address1 != null && address2 != null) {
//        return address1.equals(address2);
//    }

    // Compare by name.
    String host1 = getHost(source);
    String host2 = getHost(other);
    if (host1.isEmpty() && host2.isEmpty()) {
        return true;
    }

    return host1.compareToIgnoreCase(host2) == 0;
}

////////////////////////////////////////////////////////////////////////////////
std::string URLStreamHandler::toExternalForm(const URL& url) const {
    std::string answer;
    answer.append(url.getProtocol().toString());
    answer.append(":");
    std::string authority = url.getAuthority().toString();
    if (!authority.empty()) {
        answer.append("//");
        answer.append(url.getAuthority().toString());
    }

    String file = url.getFile();
    String ref = url.getRef();

    if (!file.isEmpty()) {
        answer.append(file.toString());
    }

    if (!ref.isEmpty()) {
        answer.append("#");
        answer.append(ref.toString());
    }

    return answer;
}

////////////////////////////////////////////////////////////////////////////////
InetAddress URLStreamHandler::getHostAddress(const URL& url) const {
    // TODO
    try {
        String host = url.getHost();
        if (host.isEmpty()) {
            return InetAddress::getLocalHost();  // null
        }
        //return InetAddress::getByName(host);
        return InetAddress::getLocalHost();  // null
    } catch (UnknownHostException& e) {
        return InetAddress::getLocalHost();  // null
    }
}

////////////////////////////////////////////////////////////////////////////////
int URLStreamHandler::getDefaultPort() const {
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
void URLStreamHandler::parseURL(URL& url, const String& spec, int start, int limit) {

    if (limit < start || limit < 0) {
        if (((limit <= Integer::MIN_VALUE + 1) && (start >= spec.length() || start < 0)) ||
            (spec.startsWith("//", start) && spec.indexOf('/', start + 2) == -1)) {

            throw StringIndexOutOfBoundsException(__FILE__, __LINE__, limit);
        }
        if (this != url.getURLStreamHandler()) {
            throw new SecurityException();
        }
        return;
    }

    String parseString = spec.substring(start, limit);
    limit -= start;
    int fileIdx = 0;

    // Default is to use info from context
    String host = url.getHost();
    int port = url.getPort();
    String ref = url.getRef();
    String file = url.getPath();
    String query = url.getQuery();
    String authority = url.getAuthority();
    String userInfo = url.getUserInfo();

    int refIdx = parseString.indexOf('#', 0);
    if (parseString.startsWith("//") && !parseString.startsWith("////")) {
        int hostIdx = 2;
        int portIdx = -1;

        port = -1;
        fileIdx = parseString.indexOf('/', hostIdx);
        int questionMarkIndex = parseString.indexOf('?', hostIdx);

        if ((questionMarkIndex != -1) && ((fileIdx == -1) || (fileIdx > questionMarkIndex))) {
            fileIdx = questionMarkIndex;
        }

        if (fileIdx == -1) {
            fileIdx = limit;
            // Use default
            file = "";
        }
        int hostEnd = fileIdx;
        if (refIdx != -1 && refIdx < fileIdx) {
            hostEnd = refIdx;
        }
        int userIdx = parseString.lastIndexOf('@', hostEnd);
        authority = parseString.substring(hostIdx, hostEnd);
        if (userIdx > -1) {
            userInfo = parseString.substring(hostIdx, userIdx);
            hostIdx = userIdx + 1;
        }

        portIdx = parseString.indexOf(':', userIdx == -1 ? hostIdx : userIdx);
        int endOfIPv6Addr = parseString.indexOf(']');
        // if there are square braces, ie. IPv6 address, use last ':'
        if (endOfIPv6Addr != -1) {
            try {
                if (parseString.length() > endOfIPv6Addr + 1) {
                    char c = parseString.charAt(endOfIPv6Addr + 1);
                    if (c == ':') {
                        portIdx = endOfIPv6Addr + 1;
                    } else {
                        portIdx = -1;
                    }
                } else {
                    portIdx = -1;
                }
            } catch (Exception& e) {
                // Ignored
            }
        }

        if (portIdx == -1 || portIdx > fileIdx) {
            host = parseString.substring(hostIdx, hostEnd);
        } else {
            host = parseString.substring(hostIdx, portIdx);
            String portString = parseString.substring(portIdx + 1, hostEnd);
            if (portString.length() == 0) {
                port = -1;
            } else {
                port = Integer::parseInt(portString.toString());
            }
        }
    }

    if (refIdx > -1) {
        ref = parseString.substring(refIdx + 1, limit);
    }
    int fileEnd = (refIdx == -1 ? limit : refIdx);

    int queryIdx = parseString.lastIndexOf('?', fileEnd);
    bool canonicalize = false;
    if (queryIdx > -1) {
        query = parseString.substring(queryIdx + 1, fileEnd);
        if (queryIdx == 0) {
            if (file.equals("")) {
                file = "/";
            } else if (file.startsWith("/")) {
                canonicalize = true;
            }
            int last = file.lastIndexOf('/') + 1;
            file = file.substring(0, last);
        }
        fileEnd = queryIdx;
    } else if (refIdx != 0) {
        // Don't inherit query unless only the ref is changed
        query = "";
    }

    if (fileIdx > -1) {
        if (fileIdx < limit && parseString.charAt(fileIdx) == '/') {
            file = parseString.substring(fileIdx, fileEnd);
        } else if (fileEnd > fileIdx) {
            if (file.equals("")) {
                file = "/"; //$NON-NLS-1$
            } else if (file.startsWith("/")) {
                canonicalize = true;
            }
            int last = file.lastIndexOf('/') + 1;
            if (last == 0) {
                file = parseString.substring(fileIdx, fileEnd);
            } else {
                file = file.substring(0, last)
                        + parseString.substring(fileIdx, fileEnd);
            }
        }
    }

    if (canonicalize) {
        // modify file if there's any relative referencing
        file = canonicalizePath(file);
    }

    setURL(url, url.getProtocol(), host, port, authority, userInfo, file, query, ref);
}

////////////////////////////////////////////////////////////////////////////////
void URLStreamHandler::setURL(URL& url, const String& protocol, const String& host, int port,
                              const String& authority, const String& userInfo,
                              const String& path, const String& query, const String& ref) {

    if (this != url.getURLStreamHandler()) {
        throw SecurityException(__FILE__, __LINE__, "Stream handler is not the URLs intance.");
    }

    url.set(protocol, host, port, authority, userInfo, path, query, ref);
}
