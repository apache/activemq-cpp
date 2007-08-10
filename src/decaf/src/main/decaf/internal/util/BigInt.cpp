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

#include "BigInt.h"

#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>
#include <decaf/internal/util/BitOps.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
BigInt::BigInt() {
}

////////////////////////////////////////////////////////////////////////////////
void BigInt::multiplyHighPrecision( unsigned long long* arg1,
                                    int length1,
                                    unsigned long long* arg2,
                                    int length2,
                                    unsigned long long* result,
                                    int length ) {

    // assumes result is large enough to hold product
    unsigned long long *temp;
    unsigned int *resultIn32;
    int count, index;

    if( length1 < length2 ) {
        temp = arg1;
        arg1 = arg2;
        arg2 = temp;
        count = length1;
        length1 = length2;
        length2 = count;
    }

    memset( result, 0, sizeof(unsigned long long) * length );

    // length1 > length2
    resultIn32 = (unsigned int*)result;
    index = -1;
    for( count = 0; count < length2; ++count ) {
        simpleMultiplyAddHighPrecision( arg1, length1,
                                        arg2[count] & BitOps::LONG_LO_MASK,
                                        resultIn32 + (++index) );
        simpleMultiplyAddHighPrecision( arg1, length1,
                                        ( arg2[count] >> 32 ),
                                        resultIn32 + (++index) );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned int BigInt::simpleAppendDecimalDigitHighPrecision(
    unsigned long long* arg1, int length, unsigned long long digit ) {

    // assumes digit is less than 32 bits
    unsigned long long arg;
    int index = 0;

    digit <<= 32;
    do {
        arg = arg1[index] & BitOps::LONG_LO_MASK;
        digit = ( digit >> 32 ) + BitOps::TIMES_TEN( arg );
        ((BitOps::LONG_UNION*)(arg1 + index ))->intValue[BitOps::LoWord] =
            (int)digit & BitOps::LONG_LO_MASK;

        arg = arg1[index] >> 32;
        digit = ( digit >> 32 ) + BitOps::TIMES_TEN( arg );
        ((BitOps::LONG_UNION*)(arg1 + index ))->intValue[BitOps::HiWord] =
            (int)digit & BitOps::LONG_LO_MASK;

    } while( ++index < length );

    return digit >> 32;
}

////////////////////////////////////////////////////////////////////////////////
double BigInt::toDoubleHighPrecision( unsigned long long* arg, int length ) {

    int highBit;
    unsigned long long mantissa = 0;
    unsigned long long test64 = 0;
    unsigned int test;
    long long result;

    while( length > 0 && arg[length - 1] == 0 ) {
        --length;
    }

    if( length == 0 ) {
        return 0.0;
    } else if( length > 16 ) {
        result = BitOps::EXPONENT_MASK;
    } else if( length == 1 ) {

        highBit = highestSetBit (arg);
        if( highBit <= 53 ) {
            highBit = 53 - highBit;
            mantissa = *arg << highBit;
            result = BitOps::CREATE_DOUBLE_BITS( mantissa, -highBit );
        } else {
            highBit -= 53;
            mantissa = *arg >> highBit;
            result = BitOps::CREATE_DOUBLE_BITS( mantissa, highBit );

            // perform rounding, round to even in case of tie
            test = ( BitOps::LOW_U32_FROM_LONG64_PTR( arg ) << ( 11 - highBit ) ) & 0x7FF;
            if( test > 0x400 || ((test == 0x400) && (mantissa & 1)) ) {
                result = result + 1;
            }
        }
    } else {
        highBit = highestSetBit (arg + (--length));
        if( highBit <= 53 ) {
            highBit = 53 - highBit;
            if( highBit > 0 ) {
                mantissa = (arg[length] << highBit) |
                           (arg[length - 1] >> (64 - highBit));
            } else {
                mantissa = arg[length];
            }
            result = BitOps::CREATE_DOUBLE_BITS( mantissa, length * 64 - highBit );

            // perform rounding, round to even in case of tie
            test64 = arg[--length] << highBit;
            if( test64 > BitOps::SIGN_MASK || ((test64 == BitOps::SIGN_MASK) && (mantissa & 1)) ) {
                result = result + 1;
            } else if( test64 == BitOps::SIGN_MASK ) {
                while( --length >= 0 ) {
                    if( arg[length] != 0 ) {
                        result = result + 1;
                        break;
                    }
                }
            }
        } else {
            highBit -= 53;
            mantissa = arg[length] >> highBit;
            result = BitOps::CREATE_DOUBLE_BITS( mantissa, length * 64 + highBit );

            // perform rounding, round to even in case of tie
            test = ( BitOps::LOW_U32_FROM_LONG64_PTR( arg + length ) << ( 11 - highBit ) ) & 0x7FF;
            if( test > 0x400 || ((test == 0x400) && (mantissa & 1)) ) {
                result = result + 1;
            } else if( test == 0x400 ) {
                do {
                    if( arg[--length] != 0 ) {
                        result = result + 1;
                        break;
                    }
                } while( length > 0 );
            }
        }
    }

    return Double::longBitsToDouble( result );
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::tenToTheEHighPrecision( unsigned long long * result,
                                    int length, int e ) {

    // size test
    if( length < ((e / 19) + 1) ) {
        return 0;
    }

    memset( result, 0, length * sizeof(unsigned long long) );
    *result = 1;

    if( e == 0 ) {
        return 1;
    }

    length = 1;
    length = timesTenToTheEHighPrecision( result, length, e );

    return length;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long long BigInt::doubleMantissa( double z ) {

      unsigned long long m = Double::doubleToLongBits( z );

      if ((m & BitOps::EXPONENT_MASK) != 0)
        m = (m & BitOps::MANTISSA_MASK) | BitOps::NORMAL_MASK;
      else
        m = (m & BitOps::MANTISSA_MASK);

      return m;
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::compareHighPrecision( unsigned long long* arg1,
                                  int length1,
                                  unsigned long long* arg2,
                                  int length2 ) {

    while( --length1 >= 0 && arg1[length1] == 0 );
    while( --length2 >= 0 && arg2[length2] == 0 );

    if( length1 > length2 ) {
        return 1;
    } else if( length1 < length2 ) {
        return -1;
    } else if( length1 > -1 ) {
        do {
            if( arg1[length1] > arg2[length1] ) {
                return 1;
            } else if( arg1[length1] < arg2[length1] ) {
                return -1;
            }
        } while( --length1 >= 0 );
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::highestSetBitHighPrecision( unsigned long long* arg, int length ) {

    int highBit;

    while( --length >= 0 ) {
        highBit = highestSetBit( arg + length );
        if( highBit ) {
            return highBit + 64 * length;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void BigInt::subtractHighPrecision( unsigned long long* arg1,
                                    int length1,
                                    unsigned long long* arg2,
                                    int length2 ) {

    // assumes arg1 > arg2
    for( int index = 0; index < length1; ++index ) {
        arg1[index] = ~arg1[index];
    }

    simpleAddHighPrecision( arg1, length1, 1 );

    while( length2 > 0 && arg2[length2 - 1] == 0 ) {
        --length2;
    }

    addHighPrecision( arg1, length1, arg2, length2 );

    for( int index = 0; index < length1; ++index ) {
        arg1[index] = ~arg1[index];
    }

    simpleAddHighPrecision( arg1, length1, 1 );
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::doubleExponent( double z ) {

    // assumes positive double
    int k = ( (unsigned long long )Double::doubleToLongBits( z ) >> 52 );

    if( k ) {
        k -= BitOps::E_OFFSET;
    } else {
        k = 1 - BitOps::E_OFFSET;
    }

    return k;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int BigInt::simpleMultiplyHighPrecision(
    unsigned long long* arg1, int length, unsigned long long arg2 ) {

    // assumes arg2 only holds 32 bits of information
    unsigned long long product = 0;
    int index = 0;

    do {

        product = ( product >> 32 ) + arg2 * BitOps::LOW_U32_FROM_LONG64_PTR( arg1 + index );
        BitOps::LOW_U32_FROM_LONG64_PTR( arg1 + index ) = BitOps::LOW_U32_FROM_LONG64( product );

        product = ( product >> 32 ) + arg2 * BitOps::HIGH_U32_FROM_LONG64_PTR( arg1 + index );
        BitOps::HIGH_U32_FROM_LONG64_PTR( arg1 + index ) = BitOps::LOW_U32_FROM_LONG64( product );

    } while( ++index < length );

    return ( product >> 32 );
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::addHighPrecision( unsigned long long* arg1,
                              int length1,
                              unsigned long long* arg2,
                              int length2 ) {

    // addition is limited by length of arg1 as it this function is
    // storing the result in arg1
    //
    // fix for cc (GCC) 3.2 20020903 (Red Hat Linux 8.0 3.2-7): code generated does not
    // do the temp1 + temp2 + carry addition correct.  carry is 64 bit because gcc has
    // subtle issues when you mix 64 / 32 bit maths.
    unsigned long long temp1, temp2, temp3;     /* temporary variables to help the SH-4, and gcc */
    unsigned long long carry;
    int index;

    if( length1 == 0 || length2 == 0 ) {
        return 0;
    } else if( length1 < length2 ) {
        length2 = length1;
    }

    carry = 0;
    index = 0;

    do {

        temp1 = arg1[index];
        temp2 = arg2[index];
        temp3 = temp1 + temp2;
        arg1[index] = temp3 + carry;

        if( arg2[index] < arg1[index] ) {
            carry = 0;
        } else if( arg2[index] != arg1[index] ) {
            carry = 1;
        }

    } while( ++index < length2 );

    if( !carry ) {
        return 0;
    } else if( index == length1 ) {
        return 1;
    }

    while( ++arg1[index] == 0 && ++index < length1 );

    return (int)( index == length1 );
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::lowestSetBit( unsigned long long* y ) {

    unsigned int x;
    int result;

    if( *y == 0 ) {
        return 0;
    }

    if( *y & 0x00000000FFFFFFFFULL ) {
      x = BitOps::LOW_U32_FROM_LONG64_PTR( y );
      result = 0;
    } else {
      x = BitOps::HIGH_U32_FROM_LONG64_PTR( y );
      result = 32;
    }

    if( !( x & 0xFFFF ) ) {
      x = BitOps::bitSection( x, 0xFFFF0000, 16 );
      result += 16;
    }

    if( !( x & 0xFF) ) {
      x = BitOps::bitSection( x, 0xFF00, 8 );
      result += 8;
    }
    if( !( x & 0xF ) ) {
      x = BitOps::bitSection( x, 0xF0, 4 );
      result += 4;
    }

    if( x & 0x1 ) {
        return result + 1;
    } else if( x & 0x2 ) {
        return result + 2;
    } else if( x & 0x4 ) {
        return result + 3;
    }

    return result + 4;
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::timesTenToTheEHighPrecision(
    unsigned long long* result, int length, int e ) {

    // assumes result can hold value
    unsigned long long overflow;
    int exp10 = e;

    if( e == 0 ) {
        return length;
    }

    // Replace the current implementaion which performs a
    // "multiplication" by 10 e number of times with an actual
    // mulitplication. 10e19 is the largest exponent to the power of ten
    // that will fit in a 64-bit integer, and 10e9 is the largest exponent to
    // the power of ten that will fit in a 64-bit integer. Not sure where the
    // break-even point is between an actual multiplication and a
    // simpleAappendDecimalDigit() so just pick 10e3 as that point for
    // now.
    while( exp10 >= 19 ) {
        overflow = simpleMultiplyHighPrecision64( result, length, BitOps::TEN_E19 );
        if( overflow ) {
            result[length++] = overflow;
        }
        exp10 -= 19;
    }

    while( exp10 >= 9 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E9 );
        if( overflow ) {
              result[length++] = overflow;
        }
        exp10 -= 9;
    }

    if( exp10 == 0 ) {
        return length;
    } else if( exp10 == 1 ) {
        overflow = simpleAppendDecimalDigitHighPrecision( result, length, 0 );
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 2 ) {
        overflow = simpleAppendDecimalDigitHighPrecision( result, length, 0 );
        if( overflow ) {
            result[length++] = overflow;
        }
        overflow = simpleAppendDecimalDigitHighPrecision( result, length, 0 );
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 3 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E3 );
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 4 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E4 );
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 5 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E5 );
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 6 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E6);
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 7 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E7 );
        if( overflow ) {
            result[length++] = overflow;
        }
    } else if( exp10 == 8 ) {
        overflow = simpleMultiplyHighPrecision( result, length, BitOps::TEN_E8 );
        if( overflow ) {
            result[length++] = overflow;
        }
    }

    return length;
}

////////////////////////////////////////////////////////////////////////////////
void BigInt::simpleMultiplyAddHighPrecision(
    unsigned long long* arg1, int length,
    unsigned long long arg2, unsigned int* result ) {

    // Assumes result can hold the product and arg2 only holds 32 bits
    // of information
    unsigned long long product = 0;
    int index = 0;
    int resultIndex = 0;

    do {

        product =
            ( product >> 32 ) + result[BitOps::at( resultIndex )] +
            arg2 * BitOps::LOW_U32_FROM_LONG64_PTR( arg1 + index );
        result[BitOps::at( resultIndex )] = BitOps::LOW_U32_FROM_LONG64( product );
        ++resultIndex;

        product =
            ( product >> 32 ) + result[BitOps::at( resultIndex )] +
            arg2 * BitOps::HIGH_U32_FROM_LONG64_PTR( arg1 + index );
        result[BitOps::at( resultIndex )] = BitOps::LOW_U32_FROM_LONG64( product );
        ++resultIndex;

    } while( ++index < length );

    result[BitOps::at( resultIndex )] += BitOps::HIGH_U32_FROM_LONG64( product );
    if( result[BitOps::at( resultIndex )] < BitOps::HIGH_U32_FROM_LONG64( product ) ) {
        // must be careful with ++ operator and macro expansion
        ++resultIndex;
        while( ++result[BitOps::at( resultIndex )] == 0 ) {
            ++resultIndex;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::highestSetBit(unsigned long long* y ) {

    unsigned int x = 0;
    int result = 0;

    if( *y == 0 ) {
        return 0;
    }

    if( *y & 0xFFFFFFFF00000000LL ) {
        x = BitOps::HIGH_U32_FROM_LONG64_PTR(y);
        result = 32;
    } else {
        x = BitOps::LOW_U32_FROM_LONG64_PTR(y);
        result = 0;
    }

    if( x & 0xFFFF0000 )
    {
        x = BitOps::bitSection( x, 0xFFFF0000, 16 );
        result += 16;
    }

    if( x & 0xFF00 ) {
        x = BitOps::bitSection( x, 0xFF00, 8 );
        result += 8;
    }

    if( x & 0xF0 ) {
        x = BitOps::bitSection( x, 0xF0, 4 );
        result += 4;
    }

    if( x > 0x7 ) {
        return result + 4;
    } else if( x > 0x3 ) {
        return result + 3;
    } else if( x > 0x1 ) {
        return result + 2;
    } else {
        return result + 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::lowestSetBitHighPrecision( unsigned long long* arg, int length ) {

    int lowBit, index = -1;

    while( ++index < length ) {
        lowBit = lowestSetBit( arg + index );
        if( lowBit ) {
            return lowBit + 64 * index;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void BigInt::simpleShiftLeftHighPrecision( unsigned long long* arg1,
                                           int length, int arg2 ) {

    // assumes length > 0
    int index, offset = 0;

    if( arg2 >= 64 ) {

        offset = arg2 >> 6;
        index = length;

        while( --index - offset >= 0 ) {
            arg1[index] = arg1[index - offset];
        }

        do {
            arg1[index] = 0;
        } while (--index >= 0);

        arg2 &= 0x3F;
    }

    if( arg2 == 0 ) {
        return;
    }

    while( --length > 0 ) {
        arg1[length] = arg1[length] << arg2 | arg1[length - 1] >> (64 - arg2);
    }

    *arg1 <<= arg2;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int BigInt::floatMantissa( float z ) {

    unsigned int m = (unsigned int)Float::floatToIntBits( z );

    if( ( m & BitOps::FLOAT_EXPONENT_MASK ) != 0 )
        m = ( m & BitOps::FLOAT_MANTISSA_MASK ) | BitOps::FLOAT_NORMAL_MASK;
    else
        m = ( m & BitOps::FLOAT_MANTISSA_MASK );

    return m;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long long BigInt::simpleMultiplyHighPrecision64(
    unsigned long long* arg1, int length, unsigned long long arg2 ) {

    unsigned long long intermediate = 0;
    unsigned long long* pArg1 = NULL;
    unsigned long long carry1 = 0;
    unsigned long long carry2 = 0;
    unsigned long long prod1 = 0;
    unsigned long long prod2 = 0;
    unsigned long long sum = 0;
    int index = 0;
    unsigned int buf32 = 0;

    // set the starting position
    pArg1 = &( arg1[index] );

    do {

        if( (*pArg1 != 0) || (intermediate != 0) ) {

            prod1 = (unsigned long long)BitOps::LOW_U32_FROM_LONG64(arg2) *
                    (unsigned long long)BitOps::LOW_U32_FROM_LONG64_PTR(pArg1);

            sum = intermediate + prod1;
            if( (sum < prod1) || (sum < intermediate) ) {
                carry1 = 1;
            } else {
                carry1 = 0;
            }

            prod1 = (unsigned long long)BitOps::LOW_U32_FROM_LONG64(arg2) *
                    (unsigned long long)BitOps::HIGH_U32_FROM_LONG64_PTR(pArg1);
            prod2 = (unsigned long long)BitOps::HIGH_U32_FROM_LONG64(arg2) *
                    (unsigned long long)BitOps::LOW_U32_FROM_LONG64_PTR(pArg1);

            intermediate = carry2 + ( sum >> 32 ) + prod1 + prod2;

            if( (intermediate < prod1) || (intermediate < prod2) ) {
                carry2 = 1;
            } else {
                carry2 = 0;
            }

            BitOps::LOW_U32_FROM_LONG64_PTR(pArg1) = BitOps::LOW_U32_FROM_LONG64(sum);
            buf32 = BitOps::HIGH_U32_FROM_LONG64_PTR (pArg1);
            BitOps::HIGH_U32_FROM_LONG64_PTR(pArg1) = BitOps::LOW_U32_FROM_LONG64(intermediate);
            intermediate = carry1 + ( intermediate >> 32 ) +
                           (unsigned long long)BitOps::HIGH_U32_FROM_LONG64 (arg2) *
                           (unsigned long long)buf32;
        }

        pArg1++;

    } while( ++index < length );

    return intermediate;
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::simpleAddHighPrecision( unsigned long long* arg1,
                                    int length,
                                    unsigned long long arg2 ) {

    // assumes length > 0
    int index = 1;

    *arg1 += arg2;
    if( arg2 <= *arg1 ) {
        return 0;
    } else if (length == 1) {
        return 1;
    }

    while( ++arg1[index] == 0 && ++index < length );

    return (int)( index == length );
}

////////////////////////////////////////////////////////////////////////////////
int BigInt::floatExponent( float z ) {

    // assumes positive float
    int k = Float::floatToIntBits( z ) >> 23;

    if( k ) {
        k -= BitOps::FLOAT_E_OFFSET;
    } else {
        k = 1 - BitOps::FLOAT_E_OFFSET;
    }

    return k;
}
