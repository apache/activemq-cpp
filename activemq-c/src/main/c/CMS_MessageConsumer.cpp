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

#include <CMS_MessageConsumer.h>

#include <Config.h>
#include <types/CMS_Types.h>

#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/StreamMessage.h>
#include <cms/MapMessage.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createDefaultConsumer(CMS_Session* session, CMS_Destination* destination, CMS_MessageConsumer** consumer) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_MessageConsumer> wrapper( new CMS_MessageConsumer );

    try{

        if (session == NULL || destination == NULL) {
            result = CMS_ERROR;
        } else {
            wrapper->consumer = session->session->createConsumer(destination->destination);
            *consumer = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createConsumer(CMS_Session* session, CMS_Destination* destination, CMS_MessageConsumer** consumer, const char* selector, int noLocal) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_MessageConsumer> wrapper( new CMS_MessageConsumer );

    try{

        if (session == NULL || destination == NULL) {
            result = CMS_ERROR;
        } else {

            std::string sel = selector == NULL ? "" : std::string(selector);

            wrapper->consumer = session->session->createConsumer(
                destination->destination, sel, noLocal > 0 ? true : false);
            *consumer = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createDurableConsumer(CMS_Session* session,
                                 CMS_Destination* destination,
                                 CMS_MessageConsumer** consumer,
                                 const char* subscriptionName,
                                 const char* selector,
                                 int noLocal) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_MessageConsumer> wrapper( new CMS_MessageConsumer );

    try{

        if (session == NULL || destination == NULL) {
            result = CMS_ERROR;
        } else {

            if (destination->type != CMS_TOPIC) {
                result = CMS_INVALID_DESTINATION;
            } else {

                cms::Topic* topic = dynamic_cast<cms::Topic*>(destination->destination);

                std::string name = subscriptionName == NULL ? "" : std::string(subscriptionName);
                std::string sel = selector == NULL ? "" : std::string(selector);

                wrapper->consumer = session->session->createDurableConsumer(
                    topic, name, sel, noLocal > 0 ? true : false);

                *consumer = wrapper.release();
            }
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status consumerReceive(CMS_MessageConsumer* consumer, CMS_Message** message) {

    cms_status result = CMS_SUCCESS;

    if(consumer != NULL) {

        try{

            std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

            cms::Message* msg = consumer->consumer->receive();

            if(msg != NULL) {
                wrapper->message = msg;

                if(dynamic_cast<cms::TextMessage*>(msg) != NULL) {
                   wrapper->type = CMS_TEXT_MESSAGE;
                } else if(dynamic_cast<cms::BytesMessage*>(msg) != NULL) {
                    wrapper->type = CMS_BYTES_MESSAGE;
                } else if(dynamic_cast<cms::MapMessage*>(msg) != NULL) {
                    wrapper->type = CMS_MAP_MESSAGE;
                } else if(dynamic_cast<cms::StreamMessage*>(msg) != NULL) {
                    wrapper->type = CMS_STREAM_MESSAAGE;
                } else {
                    wrapper->type = CMS_MESSAGE;
                }

                *message = wrapper.release();
            } else {
                *message = NULL;
            }

        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status consumerReceiveWithTimeout(CMS_MessageConsumer* consumer, CMS_Message** message, int timeout) {

    cms_status result = CMS_SUCCESS;

    if(consumer != NULL) {

        try{

            std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

            cms::Message* msg = consumer->consumer->receive(timeout);

            if (msg != NULL) {
                wrapper->message = msg;

                if(dynamic_cast<cms::TextMessage*>(msg) != NULL) {
                   wrapper->type = CMS_TEXT_MESSAGE;
                } else if(dynamic_cast<cms::BytesMessage*>(msg) != NULL) {
                    wrapper->type = CMS_BYTES_MESSAGE;
                } else if(dynamic_cast<cms::MapMessage*>(msg) != NULL) {
                    wrapper->type = CMS_MAP_MESSAGE;
                } else if(dynamic_cast<cms::StreamMessage*>(msg) != NULL) {
                    wrapper->type = CMS_STREAM_MESSAAGE;
                } else {
                    wrapper->type = CMS_MESSAGE;
                }

                *message = wrapper.release();
            } else {
                *message = NULL;
            }

        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status consumerReceiveNoWait(CMS_MessageConsumer* consumer, CMS_Message** message) {

    cms_status result = CMS_SUCCESS;

    if(consumer != NULL) {

        try{

            std::auto_ptr<CMS_Message> wrapper( new CMS_Message );

            cms::Message* msg = consumer->consumer->receive();

            if (msg != NULL) {
                wrapper->message = msg;

                if(dynamic_cast<cms::TextMessage*>(msg) != NULL) {
                   wrapper->type = CMS_TEXT_MESSAGE;
                } else if(dynamic_cast<cms::BytesMessage*>(msg) != NULL) {
                    wrapper->type = CMS_BYTES_MESSAGE;
                } else if(dynamic_cast<cms::MapMessage*>(msg) != NULL) {
                    wrapper->type = CMS_MAP_MESSAGE;
                } else if(dynamic_cast<cms::StreamMessage*>(msg) != NULL) {
                    wrapper->type = CMS_STREAM_MESSAAGE;
                } else {
                    wrapper->type = CMS_MESSAGE;
                }

                *message = wrapper.release();
            } else {
                *message = NULL;
            }

        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status closeConsumer(CMS_MessageConsumer* consumer) {

    cms_status result = CMS_SUCCESS;

    if(consumer != NULL) {

        try{
            consumer->consumer->close();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status getConsumerMessageSelector(CMS_MessageConsumer* consumer, char* dest, int size) {

    cms_status result = CMS_SUCCESS;

    if(consumer != NULL && dest != NULL && size > 0) {

        try{

            std::string selector = consumer->consumer->getMessageSelector();

            if(!selector.empty()) {

                std::size_t pos = 0;
                for(; pos < selector.size() && pos < size - 1; ++pos) {
                    dest[pos] = selector.at(pos);
                }

                dest[pos] = '\0';
            } else {
                dest[0] = '\0';
            }

        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroyConsumer(CMS_MessageConsumer* consumer) {

    cms_status result = CMS_SUCCESS;

    if(consumer != NULL) {

        try{
            delete consumer->consumer;
            delete consumer;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
