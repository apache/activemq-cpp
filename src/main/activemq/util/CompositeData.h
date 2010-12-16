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

#ifndef _ACTIVEMQ_UTIL_COMPOSITEDATA_H_
#define _ACTIVEMQ_UTIL_COMPOSITEDATA_H_

#include <activemq/util/Config.h>
#include <decaf/util/Properties.h>
#include <decaf/util/LinkedList.h>
#include <decaf/net/URI.h>
#include <decaf/net/URISyntaxException.h>

namespace activemq {
namespace util {

    using decaf::util::LinkedList;
    using decaf::net::URI;
    using decaf::util::Properties;

    /**
     * Represents a Composite URI
     *
     * @since 3.0
     */
    class AMQCPP_API CompositeData {
    private:

        std::string host;
        std::string scheme;
        std::string path;
        LinkedList<URI> components;
        Properties parameters;
        std::string fragment;

    public:

        CompositeData();
        virtual ~CompositeData();

        LinkedList<URI>& getComponents() {
            return components;
        }
        const LinkedList<URI>& getComponents() const {
            return components;
        }

        void setComponents( const LinkedList<URI>& components ) {
            this->components = components;
        }

        std::string getFragment() const {
            return fragment;
        }

        void setFragment( const std::string& fragment ) {
            this->fragment = fragment;
        }

        const Properties& getParameters() const {
            return parameters;
        }

        void setParameters( const Properties& parameters ) {
            this->parameters = parameters;
        }

        std::string getScheme() const {
            return scheme;
        }

        void setScheme( const std::string& scheme ) {
            this->scheme = scheme;
        }

        std::string getPath() const {
            return path;
        }

        void setPath( const std::string& path ) {
            this->path = path;
        }

        std::string getHost() const {
            return host;
        }

        void setHost( const std::string& host ) {
            this->host = host;
        }

        /**
         * @throws decaf::net::URISyntaxException
         */
        URI toURI() const;

    };

}}

#endif /* _ACTIVEMQ_UTIL_COMPOSITEDATA_H_ */
