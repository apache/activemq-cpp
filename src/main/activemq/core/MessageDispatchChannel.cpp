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

#include "MessageDispatchChannel.h"

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
MessageDispatchChannel::MessageDispatchChannel() {

    this->running = false;
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
MessageDispatchChannel::~MessageDispatchChannel() {
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchChannel::enqueue( const Pointer<MessageDispatch>& message ) {
    synchronized( &channel ) {
        channel.push( message );
        channel.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchChannel::enqueueFirst( const Pointer<MessageDispatch>& message ) {
    synchronized( &channel ) {
        channel.enqueueFront( message );
        channel.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool MessageDispatchChannel::isEmpty() const {
    synchronized( &channel ) {
        return channel.empty();
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> MessageDispatchChannel::dequeue( long long timeout )
    throw( exceptions::ActiveMQException ) {

    synchronized( &channel ) {
        // Wait until the channel is ready to deliver messages.
        while( timeout != 0 && !closed && ( channel.empty() || !running ) ) {
            if( timeout == -1 ) {
                channel.wait();
            } else {
                channel.wait( timeout );
                break;
            }
        }

        if( closed || !running || channel.empty() ) {
            return Pointer<MessageDispatch>();
        }

        return channel.pop();
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> MessageDispatchChannel::dequeueNoWait() {
    synchronized( &channel ) {
        if( closed || !running || channel.empty() ) {
            return Pointer<MessageDispatch>();
        }
        return channel.pop();
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageDispatch> MessageDispatchChannel::peek() const {
    synchronized( &channel ) {
        if( closed || !running || channel.empty() ) {
            return Pointer<MessageDispatch>();
        }
        return channel.front();
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchChannel::start() {
    synchronized( &channel ) {
        if( !closed ) {
            running = true;
            channel.notifyAll();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchChannel::stop() {
    synchronized( &channel ) {
        running = false;
        channel.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchChannel::close() {
    synchronized( &channel ) {
        if( !closed ) {
            running = false;
            closed = true;
        }
        channel.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageDispatchChannel::clear() {
    synchronized( &channel ) {
        channel.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
int MessageDispatchChannel::size() const {
    synchronized( &channel ) {
        return channel.size();
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector< Pointer<MessageDispatch> > MessageDispatchChannel::removeAll() {
    synchronized( &channel ) {
        std::vector< Pointer<MessageDispatch> > result = channel.toArray();
        channel.clear();
        return result;
    }
}
