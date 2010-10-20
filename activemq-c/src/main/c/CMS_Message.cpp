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

#include <CMS_Message.h>

#include <Config.h>
#include <types/CMS_Types.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createMessage(CMS_Session* session, CMS_Message** message) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {

            wrapper->message = session->session->createMessage();
            wrapper->type = CMS_MESSAGE;
            *message = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createTextMessage(CMS_Session* session, CMS_Message** message, const char* body) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {

            if (body == NULL) {
                wrapper->message = session->session->createTextMessage();
            } else {
                wrapper->message = session->session->createTextMessage(body);
            }

            wrapper->type = CMS_TEXT_MESSAGE;
            *message = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createBytesMessage(CMS_Session* session, CMS_Message** message, unsigned char* body, int length) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {

            if (body == NULL) {
                wrapper->message = session->session->createBytesMessage();
            } else {
                wrapper->message = session->session->createBytesMessage(body, length);
            }

            wrapper->type = CMS_BYTES_MESSAGE;
            *message = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createMapMessage(CMS_Session* session, CMS_Message** message) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {

            wrapper->message = session->session->createMapMessage();
            wrapper->type = CMS_MAP_MESSAGE;
            *message = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createStreamMessage(CMS_Session* session, CMS_Message** message) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {

            wrapper->message = session->session->createStreamMessage();
            wrapper->type = CMS_STREAM_MESSAAGE;
            *message = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroyMessage(CMS_Message* message) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        try{
            delete message->message;
            delete message;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
