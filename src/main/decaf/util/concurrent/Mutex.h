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

#ifndef _DECAF_CONCURRENT_MUTEX_H_
#define _DECAF_CONCURRENT_MUTEX_H_

#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace concurrent{

    class MutexProperties;

    /**
     * Mutex object that offers recursive support on all platforms as well as
     * providing the ability to use the standard wait / notify pattern used in
     * languages like Java.
     *
     * @since 1.0
     */
    class DECAF_API Mutex : public Synchronizable {
    private:

        MutexProperties* properties;

    private:

        Mutex( const Mutex& src );
        Mutex& operator= ( const Mutex& src );

    public:

        Mutex();

        virtual ~Mutex();

        virtual void lock() throw( decaf::lang::exceptions::RuntimeException );

        virtual bool tryLock() throw( decaf::lang::exceptions::RuntimeException );

        virtual void unlock() throw( decaf::lang::exceptions::RuntimeException );

        virtual void wait() throw( decaf::lang::exceptions::RuntimeException,
                                   decaf::lang::exceptions::IllegalMonitorStateException,
                                   decaf::lang::exceptions::InterruptedException );

        virtual void wait( long long millisecs )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException );

        virtual void wait( long long millisecs, int nanos )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalArgumentException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException );

        virtual void notify() throw( decaf::lang::exceptions::RuntimeException,
                                     decaf::lang::exceptions::IllegalMonitorStateException );

        virtual void notifyAll() throw( decaf::lang::exceptions::RuntimeException,
                                        decaf::lang::exceptions::IllegalMonitorStateException );

    };

}}}

#endif /*_DECAF_CONCURRENT_MUTEX_H_*/
