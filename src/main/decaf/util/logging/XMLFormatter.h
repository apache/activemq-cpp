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

#ifndef _DECAF_UTIL_LOGGING_XMLFORMATTER_H_
#define _DECAF_UTIL_LOGGING_XMLFORMATTER_H_

#include <decaf/util/Config.h>

#include <decaf/util/logging/Formatter.h>
#include <decaf/util/logging/LogRecord.h>

namespace decaf {
namespace util {
namespace logging {

    /**
     * Format a LogRecord into a standard XML format.
     *
     * TODO - Currently only outputs UTF-8
     * The XMLFormatter can be used with arbitrary character encodings, but it is
     * recommended that it normally be used with UTF-8. The character encoding can
     * be set on the output Handler.
     *
     * @since 1.0
     */
    class DECAF_API XMLFormatter : public Formatter {
    public:

        XMLFormatter();

        virtual ~XMLFormatter();

        /**
         * Converts a LogRecord into an XML string.
         *
         * @param record
         *       The log record to be formatted.
         *
         * @return the log record formatted as an XML string.
         */
        virtual std::string format( const LogRecord& record ) const;

        /**
         * Returns the header string for a set of log records formatted as XML
         * strings, using the output handler's encoding if it is defined, otherwise
         * using the default platform encoding.
         *
         * @param handler
         *       The output handler, may be NULL.
         *
         * @return the header string for log records formatted as XML strings.
         */
        virtual std::string getHead( const Handler* handler );

        /**
         * Returns the tail string for a set of log records formatted as XML
         * strings.
         *
         * @param handler
         *       The output handler, may be NULL.
         *
         * @return the tail string for log records formatted as XML strings.
         */
        virtual std::string getTail( const Handler* handler );

    };

}}}

#endif /* _DECAF_UTIL_LOGGING_XMLFORMATTER_H_ */
