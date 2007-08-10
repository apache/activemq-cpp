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

#ifndef _DECAF_INTERNAL_UTIL_BITOPS_H_
#define _DECAF_INTERNAL_UTIL_BITOPS_H_

#include <decaf/util/Config.h>
#include <apr.h>

namespace decaf{
namespace internal{
namespace util{

    /**
     * Class with static methods for operation on values in bit patterns.
     */
    class BitOps {
    public:

        // Used to make masking easier
        typedef union {
            unsigned long long longValue;
            unsigned int intValue[2];
            int sintValue[2];
            double doubleValue;
        } LONG_UNION;

        #ifdef APR_IS_BIGENDIAN
            static const int HiWord = 0;
            static const int LoWord = 1;
        #else
            static const int HiWord = 1;
            static const int LoWord = 0;
        #endif

        static const unsigned long long LONG_HI_MASK = 0xFFFFFFFF00000000ULL;
        static const unsigned long long LONG_LO_MASK = 0x00000000FFFFFFFFULL;
        static const long long TEN_E1 = 0xALL;
        static const long long TEN_E2 = 0x64LL;
        static const long long TEN_E3 = 0x3E8LL;
        static const long long TEN_E4 = 0x2710LL;
        static const long long TEN_E5 = 0x186A0LL;
        static const long long TEN_E6 = 0xF4240LL;
        static const long long TEN_E7 = 0x989680LL;
        static const long long TEN_E8 = 0x5F5E100LL;
        static const long long TEN_E9 = 0x3B9ACA00LL;
        static const long long TEN_E19 = 0x8AC7230489E80000LL;

        static const int E_OFFSET = 1075;

        static const long long INFINITE_LONGBITS = 0x7FF0000000000000LL;
        static const int INFINITE_INTBITS = 0x7F800000;
        static const int MINIMUM_INTBITS = 1;

        static const unsigned int DOUBLE_SIGN_MASK_HI = 0x80000000;
        static const unsigned int DOUBLE_EXPONENT_MASK_HI = 0x7FF00000;
        static const unsigned int DOUBLE_MANTISSA_MASK_LO = 0xFFFFFFFF;
        static const unsigned int DOUBLE_MANTISSA_MASK_HI = 0x000FFFFF;
        static const unsigned int SINGLE_SIGN_MASK = 0x80000000;
        static const unsigned int SINGLE_EXPONENT_MASK	= 0x7F800000;
        static const unsigned int SINGLE_MANTISSA_MASK	= 0x007FFFFF;
        static const unsigned int SINGLE_NAN_BITS = (SINGLE_EXPONENT_MASK | 0x00400000);

        static const unsigned long long MANTISSA_MASK = 0x000FFFFFFFFFFFFFULL;
        static const unsigned long long EXPONENT_MASK = 0x7FF0000000000000ULL;
        static const unsigned long long NORMAL_MASK = 0x0010000000000000ULL;
        static const unsigned long long SIGN_MASK = 0x8000000000000000ULL;
        static const unsigned int FLOAT_MANTISSA_MASK = 0x007FFFFF;
        static const unsigned int FLOAT_EXPONENT_MASK = 0x7F800000;
        static const unsigned int FLOAT_NORMAL_MASK = 0x00800000;
        static const unsigned int FLOAT_E_OFFSET = 150;

    public:

        BitOps();
        virtual ~BitOps() {}

    public:  // Statics

        static unsigned int TIMES_TEN( unsigned int x ) {
            return ((x) << 3) + ((x) << 1);
        }

        static unsigned long long TIMES_TEN( unsigned long long x ) {
            return ((x) << 3) + ((x) << 1);
        }

        static unsigned long long CREATE_DOUBLE_BITS(
            unsigned long long normalizedM, unsigned long long e ) {

            return ( normalizedM & MANTISSA_MASK ) | ( ( e + E_OFFSET ) << 52 );
        }

        static unsigned long long CREATE_DOUBLE_BITS(
            unsigned long long* normalizedM, int index, unsigned long long e ) {

            return ( normalizedM[index] & MANTISSA_MASK ) | ( ( e + E_OFFSET ) << 52 );
        }

        static unsigned long long bitSection(
            unsigned long long x, unsigned long long mask, int shift ) {

            return ( x & mask ) >> shift;
        }
        static unsigned int bitSection(
            unsigned int x, unsigned int mask, int shift ) {

            return ( x & mask ) >> shift;
        }

        static unsigned int& LOW_U32_FROM_LONG64( unsigned long long long64 ) {
            return LOW_U32_FROM_LONG64_PTR( &long64 );
        }

        static unsigned int& HIGH_U32_FROM_LONG64( unsigned long long long64 ) {
            return HIGH_U32_FROM_LONG64_PTR( &long64 );
        }

        static int& LOW_I32_FROM_LONG64( unsigned long long long64 ) {
            return LOW_I32_FROM_LONG64_PTR( &long64 );
        }

        static int& HIGH_I32_FROM_LONG64( unsigned long long long64 ) {
            return HIGH_I32_FROM_LONG64_PTR( &long64 );
        }

        static unsigned int& LOW_U32_FROM_LONG64_PTR( unsigned long long* long64ptr ) {
            return ( (LONG_UNION*)long64ptr )->intValue[LoWord];
        }

        static unsigned int& HIGH_U32_FROM_LONG64_PTR( unsigned long long* long64ptr ) {
            return ( (LONG_UNION*)long64ptr )->intValue[HiWord];
        }

        static int& LOW_I32_FROM_LONG64_PTR( unsigned long long* long64ptr ) {
            return ( (LONG_UNION*)long64ptr )->sintValue[LoWord];
        }

        static int& HIGH_I32_FROM_LONG64_PTR( unsigned long long* long64ptr ) {
            return ( (LONG_UNION*)long64ptr )->sintValue[HiWord];
        }

        static bool IS_DENORMAL_DBL( double dbl ) {
            return IS_DENORMAL_DBL_PTR( &dbl );
        }

        static bool IS_DENORMAL_DBL_PTR(double* dblptr ) {
            return (((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_EXPONENT_MASK_HI) == 0) &&
                    ((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_MANTISSA_MASK_HI) != 0 ||
                    (LOW_U32_FROM_DBL_PTR(dblptr) != 0)));
        }

        static unsigned int& LOW_U32_FROM_DBL_PTR( double* dblptr ) {
            return ( ( (LONG_UNION*)dblptr )->intValue[LoWord] );
        }

        static unsigned int& HIGH_U32_FROM_DBL_PTR( double* dblptr ) {
            return ( ( (LONG_UNION*)dblptr )->intValue[HiWord] );
        }

        static int& LOW_I32_FROM_DBL_PTR( double* dblptr ) {
            return ( ( (LONG_UNION*)dblptr )->sintValue[LoWord] );
        }

        static int& HIGH_I32_FROM_DBL_PTR( double* dblptr ) {
            return ( ( (LONG_UNION*)dblptr )->sintValue[HiWord] );
        }

        static unsigned int& LOW_U32_FROM_DBL( double dbl ) {
            return LOW_U32_FROM_DBL_PTR( &dbl );
        }

        static unsigned int& HIGH_U32_FROM_DBL( double dbl ) {
            return HIGH_U32_FROM_DBL_PTR( &dbl );
        }

        static int& LOW_I32_FROM_DBL( double dbl ) {
            return LOW_I32_FROM_DBL_PTR( &dbl );
        }

        static int& HIGH_I32_FROM_DBL( double dbl ) {
            return HIGH_I32_FROM_DBL_PTR( &dbl );
        }

        static unsigned int at( unsigned int i ) {
            #ifdef APR_IS_BIGENDIAN
                return i^1;
            #else
                return i;
            #endif
        }

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_BITOPS_H_*/
