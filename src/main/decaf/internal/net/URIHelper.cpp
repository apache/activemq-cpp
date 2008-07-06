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

#include "URIHelper.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Exception.h>
#include <decaf/internal/net/URIEncoderDecoder.h>
#include <decaf/net/URISyntaxException.h>
#include <decaf/util/StringTokenizer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
URIHelper::URIHelper() {
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidDomainName( const std::string& host ) {

    try {
        URIEncoderDecoder::validateSimple( host, "-." );
    } catch( URISyntaxException& e ) {
        return false;
    }

    std::string label = "";
    StringTokenizer st( host, "." );

    while( st.hasMoreTokens() ) {
        label = st.nextToken();
        if( *(label.begin()) == '-' || *(label.rbegin()) == '-' ) {
            return false;
        }
    }

    if( label != host ) {
        if( Character::isDigit( label.at( 0 ) ) ) {
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool isValidIPv4Address( const std::string& host ) {

    int index;
    int index2;

    try {

        int num;
        index = host.find( '.' );
        num = Integer::parseInt( host.substr( 0, index ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

        index2 = host.find( '.', index + 1 );
        num = Integer::parseInt( host.substr( index + 1, index2 ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

        index = host.find( '.', index2 + 1 );
        num = Integer::parseInt( host.substr( index2 + 1, index ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

        num = Integer::parseInt( host.substr( index + 1, std::string::npos ) );
        if( num < 0 || num > 255 ) {
            return false;
        }

    } catch( Exception& e ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidIP6Address( const std::string& ipAddress ) {

    int length = ipAddress.length();
    bool doubleColon = false;
    int numberOfColons = 0;
    int numberOfPeriods = 0;
    std::string word = "";
    char c = 0;
    char prevChar = 0;
    int offset = 0; // offset for [] ip addresses

    if( length < 2 ) {
        return false;
    }

    for( int i = 0; i < length; i++ ) {

        prevChar = c;
        c = ipAddress.at( i );

        switch( c ) {

            // case for an open bracket [x:x:x:...x]
            case '[':
                if( i != 0 ) {
                    return false; // must be first character
                }
                if( ipAddress.at( length - 1 ) != ']' ) {
                    return false; // must have a close ]
                }
                if( ( ipAddress.at( 1 ) == ':' ) && ( ipAddress.at( 2 )
                    != ':' ) ) {
                    return false;
                }
                offset = 1;
                if( length < 4 ) {
                    return false;
                }
                break;

                // case for a closed bracket at end of IP [x:x:x:...x]
            case ']':
                if( i != length - 1 ) {
                    return false; // must be last character
                }
                if( ipAddress.at( 0 ) != '[' ) {
                    return false; // must have a open [
                }
                break;

                // case for the last 32-bits represented as IPv4
                // x:x:x:x:x:x:d.d.d.d
            case '.':
                numberOfPeriods++;
                if( numberOfPeriods > 3 ) {
                    return false;
                }
                if( !isValidIP4Word( word ) ) {
                    return false;
                }
                if( numberOfColons != 6 && !doubleColon ) {
                    return false;
                }
                // a special case ::1:2:3:4:5:d.d.d.d allows 7 colons
                // with
                // an IPv4 ending, otherwise 7 :'s is bad
                if( numberOfColons == 7 &&
                    ipAddress.at( 0 + offset ) != ':' &&
                    ipAddress.at( 1 + offset ) != ':' ) {

                    return false;
                }
                word = "";
                break;

            case ':':
                numberOfColons++;
                if( numberOfColons > 7 ) {
                    return false;
                }
                if( numberOfPeriods > 0 ) {
                    return false;
                }
                if( prevChar == ':' ) {
                    if( doubleColon ) {
                        return false;
                    }
                    doubleColon = true;
                }
                word = "";
                break;

            default:
                if( word.length() > 3 ) {
                    return false;
                }
                if( !isValidHexChar( c ) ) {
                    return false;
                }
                word += c;
        }
    }

    // Check if we have an IPv4 ending
    if( numberOfPeriods > 0 ) {
        if( numberOfPeriods != 3 || !isValidIP4Word( word ) ) {
            return false;
        }
    } else {
        // If we're at then end and we haven't had 7 colons then there
        // is a problem unless we encountered a doubleColon
        if( numberOfColons != 7 && !doubleColon ) {
            return false;
        }

        // If we have an empty word at the end, it means we ended in
        // either a : or a .
        // If we did not end in :: then this is invalid
        if( word == "" && ipAddress.at( length - 1 - offset ) != ':' &&
            ipAddress.at( length - 2 - offset ) != ':' ) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidIP4Word( const std::string& word ) {

    if( word.length() < 1 || word.length() > 3 ) {
        return false;
    }

    for( std::size_t i = 0; i < word.length(); i++ ) {

        if( !Character::isDigit( word.at( i ) ) ) {
            return false;
        }
    }

    if( Integer::parseInt( word ) > 255 ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidHexChar( char c ) {

    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}
