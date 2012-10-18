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

#include "SimplePriorityMessageDispatchChannel.h"

#include <cms/Message.h>

#include <decaf/lang/Math.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
const int SimplePriorityMessageDispatchChannel::MAX_PRIORITIES = 10;

////////////////////////////////////////////////////////////////////////////////
SimplePriorityMessageDispatchChannel::SimplePriorityMessageDispatchChannel() :
    closed(false), running(false), mutex(), channels(MAX_PRIORITIES), enqueued(0) {
}

////////////////////////////////////////////////////////////////////////////////
SimplePriorityMessageDispatchChannel::~SimplePriorityMessageDispatchChannel() {
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannel::enqueue(const Pointer<MessageDispatch>& message) {
    synchronized(&mutex) {
        this->getChannel(message).addLast(message);
        this->enqueued++;
        mutex.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannel::enqueueFirst(const Pointer<MessageDispatch>& message) {
    synchronized(&mutex) {
        this->getChannel(message).addFirst(message);
        this->enqueued++;
        mutex.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool SimplePriorityMessageDispatchChannel::isEmpty() const {
    return this->enqueued == 0;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> SimplePriorityMessageDispatchChannel::dequeue(long long timeout) {

    synchronized(&mutex) {
        // Wait until the channel is ready to deliver messages.
        while (timeout != 0 && !closed && (isEmpty() || !running)) {
            if (timeout == -1) {
                mutex.wait();
            } else {
                mutex.wait((unsigned long) timeout);
                break;
            }
        }

        if (closed || !running || isEmpty()) {
            return Pointer<MessageDispatch>();
        }

        return removeFirst();
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> SimplePriorityMessageDispatchChannel::dequeueNoWait() {
    synchronized(&mutex) {
        if (closed || !running || isEmpty()) {
            return Pointer<MessageDispatch>();
        }
        return removeFirst();
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> SimplePriorityMessageDispatchChannel::peek() const {
    synchronized(&mutex) {
        if (closed || !running || isEmpty()) {
            return Pointer<MessageDispatch>();
        }
        return getFirst();
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannel::start() {
    synchronized(&mutex) {
        if (!closed) {
            running = true;
            mutex.notifyAll();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannel::stop() {
    synchronized(&mutex) {
        running = false;
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannel::close() {
    synchronized(&mutex) {
        if (!closed) {
            running = false;
            closed = true;
        }
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannel::clear() {
    synchronized(&mutex) {
        for (int i = 0; i < MAX_PRIORITIES; i++) {
            this->channels[i].clear();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
int SimplePriorityMessageDispatchChannel::size() const {
    synchronized(&mutex) {
        return this->enqueued;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<Pointer<MessageDispatch> > SimplePriorityMessageDispatchChannel::removeAll() {
    std::vector<Pointer<MessageDispatch> > result;

    synchronized(&mutex) {
        for (int i = MAX_PRIORITIES - 1; i >= 0; --i) {
            std::vector<Pointer<MessageDispatch> > temp(channels[i].toArray());
            result.insert(result.end(), temp.begin(), temp.end());
            this->enqueued -= (int) temp.size();
            channels[i].clear();
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
LinkedList<Pointer<MessageDispatch> >& SimplePriorityMessageDispatchChannel::getChannel(const Pointer<MessageDispatch>& dispatch) {

    int priority = cms::Message::DEFAULT_MSG_PRIORITY;

    if (dispatch->getMessage() != NULL) {
        priority = Math::max(dispatch->getMessage()->getPriority(), 0);
        priority = Math::min(dispatch->getMessage()->getPriority(), 9);
    }

    return this->channels[priority];
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> SimplePriorityMessageDispatchChannel::removeFirst() {

    if (this->enqueued > 0) {
        for (int i = MAX_PRIORITIES - 1; i >= 0; i--) {
            LinkedList<Pointer<MessageDispatch> >& channel = channels[i];
            if (!channel.isEmpty()) {
                this->enqueued--;
                return channel.pop();
            }
        }
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> SimplePriorityMessageDispatchChannel::getFirst() const {

    if (this->enqueued > 0) {
        for (int i = MAX_PRIORITIES - 1; i >= 0; i--) {
            LinkedList<Pointer<MessageDispatch> >& channel = channels[i];
            if (!channel.isEmpty()) {
                return channel.getFirst();
            }
        }
    }

    return Pointer<MessageDispatch>();
}
