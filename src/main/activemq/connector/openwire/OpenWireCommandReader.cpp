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

#include "OpenWireCommandReader.h"

#include <decaf/lang/Thread.h>
#include <iostream>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace decaf::io;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
OpenWireCommandReader::OpenWireCommandReader() {

    this->inputStream = NULL;
    this->openWireFormat = NULL;
    this->dataInputStream = NULL;
}

////////////////////////////////////////////////////////////////////////////////
OpenWireCommandReader::OpenWireCommandReader( InputStream* inputStream,
                                              OpenWireFormat* openWireFormat ) {
    this->setInputStream( inputStream );
    this->openWireFormat = openWireFormat;
}

////////////////////////////////////////////////////////////////////////////////
OpenWireCommandReader::~OpenWireCommandReader() {
    if( dataInputStream != NULL ) {
        delete dataInputStream;
    }
}

////////////////////////////////////////////////////////////////////////////////
Command* OpenWireCommandReader::readCommand()
    throw ( CommandIOException ) {

    try{

        if( inputStream == NULL ||
            dataInputStream == NULL ||
            openWireFormat == NULL) {

            throw IOException(
                __FILE__, __LINE__,
                "OpenwireCommandReader::read(char*,int) - class not properly initialized" );
        }

        return openWireFormat->unmarshal( dataInputStream );
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
size_t OpenWireCommandReader::read( unsigned char* buffer, size_t count )
   throw( decaf::io::IOException ) {

    try{

        if( inputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "OpenwireCommandReader::read(char*,int) - input stream is NULL" );
        }

        return inputStream->read( &buffer[0], 0, count );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char OpenWireCommandReader::readByte() throw( decaf::io::IOException ) {

    try{

        if( inputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "OpenwireCommandReader::read(char*,int) - "
                "input stream is NULL" );
        }

        unsigned char c = 0;
        inputStream->read( &c, 0, 1 );
        return c;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
