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
#include <decaf/util/LinkedList.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <stdio.h>
#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/util/PrimitiveValueConverter.h>

namespace activemq{
namespace util{

    /**
     * List of primitives.
     */
    class AMQCPP_API PrimitiveList : public decaf::util::LinkedList<PrimitiveValueNode> {
    private:

        PrimitiveValueConverter converter;

    public:

        /**
         * Default Constructor, creates an Empty list.
         */
        PrimitiveList();

        virtual ~PrimitiveList();

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
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual bool getBool( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setBool( int index, bool value );

        /**
         * Gets the Byte value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual unsigned char getByte( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setByte( int index, unsigned char value );

        /**
         * Gets the Character value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual char getChar( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setChar( int index, char value );

        /**
         * Gets the Short value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual short getShort( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setShort( int index, short value );

        /**
         * Gets the Integer value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual int getInt( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setInt( int index, int value );

        /**
         * Gets the Long value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual long long getLong( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setLong( int index, long long value );

        /**
         * Gets the Float value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual float getFloat( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setFloat( int index, float value );

        /**
         * Gets the Double value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual double getDouble( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setDouble( int index, double value );

        /**
         * Gets the String value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual std::string getString( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setString( int index, const std::string& value );

        /**
         * Gets the Byte Array value at the specified index.
         * @param index - index to get value from
         * @return value contained at the given index
         * @throw IndexOutOfBoundsException if index is > size()
         * @throw UnsupportedOperationException if the type at index is not of the
         * type that this method is to return or can convert to.
         */
        virtual std::vector<unsigned char> getByteArray( int index ) const;

        /**
         * Sets the value at the given index to the new value specified,
         * this method overwrites any data that was previously at the index
         * given, but does not insert a new element if the index is greater
         * than the size of the list.
         * @param index - location to set in the list
         * @param value - the new value to assign to the element at index
         * @throw IndexOutOfBoundsException if index > size().
         */
        virtual void setByteArray( int index, const std::vector<unsigned char>& value );

    };

}}

#endif /*_ACTIVEMQ_UTIL_PRIMITIVELIST_H_*/
