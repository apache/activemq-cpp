/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "FloatingPointParser.h"
#include <decaf/lang/Math.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Character.h>
#include <decaf/internal/util/HexStringParser.h>
#include <decaf/internal/util/BigInt.h>
#include <decaf/internal/util/BitOps.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <apr_lib.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
double FloatingPointParser::parseDblImpl( const std::string& value, int exp )
    throw ( exceptions::NumberFormatException ) {

    // assumes s is a null terminated string with at least one
    // character in it
    unsigned long long def[17] = {0};
    unsigned long long defBackup[17] = {0};
    unsigned long long* f = def;
    unsigned long long* fNoOverflow = defBackup;
    unsigned long long* tempBackup = NULL;
    unsigned int overflow = 0;
    unsigned long long result = 0;
    int index = 1;
    int unprocessedDigits = 0;
    std::string::const_iterator valItr = value.begin();

    do {

        if( *valItr >= '0' && *valItr <= '9' ) {

            // Make a back up of f before appending, so that we can
            // back out of it if there is no more room, i.e. index >
            // MAX_ACCURACY_WIDTH.

            memcpy( fNoOverflow, f, sizeof(unsigned long long) * index );
            overflow = BigInt::simpleAppendDecimalDigitHighPrecision(
                    f, index, *valItr - '0' );

            if( overflow ) {
                f[index++] = overflow;
                /* There is an overflow, but there is no more room
                 * to store the result. We really only need the top 52
                 * bits anyway, so we must back out of the overflow,
                 * and ignore the rest of the string.
                 */
                if( index >= MAX_ACCURACY_WIDTH ) {
                    index--;
                    memcpy( f, fNoOverflow, sizeof(unsigned long long) * index);
                    break;
                }

                if( tempBackup ) {
                    fNoOverflow = tempBackup;
                }
            }
        } else {
            index = -1;
        }

    } while( index > 0 && *(++valItr) != '\0' );

    /* We've broken out of the parse loop either because we've reached
     * the end of the string or we've overflowed the maximum accuracy
     * limit of a double. If we still have unprocessed digits in the
     * given string, then there are three possible results:
     *   1. (unprocessed digits + e) == 0, in which case we simply
     *      convert the existing bits that are already parsed
     *   2. (unprocessed digits + e) < 0, in which case we simply
     *      convert the existing bits that are already parsed along
     *      with the given e
     *   3. (unprocessed digits + e) > 0 indicates that the value is
     *      simply too big to be stored as a double, so return Infinity
     */
    if( ( unprocessedDigits = value.length() ) > 0 ) {

        exp += unprocessedDigits;
        if( index > -1 ) {

            if( exp == 0 ) {
                return BigInt::toDoubleHighPrecision( f, index );
            } else if( exp < 0 ) {
                return BitOps::CREATE_DOUBLE_BITS( f, index, exp );
            } else {
                result = BitOps::INFINITE_LONGBITS;
            }

        } else {

            BitOps::LOW_I32_FROM_LONG64( result ) = -1;
            BitOps::HIGH_I32_FROM_LONG64( result ) = -1;
        }
    } else {

        if( index > -1 ) {
            if( exp == 0 ) {
                return BigInt::toDoubleHighPrecision( f, index );
            } else {
                return BitOps::CREATE_DOUBLE_BITS( f, index, exp );
            }
        } else {
            BitOps::LOW_I32_FROM_LONG64( result ) = -1;
            BitOps::HIGH_I32_FROM_LONG64( result ) = -1;
        }
    }

    return Double::longBitsToDouble( result );
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::parseFltImpl( const std::string& value, int exp )
    throw ( exceptions::NumberFormatException ) {

    float flt = (float)exp; // TODO - FloatOps::createFloat( str, exp );

    if( Float::floatToIntBits( flt ) >= 0 ) {
        return flt;
    } else if( Float::floatToIntBits( flt ) == -1 ) {
      throw exceptions::NumberFormatException(
          __FILE__, __LINE__,
          "FloatingPointParser::parseFltImpl - Not a valid float string",
          value.c_str() );
    }

    return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
FloatingPointParser::StringExponentPair FloatingPointParser::initialParse(
    const std::string& value, int length ) throw ( exceptions::NumberFormatException ){

    std::string newValue = value;
    bool negative = false;
    char c;
    int start = 0;
    int end = 0;
    int decimal = 0;
    int e = 0;

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - Invalid zero length string");
    }

    c = newValue[length - 1];
    if( c == 'D' || c == 'd' || c == 'F' || c == 'f') {
        length--;
        if( length == 0 ) {
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "FloatingPointParser::initialParse - Invalid zero length string");
        }
    }

    end = Math::max( (int)newValue.find_first_of( 'E' ),
                     (int)newValue.find_first_of( 'e' ) );

    if( end > -1 ) {

        if( end + 1 == length ) {
            if( length == 0 ) {
                throw exceptions::NumberFormatException(
                    __FILE__, __LINE__,
                    "FloatingPointParser::initialParse - Invalid no data after e",
                    newValue.c_str() );
            }
        }

        int exponent_offset = end + 1;

        if( newValue[exponent_offset] == '+' ) {
            if( newValue[exponent_offset + 1] == '-' ) {
                if( length == 0 ) {
                    throw exceptions::NumberFormatException(
                        __FILE__, __LINE__,
                        "FloatingPointParser::initialParse - "
                        "Invalid length string");
                }
            }
            exponent_offset++; // skip the plus sign
        }

        try {
            e = Integer::parseInt( newValue.substr( exponent_offset, length ) );
        } catch( exceptions::NumberFormatException ex ) {
            // ex contains the exponent substring only so throw a
            // new exception with the correct string
            throw exceptions::NumberFormatException(
                __FILE__, __LINE__,
                "FloatingPointParser::initialParse - "
                "exponent string is not valid" );
        }

    } else {
        end = length;
    }

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - "
            "invalid length string", newValue.c_str() );
    }

    c = newValue[start];

    if( c == '-') {
        ++start;
        --length;
        negative = true;
    } else if( c == '+' ) {
        ++start;
        --length;
    }

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - "
            "invalid length string", newValue.c_str() );
    }

    decimal = newValue.find_first_of( '.' );
    if( decimal > -1 ) {
        e -= end - decimal - 1;
        newValue = newValue.substr( start, decimal ) + newValue.substr( decimal + 1, end );
    } else {
        newValue = newValue.substr( start, end );
    }

    if( ( length = newValue.length() ) == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::initialParse - "
            "invalid length string", newValue.c_str() );
    }

    end = length;
    while( end > 1 && newValue[end - 1] == '0' ) {
        --end;
    }

    start = 0;
    while( start < end - 1 && newValue[start] == '0' ) {
        start++;
    }

    if( end != length || start != 0 ) {
        e += length - end;
        newValue = newValue.substr( start, end );
    }

    return StringExponentPair( newValue, e, negative );
}

////////////////////////////////////////////////////////////////////////////////
double FloatingPointParser::parseDblName(
    const std::string& namedDouble, int length )
        throw( exceptions::NumberFormatException ) {

    // Valid strings are only +Nan, NaN, -Nan, +Infinity, Infinity,
    // -Infinity.
    if( (length != 3) && ( length != 4) && ( length != 8) && ( length != 9 ) ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseDblName - "
            "invalid name string", namedDouble.c_str() );
    }

    bool negative = namedDouble.at(0) == '-' ? true : false;

    if( namedDouble.find_first_of( "Infinity" ) != string::npos ) {
        return negative ? Double::NEGATIVE_INFINITY : Float::POSITIVE_INFINITY;
    }

    if( namedDouble.find_first_of( "NaN" ) != string::npos ) {
        return Double::NaN;
    }

    throw exceptions::NumberFormatException(
        __FILE__, __LINE__,
        "FloatingPointParser::parseDblName - "
        "invalid name string", namedDouble.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::parseFltName(
    const std::string& namedFloat, int length )
        throw( exceptions::NumberFormatException ) {

    // Valid strings are only +Nan, NaN, -Nan, +Infinity, Infinity,
    // -Infinity.
    if( ( length != 3 ) && ( length != 4 ) && ( length != 8 ) && ( length != 9 ) ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseFltName - "
            "invalid name string", namedFloat.c_str() );
    }

    bool negative = namedFloat.at(0) == '-' ? true : false;

    if( namedFloat.find_first_of( "Infinitiy" ) != string::npos ) {
        return negative ? Float::NEGATIVE_INFINITY : Float::POSITIVE_INFINITY;
    }

    if( namedFloat.find_first_of( "NaN" ) != string::npos ) {
        return Float::NaN;
    }

    throw exceptions::NumberFormatException(
        __FILE__, __LINE__,
        "FloatingPointParser::parseFltName - "
        "invalid name string", namedFloat.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
double FloatingPointParser::parseDouble( const std::string& value )
    throw( exceptions::NumberFormatException ) {

    std::string newValue = value;
    FloatingPointParser::trim( newValue );
    int length = newValue.length();

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseDouble - "
            "invalid length string", value.c_str() );
    }

    // See if this could be a named double
    char last = newValue[length - 1];

    if( (last == 'y') || (last == 'N') ) {
        return parseDblName( newValue, length );
    }

    // See if it could be a hexadecimal representation
    if( toLowerCase( newValue ).find_first_of( "0x" ) != string::npos ) {
        return HexStringParser::parseDouble(value);
    }

    StringExponentPair info = initialParse(value, length);

    double result = parseDblImpl( info.value, info.exp );

    if( info.negative ) {
        result = -result;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::parseFloat( const std::string& value )
    throw( exceptions::NumberFormatException ) {

    std::string newValue = value;
    FloatingPointParser::trim( newValue );

    int length = newValue.length();

    if( length == 0 ) {
        throw exceptions::NumberFormatException(
            __FILE__, __LINE__,
            "FloatingPointParser::parseFloat - "
            "invalid length string", value.c_str() );
    }

    // See if this could be a named float
    char last = newValue[length - 1];
    if( (last == 'y') || (last == 'N') ) {
        return parseFltName( value, length );
    }

    // See if it could be a hexadecimal representation
    if( toLowerCase( newValue ).find_first_of( "0x" ) != string::npos ) {
        return HexStringParser::parseFloat( newValue );
    }

    StringExponentPair info = initialParse( newValue, length );

    float result = parseFltImpl( info.value, info.exp );
    if( info.negative ) {
        result = -result;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string& FloatingPointParser::trim( std::string& value ) {

    char const* delims = " \t\r\n";

    // trim leading whitespace
    string::size_type notwhite = value.find_first_not_of( delims );
    value.erase( 0, notwhite );

    // trim trailing whitespace
    notwhite = value.find_last_not_of( delims );
    value.erase( notwhite+1 );

    return value;
}

////////////////////////////////////////////////////////////////////////////////
std::string& FloatingPointParser::toLowerCase( std::string& value ) {

    string::size_type index = 0;

    for( ; index < value.size(); ++index ) {
        if( Character::isLetter( value[index] ) ) {
            value[index] = apr_tolower( value[index] );
        }
    }

    return value;
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::createFloat( const std::string&s, int exp ) {

    // assumes s is a null terminated string with at least one
    // character in it
    unsigned long long def[DEFAULT_WIDTH] = {0};
    unsigned long long defBackup[DEFAULT_WIDTH] = {0};
    unsigned long long* f = def;
    unsigned long long* fNoOverflow = defBackup;
    unsigned long long* tempBackup = NULL;
    unsigned int overflow;
    unsigned int result;
    int index = 1;
    int unprocessedDigits = 0;
    std::string::const_iterator sIter = s.begin();

    do {

        if( *sIter >= '0' && *sIter <= '9' ) {

            /* Make a back up of f before appending, so that we can
             * back out of it if there is no more room, i.e. index >
             * MAX_ACCURACY_WIDTH.
             */
            memcpy( fNoOverflow, f, sizeof(unsigned long long)* index );
            overflow = BigInt::simpleAppendDecimalDigitHighPrecision(
                    f, index, *sIter - '0' );

            if( overflow ) {

                f[index++] = overflow;

                // There is an overflow, but there is no more room
                // to store the result. We really only need the top 52
                // bits anyway, so we must back out of the overflow,
                // and ignore the rest of the string.
                if( index >= MAX_ACCURACY_WIDTH ) {
                    index--;
                    memcpy( f, fNoOverflow, sizeof(unsigned long long)* index );
                    break;
                }

                if( tempBackup ) {
                  fNoOverflow = tempBackup;
                }
            }

        } else {
            index = -1;
        }
    }
    while( index > 0 && *(++sIter) != '\0' );

    // We've broken out of the parse loop either because we've reached
    // the end of the string or we've overflowed the maximum accuracy
    // limit of a double. If we still have unprocessed digits in the
    // given string, then there are three possible results:
    //   1. (unprocessed digits + e) == 0, in which case we simply
    //      convert the existing bits that are already parsed
    //   2. (unprocessed digits + e) < 0, in which case we simply
    //      convert the existing bits that are already parsed along
    //      with the given e
    //   3. (unprocessed digits + e) > 0 indicates that the value is
    //      simply too big to be stored as a double, so return Infinity

    if( ( unprocessedDigits = s.size() ) > 0 ) {

        exp += unprocessedDigits;
        if( index > -1 ) {
            if( exp <= 0 ) {
                return createFloat1( f, index, exp );
            } else {
                result = BitOps::INFINITE_INTBITS;
            }
        } else {
            result = index;
        }
    } else {

        if( index > -1 ) {
            return createFloat1( f, index, exp );
        } else {
            result = index;
        }
    }

    return Float::intBitsToFloat( result );
}

////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::createFloat1( unsigned long long* f, int length, int exp ) {

    int numBits = BigInt::highestSetBitHighPrecision (f, length) + 1;
    double dresult;
    int result;

    numBits -= BigInt::lowestSetBitHighPrecision( f, length );

    if( numBits < 25 && exp >= 0 && exp < LOG5_OF_TWO_TO_THE_N ) {
        return ((float) LOW_I32_FROM_PTR (f)) * tenToTheE (exp);
    } else if( numBits < 25 && exp < 0 && (-exp) < LOG5_OF_TWO_TO_THE_N ) {
        return ((float) LOW_I32_FROM_PTR (f)) / tenToTheE (-exp);
    } else if (exp >= 0 && exp < 39) {
        result = (float) (BigInt::toDoubleHighPrecision (f, length) * pow (10.0, exp));
    } else if( exp >= 39 ) {
        // Convert the partial result to make sure that the
        // non-exponential part is not zero. This check fixes the case
        // where the user enters 0.0e309! */
        dresult = BigInt::toDoubleHighPrecision( f, length );

        if( dresult == 0.0 ) {
            result = BitOps::MINIMUM_INTBITS;
        } else {
            result = BitOps::INFINITE_INTBITS;
        }

    } else if( exp > -309 ) {

        int dexp;
        unsigned int fmant, fovfl;
        unsigned long long dmant;
        dresult = BigInt::toDoubleHighPrecision( f, length ) / Math::pow( 10.0, -exp );
        if( IS_DENORMAL_DBL( dresult ) ) {
            return 0.0f;
        }

        dexp = BigInt::doubleExponent( dresult ) + 51;
        dmant = BigInt::doubleMantissa( dresult );

        // Is it too small to be represented by a single-precision
        // float?
        if( dexp <= -155 ) {
            return 0.0f;
        }

        // Is it a denormalized single-precision float?
        if( (dexp <= -127) && (dexp > -155) ) {

            // Only interested in 24 msb bits of the 53-bit double mantissa
            fmant = (unsigned int) (dmant >> 29);
            fovfl = ((unsigned int) (dmant & 0x1FFFFFFF)) << 3;

            while( (dexp < -127) && ((fmant | fovfl) != 0) ) {
                if( (fmant & 1) != 0 ) {
                    fovfl |= 0x80000000;
                }
                fovfl >>= 1;
                fmant >>= 1;
                dexp++;
            }

            if( (fovfl & 0x80000000) != 0) {
                if( (fovfl & 0x7FFFFFFC) != 0 ) {
                    fmant++;
                } else if( (fmant & 1) != 0 ) {
                    fmant++;
                }
            } else if( (fovfl & 0x40000000) != 0 ) {

                if( (fovfl & 0x3FFFFFFC) != 0 ) {
                    fmant++;
                }
            }

            result = fmant;
        }
        else
        {
            result = dresult;
        }
    }

    // Don't go straight to zero as the fact that x*0 = 0 independent
    // of x might cause the algorithm to produce an incorrect result.
    // Instead try the min  value first and let it fall to zero if need be.
    if( exp <= -309 || result == 0 ) {
        result = BitOps::MINIMUM_INTBITS;
    }

    return floatAlgorithm( f, length, exp, Float::intBitsToFloat( result ) );
}

/* The algorithm for the function floatAlgorithm() below can be found
 * in:
 *
 *      "How to Read Floating-Point Numbers Accurately", William D.
 *      Clinger, Proceedings of the ACM SIGPLAN '90 Conference on
 *      Programming Language Design and Implementation, June 20-22,
 *      1990, pp. 92-101.
 *
 * There is a possibility that the function will end up in an endless
 * loop if the given approximating floating-point number (a very small
 * floating-point whose value is very close to zero) straddles between
 * two approximating integer values. We modified the algorithm slightly
 * to detect the case where it oscillates back and forth between
 * incrementing and decrementing the floating-point approximation. It
 * is currently set such that if the oscillation occurs more than twice
 * then return the original approximation.
 */
////////////////////////////////////////////////////////////////////////////////
float FloatingPointParser::floatAlgorithm(
    unsigned long long* f, int length, int exp, float z ) {

    unsigned long long m;
    int k = 0;
    int comparison = 0;
    int comparison2 = 0;
    unsigned long long* x = NULL;
    unsigned long long* y = NULL;
    unsigned long long* D = NULL;
    unsigned long long* D2 = NULL;
    int xLength = 0;
    int yLength = 0;
    int DLength = 0;
    int D2Length = 0;
    int decApproxCount = 0;
    int incApproxCount = 0;

      do {

          m = BigInt::floatMantissa (z);
          k = BigInt::floatExponent (z);

          if( exp >= 0 && k >= 0 ) {

              xLength = sizeOfTenToTheE( exp ) + length;
              x = new unsigned long long[xLength];
              memset( x + length, 0, sizeof(unsigned long long) * (xLength - length) );
              memcpy( x, f, sizeof(unsigned long long) * length );
              BigInt::timesTenToTheEHighPrecision( x, xLength, exp );

              yLength = (k >> 6) + 2;
              y = new unsigned long long[yLength];
              memset( y + 1, 0, sizeof(unsigned long long) * (yLength - 1) );
              *y = m;
              BigInt::simpleShiftLeftHighPrecision (y, yLength, k);

        } else if( exp >= 0 ) {

            xLength = sizeOfTenToTheE (exp) + length + ((-k) >> 6) + 1;
              x = new unsigned long long[xLength];
              memset( x + length, 0, sizeof(unsigned long long) * (xLength - length) );
              memcpy( x, f, sizeof(unsigned long long) * length );
              BigInt::timesTenToTheEHighPrecision( x, xLength, exp );
              BigInt::simpleShiftLeftHighPrecision( x, xLength, -k );

              yLength = 1;
              y = new unsigned long long[yLength];
              *y = m;

        } else if( k >= 0 ) {

            xLength = length;
            x = f;

            yLength = sizeOfTenToTheE( -exp ) + 2 + ( k >> 6 );
            y = new unsigned long long[yLength];
            memset( y + 1, 0, sizeof(unsigned long long) * (yLength - 1)) ;
            *y = m;
            BigInt::timesTenToTheEHighPrecision( y, yLength, -exp );
            BigInt::simpleShiftLeftHighPrecision( y, yLength, k );

        } else {

            xLength = length + ((-k) >> 6) + 1;
            x = new unsigned long long[xLength];
            memset( x + length, 0, sizeof(unsigned long long) * (xLength - length) );
            memcpy( x, f, sizeof (unsigned long long) * length );
            BigInt::simpleShiftLeftHighPrecision( x, xLength, -k );

            yLength = sizeOfTenToTheE( -exp ) + 1;
            y = new unsigned long long[yLength];
            memset( y + 1, 0, sizeof(unsigned long long) * (yLength - 1) );
            *y = m;
            BigInt::timesTenToTheEHighPrecision( y, yLength, -exp );
        }

        comparison = BigInt::compareHighPrecision( x, xLength, y, yLength );
        if( comparison > 0 ) {
            // x > y
            DLength = xLength;
            D = new unsigned long long[DLength];
            memcpy( D, x, DLength * sizeof(unsigned long long) );
            BigInt::subtractHighPrecision( D, DLength, y, yLength );
        } else if( comparison ) {
            // y > x
            DLength = yLength;
            D = new unsigned long long[DLength];
            memcpy (D, y, DLength * sizeof (unsigned long long));
            BigInt::subtractHighPrecision (D, DLength, x, xLength);
        } else {
            /* y == x */
            DLength = 1;
            D = new unsigned long long[DLength];
            *D = 0;
        }

        D2Length = DLength + 1;
        allocateU64 (D2, D2Length);
        m <<= 1;
        BigInt::multiplyHighPrecision (D, DLength, &m, 1, D2, D2Length);
        m >>= 1;

        comparison2 = BigInt::compareHighPrecision (D2, D2Length, y, yLength);
        if( comparison2 < 0 ) {
            if( comparison < 0 && m == NORMAL_MASK ) {

                BigInt::simpleShiftLeftHighPrecision (D2, D2Length, 1);
                if( BigInt::compareHighPrecision (D2, D2Length, y, yLength) > 0 ) {
                    DECREMENT_FLOAT (z, decApproxCount, incApproxCount);
                } else {
                    break;
                }
            } else {
                break;
            }
        } else if (comparison2 == 0) {

            if ((m & 1) == 0) {
                if (comparison < 0 && m == NORMAL_MASK) {
                    DECREMENT_FLOAT (z, decApproxCount, incApproxCount);
                } else {
                    break;
                }
            } else if (comparison < 0) {
                DECREMENT_FLOAT (z, decApproxCount, incApproxCount);
                break;
            } else {
                INCREMENT_FLOAT (z, decApproxCount, incApproxCount);
                break;
            }
        } else if (comparison < 0) {
          DECREMENT_FLOAT (z, decApproxCount, incApproxCount);
        } else {
              if (FLOAT_TO_INTBITS (z) == EXPONENT_MASK)
                  break;
              INCREMENT_FLOAT (z, decApproxCount, incApproxCount);
        }
    }
    while (1);

    delete x;
    delete y;
    delete D;
    delete D2;

    return z;
}


/**
#define INCREMENT_FLOAT(_x, _decCount, _incCount) \
    { \
        ++FLOAT_TO_INTBITS(_x); \
        _incCount++; \
        if( (_incCount > 2) && (_decCount > 2) ) { \
            if( _decCount > _incCount ) { \
                FLOAT_TO_INTBITS(_x) += _decCount - _incCount; \
            } else if( _incCount > _decCount ) { \
                FLOAT_TO_INTBITS(_x) -= _incCount - _decCount; \
            } \
            break; \
        } \
    }
#define DECREMENT_FLOAT(_x, _decCount, _incCount) \
    { \
        --FLOAT_TO_INTBITS(_x); \
        _decCount++; \
        if( (_incCount > 2) && (_decCount > 2) ) { \
            if( _decCount > _incCount ) { \
                FLOAT_TO_INTBITS(_x) += _decCount - _incCount; \
            } else if( _incCount > _decCount ) { \
                FLOAT_TO_INTBITS(_x) -= _incCount - _decCount; \
            } \
            break; \
        } \
    }
*/
