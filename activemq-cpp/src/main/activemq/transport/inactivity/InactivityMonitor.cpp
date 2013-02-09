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

#include "InactivityMonitor.h"

#include "ReadChecker.h"
#include "WriteChecker.h"

#include <activemq/threads/CompositeTask.h>
#include <activemq/threads/CompositeTaskRunner.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/KeepAliveInfo.h>

#include <decaf/util/Timer.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/Boolean.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::threads;
using namespace activemq::transport;
using namespace activemq::transport::inactivity;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace transport {
namespace inactivity {

    class InactivityMonitorData {
    private:

        InactivityMonitorData(const InactivityMonitorData&);
        InactivityMonitorData operator=(const InactivityMonitorData&);

    public:

        // The configured WireFormat for the Transport Chain.
        Pointer<WireFormat> wireFormat;

        // Local and Remote WireFormat information.
        Pointer<WireFormatInfo> localWireFormatInfo;
        Pointer<WireFormatInfo> remoteWireFormatInfo;

        Pointer<ReadChecker> readCheckerTask;
        Pointer<WriteChecker> writeCheckerTask;

        Timer readCheckTimer;
        Timer writeCheckTimer;

        Pointer<CompositeTaskRunner> asyncTasks;

        Pointer<AsyncSignalReadErrorkTask> asyncReadTask;
        Pointer<AsyncWriteTask> asyncWriteTask;

        AtomicBoolean monitorStarted;

        AtomicBoolean commandSent;
        AtomicBoolean commandReceived;

        AtomicBoolean failed;
        AtomicBoolean inRead;
        AtomicBoolean inWrite;

        Mutex inWriteMutex;
        Mutex monitor;

        long long readCheckTime;
        long long writeCheckTime;
        long long initialDelayTime;

        bool keepAliveResponseRequired;

        InactivityMonitorData(const Pointer<WireFormat> wireFormat) :
            wireFormat(wireFormat),
            localWireFormatInfo(),
            remoteWireFormatInfo(),
            readCheckerTask(),
            writeCheckerTask(),
            readCheckTimer("InactivityMonitor Read Check Timer"),
            writeCheckTimer("InactivityMonitor Write Check Timer"),
            asyncTasks(),
            asyncReadTask(),
            asyncWriteTask(),
            monitorStarted(),
            commandSent(),
            commandReceived(true),
            failed(),
            inRead(),
            inWrite(),
            inWriteMutex(),
            monitor(),
            readCheckTime(0),
            writeCheckTime(0),
            initialDelayTime(0),
            keepAliveResponseRequired(false) {
        }
    };

    // Task that fires when the TaskRunner is signaled by the ReadCheck Timer Task.
    class AsyncSignalReadErrorkTask : public CompositeTask {
    private:

        InactivityMonitor* parent;
        std::string remote;
        AtomicBoolean failed;

    private:

        AsyncSignalReadErrorkTask(const AsyncSignalReadErrorkTask&);
        AsyncSignalReadErrorkTask operator=(const AsyncSignalReadErrorkTask&);

    public:

        AsyncSignalReadErrorkTask(InactivityMonitor* parent, const std::string& remote) :
            parent(parent), remote(remote), failed() {
        }

        void setFailed(bool failed) {
            this->failed.set(failed);
        }

        virtual bool isPending() const {
            return this->failed.get();
        }

        virtual bool iterate() {

            if (this->failed.compareAndSet(true, false)) {
                IOException ex(__FILE__, __LINE__,
                    (std::string("Channel was inactive for too long: ") + remote).c_str());
                this->parent->onException(ex);
            }

            return this->failed.get();
        }
    };

    // Task that fires when the TaskRunner is signaled by the WriteCheck Timer Task.
    class AsyncWriteTask : public CompositeTask {
    private:

        InactivityMonitor* parent;
        AtomicBoolean write;

    private:

        AsyncWriteTask(const AsyncWriteTask&);
        AsyncWriteTask operator=(const AsyncWriteTask&);

    public:

        AsyncWriteTask(InactivityMonitor* parent) : parent(parent), write() {}

        void setWrite( bool write ) {
            this->write.set( write );
        }

        virtual bool isPending() const {
            return this->write.get();
        }

        virtual bool iterate() {

            if (this->write.compareAndSet(true, false) && this->parent->members->monitorStarted.get()) {
                try {
                    Pointer<KeepAliveInfo> info(new KeepAliveInfo());
                    info->setResponseRequired(this->parent->members->keepAliveResponseRequired);
                    this->parent->oneway(info);
                } catch (IOException& e) {
                    this->parent->onException(e);
                }
            }

            return this->write.get();
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::InactivityMonitor(const Pointer<Transport> next, const Pointer<WireFormat> wireFormat) :
    TransportFilter(next), members(new InactivityMonitorData(wireFormat)) {
}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::InactivityMonitor(const Pointer<Transport> next, const decaf::util::Properties& properties, const Pointer<wireformat::WireFormat> wireFormat) :
    TransportFilter(next), members(new InactivityMonitorData(wireFormat)) {

    this->members->keepAliveResponseRequired = Boolean::parseBoolean(properties.getProperty("keepAliveResponseRequired", "false"));
}

////////////////////////////////////////////////////////////////////////////////
InactivityMonitor::~InactivityMonitor() {
    try {
        this->stopMonitorThreads();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete this->members;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
long long InactivityMonitor::getReadCheckTime() const {
    return this->members->readCheckTime;
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::setReadCheckTime(long long value) {
    this->members->readCheckTime = value;
}

////////////////////////////////////////////////////////////////////////////////
long long InactivityMonitor::getWriteCheckTime() const {
    return this->members->writeCheckTime;
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::setWriteCheckTime(long long value) {
    this->members->writeCheckTime = value;
}

////////////////////////////////////////////////////////////////////////////////
long long InactivityMonitor::getInitialDelayTime() const {
    return this->members->initialDelayTime;
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::setInitialDelayTime(long long value) const {
    this->members->initialDelayTime = value;
}

////////////////////////////////////////////////////////////////////////////////
bool InactivityMonitor::isKeepAliveResponseRequired() const {
    return this->members->keepAliveResponseRequired;
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::setKeepAliveResponseRequired(bool value) {
    this->members->keepAliveResponseRequired = value;
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::afterNextIsStarted() {
    try {
        startMonitorThreads();
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::beforeNextIsStopped() {
    try {
        stopMonitorThreads();
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::doClose() {
    try {
        stopMonitorThreads();
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::onException(const decaf::lang::Exception& ex) {

    if (this->members->failed.compareAndSet(false, true)) {
        stopMonitorThreads();
        TransportFilter::onException(ex);
    }
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::onCommand(const Pointer<Command> command) {

    this->members->commandReceived.set(true);
    this->members->inRead.set(true);

    try {

        if (command->isWireFormatInfo()) {
            synchronized(&this->members->monitor) {

                this->members->remoteWireFormatInfo = command.dynamicCast<WireFormatInfo>();
                try {
                    startMonitorThreads();
                } catch (IOException& e) {
                    onException(e);
                }
            }
        }

        TransportFilter::onCommand(command);

        this->members->inRead.set(false);

    } catch (Exception& ex) {
        this->members->inRead.set(false);
        ex.setMark(__FILE__, __LINE__);
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::oneway(const Pointer<Command> command) {

    try {
        // Disable inactivity monitoring while processing a command. Synchronize this
        // method - its not synchronized further down the transport stack and gets called
        // by more than one thread  by this class
        synchronized(&this->members->inWriteMutex) {
            this->members->inWrite.set(true);
            try {

                if (this->members->failed.get()) {
                    throw IOException(__FILE__, __LINE__,
                        (std::string("Channel was inactive for too long: ") + next->getRemoteAddress()).c_str());
                }

                if (command->isWireFormatInfo()) {
                    synchronized( &this->members->monitor ) {
                        this->members->localWireFormatInfo = command.dynamicCast<WireFormatInfo>();
                        startMonitorThreads();
                    }
                }

                this->next->oneway(command);

                this->members->commandSent.set(true);
                this->members->inWrite.set(false);

            } catch (Exception& ex) {
                this->members->commandSent.set(true);
                this->members->inWrite.set(false);
                ex.setMark(__FILE__, __LINE__);
                throw;
            }
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
bool InactivityMonitor::allowReadCheck(long long elapsed) {
    return elapsed > (this->members->readCheckTime * 9 / 10);
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::readCheck() {

    if (this->members->inRead.get() || this->members->wireFormat->inReceive()) {
        return;
    }

    if (!this->members->commandReceived.get()) {
        // Set the failed state on our async Read Failure Task and wakeup its runner.
        this->members->asyncReadTask->setFailed(true);
        this->members->asyncTasks->wakeup();
    }

    this->members->commandReceived.set(false);
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::writeCheck() {

    if (this->members->inWrite.get()) {
        return;
    }

    if (!this->members->commandSent.get()) {

        this->members->asyncWriteTask->setWrite(true);
        this->members->asyncTasks->wakeup();
    }

    this->members->commandSent.set(false);
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::startMonitorThreads() {

    if (this->members->monitorStarted.get()) {
        return;
    }

    if (this->members->localWireFormatInfo == NULL) {
        return;
    }

    if (this->members->remoteWireFormatInfo == NULL) {
        return;
    }

    synchronized( &this->members->monitor ) {

        this->members->asyncTasks.reset(new CompositeTaskRunner());
        this->members->asyncReadTask.reset(new AsyncSignalReadErrorkTask(this, this->getRemoteAddress()));
        this->members->asyncWriteTask.reset(new AsyncWriteTask(this));

        this->members->asyncTasks->addTask(this->members->asyncReadTask.get());
        this->members->asyncTasks->addTask(this->members->asyncWriteTask.get());
        this->members->asyncTasks->start();

        this->members->readCheckTime = Math::min(this->members->localWireFormatInfo->getMaxInactivityDuration(),
                this->members->remoteWireFormatInfo->getMaxInactivityDuration());

        this->members->initialDelayTime = Math::min(this->members->localWireFormatInfo->getMaxInactivityDurationInitalDelay(),
                this->members->remoteWireFormatInfo->getMaxInactivityDurationInitalDelay());

        if (this->members->readCheckTime > 0) {

            this->members->monitorStarted.set(true);
            this->members->writeCheckerTask.reset(new WriteChecker(this));
            this->members->readCheckerTask.reset(new ReadChecker(this));
            this->members->writeCheckTime = this->members->readCheckTime > 3 ? this->members->readCheckTime / 3 : this->members->readCheckTime;

            this->members->writeCheckTimer.scheduleAtFixedRate(this->members->writeCheckerTask, this->members->initialDelayTime, this->members->writeCheckTime);
            this->members->readCheckTimer.scheduleAtFixedRate(this->members->readCheckerTask, this->members->initialDelayTime, this->members->readCheckTime);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InactivityMonitor::stopMonitorThreads() {

    if (this->members->monitorStarted.compareAndSet(true, false)) {

        synchronized(&this->members->monitor) {

            this->members->readCheckerTask->cancel();
            this->members->writeCheckerTask->cancel();

            this->members->readCheckTimer.purge();
            this->members->readCheckTimer.cancel();
            this->members->writeCheckTimer.purge();
            this->members->writeCheckTimer.cancel();

            this->members->asyncTasks->shutdown();
        }
    }
}
