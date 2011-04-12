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

#ifndef _DECAF_UTIL_CONCURRENT_TIMEUNIT_H_
#define _DECAF_UTIL_CONCURRENT_TIMEUNIT_H_

#include <string>
#include <decaf/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/InterruptedException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf {
namespace lang {
    class Thread;
}
namespace util {
namespace concurrent {

    /**
     * A TimeUnit represents time durations at a given unit of granularity and
     * provides utility methods to convert across units, and to perform timing
     * and delay operations in these units. A TimeUnit does not maintain time
     * information, but only helps organize and use time representations that
     * may be maintained separately across various contexts. A nanosecond is
     * defined as one thousandth of a microsecond, a microsecond as one thousandth
     * of a millisecond, a millisecond as one thousandth of a second, a minute as
     * sixty seconds, an hour as sixty minutes, and a day as twenty four hours.
     *
     * A TimeUnit is mainly used to inform time-based methods how a given timing
     * parameter should be interpreted. For example, the following code will timeout
     * in 50 milliseconds if the lock is not available:
     *
     *  Lock lock = ...;
     *  if ( lock.tryLock( 50, TimeUnit.MILLISECONDS ) ) ...
     *
     * while this code will timeout in 50 seconds:
     *
     *  Lock lock = ...;
     *  if ( lock.tryLock( 50, TimeUnit.SECONDS ) ) ...
     *
     * Note however, that there is no guarantee that a particular timeout implementation
     * will be able to notice the passage of time at the same granularity as the given
     * TimeUnit.
     */
    class DECAF_API TimeUnit : public decaf::lang::Comparable<TimeUnit> {
    private:

        /** This TimeUnit's index */
        int index;

        /** Name of the Unit being represented. */
        std::string name;

        /** Array of Time Unit multipliers */
        static const long long multipliers[];

    public:

        /** The Actual TimeUnit enumerations */
        static const TimeUnit NANOSECONDS;
        static const TimeUnit MICROSECONDS;
        static const TimeUnit MILLISECONDS;
        static const TimeUnit SECONDS;
        static const TimeUnit MINUTES;
        static const TimeUnit HOURS;
        static const TimeUnit DAYS;

        /** The An Array of TimeUnit Instances */
        static const TimeUnit* const values[];

    protected:

        /**
         * Hidden Constructor, this class can not be instantiated directly.
         * @param index - Index into the Time Unit set.
         * @param name - Name of the unit type being represented.
         */
        TimeUnit( int index, const std::string& name );

    public:

        virtual ~TimeUnit() {}

        /**
         * Convert the given time duration in the given unit to this unit. Conversions
         * from finer to coarser granularities truncate, so lose precision. For example
         * converting 999 milliseconds to seconds results in 0. Conversions from coarser
         * to finer granularities with arguments that would numerically overflow saturate
         * to Long.MIN_VALUE if negative or Long.MAX_VALUE if positive.
         * <p>
         * For example, to convert 10 minutes to milliseconds, use:
         * TimeUnit.MILLISECONDS.convert(10L, TimeUnit.MINUTES)
         * <p>
         * @param sourceDuration - Duration value to convert.
         * @param sourceUnit - Unit type of the source duration.
         * @returns the converted duration in this unit, or Long.MIN_VALUE if conversion
         * would negatively overflow, or Long.MAX_VALUE if it would positively overflow.
         */
        long long convert( long long sourceDuration, const TimeUnit& sourceUnit ) const;

        /**
         * Equivalent to <tt>NANOSECONDS.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration,
         * or <tt>Long.MIN_VALUE</tt> if conversion would negatively
         * overflow, or <tt>Long.MAX_VALUE</tt> if it would positively overflow.
         * @see #convert
         */
        long long toNanos( long long duration ) const {
            return doConvert( this->index, NANOSECONDS.index, duration );
        }

        /**
         * Equivalent to <tt>MICROSECONDS.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration,
         * or <tt>Long.MIN_VALUE</tt> if conversion would negatively
         * overflow, or <tt>Long.MAX_VALUE</tt> if it would positively overflow.
         * @see #convert
         */
        long long toMicros( long long duration ) const {
            return doConvert( this->index, MICROSECONDS.index, duration );
        }

        /**
         * Equivalent to <tt>MILLISECONDS.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration,
         * or <tt>Long.MIN_VALUE</tt> if conversion would negatively
         * overflow, or <tt>Long.MAX_VALUE</tt> if it would positively overflow.
         * @see #convert
         */
        long long toMillis( long long duration ) const {
            return doConvert( this->index, MILLISECONDS.index, duration );
        }

        /**
         * Equivalent to <tt>SECONDS.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration.
         * @see #convert
         */
        long long toSeconds( long long duration ) const {
            return doConvert( this->index, SECONDS.index, duration );
        }

        /**
         * Equivalent to <tt>MINUTES.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration.
         * @see #convert
         */
        long long toMinutes( long long duration ) const {
            return doConvert( this->index, MINUTES.index, duration );
        }

        /**
         * Equivalent to <tt>HOURS.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration.
         * @see #convert
         */
        long long toHours( long long duration ) const {
            return doConvert( this->index, HOURS.index, duration );
        }

        /**
         * Equivalent to <tt>DAYS.convert(duration, this)</tt>.
         * @param duration the duration
         * @return the converted duration.
         * @see #convert
         */
        long long toDays( long long duration ) const {
            return doConvert( this->index, DAYS.index, duration );
        }

        /**
         * Perform a timed <tt>Object.wait</tt> using this time unit.
         * This is a convenience method that converts timeout arguments
         * into the form required by the <tt>Object.wait</tt> method.
         *
         * <p>For example, you could implement a blocking <tt>poll</tt>
         * method (see {@link BlockingQueue#poll BlockingQueue.poll})
         * using:
         *
         * <pre>
         *    Object poll( long long timeout, const TimeUnit& unit )
         *      throw( InterruptedException ) {
         *
         *      while( empty ) {
         *        unit.timedWait(this, timeout);
         *        ...
         *      }
         *  }</pre>
         *
         * @param obj the object to wait on
         * @param timeout the maximum time to wait.
         *
         * @throws InterruptedException if interrupted while waiting.
         * @throws NullPointerException if the Synchronizable object is null.
         *
         * @see Synchronizable#wait( long long, long long )
         */
        void timedWait( Synchronizable* obj, long long timeout ) const;

        /**
         * Perform a timed <tt>Thread.join</tt> using this time unit.
         * This is a convenience method that converts time arguments into the
         * form required by the <tt>Thread.join</tt> method.
         *
         * @param thread the thread to wait for
         * @param timeout the maximum time to wait
         *
         * @throws InterruptedException if interrupted while waiting.
         * @throws NullPointerException if the thread object is null.
         *
         * @see Thread#join( long long, long long )
         */
        void timedJoin( decaf::lang::Thread* thread, long long timeout );

        /**
         * Perform a <tt>Thread.sleep</tt> using this unit.
         * This is a convenience method that converts time arguments into the
         * form required by the <tt>Thread.sleep</tt> method.
         * @param timeout the minimum time to sleep
         * @see Thread#sleep
         */
        void sleep( long long timeout ) const;

        /**
         * Converts the TimeUnit type to the Name of the TimeUnit.
         * @return String name of the TimeUnit
         */
        virtual std::string toString() const;

    public:  // Static Methods

        /**
         * Returns the TimeUnit constant of this type with the specified name. The
         * string must match exactly an identifier used to declare an TimeUnit constant
         * in this type. (Extraneous whitespace characters are not permitted.)
         *
         * @param name
         *          The Name of the TimeUnit constant to be returned.
         * @returns
         *          A constant reference to the TimeUnit Constant with the given name.
         * @throws IllegalArgumentException
         *          if this enum type has no constant with the specified name
         */
        static const TimeUnit& valueOf( const std::string& name );

    public:

        virtual int compareTo( const TimeUnit& value ) const;

        virtual bool equals( const TimeUnit& value ) const;

        virtual bool operator==( const TimeUnit& value ) const;

        virtual bool operator<( const TimeUnit& value ) const;

    private:

        /* Perform the actual conversion */
        long long doConvert( int srcIndex, int destIndex, long long duration ) const;

        /*
         * Utility method to compute the excess-nanosecond argument to
         * wait, sleep, join.
         */
        int excessNanos( long long time, long long ms ) const;

        /**
         * Scale d by m, checking for overflow.
         * @param duration - The amount of time to scale by the multiplier.
         * @param multiplier - The scaling factor.
         * @param overflow - The value at which d * m would cause an overflow.
         */
        static long long scale( long long duration, long long multiplier, long long overflow );

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_TIMEUNIT_H_*/
