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

#ifndef _ACTIVEMQ_TRANSPORT_INACTIVITY_READCHECKER_H_
#define _ACTIVEMQ_TRANSPORT_INACTIVITY_READCHECKER_H_

#include <activemq/util/Config.h>

#include <decaf/util/TimerTask.h>

namespace activemq {
namespace transport {
namespace inactivity {

    class InactivityMonitor;

    /**
     * Runnable class that is used by the {@see InactivityMonitor} class the check for
     * timeouts related to transport reads.
     *
     * @since 3.1
     */
    class AMQCPP_API ReadChecker : public decaf::util::TimerTask {
    private:

        ReadChecker( const ReadChecker& );
        ReadChecker operator= ( const ReadChecker& );

    private:

        // The Inactivity Monitor that created this Read Checker.
        InactivityMonitor* parent;

        // State value of last time this object was run
        long long lastRunTime;

    public:

        ReadChecker( InactivityMonitor* parent );
        virtual ~ReadChecker();

        virtual void run();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_INACTIVITY_READCHECKER_H_ */
