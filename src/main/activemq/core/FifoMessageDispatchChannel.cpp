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

#include "FifoMessageDispatchChannel.h"

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
FifoMessageDispatchChannel::FifoMessageDispatchChannel() : closed(false), running(false), channel() {
}

////////////////////////////////////////////////////////////////////////////////
FifoMessageDispatchChannel::~FifoMessageDispatchChannel() {
}

////////////////////////////////////////////////////////////////////////////////
void FifoMessageDispatchChannel::enqueue(const Pointer<MessageDispatch>& message) {
    synchronized(&channel) {
        channel.addLast(message);
        channel.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
void FifoMessageDispatchChannel::enqueueFirst(const Pointer<MessageDispatch>& message) {
    synchronized(&channel) {
        channel.addFirst(message);
        channel.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool FifoMessageDispatchChannel::isEmpty() const {
    synchronized(&channel) {
        return channel.isEmpty();
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> FifoMessageDispatchChannel::dequeue(long long timeout) {

    synchronized(&channel) {
        // Wait until the channel is ready to deliver messages.
        while (timeout != 0 && !closed && (channel.isEmpty() || !running)) {
            if (timeout == -1) {
                channel.wait();
            } else {
                channel.wait((unsigned long) timeout);
                break;
            }
        }

        if (closed || !running || channel.isEmpty()) {
            return Pointer<MessageDispatch>();
        }

        return channel.pop();
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> FifoMessageDispatchChannel::dequeueNoWait() {
    synchronized(&channel) {
        if (closed || !running || channel.isEmpty()) {
            return Pointer<MessageDispatch>();
        }
        return channel.pop();
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> FifoMessageDispatchChannel::peek() const {
    synchronized(&channel) {
        if (closed || !running || channel.isEmpty()) {
            return Pointer<MessageDispatch>();
        }
        return channel.getFirst();
    }

    return Pointer<MessageDispatch>();
}

////////////////////////////////////////////////////////////////////////////////
void FifoMessageDispatchChannel::start() {
    synchronized(&channel) {
        if (!closed) {
            running = true;
            channel.notifyAll();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FifoMessageDispatchChannel::stop() {
    synchronized(&channel) {
        running = false;
        channel.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void FifoMessageDispatchChannel::close() {
    synchronized(&channel) {
        if (!closed) {
            running = false;
            closed = true;
        }
        channel.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void FifoMessageDispatchChannel::clear() {
    synchronized(&channel) {
        channel.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
int FifoMessageDispatchChannel::size() const {
    synchronized(&channel) {
        return (int) channel.size();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<Pointer<MessageDispatch> > FifoMessageDispatchChannel::removeAll() {
    std::vector<Pointer<MessageDispatch> > result;

    synchronized(&channel) {
        result = channel.toArray();
        channel.clear();
    }

    return result;
}
