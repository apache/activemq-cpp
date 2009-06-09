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

#include "TimeUnitTest.h"

#include <string>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testConvert1() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.convert( t, TimeUnit::SECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.convert( 1000 * t, TimeUnit::MILLISECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.convert( 1000000 * t, TimeUnit::MICROSECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.convert( 1000000000 * t, TimeUnit::NANOSECONDS ) );
        CPPUNIT_ASSERT( 1000 * t == TimeUnit::MILLISECONDS.convert( t, TimeUnit::SECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.convert( t, TimeUnit::MILLISECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.convert( 1000 * t, TimeUnit::MICROSECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.convert( 1000000 * t, TimeUnit::NANOSECONDS ) );
        CPPUNIT_ASSERT( 1000000 * t == TimeUnit::MICROSECONDS.convert( t, TimeUnit::SECONDS ) );
        CPPUNIT_ASSERT( 1000 * t == TimeUnit::MICROSECONDS.convert( t, TimeUnit::MILLISECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.convert( t, TimeUnit::MICROSECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.convert( 1000 * t, TimeUnit::NANOSECONDS ) );
        CPPUNIT_ASSERT( 1000000000 * t == TimeUnit::NANOSECONDS.convert( t, TimeUnit::SECONDS ) );
        CPPUNIT_ASSERT( 1000000 * t == TimeUnit::NANOSECONDS.convert( t, TimeUnit::MILLISECONDS ) );
        CPPUNIT_ASSERT( 1000 * t == TimeUnit::NANOSECONDS.convert( t, TimeUnit::MICROSECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.convert( t, TimeUnit::NANOSECONDS ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testConvert2() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( t == TimeUnit::DAYS.convert( t * 24, TimeUnit::HOURS ) );
        CPPUNIT_ASSERT( t == TimeUnit::HOURS.convert( t * 60, TimeUnit::MINUTES ) );
        CPPUNIT_ASSERT( t == TimeUnit::MINUTES.convert( t * 60, TimeUnit::SECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.convert( t * 1000, TimeUnit::MILLISECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.convert( t * 1000, TimeUnit::MICROSECONDS ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.convert( t * 1000, TimeUnit::NANOSECONDS ) );
        CPPUNIT_ASSERT( t * 24 == TimeUnit::HOURS.convert( t, TimeUnit::DAYS ) );
        CPPUNIT_ASSERT( t * 60 == TimeUnit::MINUTES.convert( t, TimeUnit::HOURS ) );
        CPPUNIT_ASSERT( t * 60 == TimeUnit::SECONDS.convert( t, TimeUnit::MINUTES ) );
        CPPUNIT_ASSERT( t * 1000 == TimeUnit::MILLISECONDS.convert( t, TimeUnit::SECONDS ) );
        CPPUNIT_ASSERT( t * 1000 == TimeUnit::MICROSECONDS.convert( t, TimeUnit::MILLISECONDS ) );
        CPPUNIT_ASSERT( t * 1000 == TimeUnit::NANOSECONDS.convert( t, TimeUnit::MICROSECONDS ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToNanos() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( 1000000000 * t == TimeUnit::SECONDS.toNanos( t ) );
        CPPUNIT_ASSERT( 1000000 * t == TimeUnit::MILLISECONDS.toNanos( t ) );
        CPPUNIT_ASSERT( 1000 * t == TimeUnit::MICROSECONDS.toNanos( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toNanos( t ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToMicros() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( 1000000 * t == TimeUnit::SECONDS.toMicros( t ) );
        CPPUNIT_ASSERT( 1000 * t == TimeUnit::MILLISECONDS.toMicros( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.toMicros( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toMicros( t * 1000 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToMillis() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( 1000 * t == TimeUnit::SECONDS.toMillis( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.toMillis( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.toMillis( t * 1000 ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toMillis( t * 1000000 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToSeconds() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.toSeconds( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.toSeconds( t * 1000 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.toSeconds( t * 1000000 ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toSeconds( t * 1000000000 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToMinutes() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( t == TimeUnit::MINUTES.toMinutes( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.toMinutes( t * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.toMinutes( t * 1000LL * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.toMinutes( t * 1000000LL * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toMinutes( t * 1000000000LL * 60 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToHours() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( t == TimeUnit::HOURS.toHours( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::MINUTES.toHours( t * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.toHours( t * 60 * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.toHours( t * 1000LL * 60 * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.toHours( t * 1000000LL * 60 * 60 ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toHours( t * 1000000000LL * 60 * 60 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToDays() {

    for( long long t = 0; t < 10; ++t ) {
        CPPUNIT_ASSERT( t == TimeUnit::DAYS.toDays( t ) );
        CPPUNIT_ASSERT( t == TimeUnit::HOURS.toDays( t * 24 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MINUTES.toDays( t * 60 * 24 ) );
        CPPUNIT_ASSERT( t == TimeUnit::SECONDS.toDays( t * 60 * 60 * 24 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MILLISECONDS.toDays( t * 1000LL * 60 * 60 * 24 ) );
        CPPUNIT_ASSERT( t == TimeUnit::MICROSECONDS.toDays( t * 1000000LL * 60 * 60 * 24 ) );
        CPPUNIT_ASSERT( t == TimeUnit::NANOSECONDS.toDays( t * 1000000000LL * 60 * 60 * 24 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testConvertSaturate() {
    CPPUNIT_ASSERT( Long::MAX_VALUE ==
        TimeUnit::NANOSECONDS.convert( Long::MAX_VALUE / 2, TimeUnit::SECONDS ) );
    CPPUNIT_ASSERT( Long::MIN_VALUE ==
        TimeUnit::NANOSECONDS.convert( -Long::MAX_VALUE / 4, TimeUnit::SECONDS ) );
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToNanosSaturate() {
    CPPUNIT_ASSERT( Long::MAX_VALUE ==
        TimeUnit::MILLISECONDS.toNanos( Long::MAX_VALUE / 2 ) );
    CPPUNIT_ASSERT( Long::MIN_VALUE ==
        TimeUnit::MILLISECONDS.toNanos( -Long::MAX_VALUE / 3 ) );
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testToString() {
    std::string s = TimeUnit::SECONDS.toString();
    CPPUNIT_ASSERT( s.find_first_of( "ECOND" ) != (std::size_t)0 );
}

//////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testTimedWait() {

    Mutex o;

    long long now = System::currentTimeMillis();
    synchronized( &o ) {
        TimeUnit::SECONDS.timedWait( &o, 1 );
    }
    long long later = System::currentTimeMillis();

    CPPUNIT_ASSERT( later - now + 10 >= TimeUnit::SECONDS.toMillis( 1 ) );
}

//////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testSleep() {

    long long now = System::currentTimeMillis();
    TimeUnit::SECONDS.sleep( 1 );
    long long later = System::currentTimeMillis();
    CPPUNIT_ASSERT( later - now + 10 >= TimeUnit::SECONDS.toMillis( 1 ) );
}

//////////////////////////////////////////////////////////////////////////////////
void TimeUnitTest::testValueOf() {

    CPPUNIT_ASSERT( TimeUnit::NANOSECONDS == TimeUnit::valueOf( "NANOSECONDS" ) );
    CPPUNIT_ASSERT( TimeUnit::MICROSECONDS == TimeUnit::valueOf( "MICROSECONDS" ) );
    CPPUNIT_ASSERT( TimeUnit::MILLISECONDS == TimeUnit::valueOf( "MILLISECONDS" ) );
    CPPUNIT_ASSERT( TimeUnit::SECONDS == TimeUnit::valueOf( "SECONDS" ) );
    CPPUNIT_ASSERT( TimeUnit::MINUTES == TimeUnit::valueOf( "MINUTES" ) );
    CPPUNIT_ASSERT( TimeUnit::DAYS == TimeUnit::valueOf( "DAYS" ) );
    CPPUNIT_ASSERT( TimeUnit::HOURS == TimeUnit::valueOf( "HOURS" ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArgumentException",
        TimeUnit::valueOf( "FOO" ),
        decaf::lang::exceptions::IllegalArgumentException );
}
