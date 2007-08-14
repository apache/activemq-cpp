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

#include "URI.h"

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& uri ) throw ( URISyntaxException) {
}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme,
          const std::string& ssp,
          const std::string& fragment ) throw ( URISyntaxException ) {

}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme, const std::string& userInfo,
          const std::string& host, int port,
          const std::string& path, const std::string& query,
          const std::string& fragment ) throw ( URISyntaxException ) {

}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme, const std::string& host,
          const std::string& path, const std::string& fragment )
            throw ( URISyntaxException ) {

}

////////////////////////////////////////////////////////////////////////////////
URI::URI( const std::string& scheme, const std::string& authority,
          const std::string& path, const std::string& query,
          const std::string& fragment ) throw ( URISyntaxException ) {

}

////////////////////////////////////////////////////////////////////////////////
int URI::compareTo( const URI& value ) const {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool URI::equals( const URI& value ) const {
    return compareTo( value ) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
bool URI::operator==( const URI& value ) const {
    return compareTo( value ) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
bool URI::operator<( const URI& value ) const {
    return compareTo( value ) == -1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
URI URI::create( const std::string uri )
    throw ( lang::exceptions::IllegalArgumentException ) {

    try {
        return URI( uri );
    } catch( URISyntaxException& e ) {
        throw IllegalArgumentException( e );
    }
}
