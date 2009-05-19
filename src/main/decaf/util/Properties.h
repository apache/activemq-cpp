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

#ifndef _DECAF_UTIL_PROPERTIES_H_
#define _DECAF_UTIL_PROPERTIES_H_

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{

    /**
     * Java-like properties class for mapping string names to string values.
     *
     * @since 1.0
     */
    class DECAF_API Properties{
    private:

        std::map< std::string, std::string > properties;

    public:

        Properties();

        virtual ~Properties();

        /**
         * Returns true if the properties object is empty
         * @return true if empty
         */
        virtual bool isEmpty() const;

        /**
         * Looks up the value for the given property.
         * @param name The name of the property to be looked up.
         * @return the value of the property with the given name, if it
         * exists.  If it does not exist, returns NULL.
         */
        virtual const char* getProperty( const std::string& name ) const;
        /**
         * Looks up the value for the given property.
         * @param name the name of the property to be looked up.
         * @param defaultValue The value to be returned if the given
         * property does not exist.
         * @return The value of the property specified by <code>name</code>, if it
         * exists, otherwise the <code>defaultValue</code>.
         */
        virtual std::string getProperty( const std::string& name,
                                         const std::string& defaultValue ) const;

        /**
         * Sets the value for a given property.  If the property already
         * exists, overwrites the value.
         * @param name The name of the value to be written.
         * @param value The value to be written.
         */
        virtual void setProperty( const std::string& name,
                                  const std::string& value );
        /**
         * Check to see if the Property exists in the set
         * @param name - property name to check for in this properties set.
         * @return true if property exists, false otherwise.
         */
        virtual bool hasProperty( const std::string& name ) const;

        /**
         * Removes the property with the given name.
         * @param name the name of the property to remove.
         */
        virtual void remove( const std::string& name );

        /**
         * Method that serializes the contents of the property map to
         * an array.
         * @return list of pairs where the first is the name and the second
         * is the value.
         */
        virtual std::vector< std::pair< std::string, std::string > > toArray() const;

        /**
         * Copies the contents of the given properties object to this one.
         * @param source The source properties object.
         */
        virtual void copy( const Properties* source );

        /**
         * Clones this object.
         * @returns a replica of this object.
         */
        virtual Properties* clone() const;

        /**
         * Clears all properties from the map.
         */
        virtual void clear();

        /**
         * Formats the contents of the Properties Object into a string
         * that can be logged, etc.
         * @returns string value of this object.
         */
        virtual std::string toString() const;

    };

}}

#endif /*_DECAF_UTIL_PROPERTIES_H_*/
