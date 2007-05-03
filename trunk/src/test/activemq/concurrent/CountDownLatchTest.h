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

#ifndef _ACTIVEMQ_CONCURRENT_COUNTDOWNLATCHTEST_H_
#define _ACTIVEMQ_CONCURRENT_COUNTDOWNLATCHTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/CountDownLatch.h>

namespace activemq{
namespace concurrent{

    class CountDownLatchTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( CountDownLatchTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( test2 );
        CPPUNIT_TEST_SUITE_END();

    protected:

        class MyThread : public Thread
        {
        public:

            CountDownLatch* latch;

        public:

            MyThread(){}
            virtual ~MyThread(){}

            virtual void run(){

                while( latch->getCount() > 0 ) {
                    latch->countDown();

                    Thread::sleep( 20 );
                }
            }

        };

    public:

        CountDownLatchTest() {}
        virtual ~CountDownLatchTest() {}

        virtual void test();
        virtual void test2();
    };

}}

#endif /*_ACTIVEMQ_CONCURRENT_COUNTDOWNLATCHTEST_H_*/
