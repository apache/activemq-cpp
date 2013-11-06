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
#include <decaf/internal/net/URLUtils.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
URLStreamHandler::~URLStreamHandler() {}

////////////////////////////////////////////////////////////////////////////////
namespace {

    String relativePath(const String& base, const String& path) {

        if (path.startsWith("/")) {
            return URLUtils::canonicalizePath(path, true);
        } else if (base != "") {
            String combined = base.substring(0, base.lastIndexOf('/') + 1) + path;
            return URLUtils::canonicalizePath(combined, true);
        } else {
            return path;
        }
    }
}

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

    if (s1 != s2) {
        return false;
    }

    s1 = source.getQuery();
    s2 = other.getQuery();

    return s1 == s2;
}

////////////////////////////////////////////////////////////////////////////////
int URLStreamHandler::hashCode(const URL& url) const {
    return hashCode(toExternalForm(url));
}

////////////////////////////////////////////////////////////////////////////////
bool URLStreamHandler::sameFile(const URL& source, const URL& other) const {

    String s1 = source.getProtocol();
    String s2 = other.getProtocol();

    if (s1 != s2) {
        return false;
    }

    s1 = source.getFile();
    s2 = other.getFile();
    if (s1 != s2) {
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
    String host1 = URLUtils::getHost(source);
    String host2 = URLUtils::getHost(other);
    if (host1.isEmpty() && host2.isEmpty()) {
        return true;
    }

    return host1.compareToIgnoreCase(host2) == 0;
}

////////////////////////////////////////////////////////////////////////////////
String URLStreamHandler::toExternalForm(const URL& url) const {
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

    return String(answer);
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
        if ((limit <= Integer::MIN_VALUE + 1 && (start >= spec.length() || start < 0)) ||
            (spec.startsWith("//", start) && spec.indexOf('/', start + 2) == -1)) {
            throw StringIndexOutOfBoundsException(__FILE__, __LINE__, limit);
        }
        if (this != url.getURLStreamHandler()) {
            throw SecurityException(__FILE__, __LINE__, "Only the URL's stream handler can modify");
        }
        return;
    }

    int fileStart;
    String authority;
    String userInfo;
    String host;
    int port = -1;
    String path;
    String query;
    String ref;

    if (spec.regionMatches(start, "//", 0, 2)) {
        // Parse the authority from the spec.
        int authorityStart = start + 2;
        fileStart = URLUtils::findFirstOf(spec, "/?#", authorityStart, limit);
        authority = spec.substring(authorityStart, fileStart);

        int userInfoEnd = URLUtils::findFirstOf(spec, "@", authorityStart, fileStart);
        int hostStart;
        if (userInfoEnd != fileStart) {
            userInfo = spec.substring(authorityStart, userInfoEnd);
            hostStart = userInfoEnd + 1;
        } else {
            userInfo = "";
            hostStart = authorityStart;
        }

        /*
         * Extract the host and port. The host may be an IPv6 address with
         * colons like "[::1]", in which case we look for the port delimiter
         * colon after the ']' character.
         */
        int colonSearchFrom = hostStart;
        int ipv6End = URLUtils::findFirstOf(spec, "]", hostStart, fileStart);
        if (ipv6End != fileStart) {
            if (URLUtils::findFirstOf(spec, ":", hostStart, ipv6End) == ipv6End) {
                throw IllegalArgumentException(__FILE__, __LINE__,
                    (std::string("Expected an IPv6 address: ") +
                     spec.substring(hostStart, ipv6End + 1).toString()).c_str());
            }
            colonSearchFrom = ipv6End;
        }
        int hostEnd = URLUtils::findFirstOf(spec, ":", colonSearchFrom, fileStart);
        host = spec.substring(hostStart, hostEnd);
        int portStart = hostEnd + 1;
        if (portStart < fileStart) {
            port = Integer::parseInt(spec.substring(portStart, fileStart).toString());
            if (port < 0) {
                throw IllegalArgumentException(__FILE__, __LINE__, "port < 0: %d", port);
            }
        }
        path = "";
        query = "";
        ref = "";
    } else {
        // Get the authority from the context URL.
        fileStart = start;
        authority = url.getAuthority();
        userInfo = url.getUserInfo();
        host = url.getHost();
        port = url.getPort();
        path = url.getPath();
        query = url.getQuery();
        ref = url.getRef();
    }

    /*
     * Extract the path, query and fragment. Each part has its own leading
     * delimiter character. The query can contain slashes and the fragment
     * can contain slashes and question marks.
     *    / path ? query # fragment
     */
    int pos = fileStart;
    while (pos < limit) {
        int nextPos;
        switch (spec.charAt(pos)) {
        case '#':
            nextPos = limit;
            ref = spec.substring(pos + 1, nextPos);
            break;
        case '?':
            nextPos = URLUtils::findFirstOf(spec, "#", pos, limit);
            query = spec.substring(pos + 1, nextPos);
            ref = "";
            break;
        default:
            nextPos = URLUtils::findFirstOf(spec, "?#", pos, limit);
            path = relativePath(path, spec.substring(pos, nextPos));
            query = "";
            ref = "";
            break;
        }
        pos = nextPos;
    }

    path = URLUtils::authoritySafePath(authority, path);

    setURL(url, url.getProtocol(), host, port, authority, userInfo, path, query, ref);
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
