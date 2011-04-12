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

#ifndef _ACTIVEMQ_TRANSPORT_FAILOVER_URIPOOL_H_
#define _ACTIVEMQ_TRANSPORT_FAILOVER_URIPOOL_H_

#include <activemq/util/Config.h>

#include <decaf/net/URI.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/NoSuchElementException.h>

namespace activemq {
namespace transport {
namespace failover {

    using decaf::util::LinkedList;
    using decaf::net::URI;

    class AMQCPP_API URIPool {
    private:

        LinkedList<URI> uriPool;
        bool randomize;

    public:

        /**
         * Create an Empty URI Pool.
         */
        URIPool();

        /**
         * Creates a new URI Pool using the given list as the initial Free List.
         *
         * @param uris - List of URI to place in the Pool.
         */
        URIPool( const decaf::util::List<URI>& uris );

        ~URIPool();

        /**
         * Fetches the next available URI from the pool, if there are no more
         * URIs free when this method is called it throws a NoSuchElementException.
         * Receiving the exception is not an indication that a URI won't be available
         * in the future, the caller should react accordingly.
         *
         * @return the next free URI in the Pool.
         *
         * @throw NoSuchElementException if there are none free currently.
         */
        URI getURI();

        /**
         * Adds a URI to the free list, callers that have previously taken one using
         * the <code>getURI</code> method should always return the URI when they close
         * the resource that was connected to that URI.
         *
         * @param uri - a URI previously taken from the pool.
         */
        void addURI( const URI& uri );

        /**
         * Adds a List of URIs to this Pool, the method checks for duplicates already
         * in the pool and does not add those.
         *
         * @param uris - List of URIs to add into the Pool.
         */
        void addURIs( const LinkedList<URI>& uris );

        /**
         * Remove a given URI from the Free List.
         * @param uri - the URI to find and remove from the free list
         */
        void removeURI( const URI& uri );

        /**
         * Is the URI that is given randomly picked from the pool or is
         * each one taken in sequence.
         *
         * @return true if URI gets are random.
         */
        bool isRandomize() const {
            return this->randomize;
        }

        /**
         * Sets if the URI's that are taken from the pool are chosen Randomly or
         * are taken in the order they are in the list.
         *
         * @param value - true indicates URI gets are random.
         */
        void setRandomize( bool value ) {
            this->randomize = value;
        }

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_FAILOVER_URIPOOL_H_*/
