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
const std::string URIEncoderDecoder::digits = "0123456789ABCDEF";

////////////////////////////////////////////////////////////////////////////////
URIEncoderDecoder::URIEncoderDecoder() {
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoder::validate( const std::string& s, const std::string& legal)
    throw ( URISyntaxException ) {

    std::string::const_iterator itr = s.begin();

    for( int i = 0; itr != s.end(); ++i, ++iter ) {

        if( ch == '%' ) {
            if( i + 2 >= s.length() ) {
                throw URISyntaxException(
                    __FILE__, __LINE__, s,
                    "invalid Encoded data", i );
            }

            int d1 = Character::digit( *(++iter), 16 );
            int d2 = Character::digit( *(++iter), 16 );

            if( d1 == -1 || d2 == -1 ) {
                throw URISyntaxException(
                    __FILE__, __LINE__, s,
                    "Invalid Hex Digit in char", i );
            }

            i += 2;

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

////////////////////////////////////////////////////////////////////////////////
std::string URIEncoderDecoder::quoteIllegal( const std::string& s,
                                             const std::string& legal );

    std::string buf = "";
    std::string::const_iterator iter = s.begin();

    for( ; iter != s.end(); ++iter ) {

        char ch = *iter;

        if( Character::isLetterOrDigit( ch ) ||
            legal.find( ch ) > string::npos ||
            ( (unsigned char)ch > 127 &&
              !Character.isSpaceChar(ch) &&
              !Character.isISOControl(ch) ) ) {

            buf.append(ch);
        } else {
            buf.append('%');
            buf.append( digits.at( ( ch & 0xf0 ) >> 4 ) );
            buf.append( digits.at( bytes[j] & 0xf ) );
        }
    }

    return buf;
}

////////////////////////////////////////////////////////////////////////////////
std::string URIEncoderDecoder::encodeOthers( const std::string& s ) {
    std::string buf = "";
    for( int i = 0; i < s.length(); i++ ) {
        char ch = s.at(i);
        if( ch <= 127 ) {
            buf.append( ch );
        } else {
            buf.append('%');
            buf.append( digits.at( ( ch & 0xf0 ) >> 4 ) );
            buf.append( digits.at( ch & 0xf ) );
        }
    }
    return buf;
}

////////////////////////////////////////////////////////////////////////////////
std::string URIEncoderDecoder::decode( const std::string& s ) {

    std::string result = "";
    for( int i = 0; i < s.length(); ) {
        char c = s.charAt(i);
        if( c == '%' ) {

            do {
                if( i + 2 >= s.length() ) {
                    throw IllegalArgumentException(
                        __FILE__, __LINE__,
                        "String has invalid encoding: %s", s.c_str() );
                }

                int d1 = Character.digit( s.at(i + 1), 16);
                int d2 = Character.digit( s.at(i + 2), 16);

                if( d1 == -1 || d2 == -1 ) {
                    throw IllegalArgumentException(
                        __FILE__, __LINE__,
                        "URIEncoderDecoder::decode - Bad hex encoding in: ",
                        s.c_str() );
                }

                result.append( (unsigned char)( ( d1 << 4 ) + d2 ) );
                i += 3;

            } while( i < s.length() && s.at(i) == '%' );

            continue;
        }

        result.append(c);
        i++;
    }

    return result;
}
