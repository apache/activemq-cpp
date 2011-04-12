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
#ifndef _DECAF_UTIL_LOGGING_FORMATTER_H_
#define _DECAF_UTIL_LOGGING_FORMATTER_H_

#include <decaf/util/Config.h>
#include <decaf/util/logging/Handler.h>

namespace decaf{
namespace util{
namespace logging{

    /**
     * A Formatter provides support for formatting LogRecords.
     *
     * Typically each logging Handler will have a Formatter associated with
     * it. The Formatter takes a LogRecord and converts it to a string.
     *
     * Some formatters (such as the XMLFormatter) need to wrap head and
     * tail strings around a set of formatted records. The getHeader and
     * getTail methods can be used to obtain these strings.
     */
    class DECAF_API Formatter {
    public:

        virtual ~Formatter() {}

        /**
         * Format the given log record and return the formatted string.
         * @param record The Log Record to Format
         * @returns the formatted record.
         */
        virtual std::string format( const LogRecord& record ) const = 0;

        /**
         * Format the message string from a log record.
         * @param record The Log Record to Format
         * @returns the formatted message
         */
        virtual std::string formatMessage( const LogRecord& record ) const;

        /**
         * Return the header string for a set of formatted records.  In the
         * default implementation this method should return empty string.
         *
         * @param handler
         *      The target handler, can be NULL.
         *
         * @return the head string.
         */
        virtual std::string getHead( const Handler* handler DECAF_UNUSED ) {
            return "";
        }

        /**
         * Return the tail string for a set of formatted records.  In the
         * default implementation this method should return empty string
         * @param handler the target handler, can be null
         * @return the tail string
         */
        virtual std::string getTail( const Handler* handler DECAF_UNUSED ) {
            return "";
        }

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_FORMATTER_H_*/
