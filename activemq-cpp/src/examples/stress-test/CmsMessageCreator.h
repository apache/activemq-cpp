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

#ifndef _CMS_STRESS_CMSMESSAGECREATOR_H_
#define _CMS_STRESS_CMSMESSAGECREATOR_H_

#include <decaf/util/Config.h>

#include <cms/Session.h>

#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>

namespace cms {
namespace stress {

    class CmsMessageCreator: public activemq::cmsutil::MessageCreator {
    private:

        std::string text;
        std::string headerName;
        std::string headerValue;

    public:

        CmsMessageCreator(const std::string& txt,
                          const std::string& name = "",
                          const std::string& value = "");

        virtual ~CmsMessageCreator();

        virtual cms::Message* createMessage(cms::Session* session);
    };

}}

#endif /** _CMS_STRESS_CMSMESSAGECREATOR_H_ */
