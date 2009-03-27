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
#ifndef _ACTIVEMQ_CMSUTIL_CMSACCESSOR_H_
#define _ACTIVEMQ_CMSUTIL_CMSACCESSOR_H_

#include <cms/ConnectionFactory.h>
#include <activemq/cmsutil/ResourceLifecycleManager.h>
#include <activemq/util/Config.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

namespace activemq {
namespace cmsutil {

    /**
     * Base class for {@link activemq.cms.CmsTemplate} and other
     * CMS-accessing gateway helpers, defining common properties such as the
     * CMS {@link ConnectionFactory} to operate on. The subclass
     * {@link activemq.cms.CmsDestinationAccessor}
     * adds further, destination-related properties.
     *
     * <p>Not intended to be used directly.
     *
     * @see activemq.cms.CmsDestinationAccessor
     * @see activemq.cms.CmsTemplate
     */
    class AMQCPP_API CmsAccessor {
    private:

        ResourceLifecycleManager resourceLifecycleManager;

        cms::ConnectionFactory* connectionFactory;

        cms::Session::AcknowledgeMode sessionAcknowledgeMode;

    public:

        CmsAccessor();

        virtual ~CmsAccessor();

        virtual ResourceLifecycleManager* getResourceLifecycleManager() {
            return &resourceLifecycleManager;
        }

        virtual const ResourceLifecycleManager* getResourceLifecycleManager() const {
            return &resourceLifecycleManager;
        }

        /**
         * Set the ConnectionFactory to use for obtaining CMS Connections.
         */
        virtual void setConnectionFactory( cms::ConnectionFactory* connectionFactory ) {
            this->connectionFactory = connectionFactory;
        }

        /**
         * Return the ConnectionFactory that this accessor uses for
         * obtaining CMS Connections.
         */
        virtual const cms::ConnectionFactory* getConnectionFactory() const {
            return this->connectionFactory;
        }

        /**
         * Return the ConnectionFactory that this accessor uses for
         * obtaining CMS Connections.
         */
        virtual cms::ConnectionFactory* getConnectionFactory() {
            return this->connectionFactory;
        }

        /**
         * Set the CMS acknowledgment mode that is used when creating a CMS
         * Session to send a message.
         * <p>Default is <code>AUTO_ACKNOWLEDGE</code>.
         * @param sessionAcknowledgeMode the acknowledgment mode
         */
        virtual void setSessionAcknowledgeMode(
                cms::Session::AcknowledgeMode sessionAcknowledgeMode ) {
            this->sessionAcknowledgeMode = sessionAcknowledgeMode;
        }

        /**
         * Return the acknowledgment mode for CMS sessions.
         * @return the acknowledgment mode applied by this accessor
         */
        virtual cms::Session::AcknowledgeMode getSessionAcknowledgeMode() const {
            return this->sessionAcknowledgeMode;
        }

    protected:

        /**
         * Initializes this object and prepares it for use.  This should be called
         * before any other methods are called.  This version does nothing.
         */
        virtual void init()
            throw ( cms::CMSException, decaf::lang::exceptions::IllegalStateException ) {}

        /**
         * Shuts down this object and destroys any allocated resources.
         */
        virtual void destroy()
            throw ( cms::CMSException, decaf::lang::exceptions::IllegalStateException ) {
            resourceLifecycleManager.destroy();
        }

        /**
         * Create a CMS Connection via this template's ConnectionFactory.
         * @return the new CMS Connection
         * @throws cms::CMSException if thrown by CMS API methods
         */
        virtual cms::Connection* createConnection()
            throw ( cms::CMSException, decaf::lang::exceptions::IllegalStateException );

        /**
         * Create a CMS Session for the given Connection.
         * @param con the CMS Connection to create a Session for
         * @return the new CMS Session
         * @throws cms::CMSException if thrown by CMS API methods
         */
        virtual cms::Session* createSession( cms::Connection* con )
            throw ( cms::CMSException, decaf::lang::exceptions::IllegalStateException );

        /**
         * Verifies that the connection factory is valid.
         */
        virtual void checkConnectionFactory()
            throw ( decaf::lang::exceptions::IllegalStateException );

    };

}}

#endif /* _ACTIVEMQ_CMSUTIL_CMSACCESSOR_H_ */
