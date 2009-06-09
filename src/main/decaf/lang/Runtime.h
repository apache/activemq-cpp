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

#ifndef _DECAF_LANG_RUNTIME_H_
#define _DECAF_LANG_RUNTIME_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace lang {

    class DECAF_API Runtime {
    public:

        virtual ~Runtime() {}

        /**
         * Gets the single instance of the Decaf Runtime for this Process.
         *
         * @returns pointer to the single Decaf Runtime instance that exists
         *          for this process
         */
        static Runtime* getRuntime();

        /**
         * Initialize the Decaf Library passing it the args that were passed
         * to the application at startup.
         *
         * @param argc - The number of args passed
         * @param argv - Array of char* values passed to the Process on start.
         *
         * @throws runtime_error if the library is already initialized or an
         *         error occurs during initialization.
         */
        static void initializeRuntime( int argc, char **argv );

        /**
         * Initialize the Decaf Library
         *
         * @throws runtime_error if the library is already initialized or an
         *         error occurs during initialization.
         */
        static void initializeRuntime();

        /**
         * Shutdown the Decaf Library, this call should take places after all
         * objects that were created from the Decaf library have been deallocated.
         *
         * @throws runtime_error if the library has not already been initialized or an
         *         error occurs during shutdown.
         */
        static void shutdownRuntime();

    };

}}

#endif /*_DECAF_LANG_RUNTIME_H_*/
