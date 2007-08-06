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

#ifndef _DECAF_INTERNAL_UTIL_NUMBERCONVERTER_H_
#define _DECAF_INTERNAL_UTIL_NUMBERCONVERTER_H_

namespace decaf{
namespace internal{
namespace util{

    class NumberConverter {
    private:

        int setCount;
        int getCount;
        int firstK;
        std::vector<int> uArray;

        static std::vector<long long> TEN_TO_THE;
        static const double invLogOfTenBaseTwo;

        // Internal class used to init static arrays
        struct StaticInitializer {
            StaticInitializer();
        };

        static StaticInitializer init;

    public:

        NumberConverter();
        virtual ~NumberConverter() {}

        /**
         * @param value - the double to convert
         * @returns the string representation of value
         */
        std::string convertD( double value );

        /**
         * @param value - the float to convert
         * @returns the string representation of value
         */
        std::string convertF( float value );

    public:   // Statics

        /**
         * @param value to convert to a string
         * @returns the value converted to a string
         */
        static std::string convert( double value ) {
            return NumberConverter().convertD( value );
        }

        /**
         * @param value to convert to a string
         * @returns the value converted to a string
         */
        static std::string convert( float value ) {
            return NumberConverter().convertF( value );
        }

    private:

        std::string freeFormatExponential();
        std::string freeFormat();

        void bigIntDigitGeneratorInstImpl(
            long f, int e, bool isDenormalized, bool mantissaIsZero, int p );
        void longDigitGenerator(
            long f, int e, bool isDenormalized, bool mantissaIsZero, int p );

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_NUMBERCONVERTER_H_*/
