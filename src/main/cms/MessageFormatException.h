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

#ifndef _CMS_MESSAGEFORMATEXCEPTION_H_
#define _CMS_MESSAGEFORMATEXCEPTION_H_

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * This exception must be thrown when a CMS client attempts to use a data type not
     * supported by a message or attempts to read data in a message as the wrong type. It
     * must also be thrown when equivalent type errors are made with message property values.
     * For example, this exception must be thrown if StreamMessage.readShort is used to read
     * a boolean value.
     *
     * @since 1.3
     */
    class CMS_API MessageFormatException : public CMSException {
    public:

        MessageFormatException();

        MessageFormatException( const MessageFormatException& ex );

        MessageFormatException( const std::string& message );

        MessageFormatException( const std::string& message,
                                const std::exception* cause );

        MessageFormatException( const std::string& message,
                                const std::exception* cause,
                                const std::vector< std::pair< std::string, int> >& stackTrace );

        virtual ~MessageFormatException() throw();

    };

}

#endif /*_CMS_MESSAGEFORMATEXCEPTION_H_*/
