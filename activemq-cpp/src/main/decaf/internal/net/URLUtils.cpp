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

#include <decaf/internal/net/URLUtils.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
URLUtils::URLUtils() {
}

////////////////////////////////////////////////////////////////////////////////
String URLUtils::getHost(const URL& url) {
    String host = url.getHost();
    if (url.getProtocol().equals("file") && host.isEmpty()) {
        return "localhost";
    }
    return host;
}

////////////////////////////////////////////////////////////////////////////////
String URLUtils::canonicalizePath(const String& original, bool discardRelativePrefix) {

    String path = original;

    // the first character of the current path segment
    int segmentStart = 0;

    // the number of segments seen thus far that can be erased by sequences of '..'.
    int deletableSegments = 0;

    for (int i = 0; i <= path.length(); ) {
        int nextSegmentStart;
        if (i == path.length()) {
            nextSegmentStart = i;
        } else if (path.charAt(i) == '/') {
            nextSegmentStart = i + 1;
        } else {
            i++;
            continue;
        }

        /*
         * We've encountered either the end of a segment or the end of the
         * complete path. If the final segment was "." or "..", remove the
         * appropriate segments of the path.
         */
        if (i == segmentStart + 1 && path.regionMatches(segmentStart, ".", 0, 1)) {
            // Given "abc/def/./ghi", remove "./" to get "abc/def/ghi".
            path = path.substring(0, segmentStart) + path.substring(nextSegmentStart);
            i = segmentStart;
        } else if (i == segmentStart + 2 && path.regionMatches(segmentStart, "..", 0, 2)) {
            if (deletableSegments > 0 || discardRelativePrefix) {
                // Given "abc/def/../ghi", remove "def/../" to get "abc/ghi".
                deletableSegments--;
                int prevSegmentStart = path.lastIndexOf('/', segmentStart - 2) + 1;
                path = path.substring(0, prevSegmentStart) + path.substring(nextSegmentStart);
                i = segmentStart = prevSegmentStart;
            } else {
                // There's no segment to delete; this ".." segment must be retained.
                i++;
                segmentStart = i;
            }
        } else {
            if (i > 0) {
                deletableSegments++;
            }
            i++;
            segmentStart = i;
        }
    }
    return path;
}

////////////////////////////////////////////////////////////////////////////////
String URLUtils::authoritySafePath(const String& authority, const String& path) {
    if (!authority.isEmpty() && !path.isEmpty() && !path.startsWith("/")) {
        return String("/") + path;
    }
    return path;
}

////////////////////////////////////////////////////////////////////////////////
bool URLUtils::isValidSchemeChar(int index, char c) {

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return true;
    }
    if (index > 0 && ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
String URLUtils::getSchemePrefix(const String& spec) {
    int colon = spec.indexOf(':');

    if (colon < 1) {
        return String();
    }

    for (int i = 0; i < colon; i++) {
        char c = spec.charAt(i);
        if (!isValidSchemeChar(i, c)) {
            return String();
        }
    }

    return spec.substring(0, colon).toLowerCase();
}

////////////////////////////////////////////////////////////////////////////////
int URLUtils::findFirstOf(const String& string, const String& chars, int start, int end) {
    for (int i = start; i < end; i++) {
        char c = string.charAt(i);
        if (chars.indexOf(c) != -1) {
            return i;
        }
    }
    return end;
}
