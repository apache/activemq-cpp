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

#ifndef _DECAF_UTIL_CONCURRENT_EXECUTORSTESTSUPPORT_H_
#define _DECAF_UTIL_CONCURRENT_EXECUTORSTESTSUPPORT_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>

#include <decaf/lang/Throwable.h>
#include <decaf/util/concurrent/ThreadFactory.h>
#include <decaf/util/concurrent/ExecutorService.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>

namespace decaf {
namespace util {
namespace concurrent {

    using decaf::lang::Thread;
    using decaf::lang::Runnable;

    class ExecutorsTestSupport : public CppUnit::TestFixture {
    public:

        static const int SHORT_DELAY_MS;
        static const int SMALL_DELAY_MS;
        static const int MEDIUM_DELAY_MS;
        static const int LONG_DELAY_MS;

    protected:

        bool threadFailed;

    public:

        ExecutorsTestSupport();
        virtual ~ExecutorsTestSupport();

        virtual void setUp();
        virtual void tearDown();

    protected:

        void threadFail(const std::string& reason);
        void threadUnexpectedException();
        void threadUnexpectedException(decaf::lang::Throwable& ex);

        void unexpectedException();
        void shouldThrow();

        void joinPool(ExecutorService* exec);
        void joinPool(ExecutorService& exec);

    public:

        class NoOpRunnable : public decaf::lang::Runnable {
        public:

            NoOpRunnable() : decaf::lang::Runnable() {
            }

            virtual ~NoOpRunnable() {}

            virtual void run() {
            }
        };

        class ShortRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            ShortRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~ShortRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SHORT_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        class SmallRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            SmallRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~SmallRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SMALL_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        class MediumRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            MediumRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~MediumRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(MEDIUM_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        class SimpleThreadFactory : public ThreadFactory {
        public:

            SimpleThreadFactory() : ThreadFactory() {
            }

            virtual ~SimpleThreadFactory() {}

            virtual Thread* newThread(Runnable* task) {
                return new Thread(task);
            }
        };

        class NoOpREHandler : public RejectedExecutionHandler {
        public:

            NoOpREHandler() : RejectedExecutionHandler() {
            }

            virtual ~NoOpREHandler() {}

            virtual void rejectedExecution(Runnable* r, ThreadPoolExecutor* executor) {
            }
        };

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTORSTESTSUPPORT_H_ */
