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

#ifndef _ACTIVEMQ_UTIL_USAGE_H_
#define _ACTIVEMQ_UTIL_USAGE_H_

#include <activemq/util/Config.h>

namespace activemq {
namespace util {

    class AMQCPP_API Usage {
    public:

        virtual ~Usage() {}

        /**
         * Waits forever for more space to be returned to this Usage Manager.
         */
        virtual void waitForSpace() = 0;

        /**
         * Waits for more space to be returned to this Usage Manager, times out
         * when the given time span in milliseconds elapses.
         * @param timeout The time to wait for more space.
         */
        virtual void waitForSpace( unsigned int timeout ) = 0;

        /**
         * Tries to increase the usage by value amount but blocks if this object is
         * currently full.
         * @param value Amount of usage in bytes to add.
         */
        virtual void enqueueUsage( unsigned long long value ) = 0;

        /**
         * Increases the usage by the value amount
         * @param value Amount of usage to add.
         */
        virtual void increaseUsage( unsigned long long value ) = 0;

        /**
         * Decreases the usage by the value amount.
         * @param value Amount of space to return to the pool
         */
        virtual void decreaseUsage( unsigned long long value ) = 0;

        /**
         * Returns true if this Usage instance is full, i.e. Usage >= 100%
         * @return true if Usage is at the Full point.
         */
        virtual bool isFull() const = 0;

    };

}}

#endif /* _ACTIVEMQ_UTIL_USAGE_H_ */
