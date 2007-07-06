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

#include "PrimitiveMapBenchmark.h"

#include <string>
#include <activemq/concurrent/Thread.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
PrimitiveMapBenchmark::PrimitiveMapBenchmark() {}

////////////////////////////////////////////////////////////////////////////////
PrimitiveMapBenchmark::~PrimitiveMapBenchmark() {}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMapBenchmark::setUp(){
    for( int i = 0; i < 1024; ++i ) {
        testString += "a";
        byteBuffer.push_back( 'a' );
    }
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMapBenchmark::run() {

    int numRuns = 500;

    for( int i = 0; i < numRuns; ++i ){
        map.setBool( "BOOL", true );
        map.setByte( "BYTE", 12 );
        map.setChar( "CHAR", 60 );
        map.setInt( "INT", 54275482 );
        map.setShort( "SHORT", 32767 );
        map.setLong( "LONG", 0xFFLL );
        map.setDouble( "DOUBLE", 1321.1516 );
        map.setFloat( "FLOAT", 45.45f );
        map.setString( "STRING", testString );
        map.setByteArray( "BYTES", byteBuffer );
    }

    for( int i = 0; i < numRuns; ++i ){
        map.getBool( "BOOL" );
        map.getByte( "BYTE" );
        map.getChar( "CHAR" );
        map.getInt( "INT" );
        map.getShort( "SHORT" );
        map.getLong( "LONG" );
        map.getDouble( "DOUBLE" );
        map.getFloat( "FLOAT" );
        map.getString( "STRING" );
        map.getByteArray( "BYTES" );
    }

    for( int i = 0; i < numRuns; ++i ){
        map.getKeys();
        map.getValues();
    }

    for( int i = 0; i < numRuns; ++i ){
        PrimitiveMap theCopy;
        theCopy.copy( map );
    }
}
