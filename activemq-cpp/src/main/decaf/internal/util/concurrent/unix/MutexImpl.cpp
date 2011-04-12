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
#include <decaf/internal/util/concurrent/unix/MutexHandle.h>

#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/Thread.h>

#include <list>
#include <memory>

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif

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

    if( pthread_mutex_init( &( handle->mutex ), NULL ) != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create Mutex object." );
    }

    handle->lock_count = 0;
    handle->lock_owner = 0;

    return handle.release();
}

////////////////////////////////////////////////////////////////////////////////
void MutexImpl::destroy( decaf::util::concurrent::MutexHandle* handle ) {

    if( handle == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Handle to Mutex was NULL." );
    }

    pthread_mutex_destroy( &handle->mutex );
    delete handle;
}

////////////////////////////////////////////////////////////////////////////////
void MutexImpl::lock( MutexHandle* handle ) {

    long long threadId = Thread::getId();

    if( threadId == handle->lock_owner ) {
        handle->lock_count++;
    } else {

        if( pthread_mutex_lock( &( handle->mutex ) ) != 0 ) {
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

        if( pthread_mutex_trylock( &( handle->mutex ) ) == 0 ) {
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
        pthread_mutex_unlock( &( handle->mutex ) );
    }
}
