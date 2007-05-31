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

#ifndef _DECAF_UTIL_CONCURRENT_MUTEX_H_
#define _DECAF_UTIL_CONCURRENT_MUTEX_H_

// Includes.
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/Config.h>
#include <list>

#ifdef HAVE_SYS_TIME_H
    #include <sys/time.h>
#endif

#ifdef HAVE_PTHREAD_H
    #include <pthread.h>
#else
    #include <windows.h>

    #if ( !defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0400)
        #if ( !defined(WINVER) || WINVER < 0x0400)
            #pragma message ("Unsupported platform, Windows NT 4.0 or later required")
        #endif
    #endif

#endif

#include <assert.h>

namespace decaf{
namespace util{
namespace concurrent{

    /**
     * Creates a pthread_mutex_t object. The object is created
     * such that successive locks from the same thread is allowed
     * and will be successful.
     * @see  pthread_mutex_t
     */
    class AMQCPP_API Mutex : public Synchronizable
    {
    private:       // Data

        /**
         * The mutex object.
         */
        #ifdef HAVE_PTHREAD_H
            pthread_mutex_t mutex;

            std::list<pthread_cond_t*> eventQ;
        #else
            CRITICAL_SECTION mutex;

            std::list<HANDLE> eventQ;
        #endif

        // Lock Status Members
        int           lock_count;
        unsigned long lock_owner;

    public:

        /**
         * Constructor - creates and initializes the mutex.
         */
        Mutex();

        /**
         * Destructor - destroys the mutex object.
         */
        virtual ~Mutex();

        /**
         * Locks the object.
         * @throws ActiveMQException
         */
        virtual void lock() throw( lang::Exception );

        /**
         * Unlocks the object.
         * @throws ActiveMQException
         */
        virtual void unlock() throw( lang::Exception );

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.
         * @throws ActiveMQException
         */
        virtual void wait() throw( lang::Exception );

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in milliseconds to wait.
         * @throws ActiveMQException
         */
        virtual void wait( unsigned long millisecs )
            throw( lang::Exception );

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.
         * @throws ActiveMQException
         */
        virtual void notify() throw( lang::Exception );

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.
         * @throws ActiveMQException
         */
        virtual void notifyAll() throw( lang::Exception );

    private:

        /**
         * Check if the calling thread is the Lock Owner
         * @retuns true if the caller is the lock owner
         */
        bool isLockOwner()
        {
            return lock_owner == lang::Thread::getId();
        }

    };

}}}

#endif // _DECAF_UTIL_CONCURRENT_MUTEX_H_
