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
#include <decaf/util/Properties.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/internal/AprPool.h>
#include <string>

namespace decaf{
namespace lang{

    class Runtime;
    class SystemData;

    /**
     * The System class provides static methods for accessing system level resources and performing
     * some system dependent tasks such as looking up environment values and copying memory and arrays.
     *
     * @since 1.0
     */
    class DECAF_API System {
    private:

        static SystemData* sys;

    protected:

        System();

    public:

        virtual ~System() {}

    public:  // Static Methods

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const char* src, std::size_t srcPos,
                               char* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const unsigned char* src, std::size_t srcPos,
                               unsigned char* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const short* src, std::size_t srcPos,
                               short* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const int* src, std::size_t srcPos,
                               int* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const long long* src, std::size_t srcPos,
                               long long* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const float* src, std::size_t srcPos,
                               float* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        static void arraycopy( const double* src, std::size_t srcPos,
                               double* dest, std::size_t destPos, std::size_t length );

        /**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
        template< typename E >
        static void arraycopy( const E* src, std::size_t srcPos,
                               E* dest, std::size_t destPos, std::size_t length ) {

            if( src == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Given Source Pointer was null." );
            }

            if( src == NULL ) {
                throw decaf::lang::exceptions::NullPointerException(
                    __FILE__, __LINE__, "Given Source Pointer was null." );
            }

            for( std::size_t i = 0; i < length; ++i ) {
                dest[destPos+i] = src[srcPos+i];
            }
        }

        /**
         * Enumerates the system environment and returns a map of env variable
         * names to the string values they hold.
         *
         * @return A Map of all environment variables.
         *
         * @throw Exception if an error occurs while getting the Environment Map.
         */
        static const util::Map<std::string, std::string>& getenv();

        /**
         * Reads an environment value from the system and returns it as a
         * string object
         *
         * @param name
         *      The environment variable to read.
         *
         * @return a string with the value from the variables or ""
         *
         * @throws an Exception if an error occurs while reading the Env.
         */
        static std::string getenv( const std::string& name );

        /**
         * Clears a set environment value if one is set.
         *
         * @param name
         *      The environment variables to clear.
         *
         * @throws an Exception if an error occurs while reading the environment.
         */
        static void unsetenv( const std::string& name );

        /**
         * Sets the specified system property to the value given.
         *
         * @param name
         *      The name of the environment variables to set.
         * @param value
         *      The value to assign to name.
         *
         * @throws an Exception if an error occurs when setting the environment variable.
         */
        static void setenv( const std::string& name, const std::string& value );

        /**
         * Returns the current time in milliseconds. Note that while the unit of time of
         * the return value is a millisecond, the granularity of the value depends on the
         * underlying operating system and may be larger. For example, many operating
         * systems measure time in units of tens of milliseconds.
         *
         * See the description of the class Date for a discussion of slight discrepancies
         * that may arise between "computer time" and coordinated universal time (UTC).
         *
         * @returns the difference, measured in milliseconds, between the current time
         *          and midnight, January 1, 1970 UTC.
         */
        static long long currentTimeMillis();

        /**
         * Returns the current value of the most precise available system timer, in
         * nanoseconds.
         *
         * This method can only be used to measure elapsed time and is not related to
         * any other notion of system or wall-clock time. The value returned represents
         * nanoseconds since some fixed but arbitrary time (perhaps in the future, so
         * values may be negative). This method provides nanosecond precision, but not
         * necessarily nanosecond accuracy. No guarantees are made about how frequently
         * values change. Differences in successive calls that span greater than
         * approximately 292 years (263 nanoseconds) will not accurately compute elapsed
         * time due to numerical overflow.
         *
         * For example, to measure how long some code takes to execute:
         *
         *   long long startTime = System::nanoTime();
         *   // ... the code being measured ...
         *   long long estimatedTime = System::nanoTime() - startTime;
         *
         * @returns
         *     The current value of the system timer, in nanoseconds.
         */
        static long long nanoTime();

        /**
         * Returns the number of processors available for execution of Decaf Threads.
         *
         * This value may change during a particular execution of a Decaf based application. Applications
         * that are sensitive to the number of available processors should therefore occasionally poll
         * this property and adjust their resource usage appropriately.
         *
         * @return the number of available processors.
         */
        static int availableProcessors();

        /**
         * Gets the Properties object that holds the Properties accessed from calls to
         * getProperty and setProperty.
         *
         * If the Properties has not yet been created or are not yet initialized then they
         * will be on the first call to a Properties accessor.
         *
         * @returns a reference to the static system Properties object.
         */
        static decaf::util::Properties& getProperties();

        /**
         * Gets the specified System property if set, otherwise returns an empty string.
         *
         * If the Properties has not yet been created or are not yet initialized then they
         * will be on the first call to a Properties accessor.

         * @param key
         *      The key name of the desired system property to retrieve.
         *
         * @returns an empty string if the named property is not set, otherwise returns the value.
         *
         * @throws IllegalArgumentException if key is an empty string.
         */
        static std::string getProperty( const std::string& key );

        /**
         * Gets the specified System property if set, otherwise returns the specified default value.
         *
         * If the Properties has not yet been created or are not yet initialized then they
         * will be on the first call to a Properties accessor.
         *
         * @param key
         *      The key name of the desired system property to retrieve.
         * @param defaultValue
         *      The default value to return if the key is not set in the System properties.
         *
         * @returns the value of the named system property or the defaultValue if the property isn't set..
         *
         * @throws IllegalArgumentException if key is an empty string.
         */
        static std::string getProperty( const std::string& key, const std::string& defaultValue );

        /**
         * Sets the System Property to the specified value.
         *
         * @param key
         *      The key name of the system property to set to the given value.
         * @param value
         *      The value to assign to the key.
         *
         * @returns the previous value of the property named by key if there was one, otherwise
         *          returns an empty string.
         *
         * @throws IllegalArgumentException if key is an empty string.
         */
        static std::string setProperty( const std::string& key, const std::string& value );

        /**
         * Clear any value associated with the system property specified.
         *
         * @param key
         *      The key name of the system property to clear.
         *
         * @returns the previous value of the property named by key if there was one, otherwise
         *          returns an empty string.
         *
         * @throws IllegalArgumentException if key is an empty string.
         */
        static std::string clearProperty( const std::string& key );

    private:

        /**
         * Enumerates the environment and return an array of strings
         * with the values.  Caller owns the array.  The array is terminated
         * by an element that holds the value NULL
         *
         * @returns a vector of environment name / value pairs.
         */
        static std::vector< std::string > getEnvArray();

        /**
         * Gets the one and only APR Pool instance
         *
         * @returns a reference to the global APR Pool.
         */
        static internal::AprPool& getAprPool();

    private:

        friend class decaf::lang::Runtime;

        static void initSystem( int argc, char **argv );
        static void shutdownSystem();

    };

}}

#endif /*_DECAF_LANG_SYSTEM_H_*/
