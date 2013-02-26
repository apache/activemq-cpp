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

#include "ResourceAllocationException.h"

using namespace cms;

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException::ResourceAllocationException() : CMSException() {
}

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException::ResourceAllocationException(const ResourceAllocationException& ex) :
    CMSException(ex) {
}

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException::ResourceAllocationException(const std::string& message) :
    CMSException(message, NULL) {
}

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException::ResourceAllocationException(const std::string& message, const std::exception* cause) :
    CMSException(message, cause) {
}

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException::ResourceAllocationException(const std::string& message, const std::exception* cause,
                                                         const std::vector<std::pair<std::string, int> >& stackTrace) :
    CMSException(message, cause, stackTrace) {
}

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException::~ResourceAllocationException() throw() {
}

////////////////////////////////////////////////////////////////////////////////
ResourceAllocationException* ResourceAllocationException::clone() {
    return new ResourceAllocationException(*this);
}
