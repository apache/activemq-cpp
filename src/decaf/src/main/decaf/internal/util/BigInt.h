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

#ifndef _DECAF_INTERNAL_UTIL_BIGINT_H_
#define _DECAF_INTERNAL_UTIL_BIGINT_H_

#include <decaf/util/Config.h>
#include <apr.h>

namespace decaf{
namespace internal{
namespace util{

    class BigInt {
    private:

        // Used to make masking easier
        typedef union {
            unsigned long long longValue;
            unsigned int intValue[2];
            double doubleValue;
        } LONG_UNION;

        #ifdef APR_IS_BIGENDIAN
            static const int HiWord = 0;
            static const int LoWord = 1;
        #else
            static const int HiWord = 1;
            static const int LoWord = 0;
        #endif

    public:

        BigInt();
        virtual ~BigInt() {}

        static void multiplyHighPrecision( unsigned long long* arg1,
                                           int length1,
                                           unsigned long long* arg2,
                                           int length2,
                                           unsigned long long* result,
                                           int length );

        static unsigned int simpleAppendDecimalDigitHighPrecision(
                unsigned long long* arg1, int length, unsigned long long digit );

        static double toDoubleHighPrecision( unsigned long long* arg, int length );

        static int tenToTheEHighPrecision( unsigned long long * result,
                                           int length,
                                           int e );

        static unsigned long long doubleMantissa( double z );

        static int compareHighPrecision( unsigned long long* arg1,
                                         int length1,
                                         unsigned long long* arg2,
                                         int length2 );

        static int highestSetBitHighPrecision(unsigned long long* arg, int length );
        static void subtractHighPrecision(unsigned long long* arg1, int length1, unsigned long long* arg2,
                                    int length2);
        static int doubleExponent(double z);
        static unsigned int simpleMultiplyHighPrecision(unsigned long long* arg1, int length, unsigned long long arg2 );
        static int addHighPrecision(unsigned long long * arg1, int length1, unsigned long long* arg2,
                                int length2);
        static void simpleMultiplyAddHighPrecisionBigEndianFix(unsigned long long* arg1, int length,
                                                         unsigned long long arg2, unsigned int* result );
        static int lowestSetBit(unsigned long long* y);
        static int timesTenToTheEHighPrecision(unsigned long long * result, int length, jint e );
        static void simpleMultiplyAddHighPrecision(unsigned long long * arg1, int length, unsigned long long arg2,
                                             unsigned int * result);
        static int highestSetBit(unsigned long long * y );
        static int lowestSetBitHighPrecision(unsigned long long* arg, int length );
        static void simpleShiftLeftHighPrecision(unsigned long long* arg1, int length, int arg2 );
        static unsigned int floatMantissa( float z );
        static unsigned long long simpleMultiplyHighPrecision64( unsigned long long* arg1, int length, unsigned long long arg2 );
        static int simpleAddHighPrecision( unsigned long long* arg1, int length, unsigned long long arg2 );
        static int floatExponent( float z );

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_BIGINT_H_*/
