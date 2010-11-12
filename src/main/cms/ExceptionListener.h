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

#ifndef _CMS_EXCEPTIONLISTENER_H_
#define _CMS_EXCEPTIONLISTENER_H_

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * If a CMS provider detects a serious problem, it notifies the client
     * application through an <code>ExceptionListener</code> that is registered
     * with the <code>Connection</code>.
     * <p>
     * An exception listener allows a client to be notified of a problem asynchronously.
     * Some connections only consume messages via the asynchronous event mechanism so
     * they would have no other way to learn that their connection has failed.
     *
     * @since 1.0
     */
    class CMS_API ExceptionListener {
    public:

        virtual ~ExceptionListener();

        /**
         * Called when an exception occurs.  Once notified of an exception
         * the caller should no longer use the resource that generated the
         * exception.
         *
         * @param ex
         *      Exception Object that occurred.
         */
        virtual void onException( const cms::CMSException& ex ) = 0;

    };

}

#endif /*_CMS_EXCEPTIONLISTENER_H_*/
