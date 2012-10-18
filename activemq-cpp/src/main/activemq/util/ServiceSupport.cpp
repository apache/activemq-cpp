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

#include "ServiceSupport.h"

#include <memory>
#include <activemq/util/ServiceListener.h>
#include <activemq/util/ServiceStopper.h>

#include <decaf/util/Iterator.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
ServiceSupport::ServiceSupport() : Service(), started(), stopping(), stopped(true), listeners() {
}

////////////////////////////////////////////////////////////////////////////////
ServiceSupport::~ServiceSupport() {
}

////////////////////////////////////////////////////////////////////////////////
void ServiceSupport::dispose(Service* service) {
    try {
        if (service != NULL) {
            service->stop();
        }
    } catch (Exception& e) {
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServiceSupport::start() {
    if (started.compareAndSet(false, true)) {

        bool success = false;
        try {
            this->doStart();
            success = true;
        } catch (...) {
            this->started.set(success);
        }

        this->stopping.set(false);
        this->stopped.set(!success);

        synchronized(&this->listeners) {
            std::auto_ptr<Iterator<ServiceListener*> > iter(this->listeners.iterator());
            while (iter->hasNext()) {
                iter->next()->started(this);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServiceSupport::stop() {
    if (this->stopped.compareAndSet(false, true)) {
        this->stopping.set(true);
        ServiceStopper stopper;
        try {
            this->doStop(&stopper);
        } catch (Exception& e) {
            stopper.onException(this, e);
        }

        this->stopped.set(true);
        this->started.set(false);
        this->stopping.set(false);

        synchronized(&this->listeners) {
            std::auto_ptr<Iterator<ServiceListener*> > iter(this->listeners.iterator());
            while (iter->hasNext()) {
                iter->next()->stopped(this);
            }
        }

        stopper.throwFirstException();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ServiceSupport::isStarted() const {
    return this->started.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ServiceSupport::isStopping() const {
    return this->stopping.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ServiceSupport::isStopped() const {
    return this->stopped.get();
}

////////////////////////////////////////////////////////////////////////////////
void ServiceSupport::addServiceListener(ServiceListener* listener) {
    if (listener != NULL) {
        synchronized(&this->listeners) {
            this->listeners.add(listener);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServiceSupport::removeServiceListener(ServiceListener* listener) {
    if (listener != NULL) {
        synchronized(&this->listeners) {
            this->listeners.remove(listener);
        }
    }
}
