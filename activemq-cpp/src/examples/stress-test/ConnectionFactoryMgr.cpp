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

#include "ConnectionFactoryMgr.h"
#include <activemq/core/ActiveMQConnectionFactory.h>

using namespace decaf::lang::exceptions;
using namespace activemq::core;
using namespace decaf::lang;
using namespace cms;
using namespace cms::stress;

StlMap<std::string, ConnectionFactory*> * ConnectionFactoryMgr::connectionFactories;

////////////////////////////////////////////////////////////////////////////////
ConnectionFactoryMgr::ConnectionFactoryMgr() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionFactoryMgr::~ConnectionFactoryMgr() {
    unInitialize();
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionFactoryMgr::initialize() {
    connectionFactories = new StlMap<std::string, ConnectionFactory*>();
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionFactoryMgr::unInitialize() {
    connectionFactories->lock();

    Pointer<Iterator<ConnectionFactory*> > iter(connectionFactories->values().iterator());
    while (iter->hasNext()) {
        ConnectionFactory* connectionFactory = iter->next();
        if (connectionFactory != NULL) {
            delete connectionFactory;
            connectionFactory = NULL;
        }
    }

    connectionFactories->clear();
    connectionFactories->unlock();

    delete connectionFactories;
    connectionFactories = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ConnectionFactory* ConnectionFactoryMgr::getConnectionFactory(const std::string& url) {

    ConnectionFactory* connectionFactory = NULL;

    connectionFactories->lock();
    try {
        if (connectionFactories->containsKey(url)) {
            connectionFactory = connectionFactories->get(url);
        }
    } catch (NoSuchElementException& ex) {
    }

    if (!connectionFactory) {
        connectionFactory = new ActiveMQConnectionFactory(url);
        connectionFactories->put(url, connectionFactory);
    }
    connectionFactories->unlock();

    return connectionFactory;
}
