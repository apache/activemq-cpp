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

#ifndef _ACTIVEMQ_UTIL_PRIMITIVEVALUENODE_H_
#define _ACTIVEMQ_UTIL_PRIMITIVEVALUENODE_H_

#include <activemq/util/PrimitiveValueTypes.h>
#include <decaf/lang/exceptions/NoSuchElementException.h>

namespace activemq{
namespace util{

    class PrimitiveMap;
    class PrimitiveList;

    /**
     * Class that wraps around a single value of one of the
     * many types.  Manages memory for complex types, such
     * as strings.  Note: the destructor was left non-virtual
     * so no virtual table will be created.  This probaly isn't
     * necessary, but will avoid needless memory allocation. Since
     * we'll never extend this class, not having a virtual
     * destructor isn't a concern.
     */
    class PrimitiveValueNode {
    public:

        /**
         * Define a union type comprised of the various types.
         */
        union PrimitiveValue {

            bool boolValue;
            unsigned char byteValue;
            char charValue;
            short shortValue;
            int intValue;
            long long longValue;
            double doubleValue;
            float floatValue;
            std::string* stringValue;
            PrimitiveMap* mapValue;
            PrimitiveList* listValue;
            std::vector<unsigned char>* byteArrayValue;

        };

    private:

        PrimitiveValueTypeEnum valueType;
        PrimitiveValue value;

    public:

        /**
         * Default Constructor, creates a value of the NULL_TYPE.
         */
        PrimitiveValueNode();

        /**
         * Boolean Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( bool value );

        /**
         * Byte Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( unsigned char value );

        /**
         * Char Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( char value );

        /**
         * Short Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( short value );

        /**
         * Int Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( int value );

        /**
         * Long Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( long long value );

        /**
         * Float Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( float value );

        /**
         * Double Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( double value );

        /**
         * String Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( const char* value );

        /**
         * String Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( const std::string& value );

        /**
         * Byte Array Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( const std::vector<unsigned char>& value );

        /**
         * Copy constructor
         * @param another node to copy.
         */
        PrimitiveValueNode( const PrimitiveValueNode& node );

        /**
         * Destructor, clears the data before destruction to free any data
         * in the PrimitiveValue union.
         */
        ~PrimitiveValueNode(){
            clear();
        }

        /**
         * Assignment operator, copies the data from the other node.
         */
        PrimitiveValueNode& operator =( const PrimitiveValueNode& node );

        /**
         * Comparison Operator, compares this node to the other node.
         * @return true if the values are the same false otherwise.
         */
        bool operator==( const PrimitiveValueNode& node ) const;

        /**
         * Gets the Value Type of this type wrapper.
         * @return the PrimtiveValueTypeEnum value for this wrapper.
         */
        PrimitiveValueTypeEnum getValueType() const { return valueType; }

        /**
         * Gets the internal Primitive Value object from this wrapper.
         * @return a copy of the contianed PrimitiveValue
         */
        PrimitiveValue getValue() const {
            return this->value;
        }

        /**
         * Sets the internal PrimitiveVale object to the new value
         * along with the tag for the type that it consists of.
         */
        void setValue( const PrimitiveValue& value, PrimitiveValueTypeEnum valueType ) {
            this->value = value;
            this->valueType = valueType;
        }

        /**
         * Clears the value from this wrapper converting it back to a blank
         * NULL_TYPE value.
         */
        void clear();

        void setBool( bool lvalue );
        bool getBool() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setByte( unsigned char lvalue );
        unsigned char getByte() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setChar( char lvalue );
        char getChar() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setShort( short lvalue );
        short getShort() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setInt( int lvalue );
        int getInt() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setLong( long long lvalue );
        long long getLong() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setDouble( double lvalue );
        double getDouble() const
            throw(decaf::lang::exceptions::NoSuchElementException );

        void setFloat( float lvalue );
        float getFloat() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setString( const std::string& lvalue );
        std::string getString() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        void setByteArray( const std::vector<unsigned char>& lvalue );
        std::vector<unsigned char> getByteArray() const
            throw( decaf::lang::exceptions::NoSuchElementException );

        /**
         * Creates a string representation of this value.
         * @return string value of this type wrapper.
         */
        std::string toString() const;

    };

}}

#endif /*_ACTIVEMQ_UTIL_PRIMITIVEVALUENODE_H_*/
