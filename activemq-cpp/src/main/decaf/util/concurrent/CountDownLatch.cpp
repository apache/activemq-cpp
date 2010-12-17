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

#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
CountDownLatch::CountDownLatch( int count ) : count(count), mutex() {
}

////////////////////////////////////////////////////////////////////////////////
CountDownLatch::~CountDownLatch() {
    try {

        synchronized( &mutex ) {
            mutex.notifyAll();
        }
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatch::await() {

    try {

        synchronized( &mutex ) {
            if( count == 0 ){
                return;
            }

            mutex.wait();
        }
    }
    DECAF_CATCH_RETHROW( decaf::lang::exceptions::InterruptedException )
    DECAF_CATCH_RETHROW( decaf::lang::Exception )
    DECAF_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool CountDownLatch::await( long long timeOut ) {

    try {

        if( timeOut < 0 ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Timeout value cannot be less than zero." );
        }

        synchronized( &mutex ) {
            if( count == 0 ){
                return true;
            }

            mutex.wait( timeOut );

            return count == 0;
        }

        return true;
    }
    DECAF_CATCH_RETHROW( decaf::lang::exceptions::InterruptedException )
    DECAF_CATCH_RETHROW( decaf::lang::Exception )
    DECAF_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool CountDownLatch::await( long long timeout, const TimeUnit& unit ) {

    try{
        return this->await( unit.toMillis( timeout ) );
    }
    DECAF_CATCH_RETHROW( decaf::lang::exceptions::InterruptedException )
    DECAF_CATCH_RETHROW( decaf::lang::Exception )
    DECAF_CATCHALL_THROW( decaf::lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatch::countDown() {
    try {

        if( count == 0 ) {
            return;
        }

        synchronized( &mutex ) {
            count--;

            // Signal when done.
            if( count == 0 ){
                mutex.notifyAll();
            }
        }
    }
    DECAF_CATCHALL_NOTHROW()
}
