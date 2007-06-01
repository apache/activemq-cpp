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

#ifndef _DECAF_LANG_BOOLEAN_H_
#define _DECAF_LANG_BOOLEAN_H_

#include <decaf/lang/Number.h>

namespace decaf{
namespace lang{

    class DECAF_API Boolean : public Number
    {
    public:

        Boolean() {}
        virtual ~Boolean() {}

        /**
         * Parses the String passed and extracts an bool.
         * @param String to parse
         * @return bool value
         */
        static bool parseBoolean( const std::string& value ){
            bool ret = 0;
            std::istringstream istream(value);
            istream.clear();
            istream >> std::boolalpha >> ret;
            return ret;
        }

        /**
         * Converts the bool to a String representation
         * @param bool to convert
         * @return string representation
         */
        static std::string toString( bool value ){
            std::ostringstream ostream;
            ostream << std::boolalpha << value;
            return ostream.str();
        }

    };

}}

#endif /*_DECAF_LANG_BOOLEAN_H_*/
