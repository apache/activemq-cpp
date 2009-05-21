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

#ifndef _DECAF_SECURITY_PRINCIPAL_H_
#define _DECAF_SECURITY_PRINCIPAL_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace security {

    /**
     * Base interface for a principal, which can represent an individual or
     * organization.
     */
    class DECAF_API Principal {
    public:

        virtual ~Principal() {}

        /**
         * Compares two principals to see if they are the same.
         *
         * @param another
         *      A principal to be tested for equality to this one.
         * @return true if the given principal is equivalent to this one.
         */
        virtual bool equals( const Principal& another ) const = 0;

        /**
         * Provides the name of this principal.
         *
         * @return the name of this principal.
         */
        virtual std::string getName() const = 0;
    };
}}

#endif /*_DECAF_SECURITY_PRINCIPAL_H_*/
