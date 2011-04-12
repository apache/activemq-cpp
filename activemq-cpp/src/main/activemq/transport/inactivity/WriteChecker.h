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

#ifndef _ACTIVEMQ_TRANSPORT_INACTIVITY_WRITECHECKER_H_
#define _ACTIVEMQ_TRANSPORT_INACTIVITY_WRITECHECKER_H_

#include <activemq/util/Config.h>

#include <decaf/util/TimerTask.h>

namespace activemq {
namespace transport {
namespace inactivity {

    class InactivityMonitor;

    /**
     * Runnable class used by the {@see InactivityMonitor} to make periodic writes to the underlying
     * transport if no other write activity is going on in order to more quickly detect failures
     * of the connection to the broker.
     *
     * @since 3.1.0
     */
    class AMQCPP_API WriteChecker : public decaf::util::TimerTask {
    private:

        WriteChecker( const WriteChecker& );
        WriteChecker operator= ( const WriteChecker& );

    private:

        // The InactivityMonitor instance that created this object.
        InactivityMonitor* parent;

        // State variable tracking the last execution time of this Runnable.
        long long lastRunTime;

    public:

        WriteChecker( InactivityMonitor* parent );
        virtual ~WriteChecker();

        virtual void run();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_INACTIVITY_WRITECHECKER_H_ */
