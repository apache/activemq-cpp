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

#ifndef _ACTIVEMQ_UTIL_MEMORYUSAGE_H_
#define _ACTIVEMQ_UTIL_MEMORYUSAGE_H_

#include <activemq/util/Config.h>
#include <activemq/util/Usage.h>
#include <decaf/util/concurrent/Mutex.h>

namespace activemq {
namespace util {

    class AMQCPP_API MemoryUsage : public Usage {
    private:

        // The physical limit of memory usage this object allows.
        unsigned long long limit;

        // Amount of memory currently used in.
        unsigned long long usage;

        // Mutex to lock usage and wait on.
        mutable decaf::util::concurrent::Mutex mutex;

    public:

        /**
         * Default Constructor.
         */
        MemoryUsage();

        /**
         * Creates an instance of an Usage monitor with a set limit.
         * @param limit - amount of memory this manager allows.
         */
        MemoryUsage( unsigned long long limit );

        virtual ~MemoryUsage();

        /**
         * Waits forever for more space to be returned to this Usage Manager.
         */
        virtual void waitForSpace();

        /**
         * Waits for more space to be returned to this Usage Manager, times out
         * when the given time span in milliseconds elapses.
         * @param timeout The time to wait for more space.
         */
        virtual void waitForSpace( unsigned int timeout );

        /**
         * Tries to increase the usage by value amount but blocks if this object is
         * currently full.
         * @param value Amount of usage in bytes to add.
         */
        virtual void enqueueUsage( unsigned long long value ) {
            waitForSpace();
            increaseUsage(value);
        }

        /**
         * Increases the usage by the value amount
         * @param value Amount of usage to add.
         */
        virtual void increaseUsage( unsigned long long value );

        /**
         * Decreases the usage by the value amount.
         * @param value Amount of space to return to the pool
         */
        virtual void decreaseUsage( unsigned long long value );

        /**
         * Returns true if this Usage instance is full, i.e. Usage >= 100%
         */
        virtual bool isFull() const;

        /**
         * Gets the current usage amount.
         * @return the amount of bytes currently used.
         */
        unsigned long long getUsage() const {
            return usage;
        }

        /**
         * Sets the current usage amount
         * @param usage - The amount to tag as used.
         */
        void setUsage( unsigned long long usage ) {
            this->usage = usage;
        }

        /**
         * Gets the current limit amount.
         * @return the amount that can be used before full.
         */
        unsigned long long getLimit() const {
            return limit;
        }

        /**
         * Sets the current limit amount
         * @param limit - The amount that can be used before full.
         */
        void setLimit( unsigned long long limit ) {
            this->limit = limit;
        }

    };

}}

#endif /*_ACTIVEMQ_UTIL_MEMORYUSAGE_H_*/
