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

#ifndef _DECAF_CONCURRENT_COUNTDOWNLATCH_H_
#define _DECAF_CONCURRENT_COUNTDOWNLATCH_H_

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>
#include <decaf/lang/Exception.h>

namespace decaf{
namespace util{
namespace concurrent{

    class DECAF_API CountDownLatch {
    private:

        /**
         * number to count down to
         */
        int count;

        /**
         * Mutex to protect the counts, and wait on.
         */
        Mutex mutex;

    public:

        /**
         * Constructor
         * @param count - number to count down from.
         */
        CountDownLatch( int count );

        virtual ~CountDownLatch();

        /**
         * Waits for the Count to be zero, and then returns
         * @throws CMSException
         */
        virtual void await() throw ( lang::Exception );

        /**
         * Waits for the Count to hit zero, or a timeout.
         * @param timeOut - time in milliseconds to wait.
         * @returns true if the wait made it to count zero, otherwise false
         */
        virtual bool await( unsigned long timeOut ) throw ( lang::Exception );

        /**
         * Counts down the latch, releasing all waiting threads when
         * the count hits zero.
         */
        virtual void countDown();

        /**
         * Gets the current count
         * @returns int count value
         */
        virtual int getCount() const {
            return this->count;
        }

    };

}}}

#endif /*_DECAF_CONCURRENT_COUNTDOWNLATCH_H_*/
