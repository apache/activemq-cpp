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

#ifndef _CMS_TRANSACTIONINPROGRESSEXCEPTION_H_
#define _CMS_TRANSACTIONINPROGRESSEXCEPTION_H_

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms {

    /**
     * This exception is thrown when an operation is invalid because a transaction is in progress.
     * For instance, an attempt to call Session::commit when a session is part of a distributed
     * transaction should throw a TransactionInProgressException.
     *
     * @since 2.3
     */
    class CMS_API TransactionInProgressException : public cms::CMSException {
    public:

        TransactionInProgressException();

        TransactionInProgressException( const TransactionInProgressException& ex );

        TransactionInProgressException( const std::string& message );

        TransactionInProgressException( const std::string& message,
                                        const std::exception* cause );

        TransactionInProgressException( const std::string& message,
                                        const std::exception* cause,
                                        const std::vector< std::pair< std::string, int> >& stackTrace );

        virtual ~TransactionInProgressException() throw();


    };

}

#endif /* _CMS_TRANSACTIONINPROGRESSEXCEPTION_H_ */
