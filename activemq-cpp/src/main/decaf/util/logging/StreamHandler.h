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
#ifndef _DECAF_UTIL_LOGGING_STREAMHANDLER_H_
#define _DECAF_UTIL_LOGGING_STREAMHANDLER_H_

#include <decaf/util/logging/LoggerCommon.h>
#include <decaf/util/logging/Handler.h>
#include <decaf/util/logging/Formatter.h>
#include <decaf/util/logging/Filter.h>
#include <decaf/util/logging/ErrorManager.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace io{
    class OutputStream;
}
namespace util{
namespace logging{

    class DECAF_API StreamHandler : public Handler {
    private:

        // OutputStream to write to
        decaf::io::OutputStream* stream;

        // Formats this Handlers output
        Formatter* formatter;

        // Filter object for Log Filtering
        Filter* filter;

        // ErrorManager instance for this Handler
        ErrorManager* errorManager;

        // Level at which that this Handler will start logging.
        Level level;

    public:

        /**
         * Create a StreamHandler, with no current output stream.
         */
        StreamHandler();

        /**
         * Create a StreamHandler, with no current output stream.
         */
        StreamHandler( decaf::io::OutputStream* stream, Formatter* formatter );

        virtual ~StreamHandler();

        /**
         * Close the current output stream.
         * <p>
         * The close method will perform a flush and then close the Handler.
         * After close has been called this Handler  should no longer be used.
         * Method calls may either be silently ignored or may throw runtime
         * exceptions.
         *
         * @throw IOException
         */
        virtual void close() throw ( decaf::io::IOException );

        /**
         * Flush the Handler's output, clears any buffers.
         */
        virtual void flush();

        /**
         * Publish the Log Record to this Handler
         *
         * @param record
         *      The <code>LogRecord</code> to Publish
         */
        virtual void publish( const LogRecord& record );

        /**
         * Check if this Handler would actually log a given LogRecord.
         * <p>
         * @param record
         *      <code>LogRecord</code> to check
         *
         * @return true if the record can be logged with current settings.
         */
        virtual bool isLoggable( const LogRecord& record ) const;

        /**
         * Sets the Filter that this Handler uses to filter Log Records
         * @param filter
         *      <code>Filter</code> derived instance
         */
        virtual void setFilter( Filter* filter ) {
            this->filter = filter;
        }

        /**
         * Gets the Filter that this Handler uses to filter Log Records
         * @return <code>Filter</code> derived instance
         */
        virtual const Filter* getFilter() {
            return filter;
        }

        /**
         * Set the log level specifying which message levels will be logged
         * by this Handler.
         * <p>
         * The intention is to allow developers to turn on verbose logging,
         * but to limit the messages that are sent to certain Handlers.
         *
         * @param level
         *      Level enumeration value
         */
        virtual void setLevel( Level level ) {
            this->level = level;
        }

        /**
         * Get the log level specifying which message levels will be logged
         * by this Handler.
         * @return Currently set Level enumeration value
         */
        virtual Level getLevel() {
            return level;
        }

        /**
         * Sets the <code>Formatter</code> used by this Handler
         * @param formatter
         *      <code>Formatter</code> derived instance
         */
        virtual void setFormatter( Formatter* formatter ){
            this->formatter = formatter;
        }

        /**
         * Gets the <code>Formatter</code> used by this Handler
         * @return currently configured <code>Formatter</code> derived instance
         */
        virtual const Formatter* getFormatter(){
            return formatter;
        }

        /**
         * Gets the output Stream that this Handler is using
         * @return OuputStream pointer used by this handler.
         */
        virtual io::OutputStream* getOutputStream() const{
            return stream;
        }

        /**
         * Sets the <code>Formatter</code> used by this Handler
         * <p>
         * The ErrorManager's "error" method will be invoked if any errors occur while
         * using this Handler.
         *
         * @param errorManager <code>ErrorManager</code> derived instance
         */
        virtual void setErrorManager( ErrorManager* errorManager ) {
            this->errorManager = errorManager;
        }

        /**
         * Gets the <code>ErrorManager</code> used by this Handler.
         * @returns <code>ErrorManager</code> derived pointer or NULL.
         */
        virtual const ErrorManager* getErrorManager() {
            return this->errorManager;
        }

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_STREAMHANDLER_H_*/
