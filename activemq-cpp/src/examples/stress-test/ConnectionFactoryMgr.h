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

#ifndef _CMS_STRESS_CONNECTIONFACTORYMGR_H_
#define _CMS_STRESS_CONNECTIONFACTORYMGR_H_

#include <decaf/util/Config.h>
#include <cms/ConnectionFactory.h>

#include <decaf/util/StlMap.h>

using namespace cms;
using namespace decaf::util;
using namespace std;

namespace cms {
namespace stress {

    class ConnectionFactoryMgr {
    private:

        static decaf::util::StlMap<std::string, cms::ConnectionFactory*>* connectionFactories;

        ConnectionFactoryMgr();
        virtual ~ConnectionFactoryMgr();

    public:

        static void initialize();
        static void unInitialize();

        static cms::ConnectionFactory* getConnectionFactory(const std::string& url);

    };

}}

#endif /** _CMS_STRESS_CONNECTIONFACTORYMGR_H_ */
