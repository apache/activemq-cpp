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

#ifndef _CMS_ASYNCCALLBACK_H_
#define _CMS_ASYNCCALLBACK_H_

#include <cms/Config.h>

#include <cms/ExceptionListener.h>

namespace cms {

    /**
     * Asynchronous event interface for CMS asynchronous operations.
     *
     * For operations in CMS that allow for Asynchronous execution the caller
     * provides an instance of this interface.  If the asynchronous action is
     * successful the onSuccess method is invoked, otherwise the onException
     * method of cms::ExceptionListener is called.
     *
     * @since 3.0
     */
    class CMS_API AsyncCallback : public ExceptionListener {
    public:

        virtual ~AsyncCallback();

        /**
         * Called when the asynchronous operation has completed successfully.
         */
        virtual void onSuccess() = 0;

    };

}

#endif /* ASYNCCALLBACK_H_ */
