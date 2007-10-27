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

#include "SSLError.h"
#include <activemq/util/Config.h>

#ifdef AMQ_HAVE_OPENSSL
#include <openssl/err.h>

using namespace activemq;
using namespace activemq::network;

////////////////////////////////////////////////////////////////////////////////
unsigned long SSLError::getErrorCode() {
    return ERR_peek_last_error();
}

////////////////////////////////////////////////////////////////////////////////
std::string SSLError::getErrorString() {
    
    std::string returnValue;
    
    for (unsigned long e = ERR_get_error(); e; e = ERR_get_error()) {
	char msg[256];
	ERR_error_string_n(e, msg, sizeof msg);
	returnValue += "\n";
	returnValue += msg;
    }
    
    return returnValue;
}

#endif /* AMQ_HAVE_OPENSSL */
