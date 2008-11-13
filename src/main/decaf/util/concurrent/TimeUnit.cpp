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

#include "TimeUnit.h"

#include <decaf/lang/Long.h>
#include <decaf/lang/Thread.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
const TimeUnit TimeUnit::NANOSECONDS( 0, "NANOSECONDS" );
const TimeUnit TimeUnit::MICROSECONDS( 1, "MICROSECONDS" );
const TimeUnit TimeUnit::MILLISECONDS( 2, "MILLISECONDS" );
const TimeUnit TimeUnit::SECONDS( 3, "SECONDS" );
const TimeUnit TimeUnit::MINUTES( 4, "MINUTES" );
const TimeUnit TimeUnit::HOURS( 5, "HOURS" );
const TimeUnit TimeUnit::DAYS( 6, "DAYS" );

////////////////////////////////////////////////////////////////////////////////
const TimeUnit* const TimeUnit::values[] = {
    &NANOSECONDS,
    &MICROSECONDS,
    &MILLISECONDS,
    &SECONDS,
    &MINUTES,
    &HOURS,
    &DAYS
};

////////////////////////////////////////////////////////////////////////////////
const long long TimeUnit::multipliers[] = {
    1,
    1000LL,
    1000LL * 1000LL,
    1000LL * 1000LL * 1000LL,
    1000LL * 1000LL * 1000LL * 60LL,
    1000LL * 1000LL * 1000LL * 60LL * 60LL,
    1000LL * 1000LL * 1000LL * 60LL * 60LL * 24LL,
};

////////////////////////////////////////////////////////////////////////////////
TimeUnit::TimeUnit( int index, const std::string& name ) {
    this->index = index;
    this->name = name;
}

////////////////////////////////////////////////////////////////////////////////
long long TimeUnit::convert( long long sourceDuration, const TimeUnit& sourceUnit ) const {
    return this->doConvert( sourceUnit.index, this->index, sourceDuration );
}

////////////////////////////////////////////////////////////////////////////////
long long TimeUnit::doConvert( int srcIndex, int destIndex, long long duration ) const {

    if( duration == 0 ) {
        return duration;
    } else if( srcIndex > destIndex ) {
        return scale( duration,
                      multipliers[srcIndex] / multipliers[destIndex],
                      Long::MAX_VALUE / ( multipliers[srcIndex] / multipliers[destIndex] ) );
    } else if( srcIndex < destIndex ) {
        return duration / ( multipliers[destIndex] / multipliers[srcIndex] );
    }

    // Same unit, no conversion.
    return duration;
}

////////////////////////////////////////////////////////////////////////////////
int TimeUnit::excessNanos( long long time, long long ms ) const {

    if( *this == NANOSECONDS ) {
        return (int)( time - ( ms * 1000 * 1000 ) );
    } else if( *this == MICROSECONDS ) {
        return (int)( ( time * 1000 ) - ( ms * 1000 * 1000 ) );
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnit::sleep( long long timeout ) const {
    if( timeout > 0 ) {
        long long ms = toMillis( timeout );
        int ns = excessNanos( timeout, ms );
        Thread::sleep( (int)ms );
        // TODO - Only have a wait for Milliseconds currently.
        //Thread::sleep( ms, ns );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimeUnit::timedWait( Synchronizable* obj, long long timeout ) const {
    if( timeout > 0 ) {
        long long ms = toMillis( timeout );
        int ns = excessNanos( timeout, ms );
        obj->wait( (unsigned long)ms );
        // TODO - Only have a wait for Milliseconds currently.
        //obj.wait( ms, ns );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string TimeUnit::toString() const {
    return this->name;
}

////////////////////////////////////////////////////////////////////////////////
int TimeUnit::compareTo( const TimeUnit& value ) const {
    return index == value.index ? 0 : ( index > value.index ? 1 : -1 );
}

////////////////////////////////////////////////////////////////////////////////
bool TimeUnit::equals( const TimeUnit& value ) const {
    return index == value.index;
}

////////////////////////////////////////////////////////////////////////////////
bool TimeUnit::operator==( const TimeUnit& value ) const {
    return index == value.index;
}

////////////////////////////////////////////////////////////////////////////////
bool TimeUnit::operator<( const TimeUnit& value ) const {
    return this->compareTo( value ) == -1;
}

////////////////////////////////////////////////////////////////////////////////
long long TimeUnit::scale( long long duration, long long multiplier, long long overflow ) {
    if( duration > overflow ) {
        return Long::MAX_VALUE;
    } else if( duration < -overflow ) {
        return Long::MIN_VALUE;
    }

    return duration * multiplier;
}

////////////////////////////////////////////////////////////////////////////////
const TimeUnit& TimeUnit::valueOf( const std::string& name )
    throw ( decaf::lang::exceptions::IllegalArgumentException ) {

    for( int i = 0; i < 7; ++i ) {
        if( values[i]->name == name ) {
            return *values[i];
        }
    }

    throw IllegalArgumentException(
        __FILE__, __LINE__,
        "Passed TimeUnit name; %s, Does not match any instances of TimeUnit",
        name.c_str() );
}
