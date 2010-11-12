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
#ifndef _CMS_CMSPROPERTIES_H_
#define _CMS_CMSPROPERTIES_H_

#include <cms/Config.h>

#include <map>
#include <string>
#include <vector>

namespace cms{

    /**
     * Interface for a Java-like properties object.  This is essentially
     * a map of key-value string pairs.
     *
     * @since 1.1
     */
    class CMS_API CMSProperties {
    public:

        virtual ~CMSProperties() throw();

        /**
         * Returns the current count of all the Properties that are currently stored in
         * the Properties object.
         *
         * @return the number of properties currently stored.
         */
        virtual int size() const = 0;

        /**
         * Returns true if the properties object is empty
         *
         * @return true if empty
         */
        virtual bool isEmpty() const = 0;

        /**
         * Looks up the value for the given property.
         *
         * @param name
         *      The name of the property to be looked up.
         * @return the value of the property with the given name, if it
         *         exists.  If it does not exist, returns NULL.
         */
        virtual const char* getProperty( const std::string& name ) const = 0;

        /**
         * Looks up the value for the given property.
         *
         * @param name
         *      the name of the property to be looked up.
         * @param defaultValue
         *      The value to be returned if the given property does not exist.
         * @return The value of the property specified by <code>name</code>, if it
         *         exists, otherwise the <code>defaultValue</code>.
         */
        virtual std::string getProperty( const std::string& name,
                                         const std::string& defaultValue ) const = 0;

        /**
         * Sets the value for a given property.  If the property already
         * exists, overwrites the value.
         *
         * @param name
         *      The name of the value to be written.
         * @param value
         *      The value to be written.
         */
        virtual void setProperty( const std::string& name,
                                  const std::string& value ) = 0;

        /**
         * Check to see if the Property exists in the set
         *
         * @param name
         *      the name of the property to check
         * @return true if property exists, false otherwise.
         */
        virtual bool hasProperty( const std::string& name ) const = 0;

        /**
         * Removes the property with the given name.  If the property existed in the
         * collection then it is removed and returned, otherwise an empty string is
         * returned.
         *
         * @param name
         *      the name of the property to be removed.
         *
         * @returns the value that was removed from the Properties, or empty string.
         */
        virtual std::string remove( const std::string& name ) = 0;

        /**
         * Returns a vector containing all the names of the properties currently stored
         * in the Properties object.
         *
         * @returns an STL std::vector<std::string> with all the currently stored property names.
         */
        virtual std::vector<std::string> propertyNames() const = 0;

        /**
         * Method that serializes the contents of the property map to
         * an array.
         *
         * @return list of pairs where the first is the name and the second
         *         is the value.
         */
        virtual std::vector< std::pair< std::string, std::string > > toArray() const = 0;

        /**
         * Copies the contents of the given properties object to this one.
         *
         * @param source
         *      The source properties object.
         */
        virtual void copy( const CMSProperties* source ) = 0;

        /**
         * Clones this object.
         *
         * @returns a replica of this object.
         */
        virtual CMSProperties* clone() const = 0;

        /**
         * Clears all properties from the map.
         */
        virtual void clear() = 0;

        /**
         * Formats the contents of the Properties Object into a string
         * that can be logged, etc.
         *
         * @returns string value of this object.
         */
        virtual std::string toString() const = 0;

   };

}

#endif /*_CMS_CMSPROPERTIES_H_*/
