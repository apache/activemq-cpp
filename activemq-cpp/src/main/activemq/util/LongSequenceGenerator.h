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

#ifndef _ACTIVEMQ_UTIL_LONGSEQUENCEGENERATOR_H_
#define _ACTIVEMQ_UTIL_LONGSEQUENCEGENERATOR_H_

#include <activemq/util/Config.h>
#include <decaf/util/concurrent/Mutex.h>

namespace activemq{
namespace util{

    /**
     * This class is used to generate a sequence of long long values that
     * are incremented each time a new value is requested.  This class is
     * thread safe so the ids can be requested in different threads safely.
     */
    class AMQCPP_API LongSequenceGenerator {
    private:

        long long lastSequenceId;
        decaf::util::concurrent::Mutex mutex;

    public:

        LongSequenceGenerator();
        virtual ~LongSequenceGenerator() {}

        /**
         * @returns the next id in the sequence.
         */
        long long getNextSequenceId();

        /**
         * @returns the last id that was generated.
         */
        long long getLastSequenceId();

    };

}}

#endif /*_ACTIVEMQ_UTIL_LONGSEQUENCEGENERATOR_H_*/
