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

#include <decaf/util/concurrent/Mutex.h>

#include <decaf/internal/util/concurrent/Threading.h>
#include <decaf/lang/Integer.h>

#include <list>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {

    class MutexProperties {
    private:

        MutexProperties(const MutexProperties&);
        MutexProperties& operator=(const MutexProperties&);

    public:

        MutexProperties() : monitor(NULL), name() {
            this->name = DEFAULT_NAME_PREFIX + Integer::toString( ++id );
        }

        MutexProperties(const std::string& name) : monitor(NULL), name(name) {
            if (this->name.empty()) {
                this->name = DEFAULT_NAME_PREFIX + Integer::toString(++id);
            }
        }

        MonitorHandle* monitor;
        std::string name;

        static unsigned int id;
        static std::string DEFAULT_NAME_PREFIX;

    };

    unsigned int MutexProperties::id = 0;
    std::string MutexProperties::DEFAULT_NAME_PREFIX = "Mutex-";

}}}

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex() : Synchronizable(), properties(NULL) {
    this->properties = new MutexProperties();
}

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex( const std::string& name ) : Synchronizable(), properties(NULL) {
    this->properties = new MutexProperties( name );
}

////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex() {

    if (this->properties->monitor != NULL) {
        Threading::returnMonitor(this->properties->monitor);
    }

    delete this->properties;
}

////////////////////////////////////////////////////////////////////////////////
std::string Mutex::getName() const {
    return this->properties->name;
}

////////////////////////////////////////////////////////////////////////////////
std::string Mutex::toString() const {
    return this->properties->name;
}

////////////////////////////////////////////////////////////////////////////////
bool Mutex::isLocked() const {
    if (this->properties->monitor != NULL) {
        Threading::isMonitorLocked(this->properties->monitor);
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::lock() {

    if(this->properties->monitor == NULL) {

        Threading::lockThreadsLib();

        if (this->properties->monitor == NULL) {
            this->properties->monitor = Threading::takeMonitor(true);
        }

        Threading::unlockThreadsLib();
    }

    Threading::enterMonitor(this->properties->monitor);
}

////////////////////////////////////////////////////////////////////////////////
bool Mutex::tryLock() {

    if(this->properties->monitor == NULL) {

        Threading::lockThreadsLib();

        if (this->properties->monitor == NULL) {
            this->properties->monitor = Threading::takeMonitor(true);
        }

        Threading::unlockThreadsLib();
    }

    return Threading::tryEnterMonitor(this->properties->monitor);
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::unlock() {

    if (this->properties->monitor == NULL) {
        throw IllegalMonitorStateException(__FILE__, __LINE__,
            "Call to unlock without prior call to lock or tryLock");
    }

    Threading::exitMonitor(this->properties->monitor);
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait() {
    wait(0, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait(long long millisecs) {
    wait(millisecs, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs, int nanos ) {

    if (millisecs < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Milliseconds value cannot be negative.");
    }

    if (nanos < 0 || nanos > 999999) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Nanoseconds value must be in the range [0..999999].");
    }

    if (this->properties->monitor == NULL) {
        throw IllegalMonitorStateException(__FILE__, __LINE__,
            "Call to wait without prior call to lock or tryLock");
    }

    Threading::waitOnMonitor(this->properties->monitor, millisecs, nanos);
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notify() {

    if (this->properties->monitor == NULL) {
        throw IllegalMonitorStateException(__FILE__, __LINE__,
            "Call to notify without prior call to lock or tryLock");
    }

    Threading::notifyWaiter(this->properties->monitor);
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notifyAll() {

    if (this->properties->monitor == NULL) {
        throw IllegalMonitorStateException(__FILE__, __LINE__,
            "Call to notifyAll without prior call to lock or tryLock");
    }

    Threading::notifyAllWaiters(this->properties->monitor);
}
