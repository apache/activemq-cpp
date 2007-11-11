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
#include "StompCommandWriter.h"

#include <activemq/connector/stomp/StompFrame.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::stomp;
using namespace activemq::connector::stomp::commands;
using namespace activemq::transport;
using namespace decaf::io;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
StompCommandWriter::StompCommandWriter() {
    outputStream = NULL;
}

////////////////////////////////////////////////////////////////////////////////
StompCommandWriter::StompCommandWriter( OutputStream* os ) {
    outputStream = os;
}

////////////////////////////////////////////////////////////////////////////////
void StompCommandWriter::writeCommand( Command* command )
    throw ( transport::CommandIOException ) {

    try{

        if( outputStream == NULL ) {
            throw CommandIOException(
                __FILE__, __LINE__,
                "StompCommandWriter::writeCommand - "
                "output stream is NULL" );
        }

        const StompFrame& frame = marshaler.marshal( command );

        // Write the command.
        const string& cmdString = frame.getCommand();
        write( cmdString.c_str(), cmdString.length() );
        writeByte( '\n' );

        // Write all the headers.
        vector< pair<string,string> > headers = frame.getProperties().toArray();
        for( std::size_t ix=0; ix < headers.size(); ++ix ) {
            string& name = headers[ix].first;
            string& value = headers[ix].second;

            write( name.c_str(), name.length() );
            writeByte( ':' );
            write( value.c_str(), value.length() );
            writeByte( '\n' );
        }

        // Finish the header section with a form feed.
        writeByte( '\n' );

        // Write the body.
        const std::vector<unsigned char>& body = frame.getBody();
        if( body.size() > 0 ) {
            write( &body[0], body.size() );
        }

        if( ( frame.getBodyLength() == 0 ) ||
            ( frame.getProperties().getProperty(
                  CommandConstants::toString(
                      CommandConstants::HEADER_CONTENTLENGTH ), "" ) != "" ) ) {

            writeByte( '\0' );
        }

        writeByte( '\n' );

        // Flush the stream.
        outputStream->flush();
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void StompCommandWriter::write( const unsigned char* buffer, std::size_t count )
    throw( IOException ) {

    if( outputStream == NULL ) {
        throw IOException(
        __FILE__, __LINE__,
        "StompCommandWriter::write(char*,int) - output stream is NULL" );
    }

    try {
        outputStream->write( buffer, 0, count );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void StompCommandWriter::writeByte( unsigned char v ) throw( IOException ) {

    if( outputStream == NULL ) {
        throw IOException(
        __FILE__, __LINE__,
        "StompCommandWriter::writeByte( char ) - output stream is NULL" );
    }

    try {
        outputStream->write( v );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void StompCommandWriter::write( const char* buffer, std::size_t count )
   throw( decaf::io::IOException )
{
    if( outputStream == NULL ) {
        throw IOException(
        __FILE__, __LINE__,
        "StompCommandWriter::write() - output stream is NULL" );
    }

    try {
        write( reinterpret_cast<const unsigned char*>( buffer ), count );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
