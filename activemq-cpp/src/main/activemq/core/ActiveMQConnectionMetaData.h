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

#include <activemq/util/Config.h>

#include <cms/ConnectionMetaData.h>

namespace activemq {
namespace core {

    /**
     * This class houses all the various settings and information that is used
     * by an instance of an ActiveMQConnection class.
     *
     * @since 3.0
     */
    class AMQCPP_API ActiveMQConnectionMetaData : public cms::ConnectionMetaData {
    private:

        ActiveMQConnectionMetaData( const ActiveMQConnectionMetaData& );
        ActiveMQConnectionMetaData& operator= ( const ActiveMQConnectionMetaData& );

    public:

        ActiveMQConnectionMetaData();
        virtual ~ActiveMQConnectionMetaData() throw();

    public:  // cms::ConnectionMetaData

        virtual std::string getCMSVersion() const;

        virtual int getCMSMajorVersion() const;

        virtual int getCMSMinorVersion() const;

        virtual std::string getCMSProviderName() const;

        virtual std::string getProviderVersion() const;

        virtual int getProviderMajorVersion() const;

        virtual int getProviderMinorVersion() const;

        virtual std::vector<std::string> getCMSXPropertyNames() const;

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONMETADATA_H_ */
