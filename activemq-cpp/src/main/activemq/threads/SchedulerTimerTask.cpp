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

#include "SchedulerTimerTask.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
SchedulerTimerTask::SchedulerTimerTask(Runnable* task, bool ownsTask) :
    task(task), ownsTask(ownsTask) {

    if (task == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Assigned Task cannot be NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
SchedulerTimerTask::~SchedulerTimerTask() {

    try {
        if (ownsTask) {
            delete this->task;
        }
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void SchedulerTimerTask::run() {
    if (this->task != NULL) {
        this->task->run();
    }
}
