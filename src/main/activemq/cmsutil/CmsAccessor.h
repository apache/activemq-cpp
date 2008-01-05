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
    class CmsAccessor {
    
    private:
    
        cms::ConnectionFactory* connectionFactory;
    
        bool sessionTransacted;
    
        int sessionAcknowledgeMode;
    
    public:
        
        CmsAccessor() {
            sessionTransacted = false;
            sessionAcknowledgeMode = cms::Session::AUTO_ACKNOWLEDGE;
        }
        
        virtual ~CmsAccessor() {            
        }
    
        /**
         * Set the ConnectionFactory to use for obtaining CMS Connections.
         */
        virtual void setConnectionFactory(ConnectionFactory* connectionFactory) {
            this->connectionFactory = connectionFactory;
        }
    
        /**
         * Return the ConnectionFactory that this accessor uses for
         * obtaining CMS Connections.
         */
        virtual const ConnectionFactory* getConnectionFactory() const {
            return this->connectionFactory;
        }
    
        /**
         * Return the ConnectionFactory that this accessor uses for
         * obtaining CMS Connections.
         */
        virtual ConnectionFactory* getConnectionFactory() {
            return this->connectionFactory;
        }
    
        /**
         * Set the transaction mode that is used when creating a CMS Session.
         * Default is "false".
         * 
         * @param sessionTransacted the transaction mode
         */
        virtual void setSessionTransacted(bool sessionTransacted) {
            this->sessionTransacted = sessionTransacted;
        }
    
        /**
         * Return whether the CMS sessions used by this
         * accessor are supposed to be transacted.
         * @return <code>true</code> if the CMS Sessions used are transacted
         * @see #setSessionTransacted(bool)
         */
        virtual bool isSessionTransacted() const {
            return this->sessionTransacted;
        }
    
        /**
         * Set the CMS acknowledgement mode that is used when creating a CMS
         * Session to send a message.
         * <p>Default is <code>AUTO_ACKNOWLEDGE</code>.
         * @param sessionAcknowledgeMode the acknowledgement mode
         */
        virtual void setSessionAcknowledgeMode(int sessionAcknowledgeMode) {
            this->sessionAcknowledgeMode = sessionAcknowledgeMode;
        }
    
        /**
         * Return the acknowledgement mode for CMS sessions.
         * @return the acknowledgement mode applied by this accessor
         */
        virtual int getSessionAcknowledgeMode() const {
            return this->sessionAcknowledgeMode;
        }
    
    protected:
    
        /**
         * Create a CMS Connection via this template's ConnectionFactory.
         * @return the new CMS Connection
         * @throws cms::CMSException if thrown by CMS API methods
         */
        virtual cms::Connection* createConnection() throw (cms::CMSException) {
            return getConnectionFactory()->createConnection();
        }
    
        /**
         * Create a CMS Session for the given Connection.
         * @param con the CMS Connection to create a Session for
         * @return the new CMS Session
         * @throws cms::CMSException if thrown by CMS API methods
         */
        virtual cms::Session* createSession(cms::Connection* con) throw (cms::CMSException) {
            return con->createSession(isSessionTransacted(), getSessionAcknowledgeMode());
        }
    
        /**
         * Determine whether the given Session is in client acknowledge mode.
         * @param session the CMS Session to check
         * @return whether the given Session is in client acknowledge mode
         * @throws cms::CMSException if thrown by CMS API methods
         */
        virtual bool isClientAcknowledge(cms::Session* session) throws JMSException {
            return (session.getAcknowledgeMode() == Session.CLIENT_ACKNOWLEDGE);
        }
    
    };

}}

#endif /* _ACTIVEMQ_CMSUTIL_CMSACCESSOR_H_ */
