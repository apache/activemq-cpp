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

#include <activemq/util/Config.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/util/Map.h>
#include <decaf/util/List.h>

namespace activemq{
namespace util{

    /**
     * Class that wraps around a single value of one of the
     * many types.  Manages memory for complex types, such
     * as strings.  Note: the destructor was left non-virtual
     * so no virtual table will be created.  This probably isn't
     * necessary, but will avoid needless memory allocation. Since
     * we'll never extend this class, not having a virtual
     * destructor isn't a concern.
     */
    class AMQCPP_API PrimitiveValueNode {
    public:

        /**
         * Enumeration for the various primitive types.
         */
        enum PrimitiveType{
            NULL_TYPE          = 0,
            BOOLEAN_TYPE       = 1,
            BYTE_TYPE          = 2,
            CHAR_TYPE          = 3,
            SHORT_TYPE         = 4,
            INTEGER_TYPE       = 5,
            LONG_TYPE          = 6,
            DOUBLE_TYPE        = 7,
            FLOAT_TYPE         = 8,
            STRING_TYPE        = 9,
            BYTE_ARRAY_TYPE    = 10,
            MAP_TYPE           = 11,
            LIST_TYPE          = 12,
            BIG_STRING_TYPE    = 13
        };

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
            std::vector<unsigned char>* byteArrayValue;
            decaf::util::List<PrimitiveValueNode>* listValue;
            decaf::util::Map<std::string, PrimitiveValueNode>* mapValue;

        };

    private:

        PrimitiveType valueType;
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
         * Primtive List Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode( const decaf::util::List<PrimitiveValueNode>& value );

        /**
         * Primtive Map Value Constructor
         * @param value - the new value to store.
         */
        PrimitiveValueNode(
            const decaf::util::Map<std::string, PrimitiveValueNode>& value );

        /**
         * Copy constructor
         * @param node
         *      The instance of another node to copy to this one.
         */
        PrimitiveValueNode( const PrimitiveValueNode& node );

        ~PrimitiveValueNode(){
            clear();
        }

        /**
         * Assignment operator, copies the data from the other node.
         *
         * @param node
         *      The instance of another node to copy to this one.
         */
        PrimitiveValueNode& operator =( const PrimitiveValueNode& node );

        /**
         * Comparison Operator, compares this node to the other node.
         * @return true if the values are the same false otherwise.
         */
        bool operator==( const PrimitiveValueNode& node ) const;

        /**
         * Gets the Value Type of this type wrapper.
         * @return the PrimitiveType value for this wrapper.
         */
        PrimitiveType getType() const { return valueType; }

        /**
         * Gets the internal Primitive Value object from this wrapper.
         * @return a copy of the contained PrimitiveValue
         */
        PrimitiveValue getValue() const {
            return this->value;
        }

        /**
         * Sets the internal PrimitiveVale object to the new value
         * along with the tag for the type that it consists of.
         *
         * @param value
         *      The value to set as the value contained in this Node.
         * @param valueType
         *      The type of the value being set into this one.
         */
        void setValue( const PrimitiveValue& value, PrimitiveType valueType );

        /**
         * Clears the value from this wrapper converting it back to a blank
         * NULL_TYPE value.
         */
        void clear();

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setBool( bool value );

        /**
         * Gets the Boolean value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        bool getBool() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setByte( unsigned char value );

        /**
         * Gets the Byte value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        unsigned char getByte() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setChar( char value );

        /**
         * Gets the Character value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        char getChar() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setShort( short value );

        /**
         * Gets the Short value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        short getShort() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setInt( int value );

        /**
         * Gets the Integer value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        int getInt() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setLong( long long value );

        /**
         * Gets the Long value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        long long getLong() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setFloat( float value );

        /**
         * Gets the Float value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        float getFloat() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setDouble( double value );

        /**
         * Gets the Double value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        double getDouble() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setString( const std::string& value );

        /**
         * Gets the String value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        std::string getString() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setByteArray( const std::vector<unsigned char>& value );

        /**
         * Gets the Byte Array value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        std::vector<unsigned char> getByteArray() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setList( const decaf::util::List<PrimitiveValueNode>& value );

        /**
         * Gets the Primitive List value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        const decaf::util::List<PrimitiveValueNode>& getList() const;

        /**
         * Sets the value of this value node to the new value specified,
         * this method overwrites any data that was previously at the index
         * given.
         * @param value - the new value to assign to the element at index
         */
        void setMap( const decaf::util::Map<std::string, PrimitiveValueNode>& value );

        /**
         * Gets the Primitive Map value of this Node.
         * @return value contained at the given index
         * @throw NoSuchElementException this node cannot be returned as the
         * requested type.
         */
        const decaf::util::Map<std::string, PrimitiveValueNode>& getMap() const;

        /**
         * Creates a string representation of this value.
         * @return string value of this type wrapper.
         */
        std::string toString() const;

    };

}}

#endif /*_ACTIVEMQ_UTIL_PRIMITIVEVALUENODE_H_*/
