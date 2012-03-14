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

#include "AbstractExecutorService.h"

#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
AbstractExecutorService::AbstractExecutorService() : ExecutorService() {
}

////////////////////////////////////////////////////////////////////////////////
AbstractExecutorService::~AbstractExecutorService() {
}

////////////////////////////////////////////////////////////////////////////////
void AbstractExecutorService::doSubmit(FutureType* future) {

    try {
        // Its supposed to be a RunnableFuture<?> be we should double check.
        Runnable* task = dynamic_cast<Runnable*>(future);
        if (task == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Could not cast FutureType to a Runnabke");
        }

        // Ensure that we tell the subclass it owns the Future.
        this->execute(task, true);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(RejectedExecutionException)
}
