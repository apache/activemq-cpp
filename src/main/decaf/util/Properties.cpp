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

#include "Properties.h"

#include <decaf/util/Date.h>

using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
Properties::Properties() {
}

////////////////////////////////////////////////////////////////////////////////
Properties::~Properties() {
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::isEmpty() const {
    return properties.empty();
}

////////////////////////////////////////////////////////////////////////////////
const char* Properties::getProperty( const std::string& name ) const{

    std::map< std::string, std::string >::const_iterator iter =
    properties.find( name );
    if( iter == properties.end() ){
        return NULL;
    }

    return iter->second.c_str();
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::getProperty( const std::string& name,
                                     const std::string& defaultValue ) const {

    std::map< std::string, std::string >::const_iterator iter =
    properties.find( name );
    if( iter == properties.end() ){
        return defaultValue;
    }

    return iter->second;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::setProperty( const std::string& name,
                              const std::string& value ){
    properties[name] = value;
}

////////////////////////////////////////////////////////////////////////////////
bool Properties::hasProperty( const std::string& name ) const {

    if( properties.find(name) != properties.end() ) {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::remove( const std::string& name ){
    properties.erase( name );
}

////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, std::string > > Properties::toArray() const{

    // Create a vector big enough to hold all the elements in the map.
    std::vector< std::pair<std::string, std::string> > vec(
            properties.begin(), properties.end() );

    return vec;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::copy( const Properties* source ){

    clear();
    this->properties = source->properties;
}

////////////////////////////////////////////////////////////////////////////////
Properties* Properties::clone() const{

    Properties* props = new Properties();

    props->properties = properties;

    return props;
}

////////////////////////////////////////////////////////////////////////////////
void Properties::clear(){
    properties.clear();
}

////////////////////////////////////////////////////////////////////////////////
std::string Properties::toString() const {

    std::ostringstream stream;
    std::map< std::string, std::string >::const_iterator iter;

    stream << "Begin Class decaf::util::Properties:" << std::endl;

    for( iter = properties.begin(); iter != properties.end(); ++iter ){
        stream << " properties[" << iter->first << "] = "
               << iter->second << std::endl;
    }

    stream << "End Class decaf::util::Properties:" << std::endl;

    return stream.str();
}
