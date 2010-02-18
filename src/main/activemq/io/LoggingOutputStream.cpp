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

#include "LoggingOutputStream.h"
#include <sstream>
#include <iomanip>
#include <activemq/exceptions/ExceptionDefines.h>

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

LOGDECAF_INITIALIZE( logger, LoggingOutputStream, "activemq.io.LoggingOutputStream")

////////////////////////////////////////////////////////////////////////////////
LoggingOutputStream::LoggingOutputStream( OutputStream* outputStream, bool own )
 : decaf::io::FilterOutputStream( outputStream, own ) {}

////////////////////////////////////////////////////////////////////////////////
LoggingOutputStream::~LoggingOutputStream() {}

////////////////////////////////////////////////////////////////////////////////
void LoggingOutputStream::write( const unsigned char c ) throw ( IOException ) {
    try {

        log( &c, 1 );
        FilterOutputStream::write( c );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void LoggingOutputStream::write( const unsigned char* buffer, std::size_t size,
                                 std::size_t offset, std::size_t length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::NullPointerException,
            decaf::lang::exceptions::IndexOutOfBoundsException ) {

    try {

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "LoggingOutputStream::write - Passed Buffer is Null" );
        }

        if( ( offset + length ) > size ) {
            throw decaf::lang::exceptions::IndexOutOfBoundsException(
                __FILE__, __LINE__,
                "DataOutputStream::write - given offset + length is greater than buffer size.");
        }

        log( buffer + offset, length );

        FilterOutputStream::write( buffer, size, offset, length );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( IndexOutOfBoundsException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void LoggingOutputStream::log( const unsigned char* buffer, size_t len ) {

    // Write the buffer as hex to a string stream.
    ostringstream ostream;
    ostream << "TCP Trace: Writing:" << endl << '[';

    for( size_t ix=0; ix<len; ++ix ){
        ostream << setw(2) << setfill('0') << std::hex << (int)buffer[ix];

        if( ((ix+1) % 2) == 0 ){
            ostream << ' ';
        }
    }

    ostream << "] len: " << std::dec << len << " bytes";

    // Log the data
    LOGDECAF_INFO( logger, ostream.str() )
}
