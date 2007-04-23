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

#include "SocketError.h"
#include <activemq/util/Config.h>

#if defined(HAVE_WINSOCK2_H)
    #include <Winsock2.h>
#else
    #include <errno.h>
#endif

using namespace activemq;
using namespace activemq::network;

// Platform constants.
#if defined(HAVE_WINSOCK2_H)
    const int SocketError::INTERRUPTED = WSAEINTR;
#else
    const int SocketError::INTERRUPTED = EINTR;
#endif

////////////////////////////////////////////////////////////////////////////////
int SocketError::getErrorCode() {
    
    #if defined(HAVE_WINSOCK2_H)
    
        return ::WSAGetLastError();
        
    #else
         
        return errno;
        
    #endif
}

////////////////////////////////////////////////////////////////////////////////
std::string SocketError::getErrorString() {
    
    std::string returnValue;
    
    // Get the error code.
    int errorCode = getErrorCode();
    
    #if defined(HAVE_WINSOCK2_H)
  
        // Create the error string.
        static const int errorStringSize = 512;
        char errorString[errorStringSize];
        memset( errorString, 0, errorStringSize );
        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
           0,
           errorCode,
           0,
           errorString,
           errorStringSize - 1,
           NULL);
           
        returnValue = errorString;
        
    #else
         
        // Create the error string.
        returnValue = ::strerror(errorCode);
        
    #endif
    
    return returnValue;
}

