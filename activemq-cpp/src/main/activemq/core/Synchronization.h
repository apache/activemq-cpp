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

#ifndef _ACTIVEMQ_CORE_SYNCHRONIZATION_H_
#define _ACTIVEMQ_CORE_SYNCHRONIZATION_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>

namespace activemq {
namespace core {

    /**
     * Transacted Object Synchronization, used to sync the events of a Transaction
     * with the items in the Transaction.
     */
    class Synchronization {
    public:

        virtual ~Synchronization() {}

        virtual void beforeEnd() = 0;

        virtual void afterCommit() = 0;

        virtual void afterRollback() = 0;

    };

}}

#endif /* _ACTIVEMQ_CORE_SYNCHRONIZATION_H_ */
