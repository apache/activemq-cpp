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

#include "ResourceLifecycleManager.h"

using namespace activemq::cmsutil;

////////////////////////////////////////////////////////////////////////////////
ResourceLifecycleManager::ResourceLifecycleManager() {
}

////////////////////////////////////////////////////////////////////////////////
ResourceLifecycleManager::~ResourceLifecycleManager() {

    try {

        // Destroy all the resources
        destroy();

    } catch( cms::CMSException& e ) { /* Absorb*/ }
}

////////////////////////////////////////////////////////////////////////////////
void ResourceLifecycleManager::releaseAll() {

    connections.clear();
    sessions.clear();
    destinations.clear();
    producers.clear();
    consumers.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ResourceLifecycleManager::destroy() throw ( cms::CMSException ) {

    // Close all the connections.
    for (std::size_t ix=0; ix<connections.size(); ++ix) {
        try {
            connections[ix]->close();
        } catch(...){}
    }

    // Destroy the producers.
    for (std::size_t ix=0; ix<producers.size(); ++ix) {
        try {
            delete producers[ix];
        } catch( ... ) {}
    }

    // Destroy the consumers.
    for (std::size_t ix=0; ix<consumers.size(); ++ix) {
        try {
            delete consumers[ix];
        } catch( ... ) {}
    }

    // Destroy the destinations.
    for (std::size_t ix=0; ix<destinations.size(); ++ix) {
        try {
            delete destinations[ix];
        } catch( ... ) {}
    }

    // Destroy the sessions.
    for (std::size_t ix=0; ix<sessions.size(); ++ix) {
        try {
            delete sessions[ix];
        } catch( ... ) {}
    }

    // Destroy the connections,
    for (std::size_t ix=0; ix<connections.size(); ++ix) {
        try {
            delete connections[ix];
        } catch( ... ) {}
    }

    // Empty all the vectors.
    releaseAll();
}
