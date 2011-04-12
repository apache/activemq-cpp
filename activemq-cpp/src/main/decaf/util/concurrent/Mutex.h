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

        Mutex( const std::string& name );

        virtual ~Mutex();

        std::string getName() const;

        std::string toString() const;

    public:  // Synchronizable API Implementation

        virtual void lock();

        virtual bool tryLock();

        virtual void unlock();

        virtual void wait();

        virtual void wait( long long millisecs );

        virtual void wait( long long millisecs, int nanos );

        virtual void notify();

        virtual void notifyAll();

    };

}}}

#endif /*_DECAF_CONCURRENT_MUTEX_H_*/
