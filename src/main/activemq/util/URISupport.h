/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_UTIL_URISUPPORT_H_
#define _ACTIVEMQ_UTIL_URISUPPORT_H_

#include <activemq/util/Config.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace activemq{
namespace util{

    class AMQCPP_API URISupport {
    public:

        /**
         * Parses the properties out of the provided Broker URI and sets
         * them in the passed Properties Object.
         * @param URI a Broker URI to parse
         * @param properties a Properties object to set the parsed values in
         * @throws IllegalArgumentException if the passed URI is invalid
         */
        static void parseURL( const std::string& URI,
                              decaf::util::Properties& properties )
            throw ( decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Parse the Query portion of a URI String and return a Simple
         * Properties object containing the parameter names as keys, and
         * the parameter values and values of the Properties.
         * @param query. the query string to parse.
         * @returns Properties object with the parsed output.
         */
        static decaf::util::Properties parseQuery( std::string query )
            throw ( decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Parse the Query portion of a URI String and return a Simple
         * Properties object containing the parameter names as keys, and
         * the parameter values and values of the Properties.
         * @param query - the query string to parse.
         * @param properties - object pointer to get the parsed output.
         */
        static void parseQuery( std::string query,
                                decaf::util::Properties* properties )
            throw ( decaf::lang::exceptions::IllegalArgumentException );

    private:

        /**
         * Accepts a string value and checks to see if that value is of the
         * form ${XXX} which is our accepted form for environment variables.
         * If so the we attempt to replace the value with what is stored in that
         * env var, if the var is not set then we throw an IllegalArgumentException.
         * @param value - the value to check for env
         * @returns the env var if value points to an env var else returns value
         * @throws IllegalArgumentException if the var is not set or has bad syntax
         */
        static std::string replaceEnvValues( const std::string& value )
            throw ( decaf::lang::exceptions::IllegalArgumentException );

    };

}}

#endif /*_ACTIVEMQ_UTIL_URISUPPORT_H_*/
