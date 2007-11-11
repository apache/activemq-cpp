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

#include "OpenWireCommandWriter.h"
#include <iostream>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
OpenWireCommandWriter::OpenWireCommandWriter() {

    this->outputStream = NULL;
    this->openWireFormat = NULL;
    this->dataOutputStream = NULL;
}

////////////////////////////////////////////////////////////////////////////////
OpenWireCommandWriter::OpenWireCommandWriter( OutputStream* outputStream,
                                              OpenWireFormat* openWireFormat ) {

    this->setOutputStream( outputStream );
    this->openWireFormat = openWireFormat;
}

////////////////////////////////////////////////////////////////////////////////
OpenWireCommandWriter::~OpenWireCommandWriter() {

    if( dataOutputStream != NULL ) {
        delete dataOutputStream;
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireCommandWriter::writeCommand( Command* command )
    throw ( transport::CommandIOException ) {

    try{

        if( outputStream == NULL ||
            dataOutputStream == NULL ||
            openWireFormat == NULL ) {

            throw CommandIOException(
                __FILE__, __LINE__,
                "OpenWireCommandWriter::writeCommand - "
                "Writer was not initialized correctly." );
        }

        openWireFormat->marshal( command, dataOutputStream );

        // Now ensure that it gets sent.
        dataOutputStream->flush();
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireCommandWriter::write( const unsigned char* buffer, size_t count )
    throw( IOException ) {

    if( outputStream == NULL ) {
        throw IOException(
            __FILE__, __LINE__,
            "OpenWireCommandWriter::write(char*,int) - input stream is NULL" );
    }

    try{
        outputStream->write( buffer, 0, count );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireCommandWriter::writeByte( unsigned char v ) throw( IOException ) {

    if( outputStream == NULL ) {
        throw IOException(
            __FILE__, __LINE__,
            "OpenWireCommandWriter::write(char) - input stream is NULL" );
    }

    try{
        outputStream->write( v );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
