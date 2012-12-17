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
#include <decaf/lang/Comparable.h>
#include <string>

namespace decaf{
namespace util{

    /**
     * Wrapper class around a time value in milliseconds.  This
     * class is comparable to Java's java.util.Date class.
     *
     * @since 1.0
     */
    class DECAF_API Date : public lang::Comparable<Date> {
    private:

        /**
         * The underlying time value in milliseconds�
         */
        long long time;

    public:

        /**
         * Default constructor - sets time to the current System time, rounded to the
         * nearest millisecond.
         */
        Date();

        /**
         * Constructs the date with a given time value.
         * @param milliseconds The time in milliseconds;
         */
        Date( long long milliseconds );

        /**
         * Copy constructor.
         * @param source The Date instance to copy into this one.
         */
        Date( const Date& source );

        /**
         * Assigns the value of one Date object to another.
         *
         * @param value
         *      The value to be copied into this Date object.
         *
         * @return reference to this object with the newly assigned value.
         */
        Date& operator= ( const Date& value );

        virtual ~Date();

        /**
         * Gets the underlying time.
         * @return The underlying time value in milliseconds.
         */
        long long getTime() const;

        /**
         * Sets the underlying time.
         * @param milliseconds The underlying time value in
         * milliseconds.
         */
        void setTime( long long milliseconds );

        /**
         * Determines whether or not this date falls after the
         * specified time.
         * @param when The date to compare
         * @return true if this date falls after when.
         */
        bool after( const Date& when ) const;

        /**
         * Determines whether or not this date falls before the
         * specified time.
         * @param when The date to compare
         * @return true if this date falls before when.
         */
        bool before( const Date& when ) const;

        /**
         * Converts this Date object to a String of the form:
         *
         *    dow mon dd hh:mm:ss zzz yyyy
         *
         * where:
         *
         *   - dow is the day of the week (Sun, Mon, Tue, Wed, Thu, Fri, Sat).
         *   - mon is the month (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec).
         *   - dd is the day of the month (01 through 31), as two decimal digits.
         *   - hh is the hour of the day (00 through 23), as two decimal digits.
         *   - mm is the minute within the hour (00 through 59), as two decimal digits.
         *   - ss is the second within the minute (00 through 61, as two decimal digits.
         *   - zzz is the time zone (and may reflect daylight saving time). Standard time
         *     zone abbreviations include those recognized by the method parse. If time
         *     zone information is not available, then zzz is empty - that is, it consists
         *     of no characters at all.
         *   - yyyy is the year, as four decimal digits.
         *
         * @return the String representation of the Date object.
         */
        std::string toString() const;

    public:  // Comparable

        virtual int compareTo( const Date& value ) const;

        virtual bool equals( const Date& value ) const;

        virtual bool operator==( const Date& value ) const;

        virtual bool operator<( const Date& value ) const;

    };

}}

#endif /*_DECAF_UTIL_DATE_H_*/
