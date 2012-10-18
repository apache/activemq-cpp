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

#include "ActiveMQProperties.h"

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
ActiveMQProperties::ActiveMQProperties() : properties() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQProperties::~ActiveMQProperties() {
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProperties::copy(const CMSProperties* source) {

    if (source == NULL) {
        return;
    }

    properties.clear();

    std::vector<std::pair<std::string, std::string> > vec = source->toArray();

    for (unsigned int ix = 0; ix < vec.size(); ++ix) {
        properties.setProperty(vec[ix].first, vec[ix].second);
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::CMSProperties* ActiveMQProperties::clone() const {
    ActiveMQProperties* props = new ActiveMQProperties();
    props->copy(this);
    return props;
}
