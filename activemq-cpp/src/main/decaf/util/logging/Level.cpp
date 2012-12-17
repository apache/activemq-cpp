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

#include "Level.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <decaf/util/concurrent/Mutex.h>

#include <vector>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
namespace {
    std::vector<Level> levels;
}

////////////////////////////////////////////////////////////////////////////////
const Level Level::INHERIT( "INHERIT", 0 );
const Level Level::OFF( "OFF", Integer::MAX_VALUE );
const Level Level::SEVERE( "SEVERE", 1000 );
const Level Level::WARNING( "WARNING", 900 );
const Level Level::INFO( "INFO", 800 );
const Level Level::DEBUG( "DEBUG", 700 );
const Level Level::CONFIG( "CONFIG", 600 );
const Level Level::FINE( "FINE", 500 );
const Level Level::FINER( "FINER", 400 );
const Level Level::FINEST( "FINEST", 300 );
const Level Level::ALL( "ALL", Integer::MIN_VALUE );

////////////////////////////////////////////////////////////////////////////////
Level::Level( const std::string& name, int value ) : name( name ), value( value ) {
    levels.push_back( *this );
}

////////////////////////////////////////////////////////////////////////////////
Level::~Level() {
}

////////////////////////////////////////////////////////////////////////////////
int Level::compareTo( const Level& value ) const {
    return this->value < value.value ? -1 : this->value > value.value ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Level::equals( const Level& value ) const {
    return this->value == value.value;
}

////////////////////////////////////////////////////////////////////////////////
bool Level::operator==( const Level& value ) const {
    return this->value == value.value;
}

////////////////////////////////////////////////////////////////////////////////
bool Level::operator<( const Level& value ) const {
    return this->value < value.value;
}

////////////////////////////////////////////////////////////////////////////////
Level Level::parse( const std::string& name ) {

    bool isNameAnInt;
    int nameAsInt;

    try {
        nameAsInt = Integer::parseInt( name );
        isNameAnInt = true;
    } catch( NumberFormatException& e ) {
        nameAsInt = 0;
        isNameAnInt = false;
    }

//    synchronized( &levelsLock ) {
//
//        std::vector<Level>::const_iterator level = levels.begin();
//
//        for( ; level != levels.end(); ++level ) {
//            if( name == level->getName() ) {
//                return *level;
//            }
//        }
//
//        // If the name was an int then we loop again to check the int value
//        // against each level's value.
//        if( isNameAnInt ) {
//
//            for( level = levels.begin(); level != levels.end(); ++ level ) {
//                if( nameAsInt == level->intValue()) {
//                    return *level;
//                }
//            }
//        }
//    }

    if( !isNameAnInt ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Could not find match for Level Name: %s", name.c_str() );
    }

    return Level( name, nameAsInt );
}
