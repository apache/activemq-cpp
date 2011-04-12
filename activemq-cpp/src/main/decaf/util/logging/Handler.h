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
#ifndef _DECAF_UTIL_LOGGING_HANDLER_H_
#define _DECAF_UTIL_LOGGING_HANDLER_H_

#include <decaf/io/Closeable.h>
#include <decaf/lang/Exception.h>
#include <decaf/util/logging/LogRecord.h>
#include <decaf/util/logging/Level.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <string>

namespace decaf{
namespace util{
namespace logging{

    class Filter;
    class Formatter;
    class ErrorManager;

    /**
     * A Handler object takes log messages from a Logger and exports them.
     * It might for example, write them to a console or write them to a file,
     * or send them to a network logging service, or forward them to an OS
     * log, or whatever.
     *
     * A Handler can be disabled by doing a setLevel(Level.OFF) and can be
     * re-enabled by doing a setLevel with an appropriate level.
     *
     * Handler classes typically use LogManager properties to set default
     * values for the Handler's Filter, Formatter, and Level. See the
     * specific documentation for each concrete Handler class.
     */
    class DECAF_API Handler : public io::Closeable {
    private:

        // Default Logging Level for Handler
        static const Level DEFAULT_LEVEL;

        // Formats this Handlers output
        Formatter* formatter;

        // Filter object for Log Filtering
        Filter* filter;

        // ErrorManager instance for this Handler
        ErrorManager* errorManager;

        // Level at which that this Handler will start logging.
        Level level;

        // Name of this class used to read properties
        std::string prefix;

    private:

        Handler( const Handler& );
        Handler& operator= ( const Handler& );

    public:

        Handler();

        virtual ~Handler();

        /**
         * Flush the Handler's output, clears any buffers.
         */
        virtual void flush() = 0;

        /**
         * Publish the Log Record to this Handler
         * @param record The Log Record to Publish
         */
        virtual void publish( const LogRecord& record ) = 0;

        /**
         * Check if this Handler would actually log a given LogRecord.
         * <p>
         * This method checks if the LogRecord has an appropriate Level and
         * whether it satisfies any Filter. It also may make other Handler
         * specific checks that might prevent a handler from logging the
         * LogRecord.
         * @param record <code>LogRecord</code> to check
         */
        virtual bool isLoggable( const LogRecord& record ) const;

        /**
         * Sets the Filter that this Handler uses to filter Log Records
         *
         * For each call of publish the Handler will call this Filter (if it
         * is non-null) to check if the LogRecord should be published or
         * discarded.
         *
         * @param filter <code>Filter</code> derived instance
         */
        virtual void setFilter( Filter* filter ) {
            this->filter = filter;
        }

        /**
         * Gets the Filter that this Handler uses to filter Log Records
         * @returns <code>Filter</code> derived instance
         */
        virtual Filter* getFilter() {
            return this->filter;
        }

        /**
         * Set the log level specifying which message levels will be logged
         * by this Handler.
         * <p>
         * The intention is to allow developers to turn on voluminous logging,
         * but to limit the messages that are sent to certain Handlers.
         * @param value Level enumeration value
         */
        virtual void setLevel( const Level& value ) {
            this->level = value;
        }

        /**
         * Get the log level specifying which message levels will be logged
         * by this Handler.
         * @returns Level enumeration value
         */
        virtual Level getLevel() {
            return this->level;
        }

        /**
         * Sets the <code>Formatter</code> used by this Handler
         * <p>
         * Some Handlers may not use Formatters, in which case the
         * Formatter will be remembered, but not used.
         * @param formatter <code>Filter</code> derived instance
         */
        virtual void setFormatter( Formatter* formatter );

        /**
         * Gets the <code>Formatter</code> used by this Handler
         * @returns <code>Filter</code> derived instance
         */
        virtual Formatter* getFormatter() {
            return this->formatter;
        }

        /**
         * Sets the <code>Formatter</code> used by this Handler
         * <p>
         * The ErrorManager's "error" method will be invoked if any errors occur while
         * using this Handler.
         *
         * @param errorManager <code>ErrorManager</code> derived instance
         */
        virtual void setErrorManager( ErrorManager* errorManager );

        /**
         * Gets the <code>ErrorManager</code> used by this Handler.
         * @returns <code>ErrorManager</code> derived pointer or NULL.
         */
        virtual ErrorManager* getErrorManager() {
            return this->errorManager;
        }

    protected:

        /**
         * Protected convenience method to report an error to this Handler's ErrorManager.
         *
         * @param message - a descriptive string (may be empty)
         * @param ex - an exception (may be NULL)
         * @param code - an error code defined in ErrorManager
         */
        void reportError( const std::string& message, decaf::lang::Exception* ex, int code );

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_HANDLER_H_*/
