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

#ifndef _CMS_CONNECTIONMETADATA_H_
#define _CMS_CONNECTIONMETADATA_H_

#include <cms/Config.h>
#include <cms/CMSException.h>
#include <string>

namespace cms{

    /**
     * A ConnectionMetaData object provides information describing the
     * Connection object.
     * @since 1.2
     */
    class CMS_API ConnectionMetaData {
    public:

        virtual ~ConnectionMetaData() {}

        /**
         * @returns the Major version number of CMS Supported
         */
        virtual int getCMSMajorVersion() const = 0;

        /**
         * @returns the Minor version number of CMS Supported
         */
        virtual int getCMSMinorVersion() const = 0;

        /**
         * @returns the CMS version supported as a string
         */
        virtual std::string getCMSVersion() const = 0;

        /**
         * @returns the name of the CMS provider Library
         */
        virtual std::string getCMSProviderName() const = 0;

    };

}

#endif /*_CMS_CONNECTIONMETADATA_H_*/
