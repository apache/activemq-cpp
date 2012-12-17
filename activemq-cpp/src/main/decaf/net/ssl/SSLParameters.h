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

#ifndef _DECAF_NET_SSL_SSLPARAMETERS_H_
#define _DECAF_NET_SSL_SSLPARAMETERS_H_

#include <decaf/util/Config.h>

#include <string>
#include <vector>

namespace decaf {
namespace net {
namespace ssl {

    class DECAF_API SSLParameters {
    private:

        std::vector<std::string> cipherSuites;
        std::vector<std::string> protocols;
        bool needClientAuth;
        bool wantClientAuth;

    public:

        /**
         * Creates a new SSLParameters instance with empty vectors for the protocols and the
         * cipherSuites, the wantClientAuth and needClientAuth flags are set to false.
         */
        SSLParameters();

        /**
         * Creates a new SSLParameters instance with the given cipherSuites value, the protocols
         * vector is empty and the wantClientAuth and needClientAuth flags are set to false.
         *
         * @param cipherSuites
         *      The vector of cipherSuites for this SSLParameters instance (can be empty).
         */
        SSLParameters( const std::vector<std::string>& cipherSuites );

        /**
         * Creates a new SSLParameters instance with the given cipherSuites value and protocols
         * value, the wantClientAuth and needClientAuth flags are set to false.
         *
         * @param cipherSuites
         *      The vector of cipherSuites for this SSLParameters instance (can be empty).
         * @param protocols
         *      The vector of protocols for this SSLParameters instance (can be empty).
         */
        SSLParameters( const std::vector<std::string>& cipherSuites,
                       const std::vector<std::string>& protocols );

        virtual ~SSLParameters();

        /**
         * @returns a copy of the vector of ciphersuites or an empty vector if none have been set.
         */
        std::vector<std::string> getCipherSuites() const {
            return this->cipherSuites;
        }

        /**
         * Sets the vector of ciphersuites.
         *
         * @param cipherSuites
         *      The vector of cipherSuites (can be an empty vector).
         */
        void setCipherSuites( const std::vector<std::string>& cipherSuites ) {
            this->cipherSuites = cipherSuites;
        }

        /**
         * @returns a copy of the vector of protocols or an empty vector if none have been set.
         */
        std::vector<std::string> getProtocols() const {
            return this->protocols;
        }

        /**
         * Sets the vector of protocols.
         *
         * @param protocols
         *      the vector of protocols (or an empty vector)
         */
        void setProtocols( const std::vector<std::string>& protocols ) {
            this->protocols = protocols;
        }

        /**
         * @returns whether client authentication should be requested.
         */
        bool getWantClientAuth() const {
            return this->wantClientAuth;
        }

        /**
         * Sets whether client authentication should be requested. Calling this method clears the
         * needClientAuth flag.
         *
         * @param whether client authentication should be requested.
         */
        void setWantClientAuth( bool wantClientAuth ) {
            this->wantClientAuth = wantClientAuth;
            this->needClientAuth = false;
        }

        /**
         * @returns whether client authentication should be required.
         */
        bool getNeedClientAuth() const {
            return this->needClientAuth;
        }

        /**
         * Sets whether client authentication should be required. Calling this method clears the
         * wantClientAuth flag.
         *
         * @param needClientAuth
         *      whether client authentication should be required.
         */
        void setNeedClientAuth( bool needClientAuth ) {
            this->needClientAuth = needClientAuth;
            this->wantClientAuth = false;
        }

    };

}}}

#endif /* _DECAF_NET_SSL_SSLPARAMETERS_H_ */
