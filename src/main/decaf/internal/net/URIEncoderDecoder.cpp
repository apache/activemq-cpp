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
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
const std::string URIEncoderDecoder::digits = "0123456789ABCDEF";

////////////////////////////////////////////////////////////////////////////////
URIEncoderDecoder::URIEncoderDecoder() {
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoder::validate( const std::string& s, const std::string& legal)
    throw ( URISyntaxException ) {

    std::string::const_iterator itr = s.begin();

    for( std::size_t i = 0; itr != s.end(); ++i, ++itr ) {

        char ch = s.at(i);

        if( ch == '%' ) {
            if( i + 2 >= s.length() ) {
                throw URISyntaxException(
                    __FILE__, __LINE__, s,
                    "invalid Encoded data", (int)i );
            }

            int d1 = Character::digit( *(++itr), 16 );
            int d2 = Character::digit( *(++itr), 16 );

            if( d1 == -1 || d2 == -1 ) {
                throw URISyntaxException(
                    __FILE__, __LINE__, s,
                    "Invalid Hex Digit in char", (int)i );
            }

            i += 2;

            continue;
        }

        if( !( Character::isLetterOrDigit( *itr ) ||
               legal.find( *itr, 0 ) != std::string::npos ||
               ( (unsigned char)ch > 127 &&
                 ( !Character::isWhitespace(ch) ||
                   !Character::isISOControl(ch) ) ) ) ) {

            throw URISyntaxException(
                __FILE__, __LINE__, s,
                "string contains Invalid Character", (int)i );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoder::validateSimple( const std::string& s,
                                        const std::string& legal )
    throw ( URISyntaxException ) {

    std::string::const_iterator itr = s.begin();

    for( int i = 0; itr != s.end(); ++i, ++itr ) {

        if( Character::isLetterOrDigit( *itr ) ||
            legal.find( *itr ) != std::string::npos ) {

            continue;
        }

        throw URISyntaxException(
            __FILE__, __LINE__, s,
            "string contains invalid ASCII chars", (int)i );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string URIEncoderDecoder::quoteIllegal( const std::string& s,
                                             const std::string& legal ) {

    std::string buf = "";
    std::string::const_iterator iter = s.begin();

    for( ; iter != s.end(); ++iter ) {

        char ch = *iter;

        if( Character::isLetterOrDigit( ch ) ||
            legal.find( ch ) != std::string::npos ||
            ( (unsigned char)ch > 127 &&
              ( !Character::isWhitespace(ch) &&
                !Character::isISOControl(ch) ) ) ) {

            buf += ch;
        } else {
            buf += '%';
            buf += digits.at( ( ch & 0xf0 ) >> 4 );
            buf += digits.at( ch & 0xf );
        }
    }

    return buf;
}

////////////////////////////////////////////////////////////////////////////////
std::string URIEncoderDecoder::encodeOthers( const std::string& s ) {
    std::string buf = "";
    for( std::size_t i = 0; i < s.length(); i++ ) {
        char ch = s.at(i);
        if( (unsigned char)ch <= 127 ) {
            buf += ch;
        } else {
            buf += '%';
            buf += digits.at( ( ch & 0xf0 ) >> 4 );
            buf += digits.at( ch & 0xf );
        }
    }
    return buf;
}

////////////////////////////////////////////////////////////////////////////////
std::string URIEncoderDecoder::decode( const std::string& s ) {

    std::string result = "";
    for( std::size_t i = 0; i < s.length(); ) {

        char c = s.at(i);

        if( c == '%' ) {

            do {
                if( i + 2 >= s.length() ) {
                    throw IllegalArgumentException(
                        __FILE__, __LINE__,
                        "String has invalid encoding: %s", s.c_str() );
                }

                int d1 = Character::digit( s.at(i + 1), 16);
                int d2 = Character::digit( s.at(i + 2), 16);

                if( d1 == -1 || d2 == -1 ) {
                    throw IllegalArgumentException(
                        __FILE__, __LINE__,
                        "URIEncoderDecoder::decode - Bad hex encoding in: ",
                        s.c_str() );
                }

                result += (unsigned char)( ( d1 << 4 ) + d2 );
                i += 3;

            } while( i < s.length() && s.at(i) == '%' );

            continue;
        }

        result += c;
        i++;
    }

    return result;
}
