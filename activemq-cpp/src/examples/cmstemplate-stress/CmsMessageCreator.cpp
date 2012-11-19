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

#include "CmsMessageCreator.h"

using namespace std;
using namespace cms;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace activemq::core;
using namespace cmstemplate;

////////////////////////////////////////////////////////////////////////////////
CmsMessageCreator::CmsMessageCreator(const std::string& txt) {
    m_txt = txt;
}

////////////////////////////////////////////////////////////////////////////////
CmsMessageCreator::~CmsMessageCreator() {
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsMessageCreator::createMessage(cms::Session* session) {
    cms::Message* message = NULL;
    if (session) {
        message = session->createTextMessage(m_txt);
    }
    return message;
}
