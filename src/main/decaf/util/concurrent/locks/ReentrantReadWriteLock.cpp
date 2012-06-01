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

#include "ReentrantReadWriteLock.h"

#include <decaf/lang/Exception.h>
#include <decaf/util/concurrent/locks/AbstractQueuedSynchronizer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::locks;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Sync : public AbstractQueuedSynchronizer {
    private:

        /*
         * Read vs write count extraction constants and functions.
         * Lock state is logically divided into two unsigned shorts:
         * The lower one representing the exclusive (writer) lock hold count,
         * and the upper the shared (reader) hold count.
         */

        static const int SHARED_SHIFT;
        static const int SHARED_UNIT;
        static const int MAX_COUNT;
        static const int EXCLUSIVE_MASK;

    protected:

        /** Returns the number of shared holds represented in count  */
        static int sharedCount(int c) {
            return c >> SHARED_SHIFT;
        }

        /** Returns the number of exclusive holds represented in count  */
        static int exclusiveCount(int c) {
            return c & EXCLUSIVE_MASK;
        }

    public:

    };

    const int Sync::SHARED_SHIFT   = 16;
    const int Sync::SHARED_UNIT    = (1 << SHARED_SHIFT);
    const int Sync::MAX_COUNT      = (1 << SHARED_SHIFT) - 1;
    const int Sync::EXCLUSIVE_MASK = (1 << SHARED_SHIFT) - 1;

    class FairSync : public Sync {
    public:

    };

    class NonFairSync : public Sync {
    public:

    };

    class ReadLock : public Lock {
    public:

        Sync* sync;

        ReadLock(Sync* sync) : Lock(), sync(sync) {
        }

    };

    class WriteLock : public Lock {
    public:

        Sync* sync;

        WriteLock(Sync* sync) : Lock(), sync(sync) {
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {
namespace locks {

    class ReentrantReadWriteLockImpl {
    public:

        Lock* readLock;
        Lock* writeLock;
        Sync* sync;

        ReentrantReadWriteLockImpl(bool fair) : readLock(NULL), writeLock(NULL), sync(NULL) {
            if (fair) {
                sync = new FairSync();
            } else {
                sync = new NonFairSync();
            }

//            readLock = new ReadLock(sync);
//            writeLock = new WriteLock(sync);
        }

        ~ReentrantReadWriteLockImpl() {
            delete readLock;
            delete writeLock;
            delete sync;
        }
    };

}}}}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLock::ReentrantReadWriteLock() : ReadWriteLock(), impl(new ReentrantReadWriteLockImpl(true)) {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLock::ReentrantReadWriteLock(bool fair) :
    ReadWriteLock(), impl(new ReentrantReadWriteLockImpl(fair)) {
}

////////////////////////////////////////////////////////////////////////////////
ReentrantReadWriteLock::~ReentrantReadWriteLock() {
    try {
        delete impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
Lock& ReentrantReadWriteLock::readLock() {
    throw "";
}

////////////////////////////////////////////////////////////////////////////////
Lock& ReentrantReadWriteLock::writeLock() {
    throw "";
}
