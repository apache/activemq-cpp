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

#ifndef _ACTIVEMQ_TRANSPORT_COMMANDWRITER_H_
#define _ACTIVEMQ_TRANSPORT_COMMANDWRITER_H_

#include <decaf/io/OutputStream.h>
#include <decaf/io/Writer.h>
#include <activemq/transport/CommandIOException.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace transport{

    class Command;

    /**
     * Interface for an object responsible for writing a command
     * to an output stream.
     */
    class AMQCPP_API CommandWriter : public decaf::io::Writer {
    public:

        virtual ~CommandWriter() {}

        /**
         * Writes a command to the given output stream.
         * @param command the command to write.
         * @throws CommandIOException if a problem occurs during the write.
         */
        virtual void writeCommand( Command* command )
            throw ( CommandIOException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_COMMANDWRITER_H_*/
