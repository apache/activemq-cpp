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

#ifndef _ACTIVEMQ_TRANSPORT_FAILOVER_CLOSETRANSPORTSTASK_H_
#define _ACTIVEMQ_TRANSPORT_FAILOVER_CLOSETRANSPORTSTASK_H_

#include <activemq/util/Config.h>
#include <activemq/threads/CompositeTask.h>
#include <activemq/transport/Transport.h>

#include <decaf/util/LinkedList.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace transport {
namespace failover {

    using decaf::lang::Pointer;
    using decaf::util::LinkedList;

    class AMQCPP_API CloseTransportsTask: public activemq::threads::CompositeTask {
    private:

        mutable LinkedList< Pointer<Transport> > transports;

    public:

        CloseTransportsTask();

        virtual ~CloseTransportsTask();

        /**
         * Add a new Transport to close.
         */
        void add( const Pointer<Transport>& transport );

        /**
         * This Task is pending if there are transports in the Queue that need to be
         * closed.
         *
         * @return true if there is a transport in the queue that needs closed.
         */
        virtual bool isPending() const;

        /**
         * Return true until all transports have been closed and removed from the queue.
         */
        virtual bool iterate();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_FAILOVER_CLOSETRANSPORTSTASK_H_ */
