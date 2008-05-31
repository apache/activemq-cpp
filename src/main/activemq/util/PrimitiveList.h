/**
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

#ifndef _ACTIVEMQ_UTIL_PRIMITIVELIST_H_
#define _ACTIVEMQ_UTIL_PRIMITIVELIST_H_

#include <string>
#include <vector>
#include <decaf/util/List.h>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <stdio.h>
#include <activemq/util/PrimitiveValueTypes.h>
#include <activemq/util/PrimitiveValueNode.h>

namespace activemq{
namespace util{

    /**
     * List of primitives.
     */
    class PrimitiveList : public decaf::util::List<PrimitiveValueNode> {
    public:

        PrimitiveList();
        virtual ~PrimitiveList() {}

        /**
         * Copy Constructor
         * @param list - the PrimitiveList to copy
         */
        PrimitiveList( const PrimitiveList& src );

        /**
         * Converts the contents into a formatted string that can be output
         * in a Log File or other debugging tool.
         * @returns formatted String of all elements in the list.
         */
        std::string toString() const;

        virtual bool getBool( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setBool( std::size_t index, bool value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual unsigned char getByte( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setByte( std::size_t index, unsigned char value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual char getChar( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setChar( std::size_t index, char value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual short getShort( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setShort( std::size_t index, short value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual int getInt( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setInt( std::size_t index, int value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual long long getLong( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setLong( std::size_t index, long long value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual double getDouble( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setDouble( std::size_t index, double value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual float getFloat( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setFloat( std::size_t index, float value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual std::string getString( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setString( std::size_t index, const std::string& value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        virtual std::vector<unsigned char> getByteArray( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );
        virtual void setByteArray( std::size_t index, const std::vector<unsigned char>& value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

    };

}}

#endif /*_ACTIVEMQ_UTIL_PRIMITIVELIST_H_*/
