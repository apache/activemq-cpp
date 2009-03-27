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

#ifndef _DECAF_UTIL_CONCURRENT_SYNCHRONIZABLE_H_
#define _DECAF_UTIL_CONCURRENT_SYNCHRONIZABLE_H_

#include <decaf/lang/Exception.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace concurrent{

    /**
     * The interface for all synchronizable objects (that is, objects
     * that can be locked and unlocked).
     */
    class DECAF_API Synchronizable
    {
    public:

        virtual ~Synchronizable(){}

        /**
         * Locks the object.
         * @throws Exception
         */
        virtual void lock() throw( lang::Exception ) = 0;

        /**
         * Unlocks the object.
         * @throws Exception
         */
        virtual void unlock() throw( lang::Exception ) = 0;

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void wait() throw( lang::Exception ) = 0;

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in millisecsonds to wait, or
         * WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait(unsigned long millisecs)
            throw( lang::Exception ) = 0;

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notify() throw( lang::Exception ) = 0;

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notifyAll() throw( lang::Exception ) = 0;

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_SYNCHRONIZABLE_H_*/
