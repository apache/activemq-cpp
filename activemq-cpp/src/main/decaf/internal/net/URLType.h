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

#ifndef _DECAF_INTERNAL_NET_URLTYPE_H_
#define _DECAF_INTERNAL_NET_URLTYPE_H_

#include <decaf/util/Config.h>
#include <decaf/lang/String.h>

namespace decaf {
namespace internal {
namespace net {

    /**
     * Basic type object that holds data that composes a given URL
     */
    class DECAF_API URLType {
    private:

        decaf::lang::String file;
        decaf::lang::String protocol;
        decaf::lang::String host;
        int port;
        decaf::lang::String authority;
        decaf::lang::String userInfo;
        decaf::lang::String path;
        decaf::lang::String query;
        decaf::lang::String ref;
        int hashCode;

    public:

        URLType();

        virtual ~URLType();

        /**
         * Gets the File of the URL.
         * @return File part string.
         */
        decaf::lang::String getFile() const {
            return file;
        }

        /**
         * Sets the File of the URL.
         * @param File Authority part string.
         */
        void setFile(const decaf::lang::String& file) {
            this->file = file;
        }

        /**
         * Gets the protocol of the URL, e.g. protocol ("http"/"ftp"/...).
         * @return protocol part string.
         */
        decaf::lang::String getProtocol() const {
            return protocol;
        }

        /**
         * Sets the protocol of the URL, e.g. protocol ("http"/"ftp"/...).
         * @param protocol - protocol part string.
         */
        void setProtocol(const decaf::lang::String& protocol) {
            this->protocol = protocol;
        }

        /**
         * Gets the Authority of the URL.
         * @return Authority part string.
         */
        decaf::lang::String getAuthority() const {
            return authority;
        }

        /**
         * Sets the Authority of the URL.
         * @param authority Authority part string.
         */
        void setAuthority(const decaf::lang::String& authority) {
            this->authority = authority;
        }

        /**
         * Gets the user info part of the URL, e.g. user name, as in
         * http://user:passwd@host:port/
         * @return user info part string.
         */
        decaf::lang::String getUserInfo() const {
            return userInfo;
        }

        /**
         * Sets the user info part of the URL, e.g. user name, as in
         * http://user:passwd@host:port/
         *
         * @param userInfo - user info part string.
         */
        void setUserInfo(const decaf::lang::String& userInfo) {
            this->userInfo = userInfo;
        }

        /**
         * Gets the Host name part of the URL.
         * @return Host name part string.
         */
        decaf::lang::String getHost() const {
            return host;
        }

        /**
         * Sets the Host name part of the URL.
         * @param host - Host name part string.
         */
        void setHost(const decaf::lang::String& host) {
            this->host = host;
        }

        /**
         * Gets the port part of the URL.
         * @return port part string, -1 if not set.
         */
        int getPort() const {
            return port;
        }

        /**
         * Sets the port part of the URL.
         * @param port - port part string, -1 if not set.
         */
        void setPort(int port) {
            this->port = port;
        }

        /**
         * Gets the Path part of the URL.
         * @return Path part string.
         */
        decaf::lang::String getPath() const {
            return path;
        }

        /**
         * Sets the Path part of the URL.
         * @param path - Path part string.
         */
        void setPath(const decaf::lang::String& path) {
            this->path = path;
        }

        /**
         * Gets the Query part of the URL.
         * @return Query part string.
         */
        decaf::lang::String getQuery() const {
            return query;
        }

        /**
         * Sets the Query part of the URL.
         * @param query - Query part string.
         */
        void setQuery(const decaf::lang::String& query) {
            this->query = query;
        }

        /**
         * Gets the Ref part of the URL.
         * @return Ref part string.
         */
        decaf::lang::String getRef() const {
            return ref;
        }

        /**
         * Sets the Ref part of the URL.
         * @param ref - Ref part string.
         */
        void setRef(const decaf::lang::String& ref) {
            this->ref = ref;
        }

        /**
         * Gets the computed hashCode for this URLType or return -1 if non is set
         *
         * @return the hash code for this URLType instance or -1 if not set.
         */
        int getHashCode() const {
            return this->hashCode;
        }

        /**
         * Sets the hash code for this URLType instance.
         *
         * @param hashCode
         *      The new hash code that's been computed for this URLType instance.
         */
        void setHashCode(int hashCode) {
            this->hashCode = hashCode;
        }
    };

}}}

#endif /* _DECAF_INTERNAL_NET_URLTYPE_H_ */
