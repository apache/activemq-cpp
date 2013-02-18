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

    class AMQCPP_API URIPool {
    private:

        mutable decaf::util::LinkedList<decaf::net::URI> uriPool;
        decaf::net::URI priorityURI;
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
        URIPool(const decaf::util::List<decaf::net::URI>& uris);

        /**
         * Creates a new URI Pool which will be a copy of the given URI Pool
         *
         * @param uris
         *      The URIPool instance to copy.
         */
        URIPool(const URIPool& uris);

        /**
         * Assignment operator, copies the contents of the given URIPool into this one.
         *
         * @param uris
         *      The URIPool whose contents are to be copied.
         */
        URIPool& operator= (const URIPool& uris);

        ~URIPool();

        /**
         * Gets a static view of the URI List contained in this URI Pool
         *
         * @returns a static reference to this Pools list of URIs.
         */
        const decaf::util::List<decaf::net::URI>& getURIList() const {
            return this->uriPool;
        }

        /**
         * @returns true if this URI Pool is empty.
         */
        bool isEmpty() const;

        /**
         * Returns the URI that is considered to be this Pools Priority URI, this is
         * always the first URI in the list of URIs that this pool was created with.
         */
        const decaf::net::URI& getPriorityURI() const {
            return this->priorityURI;
        }

        /**
         * Sets the URI that is considered this Pool's priority URI.
         *
         * @param uri
         *      The configured priority URI for this pool.
         */
        void setPriorityURI(const decaf::net::URI& uri) {
            this->priorityURI = uri;
        }

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
        decaf::net::URI getURI();

        /**
         * Adds a URI to the free list, callers that have previously taken one using
         * the <code>getURI</code> method should always return the URI when they close
         * the resource that was connected to that URI.
         *
         * @param uri - a URI previously taken from the pool.
         *
         * @returns true if the URI was added or false if its already in the list.
         */
        bool addURI(const decaf::net::URI& uri);

        /**
         * Adds a List of URIs to this Pool, the method checks for duplicates already
         * in the pool and does not add those.
         *
         * @param uris - List of URIs to add into the Pool.
         *
         * @returns true if any URI was added or false if they were already in the list.
         */
        bool addURIs(const decaf::util::List<decaf::net::URI>& uris);

        /**
         * Remove a given URI from the Free List.
         *
         * @param uri
         *      The URI to find and remove from the free list
         *
         * @returns true if the URI was removed or false if no change was made.
         */
        bool removeURI(const decaf::net::URI& uri);

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
        void setRandomize(bool value) {
            this->randomize = value;
        }

        /**
         * Returns true if the given URI is contained in this set of URIs.
         *
         * @returns true if the URI is in the list.
         */
        bool contains(const decaf::net::URI& uri) const;

        /**
         * Returns true if the URI given is the first in the list of
         * URIs contained in this pool.
         *
         * @returns true if the URI is index 0 in the URI list.
         */
        bool isPriority(const decaf::net::URI& uri) const;

        /**
         * Remove all URIs from the pool.
         */
        void clear();

        /**
         * Compares the URIs in this set to that of another URIPool
         *
         * @returns true if the URIPool instance contains the same values.
         */
        bool equals(const URIPool& other) const;

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_FAILOVER_URIPOOL_H_*/
