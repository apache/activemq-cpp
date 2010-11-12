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

namespace cms{

    /**
     * A ConnectionMetaData object provides information describing the Connection object.
     *
     * @since 1.3
     */
    class CMS_API ConnectionMetaData {
    public:

        virtual ~ConnectionMetaData() throw();

        /**
         * Gets the CMS API version.
         *
         * @return the CMS API Version in String form.
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual std::string getCMSVersion() const = 0;

        /**
         * Gets the CMS major version number.
         *
         * @returns the CMS API major version number
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual int getCMSMajorVersion() const = 0;

        /**
         * Gets the CMS minor version number.
         *
         * @returns the CMS API minor version number
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual int getCMSMinorVersion() const = 0;

        /**
         * Gets the CMS provider name.
         *
         * @returns the CMS provider name
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual std::string getCMSProviderName() const = 0;

        /**
         * Gets the CMS provider version.
         *
         * @returns the CMS provider version
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual std::string getProviderVersion() const = 0;

        /**
         * Gets the CMS provider major version number.
         *
         * @returns the CMS provider major version number
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual int getProviderMajorVersion() const = 0;

        /**
         * Gets the CMS provider minor version number.
         *
         * @returns the CMS provider minor version number
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual int getProviderMinorVersion() const = 0;

        /**
         * Gets an Vector of the CMSX property names.
         *
         * @returns an Vector of CMSX property names
         *
         * @throw CMSException
         *        If the CMS Provider fails to retrieve the metadata due to some internal error.
         */
        virtual std::vector<std::string> getCMSXPropertyNames() const = 0;

    };

}

#endif /*_CMS_CONNECTIONMETADATA_H_*/
