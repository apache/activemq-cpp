/*
 * Copyright 2006 The Apache Software Foundation or its licensors, as
 * applicable.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ACTIVEMQ_CORE_ACTIVEMQCONSTANTS_H_
#define ACTIVEMQ_CORE_ACTIVEMQCONSTANTS_H_

#include <string>
#include <map>
#include <activemq/util/Config.h>

namespace activemq{
namespace core{

    /**
     * Class holding constant values for various ActiveMQ specific things
     * Each constant is defined as an enumeration and has functions that
     * convert back an forth between string and enum values.
     */
    class AMQCPP_API ActiveMQConstants{
    public:

        // Flags to indicate Transaction States.
        enum TransactionState {
            TRANSACTION_STATE_BEGIN = 0,
            TRANSACTION_STATE_PREPARE = 1,
            TRANSACTION_STATE_COMMITONEPHASE = 2,
            TRANSACTION_STATE_COMMITTWOPHASE = 3,
            TRANSACTION_STATE_ROLLBACK = 4,
            TRANSACTION_STATE_RECOVER = 5,
            TRANSACTION_STATE_FORGET = 6,
            TRANSACTION_STATE_END = 7
        };

        // Flags to be applied when sending the Destination Info Command.
        enum DestinationActions {
            DESTINATION_ADD_OPERATION = 0,
            DESTINATION_REMOVE_OPERATION = 1
        };

        // Represents the Acknowledgement types that are supported for the
        // Message Ack Command.
        enum AckType {
            ACK_TYPE_DELIVERED   = 0,  // Message delivered but not consumed
            ACK_TYPE_POISON      = 1,  // Message could not be processed due to
                                       // poison pill but discard anyway
            ACK_TYPE_CONSUMED    = 2,  // Message consumed, discard
            ACK_TYPE_REDELIVERED = 3,  // Message has been re-delivered.
            ACK_TYPE_INDIVIDUAL  = 4   // Acks a single message at a time.
        };

        /**
         * These values represent the options that can be appended to an
         * Destination name, i.e. /topic/foo?consumer.exclusive=true
         */
        enum DestinationOption{
            CONSUMER_PREFECTCHSIZE,
            CUNSUMER_MAXPENDINGMSGLIMIT,
            CONSUMER_NOLOCAL,
            CONSUMER_DISPATCHASYNC,
            CONSUMER_RETROACTIVE,
            CONSUMER_SELECTOR,
            CONSUMER_EXCLUSIVE,
            CONSUMER_PRIORITY,
            NUM_OPTIONS
        };

        /**
         * These values represent the parameters that can be added to the
         * connection URI that affect the ActiveMQ Core API
         */
        enum URIParam
        {
            CONNECTION_SENDTIMEOUT,
            CONNECTION_PRODUCERWINDOWSIZE,
            CONNECTION_CLOSETIMEOUT,
            CONNECTION_ALWAYSSYNCSEND,
            CONNECTION_USEASYNCSEND,
            CONNECTION_USECOMPRESSION,
            CONNECTION_DISPATCHASYNC,
            PARAM_USERNAME,
            PARAM_PASSWORD,
            PARAM_CLIENTID,
            NUM_PARAMS
        };

        static const std::string& toString( const DestinationOption option ){
            return StaticInitializer::destOptions[option];
        }

        static DestinationOption toDestinationOption( const std::string& option ){
            std::map< std::string, DestinationOption >::iterator iter =
                StaticInitializer::destOptionMap.find( option );

            if( iter == StaticInitializer::destOptionMap.end() ){
                return NUM_OPTIONS;
            }

            return iter->second;
        }

        static const std::string& toString( const URIParam option ){
            return StaticInitializer::uriParams[option];
        }

        static URIParam toURIOption( const std::string& option ){
            std::map< std::string, URIParam >::iterator iter =
                StaticInitializer::uriParamsMap.find( option );

            if( iter == StaticInitializer::uriParamsMap.end() ){
                return NUM_PARAMS;
            }

            return iter->second;
        }

        class StaticInitializer{
        public:
            StaticInitializer();
            virtual ~StaticInitializer(){}

            static std::string destOptions[NUM_OPTIONS];
            static std::string uriParams[NUM_PARAMS];
            static std::map<std::string, DestinationOption> destOptionMap;
            static std::map<std::string, URIParam> uriParamsMap;
        };

    private:

        static StaticInitializer staticInits;

    };

}}

#endif /*ACTIVEMQ_CORE_ACTIVEMQCONSTANTS_H_*/
