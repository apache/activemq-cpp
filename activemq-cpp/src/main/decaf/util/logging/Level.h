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

#ifndef _DECAF_UTIL_LOGGING_LEVEL_H_
#define _DECAF_UTIL_LOGGING_LEVEL_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Comparable.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf {
namespace util {
namespace logging {

    /**
     * The Level class defines a set of standard logging levels that can be used to control logging
     * output. The logging Level objects are ordered and are specified by ordered integers.
     * Enabling logging at a given level also enables logging at all higher levels.
     *
     * Clients should normally use the predefined Level constants such as Level.SEVERE.
     *
     * The levels in descending order are:
     *
     *   * SEVERE (highest value)
     *   * WARNING
     *   * INFO
     *   * DEBUG
     *   * CONFIG
     *   * FINE
     *   * FINER
     *   * FINEST (lowest value)
     *
     * In addition there is a level OFF that can be used to turn off logging, and a level ALL that
     * can be used to enable logging of all messages.
     *
     * It is possible for third parties to define additional logging levels by subclassing Level.
     * In such cases subclasses should take care to chose unique integer level values.
     *
     * @since 1.0
     */
    class DECAF_API Level : public decaf::lang::Comparable<Level> {
    private:

        std::string name;
        int value;

    public:

        /**
         * NULL is a special level that indicates that the Logger should get its Level from
         * its parent Logger, the value is initialized as zero.
         */
        static const Level INHERIT;

        /**
         * OFF is a special level that can be used to turn off logging. This level is initialized
         * to Integer::MAX_VALUE
         */
        static const Level OFF;

        /**
         * SEVERE is a message level indicating a serious failure.
         *
         * In general SEVERE messages should describe events that are of considerable importance
         * and which will prevent normal program execution. They should be reasonably intelligible
         * to end users and to system administrators. This level is initialized to 1000.
         */
        static const Level SEVERE;

        /**
         * WARNING is a message level indicating a potential problem.
         *
         * In general WARNING messages should describe events that will be of interest to end users
         * or system managers, or which indicate potential problems. This level is initialized to 900.
         */
        static const Level WARNING;

        /**
         * INFO is a message level for informational messages.
         *
         * Typically INFO messages will be written to the console or its equivalent. So the INFO
         * level should only be used for reasonably significant messages that will make sense to end
         * users and system admins. This level is initialized to 800.
         */
        static const Level INFO;

        /**
         * DEBUG is a level for more verbose informative messages.
         *
         * DEBUG messages are intended to provide a more detailed message intended for use by
         * developers in tracking the behavior of a client.  DEBUG messages typically contain more
         * implementation specific information that might not be significant to end users or system
         * admins.  This level is initialized to 700.
         */
        static const Level DEBUG;

        /**
         * CONFIG is a message level for static configuration messages.
         *
         * CONFIG messages are intended to provide a variety of static configuration information, to
         * assist in debugging problems that may be associated with particular configurations. For
         * example, CONFIG message might include the CPU type, the System properties, etc. This level
         * is initialized to 600.
         */
        static const Level CONFIG;

        /**
         * FINE is a message level providing tracing information.
         *
         * All of FINE, FINER, and FINEST are intended for relatively detailed tracing. The exact
         * meaning of the three levels will vary between subsystems, but in general, FINEST should
         * be used for the most detailed output, FINER for somewhat less detailed output, and FINE
         * for the lowest volume (and most important) messages.
         *
         * In general the FINE level should be used for information that will be broadly interesting
         * to developers who do not have a specialized interest in the specific subsystem.
         *
         * FINE messages might include things like minor (recoverable) failures. Issues indicating
         * potential performance problems are also worth logging as FINE. This level is initialized
         * to 500.
         */
        static const Level FINE;

        /**
         * FINER indicates a fairly detailed tracing message. By default logging calls for entering,
         * returning, or throwing an exception are traced at this level. This level is initialized
         * to 400.
         */
        static const Level FINER;

        /**
         * FINEST indicates a highly detailed tracing message. This level is initialized to 300.
         */
        static const Level FINEST;

        /**
         * ALL indicates that all messages should be logged. This level is initialized
         * to Integer::MIN_VALUE.
         */
        static const Level ALL;

    protected:

        /**
         * Create a named Level with a given integer value.
         *
         * @param name
         *      Name of the level, e.g. SEVERE
         * @param value
         *      Unique integer value of this level, e.g. 100
         */
        Level( const std::string& name, int value );

    public:

        virtual ~Level();

        /**
         * @return the integer value of this level instance.
         */
        int intValue() const {
            return this->value;
        }

        /**
         * @return the name of this Level instance.
         */
        std::string getName() const {
            return this->name;
        }

        /**
         * @return the string value of this Level, e.g. "SEVERE".
         */
        std::string toString() const {
            return this->name;
        }

    public:  // Comparable

        virtual int compareTo( const Level& value ) const;

        virtual bool equals( const Level& value ) const;

        virtual bool operator==( const Level& value ) const;

        virtual bool operator<( const Level& value ) const;

    public:

        /**
         * Parse a level name string into a Level.
         *
         * The argument string may consist of either a level name or an integer value.
         *
         * For example:
         *
         *  * "SEVERE"
         *  * "1000"
         *
         * @param name - The name or int value of the desired Level
         *
         * @return the parsed Level value, passing in a level name that is an int value that
         *         is not one of the known Level values will result in a new Level that has
         *         been initialized with that int value and name as the string form of the int.
         *
         * @throws IllegalArgumentException if the value is not valid, validity means that the
         *         string is either a valid int (between Integer::MIN_VALUE and Integer::MAX_VALUE
         *         or is one of the known level names.
         */
        static Level parse( const std::string& name );

    };

}}}

#endif /* _DECAF_UTIL_LOGGING_LEVEL_H_ */
