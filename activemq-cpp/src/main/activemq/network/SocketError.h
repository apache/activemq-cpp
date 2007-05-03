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

#ifndef ACTIVEMQ_NETWORK_SOCKETERROR_H_
#define ACTIVEMQ_NETWORK_SOCKETERROR_H_

#include <string>

namespace activemq{
namespace network{
    
    /**
     * Static utility class to simplify handling of error codes
     * for socket operations.
     */
    class SocketError {
    public:
    
        /**
         * Indicates that a socket operation was interrupted by a signal.
         */
        static const int INTERRUPTED;
        
    public:
    
        /**
         * Gets the last error appropriate for the platform.
         */
        static int getErrorCode();
        
        /**
         * Gets the string description for the last error.
         */
        static std::string getErrorString();
    };
}}

#endif /*ACTIVEMQ_NETWORK_SOCKETERROR_H_*/
