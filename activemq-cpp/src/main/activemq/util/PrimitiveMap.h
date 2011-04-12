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
#include <activemq/util/Config.h>
#include <decaf/util/Config.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/NoSuchElementException.h>
#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/util/PrimitiveValueConverter.h>

namespace activemq{
namespace util{

    /**
     * Map of named primitives.
     */
    class AMQCPP_API PrimitiveMap : public decaf::util::StlMap<std::string, PrimitiveValueNode> {
    private:

        PrimitiveValueConverter converter;

    public:

        /**
         * Default Constructor, creates an empty map.
         */
        PrimitiveMap();

        virtual ~PrimitiveMap();

        /**
         * Copy Constructor
         *
         * @param source
         *      The Decaf Library Map instance whose elements will be copied into this Map.
         */
        PrimitiveMap( const decaf::util::Map<std::string, PrimitiveValueNode>& source );

        /**
         * Copy Constructor
         *
         * @param source
         *      The PrimitiveMap whose elements will be copied into this Map.
         */
        PrimitiveMap( const PrimitiveMap& source );

        /**
         * Converts the contents into a formatted string that can be output
         * in a Log File or other debugging tool.
         *
         * @returns formatted String of all elements in the map.
         */
        std::string toString() const;

        /**
         * Gets the Boolean value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual bool getBool( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setBool( const std::string& key, bool value );

        /**
         * Gets the Byte value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual unsigned char getByte( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setByte( const std::string& key, unsigned char value );

        /**
         * Gets the Character value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual char getChar( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setChar( const std::string& key, char value );

        /**
         * Gets the Short value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual short getShort( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setShort( const std::string& key, short value );

        /**
         * Gets the Integer value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual int getInt( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setInt( const std::string& key, int value );

        /**
         * Gets the Long value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual long long getLong( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setLong( const std::string& key, long long value );

        /**
         * Gets the Float value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual float getFloat( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setFloat( const std::string& key, float value );

        /**
         * Gets the Double value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual double getDouble( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setDouble( const std::string& key, double value );

        /**
         * Gets the String value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual std::string getString( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setString( const std::string& key, const std::string& value );

        /**
         * Gets the Byte Array value at the given key, if the key is not
         * in the map or cannot be returned as the requested value then
         * an exception of type NoSuchElementException is thrown.
         *
         * @param key - the location to return the value from.
         * @returns the value at key in the type requested.
         * @throw NoSuchElementException if key is not in the map.
         * @throw UnSupportedOperationException if the value cannot be converted
         *                                      to the type this method returns
         */
        virtual std::vector<unsigned char> getByteArray( const std::string& key ) const;

        /**
         * Sets the value at key to the specified type. Overwrites any data
         * that was previously at this key or inserts a new element at key.
         * @param key - the map key to set or insert.
         * @param value - the new value to set at the key location.
         */
        virtual void setByteArray( const std::string& key, const std::vector<unsigned char>& value );

    };

}}

#endif /*ACTIVEMQ_UTIL_PRIMITIVEMAP_H_*/
