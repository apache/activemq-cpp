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

#include "BackupTransportPool.h"

#include <memory>

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/transport/failover/FailoverTransport.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace transport {
namespace failover {

    class BackupTransportPoolImpl {
    private:

        BackupTransportPoolImpl(const BackupTransportPoolImpl&);
        BackupTransportPoolImpl& operator= (const BackupTransportPoolImpl&);

    public:

        LinkedList< Pointer<BackupTransport> > backups;
        volatile bool pending;
        volatile bool closed;
        volatile int priorityBackups;

        BackupTransportPoolImpl() : backups(), pending(false), closed(false), priorityBackups(0) {
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
BackupTransportPool::BackupTransportPool(FailoverTransport* parent,
                                         const Pointer<CompositeTaskRunner> taskRunner,
                                         const Pointer<CloseTransportsTask> closeTask,
                                         const Pointer<URIPool> uriPool,
                                         const Pointer<URIPool> updates,
                                         const Pointer<URIPool> priorityUriPool) : impl(NULL),
                                                                                   parent(parent),
                                                                                   taskRunner(taskRunner),
                                                                                   closeTask(closeTask),
                                                                                   uriPool(uriPool),
                                                                                   updates(updates),
                                                                                   priorityUriPool(priorityUriPool),
                                                                                   backupPoolSize(1),
                                                                                   enabled(false) {

    if (parent == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Parent transport passed is NULL");
    }

    if (taskRunner == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "TaskRunner passed is NULL");
    }

    if (uriPool == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "URIPool passed is NULL");
    }

    if (priorityUriPool == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Piroirty URIPool passed is NULL");
    }

    if (closeTask == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Close Transport Task passed is NULL");
    }

    this->impl = new BackupTransportPoolImpl();

    // Add this instance as a Task so that we can create backups when nothing else is
    // going on.
    this->taskRunner->addTask(this);
}

////////////////////////////////////////////////////////////////////////////////
BackupTransportPool::BackupTransportPool(FailoverTransport* parent,
                                         int backupPoolSize,
                                         const Pointer<CompositeTaskRunner> taskRunner,
                                         const Pointer<CloseTransportsTask> closeTask,
                                         const Pointer<URIPool> uriPool,
                                         const Pointer<URIPool> updates,
                                         const Pointer<URIPool> priorityUriPool) : impl(NULL),
                                                                                   parent(parent),
                                                                                   taskRunner(taskRunner),
                                                                                   closeTask(closeTask),
                                                                                   uriPool(uriPool),
                                                                                   updates(updates),
                                                                                   priorityUriPool(priorityUriPool),
                                                                                   backupPoolSize(backupPoolSize),
                                                                                   enabled(false) {

    if (parent == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Parent transport passed is NULL");
    }

    if (taskRunner == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "TaskRunner passed is NULL");
    }

    if (uriPool == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "URIPool passed is NULL");
    }

    if (priorityUriPool == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Piroirty URIPool passed is NULL");
    }

    if (closeTask == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Close Transport Task passed is NULL");
    }

    this->impl = new BackupTransportPoolImpl();

    // Add this instance as a Task so that we can create backups when nothing else is
    // going on.
    this->taskRunner->addTask(this);
}

////////////////////////////////////////////////////////////////////////////////
BackupTransportPool::~BackupTransportPool() {
    this->taskRunner->removeTask(this);

    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void BackupTransportPool::close() {

    if (this->impl->closed) {
        return;
    }

    synchronized(&this->impl->backups) {
        this->enabled = false;
        this->impl->closed = true;
        this->impl->backups.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
void BackupTransportPool::setEnabled(bool value) {

    if (this->impl->closed) {
        return;
    }

    this->enabled = value;

    if (enabled == true) {
        this->taskRunner->wakeup();
    } else {
        synchronized(&this->impl->backups) {
            this->impl->backups.clear();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<BackupTransport> BackupTransportPool::getBackup() {

    if (!isEnabled()) {
        throw IllegalStateException(__FILE__, __LINE__, "The Backup Pool is not enabled.");
    }

    Pointer<BackupTransport> result;

    synchronized(&this->impl->backups) {
        if (!this->impl->backups.isEmpty()) {
            result = this->impl->backups.removeAt(0);
        }
    }

    // Flag as pending so the task gets run again and new backups are created.
    this->impl->pending = true;
    this->taskRunner->wakeup();

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool BackupTransportPool::isPending() const {

    if (this->isEnabled()) {
        return this->impl->pending;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool BackupTransportPool::iterate() {

    LinkedList<URI> failures;

    synchronized(&this->impl->backups) {

        Pointer<URIPool> uriPool = this->uriPool;

        // We prefer the Broker updated URIs list if it has any URIs.
        if (!updates->isEmpty()) {
            uriPool = updates;
        }

        bool wakeupParent = false;

        while (isEnabled() && (int) this->impl->backups.size() < backupPoolSize) {

            URI connectTo;

            // Try for a URI, if one isn't free return and indicate this task
            // is done for now, the next time a backup is requested this task
            // will become pending again and we will attempt to fill the pool.
            // This will break the loop once we've tried all possible UIRs.
            try {
                connectTo = uriPool->getURI();
            } catch (NoSuchElementException& ex) {
                break;
            }

            Pointer<BackupTransport> backup(new BackupTransport(this));
            backup->setUri(connectTo);

            if (priorityUriPool->contains(connectTo)) {
                backup->setPriority(true);

                if (!parent->isConnectedToPriority()) {
                    wakeupParent = true;
                }
            }

            try {
                Pointer<Transport> transport = createTransport(connectTo);

                transport->setTransportListener(backup.get());
                transport->start();
                backup->setTransport(transport);

                // Put any priority connections first so a reconnect picks them
                // up automatically.
                if (backup->isPriority()) {
                    this->impl->priorityBackups++;
                    this->impl->backups.addFirst(backup);
                } else {
                    this->impl->backups.addLast(backup);
                }
            } catch (...) {
                // Store it in the list of URIs that didn't work, once done we
                // return those to the pool.
                failures.add(connectTo);
            }

            // We connected to a priority backup and the parent isn't already using one
            // so wake it up and quick the backups process for now.
            if (wakeupParent) {
                this->parent->reconnect(true);
                break;
            }
        }

        // return all failures to the URI Pool, we can try again later.
        uriPool->addURIs(failures);
        this->impl->pending = false;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void BackupTransportPool::onBackupTransportFailure(BackupTransport* failedTransport) {

    synchronized(&this->impl->backups) {

        std::auto_ptr<Iterator<Pointer<BackupTransport> > > iter(this->impl->backups.iterator());

        while (iter->hasNext()) {
            if (iter->next() == failedTransport) {
                iter->remove();
            }

            if (failedTransport->isPriority() && this->impl->priorityBackups > 0) {
                this->impl->priorityBackups--;
            }

            this->uriPool->addURI(failedTransport->getUri());
            this->closeTask->add(failedTransport->getTransport());
            this->taskRunner->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool BackupTransportPool::isPriorityBackupAvailable() const {
    bool result = false;
    synchronized(&this->impl->backups) {
        result = this->impl->priorityBackups > 0;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> BackupTransportPool::createTransport(const URI& location) const {

    try {

        TransportFactory* factory = TransportRegistry::getInstance().findFactory(location.getScheme());

        if (factory == NULL) {
            throw new IOException(__FILE__, __LINE__, "Invalid URI specified, no valid Factory Found.");
        }

        Pointer<Transport> transport(factory->createComposite(location));

        return transport;
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}
