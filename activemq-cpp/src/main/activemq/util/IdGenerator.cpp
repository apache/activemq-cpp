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

#include "IdGenerator.h"

#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Thread.h>
#include <decaf/net/InetAddress.h>
#include <decaf/net/ServerSocket.h>
#include <decaf/util/concurrent/Mutex.h>

#include <decaf/internal/util/StringUtils.h>
#include <decaf/lang/exceptions/RuntimeException.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
IdGeneratorKernel* IdGenerator::kernel = NULL;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace util {

    class IdGeneratorKernel {
    public:

        std::string UNIQUE_STUB;
        int instanceCount;
        std::string hostname;
        mutable decaf::util::concurrent::Mutex mutex;

        IdGeneratorKernel() : UNIQUE_STUB(), instanceCount(0), hostname(), mutex() {

            std::string stub = "";

            try {
                hostname = InetAddress::getLocalHost().getHostName();
                ServerSocket ss(0);
                stub = "-" + Long::toString(ss.getLocalPort()) +
                       "-" + Long::toString(System::currentTimeMillis()) + "-";
                Thread::sleep(100);
                ss.close();
            } catch (Exception& ioe) {
                hostname = "localhost";
                stub = "-1-" + Long::toString(System::currentTimeMillis()) + "-";
            }

            UNIQUE_STUB = stub;
        }
    };
}}

////////////////////////////////////////////////////////////////////////////////
IdGenerator::IdGenerator() : prefix(), seed(), sequence(0) {
}

////////////////////////////////////////////////////////////////////////////////
IdGenerator::IdGenerator(const std::string& prefix) : prefix(prefix), seed(), sequence(0) {
}

////////////////////////////////////////////////////////////////////////////////
IdGenerator::~IdGenerator() {
}

////////////////////////////////////////////////////////////////////////////////
std::string IdGenerator::generateId() const {

    std::string result;

    if (IdGenerator::kernel == NULL) {
        throw RuntimeException(__FILE__, __LINE__, "Library is not initialized.");
    }

    synchronized( &( IdGenerator::kernel->mutex ) ) {

        if (seed.empty()) {

            if (prefix.empty()) {
                this->seed = std::string("ID:") + IdGenerator::kernel->hostname + IdGenerator::kernel->UNIQUE_STUB
                        + Long::toString(IdGenerator::kernel->instanceCount++) + ":";
            } else {
                this->seed = prefix + IdGenerator::kernel->UNIQUE_STUB + Long::toString(IdGenerator::kernel->instanceCount++) + ":";
            }
        }

        result = this->seed + Long::toString(this->sequence++);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::string IdGenerator::getSeedFromId(const std::string& id) {

    std::string result = id;

    if (!id.empty()) {
        std::size_t index = id.find_last_of(':');
        if (index != std::string::npos && (index + 1) < id.length()) {
            result = id.substr(0, index + 1);
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long IdGenerator::getSequenceFromId(const std::string& id) {

    long long result = -1;

    if (!id.empty()) {
        std::size_t index = id.find_last_of(':');
        if (index != std::string::npos && (index + 1) < id.length()) {
            std::string numStr = id.substr(index + 1, id.length());
            result = Long::parseLong(numStr);
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int IdGenerator::compare(const std::string& id1, const std::string& id2) {

    int result = -1;
    std::string seed1 = IdGenerator::getSeedFromId(id1);
    std::string seed2 = IdGenerator::getSeedFromId(id2);

    if (!seed1.empty() && !seed2.empty()) {

        result = StringUtils::compare(seed1.c_str(), seed2.c_str());

        if (result == 0) {
            long long count1 = IdGenerator::getSequenceFromId(id1);
            long long count2 = IdGenerator::getSequenceFromId(id2);
            result = (int) (count1 - count2);
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void IdGenerator::initialize() {
    IdGenerator::kernel = new IdGeneratorKernel();
}

////////////////////////////////////////////////////////////////////////////////
void IdGenerator::shutdown() {
    delete IdGenerator::kernel;
}
