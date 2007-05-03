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

using namespace activemq;
using namespace activemq::concurrent;

////////////////////////////////////////////////////////////////////////////////
CountDownLatch::CountDownLatch( int count )
{
    this->count = count;
}

////////////////////////////////////////////////////////////////////////////////
CountDownLatch::~CountDownLatch()
{
    try {

        synchronized( &mutex ) {
            mutex.notifyAll();
        }
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void CountDownLatch::await() throw ( cms::CMSException ) {

    try {

        synchronized( &mutex ) {
            if( count == 0 ){
                return;
            }

            mutex.wait();
        }
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool CountDownLatch::await( unsigned long timeOut ) throw ( cms::CMSException ) {
    try {

        synchronized( &mutex ) {
            if( count == 0 ){
                return true;
            }

            mutex.wait( timeOut );

            return count == 0;
        }

        return true;
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
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
    AMQ_CATCHALL_NOTHROW()
}
