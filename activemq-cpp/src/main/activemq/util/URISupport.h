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
#include <activemq/util/CompositeData.h>
#include <decaf/util/Properties.h>
#include <decaf/util/LinkedList.h>
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
                              decaf::util::Properties& properties );

        /**
         * Parses a Composite URI into a Composite Data instance, the Composite URI
         * takes the for scheme://(uri1,uri2,...uriN)?param1=value1, each of the composite
         * URIs is stored in the CompositeData's internal list.
         *
         * @param uri - The Composite URI to parse.
         * @return a new CompositeData object with the parsed data
         *
         * @throw URISyntaxException if the URI is not well formed.
         */
        static CompositeData parseComposite( const URI& uri );

        /**
         * Parse the Query portion of a URI String and return a Simple
         * Properties object containing the parameter names as keys, and
         * the parameter values and values of the Properties.
         *
         * @param query
         *      The query string to parse and extract the encoded properties.
         *
         * @returns Properties object with the parsed output.
         *
         * @throw IllegalArgumentException if the Query string is not well formed.
         */
        static decaf::util::Properties parseQuery( std::string query );

        /**
         * Parse the Query portion of a URI String and return a Simple
         * Properties object containing the parameter names as keys, and
         * the parameter values and values of the Properties.
         * @param query - the query string to parse.
         * @param properties - object pointer to get the parsed output.
         *
         * @throw IllegalArgumentException if the Query string is not well formed.
         */
        static void parseQuery( std::string query,
                                decaf::util::Properties* properties );

        /**
         * Given a properties object create a string that can be appended to a URI
         * as a valid Query string.
         *
         * @param options
         *        Properties object containing key / value query values.
         *
         * @return a valid URI query string.
         *
         * @throw URISyntaxException if the string in the Properties object
         *        can't be encoded into a valid URI Query string.
         */
        static std::string createQueryString( const Properties& options );

    private:

        /**
         * Perform a parse on the given composite URI, placing the results in the passed
         * CompositeData
         *
         * @param uri - The URI to parse
         * @param rc - The CompositeData object to Populate
         * @param ssp - the Scheme Specific Part from the original URI.
         *
         * @throw URISyntaxException if the URI is not well formed.
         */
        static void parseComposite( const URI& uri, CompositeData& rc, const std::string& ssp );

        /**
         * Splits all the Component URIs in a Composite URI into individual strings which
         * can then be parsed separately and stored in a Composite Data object.
         *
         * @param str - the set of Composite URIs
         */
        static decaf::util::LinkedList<std::string> splitComponents( const std::string& str );

        /**
         * Given a string value and a prefix value, return a new string that has the prefix
         * removed if it was part of the passed in string, otherwise just return the passed
         * in string
         *
         * @param value - String to strip
         * @param prefix - value to strip from the passed in string
         *
         * @return the new string with prefix removed.
         */
        static std::string stripPrefix( const std::string& value, const std::string& prefix );

        /**
         * Matches all opening and closing parenthesis in a String
         *
         * @param str - The String to match parenthesis in.
         *
         * @return true if all parenthesis have valid open and close values.
         */
        static bool checkParenthesis( const std::string& str );

        /**
         * Accepts a string value and checks to see if that value is of the
         * form ${XXX} which is our accepted form for environment variables.
         * If so the we attempt to replace the value with what is stored in that
         * env var, if the var is not set then we throw an IllegalArgumentException.
         * @param value - the value to check for env
         * @returns the env var if value points to an env var else returns value
         * @throws IllegalArgumentException if the var is not set or has bad syntax
         */
        static std::string replaceEnvValues( const std::string& value );

    };

}}

#endif /*_ACTIVEMQ_UTIL_URISUPPORT_H_*/
