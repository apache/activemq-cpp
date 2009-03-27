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

#include "ThreadTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;

void ThreadTest::testDelegate(){

      Delegate test;
      int initialValue = test.getStuff();

      Thread thread( &test );
      thread.start();
      thread.join();

      int finalValue = test.getStuff();

      // The values should be different - this proves
      // that the runnable was run.
      CPPUNIT_ASSERT( finalValue != initialValue );
}

void ThreadTest::testDerived(){

      Derived test;
      int initialValue = test.getStuff();

      test.start();
      test.join();

      int finalValue = test.getStuff();

      // The values should be different - this proves
      // that the runnable was run.
      CPPUNIT_ASSERT( finalValue != initialValue );
}

void ThreadTest::testJoin(){

      JoinTest test;

      time_t startTime = time( NULL );
      test.start();
      test.join();
      time_t endTime = time( NULL );

      time_t delta = endTime - startTime;

      // Should be about 5 seconds that elapsed.
      CPPUNIT_ASSERT( delta >= 1 && delta <= 3 );
}
