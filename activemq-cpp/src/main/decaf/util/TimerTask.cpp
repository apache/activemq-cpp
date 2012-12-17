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

#include "TimerTask.h"

#include <decaf/util/concurrent/Concurrent.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
TimerTask::TimerTask() : lock(), fixedRate(false), cancelled(false), scheduledTime(0), when(0), period(-1) {
}

////////////////////////////////////////////////////////////////////////////////
bool TimerTask::cancel() {

    bool result = false;
    synchronized(&lock) {
        result = !cancelled && when > 0;
        cancelled = true;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long TimerTask::scheduledExecutionTime() const {

    long long result = 0;

    synchronized(&lock) {
        result = this->scheduledTime;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool TimerTask::isScheduled() const {

    bool result = false;

    synchronized(&lock) {
        result = when > 0 || scheduledTime > 0;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void TimerTask::setScheduledTime(long long time) {
    synchronized(&lock) {
        this->scheduledTime = time;
    }
}

////////////////////////////////////////////////////////////////////////////////
long long TimerTask::getWhen() const {

    long long result = 0;

    synchronized(&lock) {
        result = this->when;
    }

    return result;
}
