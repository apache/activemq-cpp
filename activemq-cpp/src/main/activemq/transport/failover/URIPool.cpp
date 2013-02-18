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

#include "URIPool.h"

#include <memory>
#include <decaf/util/Random.h>
#include <decaf/lang/System.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
URIPool::URIPool() : uriPool(), priorityURI(), randomize(false) {
}

////////////////////////////////////////////////////////////////////////////////
URIPool::URIPool(const decaf::util::List<URI>& uris) : uriPool(), priorityURI(), randomize(false) {
    this->uriPool.copy(uris);

    if (!this->uriPool.isEmpty()) {
        this->priorityURI = this->uriPool.getFirst();
    }
}

////////////////////////////////////////////////////////////////////////////////
URIPool::URIPool(const URIPool& uris) : uriPool(), priorityURI(), randomize(false) {
    synchronized(&uris.uriPool) {
        this->uriPool.copy(uris.uriPool);
    }

    if (!this->uriPool.isEmpty()) {
        this->priorityURI = this->uriPool.getFirst();
    }
}

////////////////////////////////////////////////////////////////////////////////
URIPool& URIPool::operator= (const URIPool& uris) {
    synchronized(&uris.uriPool) {
        this->uriPool.copy(uris.uriPool);
    }

    if (!this->uriPool.isEmpty()) {
        this->priorityURI = this->uriPool.getFirst();
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
URIPool::~URIPool() {
}

////////////////////////////////////////////////////////////////////////////////
URI URIPool::getURI() {

    synchronized(&uriPool) {
        if (!uriPool.isEmpty()) {

            int index = 0; // Take the first one in the list unless random is on.

            if (isRandomize()) {
                Random rand;
                rand.setSeed(decaf::lang::System::currentTimeMillis());
                index = rand.nextInt((int) uriPool.size());
            }

            return uriPool.removeAt(index);
        }
    }

    throw NoSuchElementException(__FILE__, __LINE__, "URI Pool is currently empty.");
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::addURI(const URI& uri) {

    synchronized(&uriPool) {

        if (priorityURI.toString() == "" && uri.toString() != "") {
            priorityURI = uri;
        }

        if (!uriPool.contains(uri)) {
            uriPool.add(uri);
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::addURIs(const List<URI>& uris) {

    bool result = false;

    synchronized(&uriPool) {
        std::auto_ptr<Iterator<URI> > iter(uris.iterator());

        while (iter->hasNext()) {
            URI uri = iter->next();

            if (priorityURI.toString() == "" && uri.toString() != "") {
                priorityURI = uri;
            }

            if (!uriPool.contains(uri)) {
                uriPool.add(uri);
                result = true;
            }
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::removeURI(const URI& uri) {

    synchronized(&uriPool) {
        if (uriPool.contains(uri)) {
            uriPool.remove(uri);
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::contains(const decaf::net::URI& uri) const {
    bool result = false;
    synchronized(&uriPool) {
        result = uriPool.contains(uri);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::isPriority(const decaf::net::URI& uri) const {
    synchronized(&uriPool) {
        if (!uriPool.isEmpty()) {
            return uriPool.getFirst().equals(uri);
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void URIPool::clear() {
    synchronized(&uriPool) {
        this->uriPool.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::isEmpty() const {
    bool result = true;
    synchronized(&uriPool) {
        result = this->uriPool.isEmpty();
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool URIPool::equals(const URIPool& other) const {

    LinkedList<URI> mine;
    LinkedList<URI> others;

    synchronized(&other.uriPool) {
        others.copy(other.uriPool);
    }

    synchronized(&this->uriPool) {
        mine.copy(this->uriPool);
    }

    return mine.equals(others);
}
