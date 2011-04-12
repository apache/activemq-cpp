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

#ifndef _CMS_STARTABLE_H
#define _CMS_STARTABLE_H

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * Interface for a class that implements the start method.
     *
     * An object that implements the Startable interface implies that until its start
     * method is called it will be considered to be in a closed or stopped state and will
     * throw an Exception to indicate that it is not in an started state if one of its
     * methods is called.
     *
     * @since 1.0
     */
    class CMS_API Startable {
    public:

        virtual ~Startable() throw();

        /**
         * Starts the service.
         *
         * @throws CMSException if an internal error occurs while starting.
         */
        virtual void start() = 0;

    };
}

#endif /*_CMS_STARTABLE_H*/
