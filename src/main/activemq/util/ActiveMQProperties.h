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

#ifndef _ACTIVEMQ_UTIL_ACTIVEMQPROPERTIES_H_
#define _ACTIVEMQ_UTIL_ACTIVEMQPROPERTIES_H_

#include <map>
#include <string>
#include <sstream>
#include <activemq/util/Config.h>
#include <cms/CMSProperties.h>
#include <decaf/util/Properties.h>

namespace activemq{
namespace util{

    /**
     * Implementation of the CMSProperties interface that
     * delegates to a decaf::util::Properties object.
     */
    class AMQCPP_API ActiveMQProperties : public cms::CMSProperties {
    private:

        decaf::util::Properties properties;

    public:

        virtual ~ActiveMQProperties(){}

        virtual decaf::util::Properties& getProperties() {
            return properties;
        }

        virtual const decaf::util::Properties& getProperties() const {
            return properties;
        }

        virtual void setProperties( decaf::util::Properties& props ) {
            properties.copy( &props );
        }

        /**
         * Returns true if the properties object is empty
         * @return true if empty
         */
        virtual bool isEmpty() const {
            return properties.isEmpty();
        }

        /**
         * Looks up the value for the given property.
         * @param name The name of the property to be looked up.
         * @return the value of the property with the given name, if it
         * exists.  If it does not exist, returns NULL.
         */
        virtual const char* getProperty( const std::string& name ) const{
            return properties.getProperty(name);
        }

        /**
         * Looks up the value for the given property.
         * @param name the name of the property to be looked up.
         * @param defaultValue The value to be returned if the given
         * property does not exist.
         * @return The value of the property specified by <code>name</code>, if it
         * exists, otherwise the <code>defaultValue</code>.
         */
        virtual std::string getProperty( const std::string& name,
                                         const std::string& defaultValue ) const {
            return properties.getProperty( name,defaultValue );
        }

        /**
         * Sets the value for a given property.  If the property already
         * exists, overwrites the value.
         * @param name The name of the value to be written.
         * @param value The value to be written.
         */
        virtual void setProperty( const std::string& name,
                                  const std::string& value ){
            properties.setProperty( name,value );
        }

        /**
         * Check to see if the Property exists in the set
         * @param name - property name to check for in this properties set.
         * @return true if property exists, false otherwise.
         */
        virtual bool hasProperty( const std::string& name ) const {
            return properties.hasProperty(name);
        }

        /**
         * Removes the property with the given name.
         * @param name the name of the property to remove.
         */
        virtual void remove( const std::string& name ){
            properties.remove( name );
        }

        /**
         * Method that serializes the contents of the property map to
         * an arryay.
         * @return list of pairs where the first is the name and the second
         * is the value.
         */
        virtual std::vector< std::pair< std::string, std::string > > toArray() const{
            return properties.toArray();
        }

        /**
         * Copies the contents of the given properties object to this one.
         * @param source The source properties object.
         */
        virtual void copy( const CMSProperties* source ){

            properties.clear();

            std::vector< std::pair< std::string, std::string > > vec =
                source->toArray();

            for( unsigned int ix=0; ix<vec.size(); ++ix ){
                properties.setProperty(vec[ix].first, vec[ix].second );
            }
        }

        /**
         * Clones this object.
         * @returns a replica of this object.
         */
        virtual CMSProperties* clone() const{

            ActiveMQProperties* props = new ActiveMQProperties();

            props->properties.copy(&properties);

            return props;
        }

        /**
         * Clears all properties from the map.
         */
        virtual void clear(){
            properties.clear();
        }

        /**
         * Formats the contents of the Properties Object into a string
         * that can be logged, etc.
         * @returns string value of this object.
         */
        virtual std::string toString() const {
            return properties.toString();
        }

    };

}}

#endif /*_ACTIVEMQ_UTIL_ACTIVEMQPROPERTIES_H_*/
