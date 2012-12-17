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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_HEXTABLE_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_HEXTABLE_H_

#include <vector>
#include <string>

#include <activemq/util/Config.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace activemq{
namespace wireformat{
namespace openwire{
namespace utils{

    /**
     * The HexTable class maps hexidecimal strings to the value of an index
     * into the table, i.e. the class will return "FF" for the index 255 in
     * the table.
     */
    class AMQCPP_API HexTable {
    private:

        // Vector of Strings mapping hex to the value of the index.
        std::vector<std::string> table;

    public:

        HexTable();
        virtual ~HexTable() {}

        /**
         * Index operator for this Table, will throw an exception if the
         * index requested is out of bounds for this table.
         *
         * @param index
         *      The index of the value in the table to fetch.
         *
         * @returns string containing the hex value if the index
         *
         * @throws IndexOutOfBoundsException if the index exceeds the table size.
         */
        virtual const std::string& operator[]( std::size_t index );
        virtual const std::string& operator[]( std::size_t index ) const;

        /**
         * Returns the max size of this Table.
         *
         * @returns an integer size value for the table.
         */
        virtual std::size_t size() const{
            return table.size();
        }

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_HEXTABLE_H_*/
