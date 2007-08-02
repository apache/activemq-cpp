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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONMETADATA_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONMETADATA_H_

#include <cms/ConnectionMetaData.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace core{

    class ActiveMQConnectionMetaData : public cms::ConnectionMetaData {
    public:

        ActiveMQConnectionMetaData() {}

        virtual ~ActiveMQConnectionMetaData() {}

        /**
         * @returns the Major version number of CMS Supported
         */
        virtual int getCMSMajorVersion() const {
            return 1;
        }

        /**
         * @returns the Minor version number of CMS Supported
         */
        virtual int getCMSMinorVersion() const {
            return 2;
        }

        /**
         * @returns the CMS version supported as a string
         */
        virtual std::string getCMSVersion() const {
            return "1.2";
        }

        /**
         * @returns the name of the CMS provider Library
         */
        virtual std::string getCMSProviderName() const {
            return "ActiveMQ-CPP";
        }

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONMETADATA_H_*/
