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

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_MARSHALER_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_MARSHALER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Command.h>
#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/wireformat/stomp/marshal/MarshalException.h>

namespace activemq{
namespace wireformat{
namespace stomp{
namespace marshal{

    /**
     * Interface for all marshalers between Commands and stomp frames.
     */
    class AMQCPP_API Marshaler {
    public:

        Marshaler(void) {}
        virtual ~Marshaler(void) {}

        /**
         * Marshal a Stomp Frame to a Stomp Command, the frame is now
         * owned by this Command, caller should not use the frame again.
         * @param frame Frame to Marshal
         * @return Newly Marshaled Stomp Message
         * @throws MarshalException
         */
        virtual commands::Command* marshal( StompFrame* frame )
            throw ( MarshalException );

        /**
         * Marshal a Stomp Command to a Stomp Frame, if the command that
         * is past is not castable to a Stomp Command an Exception will
         * be thrown
         * @param command The Stomp Command to Marshal
         * @return newly Marshaled Stomp Frame
         * @throws MarshalException
         */
        virtual const StompFrame& marshal(
            commands::Command* command )
                throw ( MarshalException );

    };

}}}}

#endif /*_ACTIVEMQ_WIREFORMAT_STOMP_MARSHALER_H_*/
