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

#ifndef _DECAF_LANG_SYSTEM_H_
#define _DECAF_LANG_SYSTEM_H_

#include <decaf/util/Config.h>
#include <decaf/util/Map.h>
#include <decaf/lang/Exception.h>
#include <decaf/internal/AprPool.h>
#include <string>

namespace decaf{
namespace lang{

    class DECAF_API System {
    public:

        System();
        virtual ~System() {}

    public:  // Static Methods

        /**
         * Enumerates the system environment and returns a map of env variable
         * names to the string values they hold.
         * @return A Map of all environment variables.
         * @throw Exception if an error occurs
         */
        static util::Map<std::string, std::string> getenv()
            throw ( lang::Exception );

        /**
         * Reads an environment value from the system and returns it as a
         * string object
         * @param name - the env var to read
         * @return a string with the value from the var or ""
         * @throws an Exception if an error occurs while reading the Env.
         */
        static std::string getenv( const std::string& name )
            throw ( lang::Exception );

        /**
         * Clears a set env value if one is set.
         * @param name - the env var to clear
         * @throws an Exception if an error occurs while reading the Env.
         */
        static void unsetenv( const std::string& name )
            throw ( lang::Exception );

        /**
         * Sets the specified system property to the value given
         * @param name - name of the env val to set
         * @param value - value to assign to name
         * @throws an Exception if an error occurs
         */
        static void setenv( const std::string& name, const std::string& value )
            throw ( lang::Exception );

        /**
         * @returns the current system time in Milliseconds
         */
        static long long currentTimeMillis();

        /**
         * Returns the current value of the most precise available system timer, in
         * nanoseconds.
         * <p>
         * This method can only be used to measure elapsed time and is not related to
         * any other notion of system or wall-clock time. The value returned represents
         * nanoseconds since some fixed but arbitrary time (perhaps in the future, so
         * values may be negative). This method provides nanosecond precision, but not
         * necessarily nanosecond accuracy. No guarantees are made about how frequently
         * values change. Differences in successive calls that span greater than
         * approximately 292 years (263 nanoseconds) will not accurately compute elapsed
         * time due to numerical overflow.
         * <p>
         * For example, to measure how long some code takes to execute:
         * <p>
         *   long long startTime = System::nanoTime();
         *   // ... the code being measured ...
         *   long long estimatedTime = System::nanoTime() - startTime;
         * <p>
         *
         * @returns
         *     The current value of the system timer, in nanoseconds.
         */
        static long long nanoTime();

    private:

        /**
         * Enumerates the environment and return an array of strings
         * with the values.  Caller owns the array.  The array is terminated
         * by an element that holds the value NULL
         * @returns a vector of env name=value paris.
         */
        static std::vector< std::string > getEnvArray();

        /**
         * Gets the one and only APR Pool instance
         */
        static internal::AprPool& getAprPool();

    };

}}

#endif /*_DECAF_LANG_SYSTEM_H_*/
