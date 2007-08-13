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

#ifndef _DECAF_NET_URI_H_
#define _DECAF_NET_URI_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <string>

namespace decaf{
namespace net{

    class DECAF_API URI : public lang::Comparable<URI> {
    public:

        /**
         * Constructs a URI from the given string
         * @param uri - string uri to parse.
         */
        URI( const std::string& uri );

        virtual ~URI() {}

        /**
         * Compares this object with the specified object for order. Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         * @param value - the value to compare to this one.
         * @returns zero if equal minus one if less than and one if greater than.
         */
        virtual int compareTo( const URI& value ) const;

        /**
         * @return true if this value is considered equal to the passed value.
         */
        virtual bool equals( const URI& value ) const;

        /**
         * Compares equality between this object and the one passed.
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator==( const URI& value ) const;

        /**
         * Compares this object to another and returns true if this object
         * is considered to be less than the one passed.  This
         * @param value - the value to be compared to this one.
         * @return true if this object is equal to the one passed.
         */
        virtual bool operator<( const URI& value ) const;

    };

}}

#endif /*_DECAF_NET_URI_H_*/
