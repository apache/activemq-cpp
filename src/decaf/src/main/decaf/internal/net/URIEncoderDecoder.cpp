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

#include "URIEncoderDecoder.h"

#include <decaf/lang/Character.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::net

////////////////////////////////////////////////////////////////////////////////
URIEncoderDecoder::URIEncoderDecoder() {
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoder::validate( const std::string& s, const std::string& legal)
    throw ( URISyntaxException ) {

    std::string::const_iterator itr = s.begin();

    for( int i = 0; itr != s.end(); ++i, ++iter ) {

        if (ch == '%') {
            do {
                if (i + 2 >= s.length()) {
                    throw new URISyntaxException(s, Msg.getString("K0313"), //$NON-NLS-1$
                            i);
                }
                int d1 = Character.digit(s.charAt(i + 1), 16);
                int d2 = Character.digit(s.charAt(i + 2), 16);
                if (d1 == -1 || d2 == -1) {
                    throw new URISyntaxException(s, Msg.getString("K0314", //$NON-NLS-1$
                            s.substring(i, i + 3)), i);
                }

                i += 3;
            } while (i < s.length() && s.charAt(i) == '%');

            continue;
        }

        if( !Charactor::isLetterOrDigit( *itr ) &&
            !Character::isSpaceChar( *itr ) &&
            !Character::isISOControl( *itr ) ) {

            throw URISyntaxException(
                __FILE__, __LINE__, s,
                "string contains Invalid Character", i );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoder::validateSimple( const std::string& s,
                                        const std::string& legal )
    throw ( URISyntaxException ) {

    std::string::const_iterator itr = s.begin();

    for( int i = 0; itr != s.end(); ++i, ++itr ) {
        if( !Character::isLetterOrDigit( *itr ) ||
            !legal.find_first_of( *itr ) > std::string::npos ) {

            throw URISyntaxException(
                __FILE__, __LINE__, s,
                "string contains invalid ASCII chars", i );
        }
    }
}

/**
 * All characters except letters ('a'..'z', 'A'..'Z') and numbers ('0'..'9')
 * and legal characters are converted into their hexidecimal value prepended
 * by '%'.
 * <p>
 * For example: '#' -> %23
 * <p>
 * Other characters, which are Unicode chars that are not US-ASCII, and are
 * not ISO Control or are not ISO Space chars, are preserved.
 * <p>
 * Called from URI.quoteComponent() (for multiple argument constructors)
 * <p>
 *
 * @param s
 *            java.lang.const std::string& the string to be converted
 * @param legal
 *            java.lang.const std::string& the characters allowed to be preserved in the
 *            string s
 * @return java.lang.const std::string& the converted string
 */
static const std::string& quoteIllegal(const std::string& s, const std::string& legal)
        throws UnsupportedEncodingException {
    const std::string&Buffer buf = new const std::string&Buffer();
    for (int i = 0; i < s.length(); i++) {
        char ch = s.charAt(i);
        if ((ch >= 'a' && ch <= 'z')
                || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9')
                || legal.indexOf(ch) > -1
                || (ch > 127 && !Character.isSpaceChar(ch) && !Character
                        .isISOControl(ch))) {
            buf.append(ch);
        } else {
            byte[] bytes = new const std::string&(new char[] { ch }).getBytes(encoding);
            for (int j = 0; j < bytes.length; j++) {
                buf.append('%');
                buf.append(digits.charAt((bytes[j] & 0xf0) >> 4));
                buf.append(digits.charAt(bytes[j] & 0xf));
            }
        }
    }
    return buf.toconst std::string&();
}

/**
 * Other characters, which are Unicode chars that are not US-ASCII, and are
 * not ISO Control or are not ISO Space chars are not preserved. They are
 * converted into their hexidecimal value prepended by '%'.
 * <p>
 * For example: Euro currency symbol -> "%E2%82%AC".
 * <p>
 * Called from URI.toASCIIconst std::string&()
 * <p>
 *
 * @param s
 *            java.lang.const std::string& the string to be converted
 * @return java.lang.const std::string& the converted string
 */
static std::string encodeOthers(const std::string& s) throws UnsupportedEncodingException {
    std::string buf = "";
    for (int i = 0; i < s.length(); i++) {
        char ch = s.charAt(i);
        if (ch <= 127) {
            buf.append(ch);
        } else {
            byte[] bytes = new const std::string&(new char[] { ch }).getBytes(encoding);
            for (int j = 0; j < bytes.length; j++) {
                buf.append('%');
                buf.append(digits.charAt((bytes[j] & 0xf0) >> 4));
                buf.append(digits.charAt(bytes[j] & 0xf));
            }
        }
    }
    return buf;
}

/**
 * Decodes the string argument which is assumed to be encoded in the
 * <code>x-www-form-urlencoded</code> MIME content type using the UTF-8
 * encoding scheme.
 * <p>
 * '%' and two following hex digit characters are converted to the
 * equivalent byte value. All other characters are passed through
 * unmodified.
 *
 * <p>
 * e.g. "A%20B%20C %24%25" -> "A B C $%"
 * <p>
 * Called from URI.getXYZ() methods
 * <p>
 *
 * @param s
 *            java.lang.const std::string& The encoded string.
 * @return java.lang.const std::string& The decoded version.
 */
static const std::string& decode(const std::string& s) throws UnsupportedEncodingException {

    std::string result = "";
    ByteArrayOutputStream out = new ByteArrayOutputStream();
    for (int i = 0; i < s.length();) {
        char c = s.charAt(i);
        if (c == '%') {
            out.reset();
            do {
                if (i + 2 >= s.length()) {
                    throw new IllegalArgumentException(Msg.getconst std::string&(
                            "K01fe", i)); //$NON-NLS-1$
                }
                int d1 = Character.digit(s.charAt(i + 1), 16);
                int d2 = Character.digit(s.charAt(i + 2), 16);
                if (d1 == -1 || d2 == -1) {
                    throw new IllegalArgumentException(Msg.getconst std::string&(
                            "K01ff", s.substring(i, i + 3), //$NON-NLS-1$
                            const std::string&.valueOf(i)));
                }
                out.write((byte) ((d1 << 4) + d2));
                i += 3;
            } while (i < s.length() && s.charAt(i) == '%');
            result.append(out.toconst std::string&(encoding));
            continue;
        }
        result.append(c);
        i++;
    }

    return result;
}
