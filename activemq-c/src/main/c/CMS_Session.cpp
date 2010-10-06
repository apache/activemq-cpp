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

#include <CMS_Session.h>

#include <Config.h>
#include <types/CMS_Types.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createDefaultSession(CMS_Connection* connection, CMS_Session** session) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Session> wrapper( new CMS_Session );

    try{

        if (connection == NULL) {
            result = CMS_ERROR;
        } else {
            wrapper->session = connection->connection->createSession();
        }

        *session = wrapper.release();
    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createSession(CMS_Connection* connection, CMS_Session** session, ACKNOWLEDGMENT_MODE ackMode) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Session> wrapper( new CMS_Session );

    try{

        if (connection == NULL) {
            result = CMS_ERROR;
        } else {

            cms::Session::AcknowledgeMode cmsAckType = cms::Session::AUTO_ACKNOWLEDGE;

            switch(ackMode) {
                case AUTO_ACKNOWLEDGE:
                    break;
                case DUPS_OK_ACKNOWLEDGE:
                    cmsAckType = cms::Session::DUPS_OK_ACKNOWLEDGE;
                    break;
                case CLIENT_ACKNOWLEDGE:
                    cmsAckType = cms::Session::CLIENT_ACKNOWLEDGE;
                    break;
                case SESSION_TRANSACTED:
                    cmsAckType = cms::Session::SESSION_TRANSACTED;
                    break;
                case INDIVIDUAL_ACKNOWLEDGE:
                    cmsAckType = cms::Session::INDIVIDUAL_ACKNOWLEDGE;
                    break;
                default:
                    return CMS_UNKNOWN_ACKTYPE;
            }
            wrapper->session = connection->connection->createSession(cmsAckType);
        }

        *session = wrapper.release();
    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroySession(CMS_Session* session) {

    cms_status result = CMS_SUCCESS;

    if(session != NULL) {

        try{
            delete session->session;
            delete session;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status closeSession(CMS_Session* session) {

    cms_status result = CMS_SUCCESS;

    if(session != NULL) {

        try{
            session->session->close();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status commitSession(CMS_Session* session) {

    cms_status result = CMS_SUCCESS;

    if(session != NULL) {

        try{
            session->session->commit();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status rollbackSession(CMS_Session* session) {

    cms_status result = CMS_SUCCESS;

    if(session != NULL) {

        try{
            session->session->rollback();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
