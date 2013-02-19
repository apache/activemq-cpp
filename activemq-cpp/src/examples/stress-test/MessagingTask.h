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

#ifndef _CMS_STRESS_MESSAGINGTASK_H_
#define _CMS_STRESS_MESSAGINGTASK_H_

#include <decaf/util/Config.h>

#include "Receiver.h"
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/TimeUnit.h>

namespace cms {
namespace stress {

    class MessagingTask:public decaf::lang::Runnable {
    private:

        Receiver* receiver;
        std::string message;

        static decaf::util::concurrent::ThreadPoolExecutor* threadPoolExecutor;

    public:

        MessagingTask(Receiver* receiver, const std::string& message);

        virtual ~MessagingTask();

        virtual void run();

        static void initializeThreads(int min, int max);
        static void terminateThreads();

        virtual void queue();

    };

}}

#endif /** _CMS_STRESS_MESSAGINGTASK_H_ */
