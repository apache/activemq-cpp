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

#ifndef _CMS_UNSUPPORTEDOPERATIONEXCEPTION_H_
#define _CMS_UNSUPPORTEDOPERATIONEXCEPTION_H_

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms {

    /**
     * This exception must be thrown when a CMS client attempts use a CMS method that is not
     * implemented or not supported by the CMS Provider in use.
     *
     * @since 2.0
     */
    class CMS_API UnsupportedOperationException : public CMSException {
    public:

        UnsupportedOperationException();

        UnsupportedOperationException( const UnsupportedOperationException& ex );

        UnsupportedOperationException( const std::string& message );

        UnsupportedOperationException( const std::string& message,
                                       const std::exception* cause );

        UnsupportedOperationException( const std::string& message,
                                       const std::exception* cause,
                                       const std::vector< std::pair< std::string, int> >& stackTrace );

        virtual ~UnsupportedOperationException() throw();

    };

}

#endif /* _CMS_UNSUPPORTEDOPERATIONEXCEPTION_H_ */
