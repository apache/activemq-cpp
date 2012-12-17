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

#ifndef _DECAF_LANG_NUMBER_H_
#define _DECAF_LANG_NUMBER_H_

#include <decaf/util/Config.h>

namespace decaf{
namespace lang{

    /**
     * The abstract class Number is the superclass of classes Byte, Double,
     * Float, Integer, Long, and Short.
     *
     * Subclasses of Number must provide methods to convert the represented
     * numeric value to byte, double, float, int, long, and short.
     *
     * @since 1.0
     */
    class DECAF_API Number {
    public:

        virtual ~Number() {}

        /**
         * Answers the byte value which the receiver represents
         * @return byte the value of the receiver.
         */
        virtual unsigned char byteValue() const {
            return (unsigned char)intValue();
        }

        /**
         * Answers the double value which the receiver represents
         * @return double the value of the receiver.
         */
        virtual double doubleValue() const = 0;

        /**
         * Answers the float value which the receiver represents
         * @return float the value of the receiver.
         */
        virtual float floatValue() const = 0;

        /**
         * Answers the int value which the receiver represents
         * @return int the value of the receiver.
         */
        virtual int intValue() const = 0;

        /**
         * Answers the long value which the receiver represents
         * @return long long the value of the receiver.
         */
        virtual long long longValue() const = 0;

        /**
         * Answers the short value which the receiver represents
         * @return short the value of the receiver.
         */
        virtual short shortValue() const {
            return (short) intValue();
        }

    };

}}

#endif /*_DECAF_LANG_NUMBER_H_*/
