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

#include "WriteChecker.h"

#include <activemq/transport/inactivity/InactivityMonitor.h>

#include <decaf/lang/System.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::inactivity;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
WriteChecker::WriteChecker(InactivityMonitor* parent) : TimerTask(), parent(parent), lastRunTime(0) {

    if (this->parent == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "WriteChecker created with NULL parent.");
    }
}

////////////////////////////////////////////////////////////////////////////////
WriteChecker::~WriteChecker() {}

////////////////////////////////////////////////////////////////////////////////
void WriteChecker::run() {
    this->lastRunTime = System::currentTimeMillis();
    this->parent->writeCheck();
}
