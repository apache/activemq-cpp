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

#ifndef _ACTIVEMQ_UTIL_SERVICESUPPORT_H_
#define SERVICESUPPORT_H_

#include <activemq/util/Config.h>
#include <activemq/util/Service.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/CopyOnWriteArrayList.h>

namespace activemq {
namespace util {

    class ServiceListener;
    class ServiceStopper;

    /**
     * Provides a base class for Service implementations.
     *
     * @since 3.3.0
     */
    class AMQCPP_API ServiceSupport : public Service {
    private:

        decaf::util::concurrent::atomic::AtomicBoolean started;
        decaf::util::concurrent::atomic::AtomicBoolean stopping;
        decaf::util::concurrent::atomic::AtomicBoolean stopped;
        decaf::util::concurrent::CopyOnWriteArrayList<ServiceListener*> listemers;

    public:

        ServiceSupport(const ServiceSupport&);
        ServiceSupport& operator=(const ServiceSupport&);

    public:

        ServiceSupport();

        virtual ~ServiceSupport();

    public:

        /**
         * Safely shuts down a service.
         *
         * @param service
         *      The service to stop.
         */
        static void dispose(Service* service);

        /**
         * Starts the Service, notifying any registered listeners of the start
         * if it is successful.
         */
        void start();

        /**
         * Stops the Service.
         */
        void stop();

        /**
         * @return true if this service has been started
         */
        bool isStarted() const;

        /**
         * @return true if this service is in the process of closing
         */
        bool isStopping() const;

        /**
         * @return true if this service is closed
         */
        bool isStopped() const;

        /**
         * Adds the given listener to this Service's list of listeners, call retains
         * ownership of the pointer.
         */
        void addServiceListener(ServiceListener* listener);

        /**
         * Removes the given listener to this Service's list of listeners, call retains
         * ownership of the pointer.
         */
        void removeServiceListener(ServiceListener* llistener);

    protected:

        /**
         * Performs the actual stop operation on the service, ensuring that all
         * resources held are released, must be implemented in derived class.
         */
        virtual void doStop(ServiceStopper* stopper) = 0;

        /**
         * Performs the actual start operation on the service, acquiring all the
         * resources needed to run the service.  Must be implemented in derived class.
         */
        virtual void doStart() = 0;

    };

}}

#endif /* _ACTIVEMQ_UTIL_SERVICESUPPORT_H_ */
