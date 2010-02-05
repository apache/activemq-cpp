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

#ifndef _DECAF_UTIL_LOGGING_ERRORMANAGER_H_
#define _DECAF_UTIL_LOGGING_ERRORMANAGER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Exception.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

#include <string>

namespace decaf {
namespace util {
namespace logging {

    /**
     * ErrorManager objects can be attached to Handlers to process any error that occur on a
     * Handler during Logging.
     *
     * When processing logging output, if a Handler encounters problems then rather than throwing
     * an Exception back to the issuer of the logging call (who is unlikely to be interested) the
     * Handler should call its associated ErrorManager.
     *
     * @since 1.0
     */
    class DECAF_API ErrorManager {
    private:

        decaf::util::concurrent::atomic::AtomicBoolean wasCalled;

    public:

        /**
         * GENERIC_FAILURE is used for failure that don't fit into one of the other categories.
         */
        static const int GENERIC_FAILURE;

        /**
         * WRITE_FAILURE is used when a write to an output stream fails.
         */
        static const int WRITE_FAILURE;

        /**
         * FLUSH_FAILURE is used when a flush to an output stream fails.
         */
        static const int FLUSH_FAILURE;

        /**
         * CLOSE_FAILURE is used when a close of an output stream fails.
         */
        static const int CLOSE_FAILURE;

        /**
         * OPEN_FAILURE is used when an open of an output stream fails.
         */
        static const int OPEN_FAILURE;

        /**
         * FORMAT_FAILURE is used when formatting fails for any reason.
         */
        static const int FORMAT_FAILURE;

    public:

        ErrorManager();

        virtual ~ErrorManager();

        /**
         * The error method is called when a Handler failure occurs.
         *
         * This method may be overridden in subclasses. The default behavior in this base class is
         * that the first call is reported to System.err, and subsequent calls are ignored.
         *
         * @param msg - a descriptive string (may be empty)
         * @param ex - an exception (may be NULL)
         * @param code - an error code defined in ErrorManager
         */
        virtual void error( const std::string& message, decaf::lang::Exception* ex, int code );

    };

}}}

#endif /* _DECAF_UTIL_LOGGING_ERRORMANAGER_H_ */
