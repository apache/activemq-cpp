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

#include "ThreadLocalImpl.h"

#include <decaf/internal/util/concurrent/Threading.h>

#include <decaf/lang/exceptions/RuntimeException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
ThreadLocalImpl::ThreadLocalImpl() : tlsKey(0) {
    tlsKey = Threading::createThreadLocalSlot(this);

    if (tlsKey < 0) {
        throw RuntimeException(
            __FILE__, __LINE__, "Thread Local storage limit reached.");
    }
}

////////////////////////////////////////////////////////////////////////////////
ThreadLocalImpl::~ThreadLocalImpl() {
}

////////////////////////////////////////////////////////////////////////////////
void* ThreadLocalImpl::getRawValue() const {
    return Threading::getThreadLocalValue(this->tlsKey);
}

////////////////////////////////////////////////////////////////////////////////
void ThreadLocalImpl::setRawValue(void* value) {
    void* oldValue = this->getRawValue();
    Threading::setThreadLocalValue(this->tlsKey, value);

    if (oldValue != NULL) {
        doDelete(oldValue);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ThreadLocalImpl::removeAll() {
    Threading::destoryThreadLocalSlot(this->tlsKey);
}
