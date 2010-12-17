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
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace io{
    class OutputStream;
    class Writer;
}
namespace util{
namespace logging{

    /**
     * Stream based logging Handler.
     *
     * This is primarily intended as a base class or support class to be used in implementing
     * other logging Handlers.
     *
     * LogRecords are published to a given <code>decaf::io::OutputStream</code>.
     *
     * Configuration: By default each StreamHandler is initialized using the following LogManager
     * configuration properties. If properties are not defined (or have invalid values) then the
     * specified default values are used.
     *
     *   * decaf.util.logging.StreamHandler.level specifies the default level for the Handler
     *     (defaults to Level.INFO).
     *   * decaf.util.logging.StreamHandler.filter specifies the name of a Filter class to use
     *     (defaults to no Filter).
     *   * decaf.util.logging.StreamHandler.formatter specifies the name of a Formatter class
     *     to use (defaults to decaf.util.logging.SimpleFormatter).
     *
     * @since 1.0
     */
    class DECAF_API StreamHandler : public Handler {
    private:

        // OutputStream to write to
        decaf::io::OutputStream* stream;

        // A Writer to wrap the OutputStream
        decaf::io::Writer* writer;

        // Indicates if the writer has been initialized already
        bool writerNotInitialized;

    private:

        StreamHandler( const StreamHandler& );
        StreamHandler& operator= ( const StreamHandler& );

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
         *
         * The close method will perform a flush and then close the Handler.
         * After close has been called this Handler  should no longer be used.
         * Method calls may either be silently ignored or may throw runtime
         * exceptions.
         *
         * @throw IOException if an I/O error occurs.
         */
        virtual void close();

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

    protected:

        /**
         * Change the output stream.
         *
         * If there is a current output stream then the Formatter's tail string is written and
         * the stream is flushed and closed. Then the output stream is replaced with the new
         * output stream.
         *
         * @param stream
         *      The new output stream. May not be NULL.
         *
         * @throws NullPointerException if the passed stream is NULL.
         */
        virtual void setOuputStream( decaf::io::OutputStream* stream );

        /**
         * Closes this handler, but the underlying output stream is only closed if
         * closeStream is true.
         *
         * @param closeStream
         *      whether to close the underlying output stream.
         */
        void close( bool closeStream );

    private:

        // Safely writes the string to the output stream, calling the ErrorManager
        // if any exceptions are thrown while writing.
        void write( const std::string& value );

        // Initialize the Writer if its not already been initialized.
        void initializeWritter();

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_STREAMHANDLER_H_*/
