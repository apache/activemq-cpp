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

#ifndef _DECAF_UTIL_DATE_H_
#define _DECAF_UTIL_DATE_H_

#include <decaf/util/Config.h>

namespace decaf{
namespace util{

    /**
     * Wrapper class around a time value in milliseconds.  This
     * class is comparable to Java's java.util.Date class.
     */
    class DECAF_API Date
    {
    private:

        /**
         * The underlying time value in milliseconds�
         */
        long long time;

    public:

        /**
         * Default constructor - sets time to now.
         */
        Date(){
            time = getCurrentTimeMilliseconds();
        }

        /**
         * Constructs the date with a given time value.
         * @param milliseconds The time in milliseconds;
         */
        Date( long long milliseconds ){
            this->time = milliseconds;
        }

        /**
         * Copy constructor.
         */
        Date( const Date& source ){
            (*this) = source;
        }

        virtual ~Date(){}

        /**
         * Gets the underlying time.
         * @return The underlying time value in milliseconds.
         */
        long long getTime() const{
            return time;
        }

        /**
         * Sets the underlying time.
         * @param milliseconds The underlying time value in
         * milliseconds.
         */
        void setTime( long long milliseconds ){
            this->time = milliseconds;
        }

        /**
         * Determines wether or not this date falls after the
         * specified time.
         * @param when The date to compare
         * @return true if this date falls after when.
         */
        bool after( Date& when ) const{
            return time > when.time;
        }

        /**
         * Determines wether or not this date falls before the
         * specified time.
         * @param when The date to compare
         * @return true if this date falls before when.
         */
        bool before( Date& when ) const{
            return time < when.time;
        }

        /**
         * Determines wether or not this date is equal to the
         * specified time.
         * @param when The date to compare
         * @return true if this date is equal to when.
         */
        bool equals( Date& when ) const{
            return time == when.time;
        }

        /**
         * Assignment operator.
         */
        Date& operator =( const Date& source ){
            this->time = source.time;
            return *this;
        }

        /**
         * Returns the current time in milliseconds.  Comparable
         * to Java's System.currentTimeMillis method.
         * @return The current time in milliseconds.
         */
        static long long getCurrentTimeMilliseconds();
    };

}}

#endif /*_DECAF_UTIL_DATE_H_*/
