/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_WIREFORMATINFO_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_WIREFORMATINFO_H_

#include <activemq/connector/openwire/commands/BaseDataStructure.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class WireFormatInfo : public BaseDataStructure
    {
    public:
    
        WireFormatInfo();
    
        virtual ~WireFormatInfo();

        /**
         * Checks if the stackTraceEnabled flag is on
         * @return true if the flag is on.
         */
        bool isStackTraceEnabled() const {
            return stackTraceEnabled;
        }
    
        /**
         * Sets if the stackTraceEnabled flag is on
         * @param stackTraceEnabled - ture to turn flag is on
         */
        void setStackTraceEnabled( bool stackTraceEnabled ) {
            this->stackTraceEnabled = stackTraceEnabled;
        }
    
        /**
         * Checks if the tcpNoDelayEnabled flag is on
         * @return true if the flag is on.
         */
        bool isTcpNoDelayEnabled() const {
            return tcpNoDelayEnabled;
        }
    
        /**
         * Sets if the tcpNoDelayEnabled flag is on
         * @param tcpNoDelayEnabled - ture to turn flag is on
         */
        void setTcpNoDelayEnabled( bool tcpNoDelayEnabled ) {
            this->tcpNoDelayEnabled = tcpNoDelayEnabled;
        }
    
        /**
         * Get the current Wireformat Version
         * @return int that identifies the version
         */
        int getVersion() const {
            return version;
        }
    
        /**
         * Set the current Wireformat Version
         * @param version - int that identifies the version
         */
        void setVersion( int version ) {
            this->version = version;
        }
    
        /**
         * Checks if the cacheEnabled flag is on
         * @return true if the flag is on.
         */
        bool isCacheEnabled() const {
            return cacheEnabled;
        }
    
        /**
         * Sets if the cacheEnabled flag is on
         * @param cacheEnabled - ture to turn flag is on
         */
        void setCacheEnabled( bool cacheEnabled ) {
            this->cacheEnabled = cacheEnabled;
        }
    
        /**
         * Checks if the tightEncodingEnabled flag is on
         * @return true if the flag is on.
         */
        bool isTightEncodingEnabled() const {
            return tightEncodingEnabled;
        }
    
        /**
         * Sets if the tightEncodingEnabled flag is on
         * @param tightEncodingEnabled - ture to turn flag is on
         */
        void setTightEncodingEnabled( bool tightEncodingEnabled ) {
            this->tightEncodingEnabled = tightEncodingEnabled;
        }
    
        /**
         * Checks if the sizePrefixDisabled flag is on
         * @return true if the flag is on.
         */
        bool isSizePrefixDisabled() const {
            return sizePrefixDisabled;
        }
    
        /**
         * Sets if the sizePrefixDisabled flag is on
         * @param sizePrefixDisabled - ture to turn flag is on
         */
        void setSizePrefixDisabled( bool sizePrefixDisabled ) {
            this->sizePrefixDisabled = sizePrefixDisabled;
        }
    
        /**
         * Get the current value of the maxInactivityDuration feild
         * @return the value of the maxInactivityDuration
         */
        long getMaxInactivityDuration() const {
            return maxInactivityDuration;
        }
    
        /**
         * Set the current Wireformat Version
         * @param version - int that identifies the version
         */
        void setMaxInactivityDuration( long maxInactivityDuration ) {
            this->maxInactivityDuration = maxInactivityDuration;
        }

    private:
    
        int version;
        bool stackTraceEnabled;
        bool tcpNoDelayEnabled;
        bool cacheEnabled;
        bool tightEncodingEnabled;
        bool sizePrefixDisabled;
        long maxInactivityDuration;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_WIREFORMATINFO_H_*/
