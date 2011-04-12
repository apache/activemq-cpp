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

#include <decaf/internal/util/concurrent/MutexImpl.h>
#include <decaf/internal/util/concurrent/windows/MutexHandle.h>

#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/Thread.h>

#include <memory>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
MutexHandle* MutexImpl::create() {

    std::auto_ptr<MutexHandle> handle( new MutexHandle );

    try{
        InitializeCriticalSection( &handle->mutex );
    } catch(...) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create OS Mutex object." );
    }

    handle->lock_count = 0;
    handle->lock_owner = 0;

    return handle.release();
}

////////////////////////////////////////////////////////////////////////////////
void MutexImpl::destroy( decaf::util::concurrent::MutexHandle* handle ) {

    if( handle == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Handle to Mutex Object was Null." );
    }

    DeleteCriticalSection( &handle->mutex );

    delete handle;
}

////////////////////////////////////////////////////////////////////////////////
void MutexImpl::lock( MutexHandle* handle ) {

    long long threadId = Thread::getId();

    if( threadId == handle->lock_owner ) {
        handle->lock_count++;
    } else {

        try{
            EnterCriticalSection( &handle->mutex );
        } catch(...) {
            throw RuntimeException(
                __FILE__, __LINE__, "Failed to Lock OS Mutex" );
        }

        handle->lock_owner = threadId;
        handle->lock_count = 1;
    }

}

////////////////////////////////////////////////////////////////////////////////
bool MutexImpl::trylock( MutexHandle* handle ) {

    long long threadId = Thread::getId();

    if( threadId == handle->lock_owner ) {
        handle->lock_count++;
    } else {

        unsigned int result = 0;

        try{
            result = TryEnterCriticalSection( &handle->mutex );
        } catch(...) {
            throw RuntimeException(
                __FILE__, __LINE__, "Failed to Lock OS Mutex" );
        }

        if( result != 0 ) {
            handle->lock_owner = threadId;
            handle->lock_count = 1;
        } else {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void MutexImpl::unlock( MutexHandle* handle ) {

    if( handle->lock_owner == 0 ) {
        return;
    }

    if( handle->lock_owner != Thread::getId() ) {
        throw RuntimeException(
            __FILE__, __LINE__,
            "Unlock Failed, this thread is not the Lock Owner!" );
    }

    handle->lock_count--;

    if( handle->lock_count == 0 ) {
        handle->lock_owner = 0;

        try{
            LeaveCriticalSection( &handle->mutex );
        } catch(...) {
            throw RuntimeException(
                __FILE__, __LINE__, "Failed to Unlock OS Mutex" );
        }
    }
}
