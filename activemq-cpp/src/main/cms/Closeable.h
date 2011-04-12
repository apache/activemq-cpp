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

#ifndef _CMS_CLOSEABLE_H
#define _CMS_CLOSEABLE_H

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * Interface for a class that implements the close method.
     *
     * A class that implements this interface should release all resources upon the close
     * call and should throw an exception from any methods that require those resources
     * after they have been closed.
     *
     * @since 1.0
     */
    class CMS_API Closeable{

    public:

        virtual ~Closeable() throw();

        /**
         * Closes this object and deallocates the appropriate resources.
         * The object is generally no longer usable after calling close.
         *
         * @throws CMSException - If an error occurs while the resource is being closed.
         */
        virtual void close() = 0;

    };
}

#endif /*_CMS_CLOSEABLE_H*/
