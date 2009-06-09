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

#include "CmsDestinationAccessor.h"

using namespace activemq::cmsutil;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CmsDestinationAccessor::CmsDestinationAccessor() {

    // Default to using queues.
    pubSubDomain = false;

    // Start with the default destinationResolver.
    destinationResolver = &defaultDestinationResolver;
}

////////////////////////////////////////////////////////////////////////////////
CmsDestinationAccessor::~CmsDestinationAccessor() {
}

////////////////////////////////////////////////////////////////////////////////
void CmsDestinationAccessor::init()
    throw ( cms::CMSException, IllegalStateException ) {

    CmsAccessor::init();

    // Make sure we have a destination resolver.
    checkDestinationResolver();

    // Give the resolver our lifecycle manager.
    destinationResolver->init( getResourceLifecycleManager() );
}

////////////////////////////////////////////////////////////////////////////////
void CmsDestinationAccessor::destroy()
    throw ( cms::CMSException, IllegalStateException ) {

    if( destinationResolver != NULL ) {
        destinationResolver->destroy();
    }

    CmsAccessor::destroy();
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsDestinationAccessor::resolveDestinationName(
    cms::Session* session,
    const std::string& destName )
        throw ( cms::CMSException, IllegalStateException ) {

    checkDestinationResolver();

    return getDestinationResolver()->resolveDestinationName(
        session, destName, isPubSubDomain() );
}

////////////////////////////////////////////////////////////////////////////////
void CmsDestinationAccessor::checkDestinationResolver()
    throw ( IllegalStateException ) {

    if( getDestinationResolver() == NULL ) {
        throw IllegalStateException(
                __FILE__, __LINE__,
                "Property 'destinationResolver' is required" );
    }
}
