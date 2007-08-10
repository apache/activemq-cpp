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

#include "NumberConverter.h"

#include <decaf/lang/Math.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Integer.h>

#include <decaf/internal/util/BigInt.h>
#include <decaf/internal/util/BitOps.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
const double NumberConverter::invLogOfTenBaseTwo =
    Math::log(2.0) / Math::log(10.0);
NumberConverter::StaticInitializer NumberConverter::init;
std::vector<long long> NumberConverter::TEN_TO_THE;

////////////////////////////////////////////////////////////////////////////////
NumberConverter::StaticInitializer::StaticInitializer() {

    NumberConverter::TEN_TO_THE.resize(20);
    NumberConverter::TEN_TO_THE[0] = 1L;

    for( std::size_t i = 1; i < TEN_TO_THE.size(); ++i ) {
        long long previous = TEN_TO_THE[i - 1];
        TEN_TO_THE[i] = ( previous << 1 ) + ( previous << 3 );
    }
}

////////////////////////////////////////////////////////////////////////////////
NumberConverter::NumberConverter() {

    this->getCount = 0;
    this->setCount = 0;
    this->firstK = 0;
    this->uArray.resize(64);
}

////////////////////////////////////////////////////////////////////////////////
std::string NumberConverter::convertD( double value ) {

    unsigned int p = 1023 + 52; // the power offset (precision)

    // the mask to get the sign of the number
    unsigned long long signMask = 0x8000000000000000ULL;
    // the mask to get the power bits
    unsigned long long eMask = 0x7FF0000000000000ULL;
    // the mask to get the significand bits
    unsigned long long fMask = 0x000FFFFFFFFFFFFFULL;

    unsigned long long inputNumberBits = Double::doubleToLongBits( value );
    // the value of the sign... 0 is positive, ~0 is negative
    std::string signString = ( inputNumberBits & signMask ) == 0 ? "" : "-";
    // the value of the 'power bits' of the value
    unsigned int e = (int)(( inputNumberBits & eMask ) >> 52 );
    // the value of the 'significand bits' of the value
    unsigned long long f = inputNumberBits & fMask;
    bool mantissaIsZero = (f == 0);
    int pow = 0, numBits = 52;

    if( e == 2047 ) {
        return mantissaIsZero ? signString + "Infinity" : "NaN";
    }

    if( e == 0 ) {

        if( mantissaIsZero ) {
            return signString + "0.0";
        }

        if( f == 1 ) {
            // special case to increase precision even though 2 *
            // Double::MIN_VALUE is 1.0e-323
            return signString + "4.9E-324";
        }

        pow = 1 - p; // a denormalized number
        long long ff = f;
        while( (ff & 0x0010000000000000ULL ) == 0 ) {
            ff = ff << 1;
            numBits--;
        }
    } else {
        // 0 < e < 2047
        // a "normalized" number
        f = f | 0x0010000000000000ULL;
        pow = e - p;
    }

    if( -59 < pow && pow < 6 || (pow == -59 && !mantissaIsZero) ) {
        longDigitGenerator( f, pow, e == 0, mantissaIsZero, numBits );
    } else {
        bigIntDigitGeneratorInstImpl( f, pow, e == 0, mantissaIsZero, numBits );
    }

    if( value >= 1e7 || value <= -1e7 ||
        ( value > -1e-3 && value < 1e-3 ) ) {
        return signString + freeFormatExponential();
    }

    return signString + freeFormat();
}

////////////////////////////////////////////////////////////////////////////////
std::string NumberConverter::convertF(float value) {

    unsigned int p = 127 + 23; // the power offset (precision)
    unsigned int signMask = 0x80000000; // the mask to get the sign of the number
    unsigned int eMask = 0x7F800000; // the mask to get the power bits
    unsigned int fMask = 0x007FFFFF; // the mask to get the significand bits

    unsigned int inputNumberBits = Float::floatToIntBits(value);
    // the value of the sign... 0 is positive, ~0 is negative
    std::string signString = (inputNumberBits & signMask) == 0 ? "" : "-";
    // the value of the 'power bits' of the value
    unsigned int e = (inputNumberBits & eMask) >> 23;
    // the value of the 'significand bits' of the value
    unsigned int f = inputNumberBits & fMask;
    bool mantissaIsZero = ( f == 0 );
    int pow = 0, numBits = 23;

    if( e == 255 ) {
        return mantissaIsZero ? signString + "Infinity" : "NaN";
    }

    if( e == 0 ) {

        if( mantissaIsZero ) {
            return signString + "0.0";
        }

        pow = 1 - p; // a denormalized number
        if( f < 8 ) { // want more precision with smallest values
            f = f << 2;
            pow -= 2;
        }
        int ff = f;
        while( (ff & 0x00800000) == 0) {
            ff = ff << 1;
            numBits--;
        }
    } else {
        // 0 < e < 255
        // a "normalized" number
        f = f | 0x00800000;
        pow = e - p;
    }

    if( -59 < pow && pow < 35 || (pow == -59 && !mantissaIsZero) ) {
        longDigitGenerator( f, pow, e == 0, mantissaIsZero, numBits );
    } else {
        bigIntDigitGeneratorInstImpl( f, pow, e == 0, mantissaIsZero, numBits);
    }

    if( value >= 1e7f || value <= -1e7f ||
        ( value > -1e-3f && value < 1e-3f ) ) {
        return signString + freeFormatExponential();
    }

    return signString + freeFormat();
}

////////////////////////////////////////////////////////////////////////////////
std::string NumberConverter::freeFormatExponential() {

    // corresponds to process "Free-Format Exponential"
    char formattedDecimal[25] = {0};
    formattedDecimal[0] = (char)( '0' + uArray[getCount++] );
    formattedDecimal[1] = '.';
    // the position the next character is to be inserted into
    // formattedDecimal
    int charPos = 2;

    int k = firstK;
    int expt = k;
    while( true ) {
        k--;
        if( getCount >= setCount) {
            break;
        }

        formattedDecimal[charPos++] = (char)( '0' + uArray[getCount++] );
    }

    if( k == expt - 1 ) {
        formattedDecimal[charPos++] = '0';
    }
    formattedDecimal[charPos++] = 'E';

    return std::string( formattedDecimal, 0, charPos ) + Integer::toString( expt );
}

////////////////////////////////////////////////////////////////////////////////
std::string NumberConverter::freeFormat() {

    // corresponds to process "Free-Format"
    char formattedDecimal[25] = {0};
    // the position the next character is to be inserted into
    // formattedDecimal
    int charPos = 0;
    int k = firstK;
    if( k < 0 ) {
        formattedDecimal[0] = '0';
        formattedDecimal[1] = '.';
        charPos += 2;
        for( int i = k + 1; i < 0; i++ ) {
            formattedDecimal[charPos++] = '0';
        }
    }

    int U = uArray[getCount++];
    do{
        if( U != -1 ) {
            formattedDecimal[charPos++] = (char) ('0' + U);
        } else if (k >= -1) {
            formattedDecimal[charPos++] = '0';
        }

        if( k == 0 ) {
            formattedDecimal[charPos++] = '.';
        }

        k--;
        U = getCount < setCount ? uArray[getCount++] : -1;

    } while( U != -1 || k >= -1 );

    return std::string( formattedDecimal, 0, charPos );
}

////////////////////////////////////////////////////////////////////////////////
void NumberConverter::bigIntDigitGeneratorInstImpl(
    long long f, int e, bool isDenormalized, bool mantissaIsZero, int p ) {

    static const std::size_t RM_SIZE = 21;
    static const std::size_t STemp_SIZE = 22;

    unsigned int RLength, SLength, TempLength, mplus_Length, mminus_Length;
    int high, low, i;
    int k, U;

    unsigned long long R[RM_SIZE] = {0};
    unsigned long long S[STemp_SIZE] = {0};
    unsigned long long mplus[RM_SIZE] = {0};
    unsigned long long mminus[RM_SIZE] = {0};
    unsigned long long Temp[STemp_SIZE] = {0};

    if( e >= 0 ) {

        *R = f;
        *mplus = *mminus = 1;
        BigInt::simpleShiftLeftHighPrecision( mminus, RM_SIZE, e );

        if( f != (2 << (p - 1)) ) {

            BigInt::simpleShiftLeftHighPrecision( R, RM_SIZE, e + 1 );
            *S = 2;

            /*
             * m+ = m+ << e results in 1.0e23 to be printed as
             * 0.9999999999999999E23
             * m+ = m+ << e+1 results in 1.0e23 to be printed as
             * 1.0e23 (caused too much rounding)
             *      470fffffffffffff = 2.0769187434139308E34
             *      4710000000000000 = 2.076918743413931E34
             */
            BigInt::simpleShiftLeftHighPrecision(mplus, RM_SIZE, e);

        } else {

            BigInt::simpleShiftLeftHighPrecision( R, RM_SIZE, e + 2 );
            *S = 4;
            BigInt::simpleShiftLeftHighPrecision( mplus, RM_SIZE, e + 1 );
        }

    } else {

        if( isDenormalized || (f != (2 << (p - 1))) ) {

            *R = f << 1;
            *S = 1;
            BigInt::simpleShiftLeftHighPrecision( S, STemp_SIZE, 1 - e );
            *mplus = *mminus = 1;

        } else {

            *R = f << 2;
            *S = 1;
            BigInt::simpleShiftLeftHighPrecision( S, STemp_SIZE, 2 - e );
            *mplus = 2;
            *mminus = 1;
        }
    }

    k = (int)Math::ceil( (e + p - 1) * invLogOfTenBaseTwo - 1e-10 );

    if( k > 0 ) {
        BigInt::timesTenToTheEHighPrecision( S, STemp_SIZE, k );
    } else {
        BigInt::timesTenToTheEHighPrecision( R, RM_SIZE, -k );
        BigInt::timesTenToTheEHighPrecision( mplus, RM_SIZE, -k );
        BigInt::timesTenToTheEHighPrecision( mminus, RM_SIZE, -k );
    }

    RLength = mplus_Length = mminus_Length = RM_SIZE;
    SLength = TempLength = STemp_SIZE;

    memset( Temp + RM_SIZE, 0, (STemp_SIZE - RM_SIZE) * sizeof (unsigned long long) );
    memcpy( Temp, R, RM_SIZE * sizeof (unsigned long long) );

    while( RLength > 1 && R[RLength - 1] == 0 ) {
        --RLength;
    }
    while( mplus_Length > 1 && mplus[mplus_Length - 1] == 0 ) {
        --mplus_Length;
    }
    while( mminus_Length > 1 && mminus[mminus_Length - 1] == 0 ) {
        --mminus_Length;
    }
    while( SLength > 1 && S[SLength - 1] == 0 ) {
        --SLength;
    }

    TempLength = (RLength > mplus_Length ? RLength : mplus_Length) + 1;
    BigInt::addHighPrecision( Temp, TempLength, mplus, mplus_Length );

    if( BigInt::compareHighPrecision (Temp, TempLength, S, SLength) >= 0 ) {
        firstK = k;
    } else {

        firstK = k - 1;
        BigInt::simpleAppendDecimalDigitHighPrecision( R, ++RLength, 0 );
        BigInt::simpleAppendDecimalDigitHighPrecision( mplus, ++mplus_Length, 0 );
        BigInt::simpleAppendDecimalDigitHighPrecision( mminus, ++mminus_Length, 0 );
        while( RLength > 1 && R[RLength - 1] == 0 ) {
            --RLength;
        }
        while( mplus_Length > 1 && mplus[mplus_Length - 1] == 0 ) {
            --mplus_Length;
        }
        while( mminus_Length > 1 && mminus[mminus_Length - 1] == 0 ) {
            --mminus_Length;
        }
    }

    getCount = setCount = 0;
    do{

        U = 0;
        for( i = 3; i >= 0; --i ) {
            TempLength = SLength + 1;
            Temp[SLength] = 0;
            memcpy( Temp, S, SLength * sizeof(unsigned long long) );
            BigInt::simpleShiftLeftHighPrecision( Temp, TempLength, i );
            if( BigInt::compareHighPrecision( R, RLength, Temp, TempLength ) >= 0 ) {
                BigInt::subtractHighPrecision( R, RLength, Temp, TempLength );
                U += 1 << i;
            }
        }

        low = BigInt::compareHighPrecision( R, RLength, mminus, mminus_Length ) <= 0;

        memset( Temp + RLength, 0, (STemp_SIZE - RLength) * sizeof(unsigned long long) );
        memcpy( Temp, R, RLength * sizeof(unsigned long long) );
        TempLength = (RLength > mplus_Length ? RLength : mplus_Length) + 1;
        BigInt::addHighPrecision( Temp, TempLength, mplus, mplus_Length );

        high = BigInt::compareHighPrecision( Temp, TempLength, S, SLength ) >= 0;

        if( low || high ) {
            break;
        }

        BigInt::simpleAppendDecimalDigitHighPrecision( R, ++RLength, 0 );
        BigInt::simpleAppendDecimalDigitHighPrecision( mplus, ++mplus_Length, 0 );
        BigInt::simpleAppendDecimalDigitHighPrecision( mminus, ++mminus_Length, 0 );
        while( RLength > 1 && R[RLength - 1] == 0 ) {
            --RLength;
        }
        while( mplus_Length > 1 && mplus[mplus_Length - 1] == 0 ) {
            --mplus_Length;
        }
        while( mminus_Length > 1 && mminus[mminus_Length - 1] == 0 ) {
            --mminus_Length;
        }
        uArray[setCount++] = U;
    }
    while( true );

    BigInt::simpleShiftLeftHighPrecision( R, ++RLength, 1 );

    if( low && !high ) {
        uArray[setCount++] = U;
    } else if( high && !low ) {
        uArray[setCount++] = U + 1;
    } else if( BigInt::compareHighPrecision( R, RLength, S, SLength) < 0 ) {
        uArray[setCount++] = U;
    } else {
        uArray[setCount++] = U + 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
void NumberConverter::longDigitGenerator(
    long long f, int e, bool isDenormalized, bool mantissaIsZero, int p ) {

    unsigned long long R, S, M;

    if( e >= 0 ) {
        M = 1l << e;
        if( !mantissaIsZero ) {
            R = f << (e + 1);
            S = 2;
        } else {
            R = f << (e + 2);
            S = 4;
        }
    } else {
        M = 1;
        if( isDenormalized || !mantissaIsZero ) {
            R = f << 1;
            S = 1l << (1 - e);
        } else {
            R = f << 2;
            S = 1l << (2 - e);
        }
    }

    int k = (int)Math::ceil( (e + p - 1) * invLogOfTenBaseTwo - 1e-10 );

    if( k > 0 ) {
        S = S * TEN_TO_THE[k];
    } else if( k < 0 ) {
        long long scale = TEN_TO_THE[-k];
        R = R * scale;
        M = M == 1 ? scale : M * scale;
    }

    if( R + M > S ) { // was M_plus
        firstK = k;
    } else {
        firstK = k - 1;
        R = R * 10;
        M = M * 10;
    }

    getCount = setCount = 0; // reset indices
    bool low, high;
    int U;
    long long Si[4] = { S, S << 1, S << 2, S << 3 };
    while( true ) {
        // set U to be floor (R / S) and R to be the remainder
        // using a kind of "binary search" to find the answer.
        // It's a lot quicker than actually dividing since we know
        // the answer will be between 0 and 10
        U = 0;
        long long remainder;
        for( int i = 3; i >= 0; i-- ) {
            remainder = R - Si[i];
            if( remainder >= 0 ) {
                R = remainder;
                U += 1 << i;
            }
        }

        low = R < M; // was M_minus
        high = R + M > S; // was M_plus

        if( low || high ) {
            break;
        }

        R = R * 10;
        M = M * 10;
        uArray[setCount++] = U;
    }

    if( low && !high ) {
        uArray[setCount++] = U;
    } else if( high && !low ) {
        uArray[setCount++] = U + 1;
    } else if( ( R << 1 ) < S ) {
        uArray[setCount++] = U;
    } else {
        uArray[setCount++] = U + 1;
    }
}
