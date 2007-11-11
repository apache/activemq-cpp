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

#ifndef ACTIVEMQ_UTIL_PRIMITIVEMAP_H_
#define ACTIVEMQ_UTIL_PRIMITIVEMAP_H_

#include <string>
#include <vector>
#include <decaf/util/Map.h>
#include <decaf/lang/exceptions/NoSuchElementException.h>

namespace activemq{
namespace util{

    /**
     * Map of named primitives.
     */
    class PrimitiveMap {
    public:

        /**
         * Enumeration for the various primitive types.
         */
        enum ValueTypeEnum{
            NULL_TYPE               = 0,
            BOOLEAN_TYPE            = 1,
            BYTE_TYPE               = 2,
            CHAR_TYPE               = 3,
            SHORT_TYPE              = 4,
            INTEGER_TYPE            = 5,
            LONG_TYPE               = 6,
            DOUBLE_TYPE             = 7,
            FLOAT_TYPE              = 8,
            STRING_TYPE             = 9,
            BYTE_ARRAY_TYPE         = 10,
            MAP_TYPE                = 11,
            LIST_TYPE               = 12,
            BIG_STRING_TYPE         = 13
        };

        /**
         * Define a union type comprised of the various types.
         */
        union Value {

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
        };

        /**
         * Class that wraps around a single value of one of the
         * many types.  Manages memory for complex types, such
         * as strings.  Note: the destructor was left non-virtual
         * so no virtual table will be created.  This probaly isn't
         * necessary, but will avoid needless memory allocation. Since
         * we'll never extend this class, not having a virtual
         * destructor isn't a concern.
         */
        class ValueNode{
        private:

            ValueTypeEnum valueType;
            Value value;

        public:

            ValueNode(){
                valueType = NULL_TYPE;
                memset( &value, 0, sizeof(value) );
            }

            ValueNode( const ValueNode& node ){
                valueType = NULL_TYPE;
                memset( &value, 0, sizeof(value) );
                (*this) = node;
            }

            ~ValueNode(){
                clear();
            }

            ValueNode& operator =( const ValueNode& node ){
                clear();
                valueType = node.valueType;

                if( valueType == STRING_TYPE && node.value.stringValue != NULL ){
                    value.stringValue = new std::string( *node.value.stringValue );
                } else if( valueType == BYTE_ARRAY_TYPE && node.value.byteArrayValue != NULL ){
                    value.byteArrayValue = new std::vector<unsigned char>( *node.value.byteArrayValue );
                } else{
                    value = node.value;
                }

                return *this;
            }

            bool operator==( const ValueNode& node ) const{

                if( valueType != node.valueType ) {
                     return false;
                }

                if( valueType == BOOLEAN_TYPE &&
                    value.boolValue == node.value.boolValue ) {
                        return true;
                } else if( valueType == BYTE_TYPE &&
                    value.byteValue == node.value.byteValue ) {
                        return true;
                } else if( valueType == CHAR_TYPE &&
                    value.charValue == node.value.charValue ) {
                        return true;
                } else if( valueType == SHORT_TYPE &&
                    value.shortValue == node.value.shortValue ) {
                        return true;
                } else if(  valueType == INTEGER_TYPE &&
                    value.intValue == node.value.intValue ) {
                        return true;
                } else if( valueType == LONG_TYPE &&
                    value.longValue == node.value.longValue ) {
                        return true;
                } else if( valueType == DOUBLE_TYPE &&
                    value.doubleValue == node.value.doubleValue ) {
                        return true;
                } else if( valueType == FLOAT_TYPE &&
                    value.floatValue == node.value.floatValue ) {
                        return true;
                } else if( valueType == STRING_TYPE &&
                    *value.stringValue == *node.value.stringValue ) {
                        return true;
                } else if( valueType == BYTE_ARRAY_TYPE &&
                    *value.byteArrayValue == *node.value.byteArrayValue ) {
                        return true;
                }

                return false;
            }

            ValueTypeEnum getValueType() const { return valueType; }

            void clear(){
                if( valueType == STRING_TYPE && value.stringValue != NULL ){
                    delete value.stringValue;
                }
                if( valueType == BYTE_ARRAY_TYPE && value.byteArrayValue != NULL ){
                    delete value.byteArrayValue;
                }
                valueType = NULL_TYPE;
                memset( &value, 0, sizeof(value) );
            }

            void setBool( bool lvalue ){
                clear();
                valueType = BOOLEAN_TYPE;
                value.boolValue = lvalue;
            }

            bool getBool() const throw( decaf::lang::exceptions::NoSuchElementException ){
                if( valueType != BOOLEAN_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not BOOLEAN_TYPE" );
                }

                return value.boolValue;
            }

            void setByte( unsigned char lvalue ){
                clear();
                valueType = BYTE_TYPE;
                value.byteValue = lvalue;
            }

            unsigned char getByte() const throw( decaf::lang::exceptions::NoSuchElementException){
                if( valueType != BYTE_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not BYTE_TYPE" );
                }

                return value.byteValue;
            }

            void setChar( char lvalue ){
                clear();
                valueType = CHAR_TYPE;
                value.charValue = lvalue;
            }

            char getChar() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != CHAR_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not CHAR_TYPE" );
                }

                return value.charValue;
            }

            void setShort( short lvalue ){
                clear();
                valueType = SHORT_TYPE;
                value.shortValue = lvalue;
            }

            short getShort() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != SHORT_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not SHORT_TYPE" );
                }

                return value.shortValue;
            }

            void setInt( int lvalue ){
                clear();
                valueType = INTEGER_TYPE;
                value.intValue = lvalue;
            }

            int getInt() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != INTEGER_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not INTEGER_TYPE" );
                }

                return value.intValue;
            }

            void setLong( long long lvalue ){
                clear();
                valueType = LONG_TYPE;
                value.longValue = lvalue;
            }

            long long getLong() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != LONG_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not LONG_TYPE" );
                }

                return value.longValue;
            }

            void setDouble( double lvalue ){
                clear();
                valueType = DOUBLE_TYPE;
                value.doubleValue = lvalue;
            }

            double getDouble() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != DOUBLE_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not DOUBLE_TYPE" );
                }

                return value.doubleValue;
            }

            void setFloat( float lvalue ){
                clear();
                valueType = FLOAT_TYPE;
                value.floatValue = lvalue;
            }

            float getFloat() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != FLOAT_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not FLOAT_TYPE" );
                }

                return value.floatValue;
            }

            void setString( const std::string& lvalue ){
                clear();
                valueType = STRING_TYPE;
                value.stringValue = new std::string( lvalue );
            }

            std::string getString() const throw(decaf::lang::exceptions::NoSuchElementException){
                if( valueType != STRING_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException( __FILE__,
                        __LINE__, "Value is not STRING_TYPE" );
                }

                if( value.stringValue == NULL ){
                    return std::string();
                }

                return *value.stringValue;
            }

            void setByteArray( const std::vector<unsigned char>& lvalue ){
                clear();
                valueType = BYTE_ARRAY_TYPE;
                value.byteArrayValue = new std::vector<unsigned char>( lvalue );
            }

            std::vector<unsigned char> getByteArray() const throw( decaf::lang::exceptions::NoSuchElementException ) {

                if( valueType != BYTE_ARRAY_TYPE ){
                    throw decaf::lang::exceptions::NoSuchElementException(
                        __FILE__,
                        __LINE__,
                        "Value is not BYTE_ARRAY_TYPE" );
                }

                if( value.byteArrayValue == NULL ){
                    return std::vector<unsigned char>();
                }

                return *value.byteArrayValue;
            }

            std::string toString() const {
                std::ostringstream stream;

                if( valueType == BOOLEAN_TYPE ) {
                    stream << value.boolValue;
                } else if( valueType == BYTE_TYPE ) {
                    stream << value.byteValue;
                } else if( valueType == CHAR_TYPE ) {
                    stream << value.charValue;
                } else if( valueType == SHORT_TYPE ) {
                    stream << value.shortValue;
                } else if(  valueType == INTEGER_TYPE ) {
                    stream << value.intValue;
                } else if( valueType == LONG_TYPE ) {
                    stream << value.longValue;
                } else if( valueType == DOUBLE_TYPE ) {
                    stream << value.doubleValue;
                } else if( valueType == FLOAT_TYPE ) {
                    stream << value.floatValue;
                } else if( valueType == STRING_TYPE ) {
                    stream << *value.stringValue;
                } else if( valueType == BYTE_ARRAY_TYPE ) {
                    std::vector<unsigned char>::const_iterator iter =
                        value.byteArrayValue->begin();
                    for( ; iter != value.byteArrayValue->end(); ++iter ) {
                        stream << '[' << (int)(*iter) << ']';
                    }
                }

                return stream.str();
            }

        };


    private:

        decaf::util::Map<std::string, ValueNode> valueNodeMap;

    public:

        PrimitiveMap();
        virtual ~PrimitiveMap() {}

        /**
         * Copy Constructor
         * @param map - the PrimitiveMap to copy
         */
        PrimitiveMap( const PrimitiveMap& src );

        /**
         * Compares this PrimitiveMap to another
         * @param source - Map to compare to
         * @return true if the maps are equeal
         */
        bool equals( const PrimitiveMap& source ) const;

        /**
         * Converts the contents into a formatted string that can be output
         * in a Log File or other debugging tool.
         * @returns formatted String of all elements in the map.
         */
        std::string toString() const;

        /**
         * Comparison Operator
         * @param src - Map to compare to this one.
         * @returns true if Maps are equal.
         */
        void copy( const PrimitiveMap& src );

        /**
         * Removes all keys and values from all maps.
         */
        virtual void clear();

        /**
         * Indicates whether or this map contains a value for the
         * given key.
         * @param key The key to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool contains( const std::string& key ) const;

        /**
         * @return The number of elements (key/value pairs) in this map.
         */
        virtual std::size_t size() const;

        /**
         * @returns if there are any entries in the map.
         */
        virtual bool isEmpty() const;

        virtual bool getBool( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setBool( const std::string& key, bool value );

        virtual unsigned char getByte( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setByte( const std::string& key, unsigned char value );

        virtual char getChar( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setChar( const std::string& key, char value );

        virtual short getShort( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setShort( const std::string& key, short value );

        virtual int getInt( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setInt( const std::string& key, int value );

        virtual long long getLong( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setLong( const std::string& key, long long value );

        virtual double getDouble( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setDouble( const std::string& key, double value );

        virtual float getFloat( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setFloat( const std::string& key, float value );

        virtual std::string getString( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setString( const std::string& key, const std::string& value );

        virtual std::vector<unsigned char> getByteArray( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );
        virtual void setByteArray( const std::string& key, const std::vector<unsigned char>& value );

        /**
         * Removes the value (key/value pair) for the specified key from
         * the map.
         * @param key The search key.
         */
        virtual void remove( const std::string& key );

        /**
         * @return the entire set of keys in this map as a std::vector.
         */
        virtual std::vector<std::string> getKeys() const;

        /**
         * @return the entire set of values in this map as a std::vector.
         */
        virtual std::vector<ValueNode> getValues() const;

        /**
         * Get a Value from the Map, or throws a NoSuchElementException
         * @param key - string key to lookup
         * @returns the concrete Value
         * @throws NoSuchElementException
         */
        ValueNode getValue( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );

    };

}}

#endif /*ACTIVEMQ_UTIL_PRIMITIVEMAP_H_*/
