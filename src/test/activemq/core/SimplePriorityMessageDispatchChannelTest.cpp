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

#include "SimplePriorityMessageDispatchChannelTest.h"

#include <activemq/core/SimplePriorityMessageDispatchChannel.h>
#include <activemq/commands/MessageDispatch.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/System.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testCtor() {

    SimplePriorityMessageDispatchChannel channel;
    CPPUNIT_ASSERT( channel.isRunning() == false );
    CPPUNIT_ASSERT( channel.isEmpty() == true );
    CPPUNIT_ASSERT( channel.size() == 0 );
    CPPUNIT_ASSERT( channel.isClosed() == false );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testStart() {

    SimplePriorityMessageDispatchChannel channel;
    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == true );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testStop() {

    SimplePriorityMessageDispatchChannel channel;
    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == true );
    channel.stop();
    CPPUNIT_ASSERT( channel.isRunning() == false );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testClose() {

    SimplePriorityMessageDispatchChannel channel;
    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == true );
    CPPUNIT_ASSERT( channel.isClosed() == false );
    channel.close();
    CPPUNIT_ASSERT( channel.isRunning() == false );
    CPPUNIT_ASSERT( channel.isClosed() == true );
    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == false );
    CPPUNIT_ASSERT( channel.isClosed() == true );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testEnqueue() {

    SimplePriorityMessageDispatchChannel channel;
    Pointer<MessageDispatch> dispatch1( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch2( new MessageDispatch() );

    CPPUNIT_ASSERT( channel.isEmpty() == true );
    CPPUNIT_ASSERT( channel.size() == 0 );

    channel.enqueue( dispatch1 );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 1 );

    channel.enqueue( dispatch2 );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 2 );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testEnqueueFront() {

    SimplePriorityMessageDispatchChannel channel;
    Pointer<MessageDispatch> dispatch1( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch2( new MessageDispatch() );

    Pointer<Message> message1( new Message() );
    Pointer<Message> message2( new Message() );

    message1->setPriority( 2 );
    message2->setPriority( 1 );

    dispatch1->setMessage( message1 );
    dispatch2->setMessage( message2 );

    channel.start();

    CPPUNIT_ASSERT( channel.isEmpty() == true );
    CPPUNIT_ASSERT( channel.size() == 0 );

    channel.enqueueFirst( dispatch1 );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 1 );

    channel.enqueueFirst( dispatch2 );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 2 );

    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch1 );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch2 );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testPeek() {

    SimplePriorityMessageDispatchChannel channel;
    Pointer<MessageDispatch> dispatch1( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch2( new MessageDispatch() );

    Pointer<Message> message1( new Message() );
    Pointer<Message> message2( new Message() );

    message1->setPriority( 2 );
    message2->setPriority( 1 );

    dispatch1->setMessage( message1 );
    dispatch2->setMessage( message2 );

    CPPUNIT_ASSERT( channel.isEmpty() == true );
    CPPUNIT_ASSERT( channel.size() == 0 );

    channel.enqueueFirst( dispatch1 );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 1 );

    channel.enqueueFirst( dispatch2 );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 2 );

    CPPUNIT_ASSERT( channel.peek() == NULL );

    channel.start();

    CPPUNIT_ASSERT( channel.peek() == dispatch1 );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch1 );
    CPPUNIT_ASSERT( channel.peek() == dispatch2 );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch2 );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testDequeueNoWait() {

    SimplePriorityMessageDispatchChannel channel;

    Pointer<MessageDispatch> dispatch1( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch2( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch3( new MessageDispatch() );

    Pointer<Message> message1( new Message() );
    Pointer<Message> message2( new Message() );
    Pointer<Message> message3( new Message() );

    message1->setPriority( 2 );
    message2->setPriority( 3 );
    message3->setPriority( 1 );

    dispatch1->setMessage( message1 );
    dispatch2->setMessage( message2 );
    dispatch3->setMessage( message3 );

    CPPUNIT_ASSERT( channel.isRunning() == false );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == NULL );

    channel.enqueue( dispatch1 );
    channel.enqueue( dispatch2 );
    channel.enqueue( dispatch3 );

    CPPUNIT_ASSERT( channel.dequeueNoWait() == NULL );
    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == true );

    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 3 );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch2 );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch1 );
    CPPUNIT_ASSERT( channel.dequeueNoWait() == dispatch3 );

    CPPUNIT_ASSERT( channel.size() == 0 );
    CPPUNIT_ASSERT( channel.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testDequeue() {

    SimplePriorityMessageDispatchChannel channel;

    Pointer<MessageDispatch> dispatch1( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch2( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch3( new MessageDispatch() );

    Pointer<Message> message1( new Message() );
    Pointer<Message> message2( new Message() );
    Pointer<Message> message3( new Message() );

    message1->setPriority( 2 );
    message2->setPriority( 3 );
    message3->setPriority( 1 );

    dispatch1->setMessage( message1 );
    dispatch2->setMessage( message2 );
    dispatch3->setMessage( message3 );

    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == true );

    long long timeStarted = System::currentTimeMillis();

    CPPUNIT_ASSERT( channel.dequeue( 1000 ) == NULL );

    CPPUNIT_ASSERT( System::currentTimeMillis() - timeStarted >= 999 );

    channel.enqueue( dispatch1 );
    channel.enqueue( dispatch2 );
    channel.enqueue( dispatch3 );
    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 3 );
    CPPUNIT_ASSERT( channel.dequeue( -1 ) == dispatch2 );
    CPPUNIT_ASSERT( channel.dequeue( 0 ) == dispatch1 );
    CPPUNIT_ASSERT( channel.dequeue( 1000 ) == dispatch3 );

    CPPUNIT_ASSERT( channel.size() == 0 );
    CPPUNIT_ASSERT( channel.isEmpty() == true );
}

////////////////////////////////////////////////////////////////////////////////
void SimplePriorityMessageDispatchChannelTest::testRemoveAll() {

    SimplePriorityMessageDispatchChannel channel;

    Pointer<MessageDispatch> dispatch1( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch2( new MessageDispatch() );
    Pointer<MessageDispatch> dispatch3( new MessageDispatch() );

    Pointer<Message> message1( new Message() );
    Pointer<Message> message2( new Message() );
    Pointer<Message> message3( new Message() );

    message1->setPriority( 2 );
    message2->setPriority( 3 );
    message3->setPriority( 1 );

    dispatch1->setMessage( message1 );
    dispatch2->setMessage( message2 );
    dispatch3->setMessage( message3 );

    channel.enqueue( dispatch1 );
    channel.enqueue( dispatch2 );
    channel.enqueue( dispatch3 );

    channel.start();
    CPPUNIT_ASSERT( channel.isRunning() == true );
    CPPUNIT_ASSERT( channel.isEmpty() == false );
    CPPUNIT_ASSERT( channel.size() == 3 );
    CPPUNIT_ASSERT( channel.removeAll().size() == 3 );
    CPPUNIT_ASSERT( channel.size() == 0 );
    CPPUNIT_ASSERT( channel.isEmpty() == true );
}
