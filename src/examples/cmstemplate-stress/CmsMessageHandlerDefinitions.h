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

#ifndef _CMSTEMPLATE_CMSMESSAGEHANDLERDEFINITIONS_H_
#define _CMSTEMPLATE_CMSMESSAGEHANDLERDEFINITIONS_H_

#include <decaf/util/Config.h>

namespace cmstemplate {

    enum ErrorCode {         // NOTE: When added an entry to ErrorCode you must add the corresponding error string to ErrorDescription
       CMS_SUCCESS = 0,
       CMS_ERROR_UNABLE_TO_PARSE_XML,
       CMS_ERROR_MESSAGE_HAS_BEEN_DEFINED_ALREADY,
       CMS_ERROR_HEADER_HAS_BEEN_DEFINED_ALREADY,
       CMS_ERROR_CLIENT_HAS_BEEN_DEFINED_ALREADY,
       CMS_ERROR_DESTINATION_HAS_BEEN_DEFINED_ALREADY,
       CMS_ERROR_INVALID_CLIENT,
       CMS_ERROR_INVALID_DESTINATION,
       CMS_ERROR_INVALID_MESSAGE,
       CMS_ERROR_INVALID_HEADERS,
       CMS_ERROR_INVALID_MESSAGELISTENER,
       CMS_ERROR_A_MESSAGELISTENER_HAS_BEEN_REGISTERED_ALREADY,
       CMS_ERROR_RECEIVER_TIMEDOUT,
       CMS_ERROR_DESTINATION_NOT_CONFIGURED_FOR_SENDING_MESSAGES,
       CMS_ERROR_DESTINATION_NOT_CONFIGURED_FOR_RECEIVING_MESSAGES,
       CMS_ERROR_CAUGHT_CMS_EXCEPTION,
       CMS_ERROR_CAUGHT_TBGENOBJ_ERROR,
       CMS_ERROR_MESSAGE_BROKER_ERROR,
       CMS_ERROR_BROKER_MONITOR_NOT_FOUND,
       CMS_ERROR_BROKER_MONITORING_NOT_TURNED_ON,
       CMS_ERROR_INVALID_BROKERSTATUSLISTENER,
       CMS_ERROR_A_BROKERSTATUSLISTENER_HAS_BEEN_REGISTERED_ALREADY,
       CMS_ERROR_CAUGHT_EXCEPTION_IN_INIT,
       CMS_ERROR_CAUGHT_EXCEPTION_IN_UNINIT,
       CMS_LAST                             // Put all error enums BEFORE this one. This one must be listed last.
    };

    const char ErrorDescription[][100] = {
        "Success",
        "Unable to parse xml",
        "Message has been defined already",
        "Header has been defined already",
        "Client has been defined already",
        "Destination has been defined already",
        "Invalid client",
        "Invalid destination",
        "Invalid message",
        "Invalid headers",
        "Invalid messagelistener",
        "A messagelistener has been registered already with the destination",
        "Receiver timed out",
        "Destination not configured for sending messages",
        "Destination not configured for receiving messages",
        "Caught CMS exception",
        "Caught TBGenObj error",
        "Message broker appears to be offline",
        "Can not find broker monitor",
        "The broker monitoring functinaliy has not been turned on",
        "Invalid brokerStatuslistener",
        "This brokerStatuslistener has been registered already with the broker",
        "Caught an exception when initializing CmsMessageHandler",
        "Caught an exception when uninitializing CmsMessageHandler",
        "CMS_LAST - ErrorCodeToString macro index out of range",
    };

    #define ErrorCodeToString(i) (((i >= CMS_SUCCESS) && (i <= CMS_LAST)) ? ErrorDescription[i] : ErrorDescription[CMS_LAST])
    #define IsError(i) (i != CMS_SUCCESS)
}

#endif /** _CMSTEMPLATE_CMSMESSAGEHANDLERDEFINITIONS_H_ */
