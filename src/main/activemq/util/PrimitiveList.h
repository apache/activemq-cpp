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
#include <activemq/util/PrimitiveValueNode.h>

namespace activemq{
namespace util{

    /**
     * List of primitives.
     */
    class AMQCPP_API PrimitiveList : public decaf::util::List<PrimitiveValueNode> {
    public:

        /**
         * Default Constructor, creates an Empry list.
         */
        PrimitiveList();

        virtual ~PrimitiveList() {
            clear();
        }

        /**
         * Copy Constructor
         * @param src - the Decaf List of PrimtiveNodeValues to copy
         */
        PrimitiveList( const decaf::util::List<PrimitiveValueNode>& src );

        /**
         * Copy Constructor
         * @param src - the PrimitiveList to copy
         */
        PrimitiveList( const PrimitiveList& src );

        /**
         * Converts the contents into a formatted string that can be output
         * in a Log File or other debugging tool.
         * @returns formatted String of all elements in the list.
         */
        std::string toString() const;

        /**
         * Gets the Boolean value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual bool getBool( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setBool( std::size_t index, bool value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Byte value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual unsigned char getByte( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setByte( std::size_t index, unsigned char value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Character value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual char getChar( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setChar( std::size_t index, char value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Short value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual short getShort( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setShort( std::size_t index, short value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Integer value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual int getInt( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setInt( std::size_t index, int value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Long value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual long long getLong( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setLong( std::size_t index, long long value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Float value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual float getFloat( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setFloat( std::size_t index, float value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Double value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual double getDouble( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setDouble( std::size_t index, double value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the String value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual std::string getString( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setString( std::size_t index, const std::string& value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Gets the Byte Array value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw NoSuchElementException if the type at index is not of the
         * type that this method is to return.
         */
        virtual std::vector<unsigned char> getByteArray( std::size_t index ) const
            throw( decaf::lang::exceptions::IndexOutOfBoundsException,
                   decaf::lang::exceptions::NoSuchElementException );

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setByteArray( std::size_t index, const std::vector<unsigned char>& value )
            throw( decaf::lang::exceptions::IndexOutOfBoundsException );

    };

}}

#endif /*_ACTIVEMQ_UTIL_PRIMITIVELIST_H_*/
