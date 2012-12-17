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

#include "CountDownLatch.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    class LatchSync : public AbstractQueuedSynchronizer {
    private:

        LatchSync(const LatchSync&);
        LatchSync& operator= (const LatchSync&);

    public:

        LatchSync(int count) : AbstractQueuedSynchronizer() {
            this->setState(count);
        }

        virtual ~LatchSync() {}

        int getCount() const {
            return getState();
        }

    protected:

        virtual int tryAcquireShared(int acquires DECAF_UNUSED) {
            return getState() == 0 ? 1 : -1;
        }

        virtual bool tryReleaseShared(int releases DECAF_UNUSED) {

            for (;;) {

                int current = getState();
                if (current == 0) {
                    return false;
                }

                int next = current - 1;
                if (compareAndSetState(current, next)) {
                    return next == 0;
                }
            }
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
CountDownLatch::CountDownLatch(int count) : sync(NULL) {
    if (count < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Count must be non-negative.");
    }

    this->sync = new LatchSync(count);
}

////////////////////////////////////////////////////////////////////////////////
CountDownLatch::~CountDownLatch() {
    try {
        delete sync;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatch::await() {
    try {
        this->sync->acquireSharedInterruptibly(1);
    }
    DECAF_CATCH_RETHROW( decaf::lang::exceptions::InterruptedException )
    DECAF_CATCH_RETHROW( decaf::lang::Exception )
    DECAF_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool CountDownLatch::await( long long timeOut ) {
    try {
        return this->sync->tryAcquireSharedNanos(1, TimeUnit::MILLISECONDS.toNanos(timeOut));
    }
    DECAF_CATCH_RETHROW( decaf::lang::exceptions::InterruptedException )
    DECAF_CATCH_RETHROW( decaf::lang::Exception )
    DECAF_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool CountDownLatch::await( long long timeout, const TimeUnit& unit ) {
    try{
        return this->sync->tryAcquireSharedNanos(1, unit.toNanos(timeout));
    }
    DECAF_CATCH_RETHROW( decaf::lang::exceptions::InterruptedException )
    DECAF_CATCH_RETHROW( decaf::lang::Exception )
    DECAF_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatch::countDown() {
    try {
        this->sync->releaseShared(1);
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int CountDownLatch::getCount() const {
    return this->sync->getCount();
}

////////////////////////////////////////////////////////////////////////////////
std::string CountDownLatch::toString() const {
    return std::string("CountDownLatch[count = ") +
           Integer::toString(this->sync->getCount()) + "]";
}
