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
     *
     * @since 2.0
     */
    class AMQCPP_API ActiveMQProperties : public cms::CMSProperties {
    private:

        decaf::util::Properties properties;

    public:

        ActiveMQProperties();

        virtual ~ActiveMQProperties() throw();

        virtual decaf::util::Properties& getProperties() {
            return this->properties;
        }

        virtual const decaf::util::Properties& getProperties() const {
            return this->properties;
        }

        virtual void setProperties( decaf::util::Properties& props ) {
            this->properties = props;
        }

        virtual int size() const {
            return properties.size();
        }

        virtual bool isEmpty() const {
            return properties.isEmpty();
        }

        virtual const char* getProperty( const std::string& name ) const{
            return properties.getProperty(name);
        }

        virtual std::string getProperty( const std::string& name,
                                         const std::string& defaultValue ) const {
            return properties.getProperty( name,defaultValue );
        }

        virtual void setProperty( const std::string& name,
                                  const std::string& value ){
            properties.setProperty( name,value );
        }

        virtual bool hasProperty( const std::string& name ) const {
            return properties.hasProperty(name);
        }

        virtual std::string remove( const std::string& name ){
            return properties.remove( name );
        }

        virtual std::vector<std::string> propertyNames() const {
            return properties.propertyNames();
        }

        virtual std::vector< std::pair< std::string, std::string > > toArray() const{
            return properties.toArray();
        }

        virtual void copy( const CMSProperties* source );

        virtual CMSProperties* clone() const;

        virtual void clear(){
            properties.clear();
        }

        virtual std::string toString() const {
            return properties.toString();
        }

    };

}}

#endif /*_ACTIVEMQ_UTIL_ACTIVEMQPROPERTIES_H_*/
