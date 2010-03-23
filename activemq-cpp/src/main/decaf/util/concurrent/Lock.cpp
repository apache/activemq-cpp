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

#include "Lock.h"

#include <decaf/lang/Exception.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Lock::Lock( Synchronizable* object, const bool intiallyLocked ) : locked( false ), syncObject( object ) {

    try{
        if( intiallyLocked ) {
            lock();
        }
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
Lock::~Lock() {

    try{
        if( locked ) {
            syncObject->unlock();
        }
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Lock::lock() {
    try{
        syncObject->lock();
        locked = true;
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}

////////////////////////////////////////////////////////////////////////////////
void Lock::unlock() {

    try{
         if( locked ) {
             syncObject->unlock();
             locked = false;
         }
    }
    DECAF_CATCH_RETHROW( Exception )
    DECAF_CATCHALL_THROW( Exception )
}
