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

#include <CMS_MessageProducer.h>

#include <Config.h>
#include <types/CMS_Types.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createProducer(CMS_Session* session, CMS_Destination* destination, CMS_MessageProducer** producer) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_MessageProducer> wrapper( new CMS_MessageProducer );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {
            if( destination != NULL && destination->destination != NULL ) {
                wrapper->producer = session->session->createProducer(destination->destination);
            } else {
                wrapper->producer = session->session->createProducer(NULL);
            }
        }

        *producer = wrapper.release();
    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status producerDefaultSend(CMS_MessageProducer* producer, CMS_Message* message) {

    cms_status result = CMS_SUCCESS;

    try{

        if (producer == NULL || message == NULL) {
            result = CMS_ERROR;
        } else {
            producer->producer->send(message->message);
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status closeProducer(CMS_MessageProducer* producer) {

    cms_status result = CMS_SUCCESS;

    if(producer != NULL) {

        try{
            producer->producer->close();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroyProducer(CMS_MessageProducer* producer) {

    cms_status result = CMS_SUCCESS;

    if(producer != NULL) {

        try{
            delete producer->producer;
            delete producer;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
