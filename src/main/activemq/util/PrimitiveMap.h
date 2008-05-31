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
#include <activemq/util/PrimitiveValueTypes.h>
#include <activemq/util/PrimitiveValueNode.h>

namespace activemq{
namespace util{

    /**
     * Map of named primitives.
     */
    class PrimitiveMap {
    private:

        decaf::util::Map<std::string, PrimitiveValueNode> valueNodeMap;

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
        virtual std::vector<PrimitiveValueNode> getValues() const;

        /**
         * Get a Value from the Map, or throws a NoSuchElementException
         * @param key - string key to lookup
         * @return the concrete Value
         * @throws NoSuchElementException
         */
        PrimitiveValueNode getValue( const std::string& key ) const
            throw( decaf::lang::exceptions::NoSuchElementException );

    };

}}

#endif /*ACTIVEMQ_UTIL_PRIMITIVEMAP_H_*/
