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

#include "Threading.h"

#include <decaf/lang/Thread.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/concurrent/Executors.h>

#include <decaf/internal/util/concurrent/ThreadLocalImpl.h>
#include <decaf/internal/util/concurrent/ThreadingTypes.h>
#include <decaf/internal/util/concurrent/PlatformThread.h>
#include <decaf/internal/util/concurrent/Atomics.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>

#include <vector>
#include <list>

#ifdef __SUNPRO_CC
#include <stdlib.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class SuspendedCompletionCondition : public CompletionCondition {
    private:

        ThreadHandle* thread;

    private:

        SuspendedCompletionCondition(const SuspendedCompletionCondition&);
        SuspendedCompletionCondition& operator= (const SuspendedCompletionCondition&);

    public:

        SuspendedCompletionCondition(ThreadHandle* thread) : thread(thread) {}

        bool operator()() {
            return !thread->suspended;
        }
    };

    class MonitorWaitCompletionCondition : public CompletionCondition {
    private:

        ThreadHandle* handle;

    private:

        MonitorWaitCompletionCondition(const MonitorWaitCompletionCondition&);
        MonitorWaitCompletionCondition& operator= (const MonitorWaitCompletionCondition&);

    public:

        MonitorWaitCompletionCondition(ThreadHandle* handle) : handle(handle) {}

        virtual bool operator()(bool timedOut) {

            PlatformThread::lockMutex(handle->mutex);

            if (handle->notified || (handle->interruptible && handle->interrupted)) {
                return true;
            }

            if (!timedOut) {
                // Not timed out and not complete so unlock the thread so others can
                // notify or interrupt it.
                PlatformThread::unlockMutex(handle->mutex);
            }

            return false;
        }

        virtual bool operator()() {

            PlatformThread::lockMutex(handle->mutex);

            if (handle->notified || (handle->interruptible && handle->interrupted)) {
                return true;
            }

            PlatformThread::unlockMutex(handle->mutex);
            return false;
        }
    };

    struct MonitorPool {
        MonitorHandle* head;
        unsigned int count;
    };

    struct ThreadingLibrary {
    private:

        ThreadingLibrary(const ThreadingLibrary&);
        ThreadingLibrary& operator= (const ThreadingLibrary&);

    public:

        ThreadingLibrary() : threadKey(),
                             selfKey(),
                             globalLock(),
                             tlsLock(),
                             tlsSlots(),
                             osThreads(),
                             mainThread(),
                             activeThreads(),
                             priorityMapping(),
                             osThreadId(),
                             monitors() {
        }

        decaf_tls_key threadKey;
        decaf_tls_key selfKey;
        decaf_mutex_t globalLock;
        decaf_mutex_t tlsLock;
        std::vector<ThreadLocalImpl*> tlsSlots;
        std::vector<Thread*> osThreads;
        decaf_thread_t mainThread;
        std::list<ThreadHandle*> activeThreads;
        std::vector<int> priorityMapping;
        AtomicInteger osThreadId;
        MonitorPool* monitors;
    };

    #define MONITOR_POOL_BLOCK_SIZE 64

    ThreadingLibrary* library = NULL;

    // ------------------------ Forward Declare All Utility Methds ----------------------- //
    void threadExitTlsCleanup(ThreadHandle* thread);
    void unblockThreads(ThreadHandle* monitor);
    void createThreadInstance(ThreadHandle* thread, long long stackSize, int priority,
                              bool suspended, threadingTask threadMain, void* threadArg);
    void dereferenceThread(ThreadHandle* thread);
    ThreadHandle* initThreadHandle(ThreadHandle* thread);
    MonitorHandle* initMonitorHandle(MonitorHandle* monitor);
    bool interruptWaitingThread(ThreadHandle* self, ThreadHandle* target);
    void unblockThreads(ThreadHandle* queueHead);
    void enqueueThread(ThreadHandle** queue, ThreadHandle* thread);
    void dequeueThread(ThreadHandle** queue, ThreadHandle* thread);
    unsigned int getNumberOfWaiters(MonitorHandle* monitor);
    void purgeMonitorsPool(MonitorPool* pool);
    MonitorHandle* batchAllocateMonitors();
    void doMonitorExit(MonitorHandle* monitor, ThreadHandle* thread);
    void doMonitorEnter(MonitorHandle* monitor, ThreadHandle* thread);
    void doNotifyWaiters(MonitorHandle* monitor, bool notifyAll);
    void doNotifyThread(ThreadHandle* thread, bool markAsNotified);
    bool doWaitOnMonitor(MonitorHandle* monitor, ThreadHandle* thread, long long mills, int nanos, bool interruptible);
    // ------------------------ Forward Declare All Utility Methds ----------------------- //

    void threadExit(ThreadHandle* self, bool destroy = false) {

        PlatformThread::lockMutex(library->globalLock);
        PlatformThread::lockMutex(self->mutex);

        self->state = Thread::TERMINATED;

        // Must ensure that any interrupting threads get released
        if (self->interruptingThread) {
            PlatformThread::lockMutex(self->interruptingThread->mutex);
            self->interruptingThread->canceled = true;
            PlatformThread::unlockMutex(self->interruptingThread->mutex);
            self->interruptingThread = NULL;
        }

        decaf_thread_t handle = self->handle;

        // Wake up any blocked threads
        PlatformThread::notifyAll(self->condition);
        unblockThreads(self->joiners);

        PlatformThread::setTlsValue(library->threadKey, NULL);
        PlatformThread::setTlsValue(library->selfKey, NULL);

        // Ensure all of this thread's local values are purged.
        threadExitTlsCleanup(self);

        // Remove from the set of active threads under global lock, threads that
        // are iterating on global state need a stable list.
        library->activeThreads.remove(self);

        PlatformThread::unlockMutex(self->mutex);
        PlatformThread::unlockMutex(library->globalLock);

        if (destroy == true) {
            free(self->name);
            PlatformThread::destroyMutex(self->mutex);
            PlatformThread::destroyCondition(self->condition);
            delete self;
        } else {
            dereferenceThread(self);
        }

        PlatformThread::detachThread(handle);
        PlatformThread::exitThread();
    }

    PLATFORM_THREAD_CALLBACK_TYPE PLATFORM_CALLING_CONV threadEntryMethod(PLATFORM_THREAD_ENTRY_ARG arg) {

        ThreadHandle* thread = (ThreadHandle*)arg;

        PlatformThread::setTlsValue(library->threadKey, thread->parent);
        PlatformThread::setTlsValue(library->selfKey, thread);

        PlatformThread::lockMutex(thread->mutex);

        if (thread->suspended == true) {
            SuspendedCompletionCondition completion(thread);
            PlatformThread::interruptibleWaitOnCondition(thread->condition, thread->mutex, completion);
        }

        PlatformThread::unlockMutex(thread->mutex);

        if (thread->canceled == true) {
            threadExit(thread);
            PLATFORM_THREAD_RETURN()
        }

        PlatformThread::lockMutex(library->globalLock);
        library->activeThreads.push_back(thread);
        PlatformThread::unlockMutex(library->globalLock);

        thread->state = Thread::RUNNABLE;

        thread->threadMain(thread->threadArg);

        threadExit(thread);
        PLATFORM_THREAD_RETURN()
    }

    void runCallback(void* arg) {

        ThreadHandle* thread = (ThreadHandle*)arg;

        // Invoke run on the task.
        try {
            thread->parent->run();
        } catch( decaf::lang::Throwable& error ){

            if (thread->parent->getUncaughtExceptionHandler() != NULL) {
                thread->parent->getUncaughtExceptionHandler()->uncaughtException(thread->parent, error);
            } else if (thread->parent->getDefaultUncaughtExceptionHandler() != NULL) {
                thread->parent->getDefaultUncaughtExceptionHandler()->uncaughtException(thread->parent, error);
            }

        } catch(std::exception& stdEx) {

            const RuntimeException error(__FILE__, __LINE__, stdEx.what());

            if (thread->parent->getUncaughtExceptionHandler() != NULL) {
                thread->parent->getUncaughtExceptionHandler()->uncaughtException(thread->parent, error);
            } else if (thread->parent->getDefaultUncaughtExceptionHandler() != NULL) {
                thread->parent->getDefaultUncaughtExceptionHandler()->uncaughtException(thread->parent, error);
            }

        } catch(...) {

            const RuntimeException error(__FILE__, __LINE__,
                                         "Uncaught exception bubbled up to Thread::run, Thread Terminating.");

            if (thread->parent->getUncaughtExceptionHandler() != NULL) {
                thread->parent->getUncaughtExceptionHandler()->uncaughtException(thread->parent, error);
            } else if (thread->parent->getDefaultUncaughtExceptionHandler() != NULL) {
                thread->parent->getDefaultUncaughtExceptionHandler()->uncaughtException(thread->parent, error);
            }
        }
    }

    void interruptionThread(void *arg) {

        ThreadHandle* self = Threading::getCurrentThreadHandle();
        ThreadHandle* target = (ThreadHandle*)arg;
        MonitorHandle* monitor = NULL;

        PlatformThread::lockMutex(library->globalLock);

        // If the target was already canceled then we are done.
        if (self->canceled == true) {
            PlatformThread::unlockMutex(library->globalLock);
            threadExit(self, true);
        }

        PlatformThread::lockMutex(target->mutex);

        if (target->interruptingThread != self) {
            PlatformThread::unlockMutex(target->mutex);
            PlatformThread::unlockMutex(library->globalLock);
            threadExit(self, true);
        }

        // This is the monitor the target is waiting on.
        monitor = target->monitor;

        PlatformThread::unlockMutex(target->mutex);
        PlatformThread::unlockMutex(library->globalLock);

        // try to take the monitor so that we can notify the thread to interrupt.
        doMonitorEnter(monitor, self);

        PlatformThread::lockMutex(library->globalLock);

        // If the target was interrupted already it will cancel this thread.
        if (self->canceled == true) {
            PlatformThread::unlockMutex(library->globalLock);
            threadExit(self, true);
        }

        PlatformThread::lockMutex(target->mutex);
        if (target->interruptingThread == self && target->waiting == true) {
            doNotifyThread(target, false);
        }

        target->interruptingThread = NULL;

        PlatformThread::unlockMutex(target->mutex);
        PlatformThread::unlockMutex(library->globalLock);

        Threading::exitMonitor(monitor);

        threadExit(self, true);
    }

    void createThreadInstance(ThreadHandle* thread, long long stackSize, int priority,
                              bool suspended, threadingTask threadMain, void* threadArg) {

        if (stackSize <= 0) {
            stackSize = PLATFORM_DEFAULT_STACK_SIZE;
        }

        thread->stackSize = stackSize;
        thread->priority = priority;
        thread->suspended = suspended;
        thread->threadMain = threadMain;
        thread->threadArg = threadArg;

        PlatformThread::createNewThread(&thread->handle, threadEntryMethod, thread,
                                        library->priorityMapping[priority], stackSize,
                                        &thread->threadId);
    }

    ThreadHandle* initThreadHandle(ThreadHandle* thread) {

        thread->parent = NULL;
        thread->name = NULL;
        thread->interruptible = false;
        thread->interrupted = false;
        thread->parked = false;
        thread->priority = Thread::NORM_PRIORITY;
        thread->stackSize = -1;
        thread->state = Thread::NEW;
        thread->references = 2;
        thread->unparked = false;
        thread->numAttached = 0;
        thread->interruptingThread = NULL;
        thread->osThread = false;
        thread->handle = PlatformThread::getCurrentThread();
        thread->threadId = 0;
        thread->next = NULL;
        thread->joiners = NULL;
        thread->interruptingThread = NULL;
        thread->monitor = NULL;

        ::memset(thread->tls, 0, sizeof(thread->tls));

        try {
            PlatformThread::createMutex(&thread->mutex);
        } catch(RuntimeException& ex) {
            throw ex;
        }

        try {
            PlatformThread::createCondition(&thread->condition);
        } catch(RuntimeException& ex) {
            PlatformThread::destroyMutex(thread->mutex);
            throw ex;
        }

        return thread;
    }

    void dereferenceThread(ThreadHandle* thread) {

        // Both the Thread class and the thread hold a reference to the thread
        // kernel, so one or the other must delete it when both are finished.
        if (Atomics::decrementAndGet(&(thread->references)) <= 0) {
            free(thread->name);
            PlatformThread::destroyMutex(thread->mutex);
            PlatformThread::destroyCondition(thread->condition);
            PlatformThread::detachOSThread(thread->handle);

            delete thread;
        }
    }

    MonitorHandle* initMonitorHandle(MonitorHandle* monitor) {
        monitor->owner = NULL;
        monitor->count = 0;
        monitor->blocking = NULL;
        monitor->waiting = NULL;
        monitor->next = NULL;
        return monitor;
    }

    bool interruptWaitingThread(ThreadHandle* self DECAF_UNUSED, ThreadHandle* target) {

        bool result = false;
        //MonitorHandle* monitor;

        // TODO - Currently gets into a deadlock.
        // If this thread owns the target thread's monitor lock then there's no
        // reason to spawn an interruption thread, otherwise it has to be done
        // asynchronously.
        //monitor = target->monitor;
        //    if (monitorTryEnterUsingThreadId(monitor, self) == true) {
        //        PlatformThread::notifyAll(target->condition);
        //        monitorExitUsingThreadId(monitor, self);
        //        result = true;
        //    } else {

        // Spawn the thread so that we don't deadlock on the monitor.
        target->interruptingThread = initThreadHandle(new ThreadHandle());
        createThreadInstance(target->interruptingThread, 0, Thread::NORM_PRIORITY, false, interruptionThread, target);

        return result;
    }

    void threadExitTlsCleanup(ThreadHandle* thread) {

        for (int index = 0; index < DECAF_MAX_TLS_SLOTS; ++index) {
            if (thread->tls[index] != NULL) {
                ThreadLocalImpl* handler = NULL;
                void *value = NULL;

                PlatformThread::lockMutex(library->tlsLock);
                value = thread->tls[index];
                handler = library->tlsSlots[index];
                PlatformThread::unlockMutex(library->tlsLock);

                if (value != NULL) {
                    handler->doDelete(value);
                }
            }
        }
    }

    void unblockThreads(ThreadHandle* queueHead) {

        ThreadHandle* current = NULL;
        ThreadHandle* next = NULL;

        next = queueHead;

        while (next != NULL) {
            current = next;
            next = current->next;
            PlatformThread::notifyAll(current->condition);
        }
    }

    void enqueueThread(ThreadHandle** queue, ThreadHandle* thread) {

        ThreadHandle* qThread = *queue;

        if (thread->next != NULL) {
            throw RuntimeException(__FILE__, __LINE__, "Thread was on a monitor queue already");
        }

        if (qThread != NULL) {
            while(qThread->next) {
                qThread = qThread->next;
            }
            qThread->next = thread;
        } else {
            *queue = thread;
        }
    }

    void dequeueThread(ThreadHandle** queue, ThreadHandle* thread) {

        ThreadHandle* current = NULL;
        ThreadHandle* next = NULL;

        if ((current = *queue) == NULL) {
            return;
        }

        if (current == thread) {
            *queue = thread->next;
            thread->next = NULL;
        } else {

            while((next = current->next) != NULL && next != thread) {
                current = next;
            }

            if (next != NULL) {
                current->next = thread->next;
                thread->next = NULL;
            }
        }
    }

    unsigned int getNumberOfWaiters(MonitorHandle* monitor) {

        unsigned int numWaiting = 0;
        ThreadHandle* current;

        PlatformThread::lockMutex(monitor->mutex);

        current = monitor->waiting;
        while (current != NULL) {
            numWaiting++;
            current = current->next;
        }

        PlatformThread::unlockMutex(monitor->mutex);

        return numWaiting;
    }

    void purgeMonitorsPool(MonitorPool* pool) {

        MonitorHandle* current = NULL;
        MonitorHandle* next = NULL;

        next = pool->head;

        while (next != NULL) {
            current = next;
            next = current->next;

            // Cleanup the OS level resources.
            if (current->initialized == true) {
                PlatformThread::destroyMutex(current->mutex);
                PlatformThread::destroyMutex(current->lock);
            }

            delete current;
        }
    }

    MonitorHandle* batchAllocateMonitors() {

        MonitorHandle* current = NULL;
        MonitorHandle* last = NULL;

        for (int i = 0; i < MONITOR_POOL_BLOCK_SIZE; ++i) {
            current = new MonitorHandle;
            initMonitorHandle(current);
            current->next = last;
            current->initialized = false;
            last = current;
        }

        return current;
    }

    void doNotifyThread(ThreadHandle* thread, bool markAsNotified) {

        thread->waiting = false;
        thread->blocked = true;

        if (markAsNotified) {
            thread->notified = true;
        }

        PlatformThread::notifyAll(thread->condition);
    }

    void doNotifyWaiters(MonitorHandle* monitor, bool notifyAll) {

        ThreadHandle* self = Threading::getCurrentThreadHandle();
        ThreadHandle* current = NULL;
        ThreadHandle* next = NULL;
        bool signalled = false;

        if (self != monitor->owner) {
            throw IllegalMonitorStateException(__FILE__, __LINE__, "Current Thread is not the lock holder.");
        }

        PlatformThread::lockMutex(monitor->mutex);

        next = monitor->waiting;

        while (next != NULL) {
            current = next;
            next = current->next;

            PlatformThread::lockMutex(current->mutex);

            if (current->waiting == true) {
                doNotifyThread(current, true);
                signalled = true;
            }

            PlatformThread::unlockMutex(current->mutex);

            if (signalled && !notifyAll) {
                break;
            }
        }

        PlatformThread::unlockMutex(monitor->mutex);
    }

    void doMonitorEnter(MonitorHandle* monitor, ThreadHandle* thread) {

        while (true) {

            if (PlatformThread::tryLockMutex(monitor->lock) == true) {
                monitor->owner = thread;
                monitor->count = 1;
                break;
            }

            PlatformThread::lockMutex(monitor->mutex);

            if (PlatformThread::tryLockMutex(monitor->lock) == true) {
                PlatformThread::unlockMutex(monitor->mutex);
                monitor->owner = thread;
                monitor->count = 1;
                break;
            }

            PlatformThread::lockMutex(thread->mutex);

            thread->blocked = true;
            thread->state = Thread::BLOCKED;
            thread->monitor = monitor;

            PlatformThread::unlockMutex(thread->mutex);

            enqueueThread(&monitor->blocking, thread);

            PlatformThread::waitOnCondition(thread->condition, monitor->mutex);

            dequeueThread(&monitor->blocking, thread);

            PlatformThread::unlockMutex(monitor->mutex);
        }

        // Monitor is now owned by this thread, lets clean up the state in case
        // the lock was acquired after blocking.
        if (thread->monitor != NULL) {
            PlatformThread::lockMutex(thread->mutex);
            thread->blocked = false;
            thread->state = Thread::RUNNABLE;
            thread->monitor = NULL;
            PlatformThread::unlockMutex(thread->mutex);
        }
    }

    void doMonitorExit(MonitorHandle* monitor, ThreadHandle* thread DECAF_UNUSED) {

        monitor->count--;

        if (monitor->count == 0) {
            monitor->owner = NULL;

            // Wake any blocked threads so they can attempt to enter the monitor.
            PlatformThread::lockMutex(monitor->mutex);
            unblockThreads(monitor->blocking);

            // since we are signaling waiting threads we unlock this under lock so that they
            // don't go back to sleep before we are done
            PlatformThread::unlockMutex(monitor->lock);

            PlatformThread::unlockMutex(monitor->mutex);
        }
    }

    bool doWaitOnMonitor(MonitorHandle* monitor, ThreadHandle* thread,
                         long long mills, int nanos, bool interruptible) {

        int count = -1;
        bool interrupted = false;
        bool notified = false;
        bool timedOut = false;

        if (monitor->owner != thread) {
            throw IllegalMonitorStateException(__FILE__, __LINE__, "Current Thread is not the lock holder.");
        }

        count = monitor->count;

        PlatformThread::lockMutex(thread->mutex);

        // Before we wait, check if we've already been either interrupted
        if (interruptible && thread->interrupted) {
            thread->interrupted = false;
            PlatformThread::unlockMutex(thread->mutex);
            throw InterruptedException(__FILE__, __LINE__, "Thread interrupted");
        }

        thread->waiting = true;
        thread->interruptible = interruptible;

        if (mills || nanos) {
            thread->timerSet = true;
            thread->state = Thread::TIMED_WAITING;
        } else {
            thread->state = Thread::WAITING;
        }

        thread->monitor = monitor;
        PlatformThread::unlockMutex(thread->mutex);

        monitor->owner = NULL;
        monitor->count = 0;

        PlatformThread::lockMutex(monitor->mutex);

        // Release the lock and wake up any blocked threads.
        PlatformThread::unlockMutex(monitor->lock);
        unblockThreads(monitor->blocking);

        // This thread now enters the wait queue.
        enqueueThread(&monitor->waiting, thread);

        MonitorWaitCompletionCondition completion(thread);

        if (mills || nanos) {
            timedOut = PlatformThread::interruptibleWaitOnCondition(thread->condition, monitor->mutex, mills, nanos, completion);
        } else {
            PlatformThread::interruptibleWaitOnCondition(thread->condition, monitor->mutex, completion);
        }

        dequeueThread(&monitor->waiting, thread);

        PlatformThread::unlockMutex(monitor->mutex);

        // We should own the Thread's mutex from the CompletionCondition locking it.

        interrupted = thread->interrupted;
        notified = thread->notified;

        thread->waiting = false;
        thread->notified = false;
        thread->timerSet = false;
        thread->interruptible = false;
        thread->state = Thread::RUNNABLE;

        if (interrupted && !notified) {
            thread->interrupted = false;
        }

        if (thread->interruptingThread) {
            PlatformThread::lockMutex(thread->interruptingThread->mutex);
            thread->interruptingThread->canceled = true;
            PlatformThread::unlockMutex(thread->interruptingThread->mutex);
            thread->interruptingThread = NULL;
        }

        PlatformThread::unlockMutex(thread->mutex);

        // Re-acquire the lock now and restore its old state.
        doMonitorEnter(monitor, thread);

        monitor->count = count;

        if (notified) {
            return false;
        }

        if (interrupted) {
            throw InterruptedException(__FILE__, __LINE__, "Thread interrupted");
        }

        if (!timedOut) {
            throw RuntimeException(__FILE__, __LINE__, "Invalid state detected at end of Monitor Wait");
        }

        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////
Threading::Threading() {
}

////////////////////////////////////////////////////////////////////////////////
void Threading::initialize() {

    library = new ThreadingLibrary();

    // Figure out what the OS level thread priority mappings are for the Thread
    // classes generic priority value range.
    PlatformThread::initPriorityMapping(Thread::MAX_PRIORITY + 1, library->priorityMapping);

    PlatformThread::createTlsKey(&(library->threadKey));
    PlatformThread::createTlsKey(&(library->selfKey));
    PlatformThread::createMutex(&(library->globalLock));
    PlatformThread::createMutex(&(library->tlsLock));

    library->monitors = new MonitorPool;
    library->monitors->head = batchAllocateMonitors();
    library->monitors->count = MONITOR_POOL_BLOCK_SIZE;

    library->tlsSlots.resize(DECAF_MAX_TLS_SLOTS);

    // We mark the thread where Decaf's Init routine is called from as our Main Thread.
    library->mainThread = PlatformThread::getCurrentThread();

    // Initialize the Executors static data for use in ExecutorService classes and Atomics
    Executors::initialize();
    Atomics::initialize();
}

////////////////////////////////////////////////////////////////////////////////
void Threading::shutdown() {

    // First shutdown the Executors static data to remove dependencies on Threading.
    Executors::shutdown();

    // Destroy any Foreign Thread Facades that were created during runtime.
    std::vector<Thread*>::iterator iter = library->osThreads.begin();
    for (; iter != library->osThreads.end(); ++iter) {
        delete *iter;
    }
    library->osThreads.clear();

    PlatformThread::destroyTlsKey(library->threadKey);
    PlatformThread::destroyTlsKey(library->selfKey);
    PlatformThread::destroyMutex(library->globalLock);
    PlatformThread::destroyMutex(library->tlsLock);

    purgeMonitorsPool(library->monitors);
    delete library->monitors;
    delete library;

    // Atomics only uses platform Thread primitives when there are no atomic
    // builtins and Atomics are used in thread so make sure this is always last
    // in the shutdown order.
    Atomics::shutdown();
}

////////////////////////////////////////////////////////////////////////////////
void Threading::lockThreadsLib() {
    PlatformThread::lockMutex(library->globalLock);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::unlockThreadsLib() {
    PlatformThread::unlockMutex(library->globalLock);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::dumpRunningThreads() {
    lockThreadsLib();

    if (library == NULL) {
        return;
    }

    std::list<ThreadHandle*>::const_iterator threads = library->activeThreads.begin();

    std::cout << "------------------------------------------------------------------------" << std::endl;
    std::cout << " Active Threads: " << library->activeThreads.size() << std::endl;
    std::cout << " Wrapped OS Threads: " << library->osThreads.size() << std::endl;
    std::cout << std::endl;

    for(; threads != library->activeThreads.end(); ++threads) {
        ThreadHandle* thread = *threads;

        if (thread == NULL) {
            continue;
        }

        std::string threadName = thread->name;

        std::cout << "Thread name = " << threadName << std::endl;
    }

    std::cout << "------------------------------------------------------------------------" << std::endl;

    unlockThreadsLib();
}

////////////////////////////////////////////////////////////////////////////////
ThreadHandle* Threading::createNewThread(Thread* parent, const char* name, long long stackSize) {

    if (parent == NULL || name == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "One or more arguments was NULL");
    }

    Pointer<ThreadHandle> thread(new ThreadHandle());

    initThreadHandle(thread.get());

    thread->parent = parent;
    thread->name = ::strdup(name);

    createThreadInstance(thread.get(), stackSize, Thread::NORM_PRIORITY, true, runCallback, thread.get());

    return thread.release();
}

////////////////////////////////////////////////////////////////////////////////
void Threading::destroyThread(ThreadHandle* thread) {

    if (!thread->osThread) {

        // If the thread was created but never started then we need to wake it
        // up from the suspended state so that it can terminate, we mark it
        // canceled to ensure it doesn't call its runnable.
        if (thread->state == Thread::NEW) {
            PlatformThread::lockMutex(thread->mutex);

            if (thread->state == Thread::NEW && thread->suspended == true) {
                thread->canceled = true;
                thread->suspended = false;
                PlatformThread::notifyAll(thread->condition);
            }

            PlatformThread::unlockMutex(thread->mutex);
        }

        try {
            Threading::join(thread, 0, 0);
        } catch (InterruptedException& ex) {}
    } else {
        PlatformThread::detachOSThread(thread->handle);
    }

    dereferenceThread(thread);
}

////////////////////////////////////////////////////////////////////////////////
ThreadHandle* Threading::attachToCurrentThread() {

    Pointer<ThreadHandle> thread(initThreadHandle(new ThreadHandle()));

    thread->handle = PlatformThread::getCurrentThread();
    thread->state = Thread::RUNNABLE;
    thread->stackSize = PlatformThread::getStackSize(thread->handle);
    thread->name = ::strdup(
        std::string(std::string("OS-Thread") + Integer::toString(library->osThreadId.getAndIncrement())).c_str());
    thread->threadId = PlatformThread::getCurrentThreadId();

    // An OS Thread doesn't have a running thread, this is only a proxy to only one ref.
    thread->references = 1;

    // Now create a Decaf Thread as a proxy to the OS thread.
    Pointer<Thread> osThread(new Thread(thread.get()));
    thread->parent = osThread.get();
    thread->osThread = true;

    PlatformThread::setTlsValue(library->threadKey, osThread.get());
    PlatformThread::setTlsValue(library->selfKey, thread.get());

    // Store the Thread that wraps this OS thread for later deletion since
    // no other owners exist.
    PlatformThread::lockMutex(library->globalLock);
    library->osThreads.push_back(osThread.release());
    PlatformThread::unlockMutex(library->globalLock);

    return thread.release();
}

////////////////////////////////////////////////////////////////////////////////
void Threading::start(ThreadHandle* thread) {

    try {

        if (thread->state > Thread::NEW) {
            throw IllegalThreadStateException(__FILE__, __LINE__, "Thread already started");
        }

        PlatformThread::lockMutex(thread->mutex);

        thread->state = Thread::RUNNABLE;

        if (thread->suspended == true) {
            thread->suspended = false;
            PlatformThread::notifyAll(thread->condition);
        }

        PlatformThread::unlockMutex(thread->mutex);
    }
    DECAF_CATCH_RETHROW(IllegalThreadStateException)
    DECAF_CATCH_RETHROW(RuntimeException)
    DECAF_CATCH_EXCEPTION_CONVERT(NullPointerException, RuntimeException)
    DECAF_CATCHALL_THROW(RuntimeException)
}

////////////////////////////////////////////////////////////////////////////////
void Threading::yeild() {
    PlatformThread::yeild();
}

////////////////////////////////////////////////////////////////////////////////
void Threading::interrupt(ThreadHandle* thread) {

    ThreadHandle* self = Threading::getCurrentThreadHandle();

    PlatformThread::lockMutex(library->globalLock);
    PlatformThread::lockMutex(thread->mutex);

    if (thread->interrupted == true) {
        PlatformThread::unlockMutex(thread->mutex);
        PlatformThread::unlockMutex(library->globalLock);
        return;
    }

    if (thread->interruptible == true) {

        if (thread->sleeping || thread->parked) {
            PlatformThread::notifyAll(thread->condition);
        } else if(thread->waiting == true) {
            if (interruptWaitingThread(self, thread)) {
                thread->blocked = true;
            }
        }
    }

    thread->interrupted = true;

    PlatformThread::unlockMutex(thread->mutex);
    PlatformThread::unlockMutex(library->globalLock);
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::interrupted() {
    ThreadHandle* self = Threading::getCurrentThreadHandle();
    return Threading::isInterrupted(self, true);
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::isInterrupted(ThreadHandle* handle, bool reset) {

    bool currentValue = handle->interrupted;

    if (reset == true) {
        PlatformThread::lockMutex(handle->mutex);
        currentValue = handle->interrupted;
        handle->interrupted = false;
        PlatformThread::unlockMutex(handle->mutex);
    }

    return currentValue;
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class JoinCompletionCondition : public CompletionCondition {
    private:

        ThreadHandle* self;
        ThreadHandle* target;

    private:

        JoinCompletionCondition(const JoinCompletionCondition&);
        JoinCompletionCondition& operator= (const JoinCompletionCondition&);

    public:

        JoinCompletionCondition(ThreadHandle* self, ThreadHandle* target) : self(self), target(target) {}

        virtual bool operator()() {

            if (target != NULL) {
                if (target->state == Thread::TERMINATED) {
                    return true;
                }

                PlatformThread::lockMutex(self->mutex);

                if (self->interrupted == true) {
                    PlatformThread::unlockMutex(self->mutex);
                    return true;
                }

                PlatformThread::unlockMutex(self->mutex);
            } else if (self->interrupted == true) {
                return true;
            }

            return false;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::join(ThreadHandle* thread, long long mills, int nanos) {

    if ((mills < 0) || (nanos < 0) || (nanos >= 1000000)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Timeout arguments out of range.");
    }

    bool timedOut = false;
    bool interrupted = false;

    ThreadHandle* self = getCurrentThreadHandle();

    PlatformThread::lockMutex(self->mutex);

    if (self->interrupted == true) {
        interrupted = true;
    } else if (self == thread && self->state != Thread::TERMINATED) {

        // When blocking on ourself, we just enter a wait and hope there's
        // either a timeout, or we are interrupted.

        JoinCompletionCondition completion(self, NULL);

        self->sleeping = true;
        self->interruptible = true;
        self->state = Thread::SLEEPING;

        if (mills > 0 || nanos > 0) {
            self->timerSet = true;
            timedOut = PlatformThread::interruptibleWaitOnCondition(self->condition, self->mutex,
                                                                    mills, nanos, completion);
        } else {
            PlatformThread::interruptibleWaitOnCondition(self->condition, self->mutex, completion);
        }

    } else {

        PlatformThread::lockMutex(thread->mutex);

        if (thread->state >= Thread::RUNNABLE && thread->state != Thread::TERMINATED) {

            enqueueThread(&thread->joiners, self);

            self->sleeping = true;
            self->interruptible = true;
            self->state = Thread::SLEEPING;

            JoinCompletionCondition completion(self, thread);

            if (mills > 0 || nanos > 0) {
                self->timerSet = true;

                PlatformThread::unlockMutex(self->mutex);
                timedOut = PlatformThread::interruptibleWaitOnCondition(self->condition, thread->mutex,
                                                                        mills, nanos, completion);
            } else {
                PlatformThread::unlockMutex(self->mutex);
                PlatformThread::interruptibleWaitOnCondition(self->condition, thread->mutex, completion);
            }

            dequeueThread(&thread->joiners, self);
            PlatformThread::unlockMutex(thread->mutex);

            PlatformThread::lockMutex(self->mutex);

            self->timerSet = false;
            self->state = Thread::RUNNABLE;
            self->sleeping = false;
            self->interruptible = false;

            if (self->interrupted == true) {
                interrupted = true;
                self->interrupted = false;
            }
        } else {
            PlatformThread::unlockMutex(thread->mutex);
        }
    }

    PlatformThread::unlockMutex(self->mutex);

    if (interrupted) {
        throw InterruptedException(__FILE__, __LINE__, "Sleeping Thread interrupted");
    }

    return timedOut;
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class SleepCompletionCondition : public CompletionCondition {
    private:

        ThreadHandle* handle;

    private:

        SleepCompletionCondition(const SleepCompletionCondition&);
        SleepCompletionCondition& operator= (const SleepCompletionCondition&);

    public:

        SleepCompletionCondition(ThreadHandle* handle) : handle(handle) {}

        bool operator()() {
            if (handle->interrupted) {
                return true;
            }

            return false;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::sleep(long long mills, int nanos) {

    if ((mills < 0) || (nanos < 0) || (nanos >= 1000000)) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Timeout arguments out of range.");
    }

    bool timedOut = false;
    bool interrupted = false;

    ThreadHandle* self = getCurrentThreadHandle();

    PlatformThread::lockMutex(self->mutex);

    if (self->interrupted == true) {
        interrupted = true;
    } else {

        self->sleeping = true;
        self->state = Thread::SLEEPING;
        self->interruptible = true;
        self->timerSet = true;

        SleepCompletionCondition completion(self);

        timedOut = PlatformThread::interruptibleWaitOnCondition(self->condition, self->mutex,
                                                                mills, nanos, completion);
    }

    self->timerSet = false;
    self->sleeping = false;
    self->interruptible = false;
    self->state = Thread::RUNNABLE;

    if (self->interrupted == true) {
        interrupted = true;
        self->interrupted = false;
    }

    PlatformThread::unlockMutex(self->mutex);

    if (interrupted) {
        throw InterruptedException(__FILE__, __LINE__, "Sleeping Thread interrupted");
    }

    return timedOut;
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::isThreadAlive(ThreadHandle* handle DECAF_UNUSED) {
    return handle->state >= Thread::RUNNABLE && handle->state != Thread::TERMINATED;
}

////////////////////////////////////////////////////////////////////////////////
long long Threading::getThreadId(ThreadHandle* handle) {
    return handle->threadId;
}

////////////////////////////////////////////////////////////////////////////////
int Threading::getThreadPriority(ThreadHandle* handle) {
    return handle->priority;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::setThreadPriority(ThreadHandle* handle, int priority) {
    PlatformThread::setPriority(handle->handle, library->priorityMapping[priority]);
    handle->priority = priority;
}

////////////////////////////////////////////////////////////////////////////////
const char* Threading::getThreadName(ThreadHandle* handle) {
    return handle->name;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::setThreadName(ThreadHandle* thread, const char* name) {

    if (thread->name != NULL) {
        free(thread->name);
    }

    thread->name = ::strdup(name);
}

////////////////////////////////////////////////////////////////////////////////
Thread::State Threading::getThreadState(ThreadHandle* handle) {
    return (Thread::State)handle->state;
}

////////////////////////////////////////////////////////////////////////////////
Thread* Threading::getCurrentThread() {
    return getCurrentThreadHandle()->parent;
}

////////////////////////////////////////////////////////////////////////////////
ThreadHandle* Threading::getCurrentThreadHandle() {
    ThreadHandle* self = (ThreadHandle*)PlatformThread::getTlsValue(library->selfKey);

    if (self == NULL) {
        self = attachToCurrentThread();
    }

    return self;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::park(Thread* thread) {

    if (thread == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Null Thread Pointer Passed.");
    }

    Threading::park(thread, 0LL, 0LL);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ParkCompletionCondition : public CompletionCondition {
    private:

        ThreadHandle* handle;

    private:

        ParkCompletionCondition(const ParkCompletionCondition&);
        ParkCompletionCondition& operator= (const ParkCompletionCondition&);

    public:

        ParkCompletionCondition(ThreadHandle* handle) : handle(handle) {}

        bool operator()() {
            if (handle->unparked == true) {
                handle->unparked = false;
                return true;
            } else if (handle->interrupted) {
                return true;
            }

            return false;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::park( Thread* thread, long long mills, int nanos) {

    if (thread == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Null Thread Pointer Passed.");
    }

    bool timedOut = false;
    bool interrupted = false;

    ThreadHandle* handle = thread->getHandle();

    PlatformThread::lockMutex(handle->mutex);

    if (handle->unparked == true) {
        handle->unparked = false;
    } else if (handle->interrupted == true) {
        interrupted = true;
    } else {

        handle->state = Thread::BLOCKED;
        handle->parked = true;
        handle->interruptible = true;

        ParkCompletionCondition completion(handle);

        if (mills > 0 || nanos > 0) {
            handle->timerSet = true;
            timedOut = PlatformThread::interruptibleWaitOnCondition(handle->condition, handle->mutex,
                                                                    mills, nanos, completion);
        } else {
            PlatformThread::interruptibleWaitOnCondition(handle->condition, handle->mutex, completion);
        }
    }

    if (interrupted) {
        // TODO
    }

    handle->timerSet = false;
    handle->parked = false;
    handle->interruptible = false;
    handle->state = Thread::RUNNABLE;

    PlatformThread::unlockMutex(handle->mutex);

    return timedOut;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::unpark(Thread* thread) {

    if (thread == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Null Thread Pointer Passed.");
    }

    ThreadHandle* handle = thread->getHandle();

    PlatformThread::lockMutex(handle->mutex);

    // Set the un-parked token, if the thread is parked it will consume
    // it when it resumes, otherwise the next call to park will consume
    // it without needing to actually wait.
    handle->unparked = true;

    // If the thread is actually parked then we send it a signal so
    // that it will resume.
    if (handle->parked) {
        PlatformThread::notifyAll(handle->condition);
    }

    PlatformThread::unlockMutex(handle->mutex);
}

////////////////////////////////////////////////////////////////////////////////
MonitorHandle* Threading::takeMonitor(bool alreadyLocked) {

    MonitorHandle* monitor = NULL;

    if (!alreadyLocked) {
        PlatformThread::lockMutex(library->globalLock);
    }

    if (library->monitors->head == NULL) {
        library->monitors->head = batchAllocateMonitors();
        library->monitors->count = MONITOR_POOL_BLOCK_SIZE;
    }

    monitor = library->monitors->head;
    library->monitors->head = monitor->next;
    library->monitors->count--;
    monitor->next = NULL;

    if (monitor->initialized == false) {
        PlatformThread::createMutex(&monitor->mutex);
        PlatformThread::createMutex(&monitor->lock);
        monitor->initialized = true;
    }

    if (!alreadyLocked) {
        PlatformThread::unlockMutex(library->globalLock);
    }

    return monitor;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::returnMonitor(MonitorHandle* monitor, bool alreadyLocked) {

    if (monitor == NULL) {
        throw RuntimeException(__FILE__, __LINE__, "Monitor pointer was null");
    }

    // The own can return the Monitor in a locked state if its held by the thread that's
    // returning it, we will unlock it for the caller in this case, otherwise we throw
    // an error, and in every case we need to thrown if someone's still waiting on the
    // target monitor otherwise we could see a segfault.
    if ((monitor->owner && monitor->owner != getCurrentThreadHandle()) || monitor->waiting) {
        throw IllegalMonitorStateException(__FILE__, __LINE__, "Monitor is still in use!");
    }

    if (monitor->owner) {
        Threading::exitMonitor(monitor);
    }

    if (!alreadyLocked) {
        PlatformThread::lockMutex(library->globalLock);
    }

    initMonitorHandle(monitor);
    monitor->next = library->monitors->head;
    library->monitors->head = monitor;
    library->monitors->count++;

    if (!alreadyLocked) {
        PlatformThread::unlockMutex(library->globalLock);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::isMonitorLocked(MonitorHandle* monitor) {

    if (monitor != NULL) {
        return monitor->owner != NULL;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::enterMonitor(MonitorHandle* monitor) {

    ThreadHandle* thisThread = getCurrentThreadHandle();

    if (thisThread == monitor->owner) {
        monitor->count++;
        return;
    }

    doMonitorEnter(monitor, thisThread);
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::tryEnterMonitor(MonitorHandle* monitor) {
    ThreadHandle* self = getCurrentThreadHandle();
    return monitorTryEnterUsingThreadId(monitor, self);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::exitMonitor(MonitorHandle* monitor) {

    ThreadHandle* thisThread = getCurrentThreadHandle();

    if (thisThread != monitor->owner) {
        throw IllegalMonitorStateException(__FILE__, __LINE__, "Thread is not the owner of this monitor");
    }

    doMonitorExit(monitor, thisThread);
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::waitOnMonitor(MonitorHandle* monitor, long long mills, int nanos) {
    ThreadHandle* self = getCurrentThreadHandle();
    // Wait but do so in a non-interruptible state.
    return doWaitOnMonitor(monitor, self, mills, nanos, true);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::notifyWaiter(MonitorHandle* monitor) {
    doNotifyWaiters(monitor, false);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::notifyAllWaiters(MonitorHandle* monitor) {
    doNotifyWaiters(monitor, true);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::monitorExitUsingThreadId(MonitorHandle* monitor, ThreadHandle* thread) {

    if (monitor->owner != thread) {
        throw IllegalMonitorStateException(__FILE__, __LINE__, "Specified thread is not the monitor owner.");
    }

    doMonitorExit(monitor, thread);
}

////////////////////////////////////////////////////////////////////////////////
void Threading::monitorEnterUsingThreadId(MonitorHandle* monitor, ThreadHandle* thread) {

    if (monitor->owner == thread) {
        monitor->count++;
        return;
    }

    doMonitorEnter(monitor, thread);
}

////////////////////////////////////////////////////////////////////////////////
bool Threading::monitorTryEnterUsingThreadId(MonitorHandle* monitor, ThreadHandle* thread) {

    if (monitor->owner == thread) {
        monitor->count++;
        return true;
    }

    if (PlatformThread::tryLockMutex(monitor->lock) == true) {
        monitor->owner = thread;
        monitor->count = 1;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
int Threading::createThreadLocalSlot(ThreadLocalImpl* threadLocal) {

    if (threadLocal == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Null ThreadLocalImpl Pointer Passed." );
    }

    int index;

    PlatformThread::lockMutex(library->tlsLock);

    for (index = 0; index < DECAF_MAX_TLS_SLOTS; index++) {
        if (library->tlsSlots[index] == NULL) {
            library->tlsSlots[index] = threadLocal;
            break;
        }
    }

    PlatformThread::unlockMutex(library->tlsLock);

    return index < DECAF_MAX_TLS_SLOTS ? index : -1;
}

////////////////////////////////////////////////////////////////////////////////
void* Threading::getThreadLocalValue(int slot) {
    ThreadHandle* thisThread = getCurrentThreadHandle();
    return thisThread->tls[slot];
}

////////////////////////////////////////////////////////////////////////////////
void Threading::setThreadLocalValue(int slot, void* value) {
    ThreadHandle* thisThread = getCurrentThreadHandle();
    thisThread->tls[slot] = value;
}

////////////////////////////////////////////////////////////////////////////////
void Threading::destoryThreadLocalSlot(int slot) {

    ThreadHandle* current = NULL;
    ThreadLocalImpl* local = library->tlsSlots[slot];

    // Must lock globally so that no thread can terminate and call its own
    // tls cleanup and our list of thread remains stable.
    PlatformThread::lockMutex(library->globalLock);

    std::list<ThreadHandle*>::const_iterator iter = library->activeThreads.begin();
    while (iter != library->activeThreads.end()) {
        current = *(iter++);
        void* value = current->tls[slot];
        if (value != NULL) {
            local->doDelete(value);
            current->tls[slot] = NULL;
        }
    }

    std::vector<decaf::lang::Thread*>::const_iterator osIter = library->osThreads.begin();
    while (osIter != library->osThreads.end()) {
        current = (*(osIter++))->getHandle();
        void* value = current->tls[slot];
        if (value != NULL) {
            local->doDelete(value);
            current->tls[slot] = NULL;
        }
    }

    PlatformThread::unlockMutex(library->globalLock);

    // Return the slot to the pool under lock so that a taker waits.
    PlatformThread::lockMutex(library->tlsLock);
    library->tlsSlots[slot] = NULL;
    PlatformThread::unlockMutex(library->tlsLock);
}
