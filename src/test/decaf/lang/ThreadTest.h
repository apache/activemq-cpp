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

#ifndef _DECAF_LANG_THREADTEST_H_
#define _DECAF_LANG_THREADTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Thread.h>
#include <time.h>

namespace decaf{
namespace lang{

    class ThreadTest : public CppUnit::TestFixture {

      CPPUNIT_TEST_SUITE( ThreadTest );
      CPPUNIT_TEST( testDelegate );
      CPPUNIT_TEST( testDerived );
      CPPUNIT_TEST( testJoin );
      CPPUNIT_TEST_SUITE_END();

    public:

        class Delegate : public Runnable{
        private:

            int stuff;

        public:

            Delegate(){ stuff = 0; }
            virtual ~Delegate(){}

            int getStuff(){
                return stuff;
            }

            virtual void run(){
                stuff = 1;
            }

        };

        class Derived : public Thread{
        private:

            int stuff;

        public:

            Derived(){ stuff = 0; }
            virtual ~Derived(){}

            int getStuff(){
                return stuff;
            }

            virtual void run(){
                stuff = 1;
            }

        };

        class JoinTest : public Thread{
        public:

            JoinTest(){}
            virtual ~JoinTest(){}

            virtual void run(){

                // Sleep for 2 seconds.
                Thread::sleep( 2000 );
            }

        };

    public:

        virtual ~ThreadTest(){}

        virtual void setUp(){}
        virtual void tearDown(){}

        void testDelegate();
        void testDerived();
        void testJoin();
    };

}}

#endif /*_DECAF_LANG_THREADTEST_H_*/
