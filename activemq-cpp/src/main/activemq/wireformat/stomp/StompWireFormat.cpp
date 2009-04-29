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

#include "StompWireFormat.h"

#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/wireformat/stomp/StompCommandConstants.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Integer.h>
#include <decaf/io/IOException.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StompWireFormat::StompWireFormat() {
}

////////////////////////////////////////////////////////////////////////////////
StompWireFormat::~StompWireFormat() {
}

////////////////////////////////////////////////////////////////////////////////
void StompWireFormat::marshal( const Pointer<Command>& command, decaf::io::DataOutputStream* out )
    throw ( decaf::io::IOException ) {

    try{

        if( out == NULL ) {
            throw decaf::io::IOException(
                __FILE__, __LINE__,
                "StompCommandWriter::writeCommand - "
                "output stream is NULL" );
        }

        Pointer<StompFrame> frame = marshaler.marshal( command );

        // Some commands just don't translate to Stomp Commands, we ignore them
        // and hope that bad things don't happen.
        if( frame == NULL ) {
            return;
        }

        // Write the command.
        const string& cmdString = frame->getCommand();
        out->write( (unsigned char*)cmdString.c_str(), 0, cmdString.length() );
        out->writeByte( '\n' );

        // Write all the headers.
        vector< pair<string,string> > headers = frame->getProperties().toArray();
        for( std::size_t ix=0; ix < headers.size(); ++ix ) {
            string& name = headers[ix].first;
            string& value = headers[ix].second;

            out->write( (unsigned char*)name.c_str(), 0, name.length() );
            out->writeByte( ':' );
            out->write( (unsigned char*)value.c_str(), 0, value.length() );
            out->writeByte( '\n' );
        }

        // Finish the header section with a form feed.
        out->writeByte( '\n' );

        // Write the body.
        const std::vector<unsigned char>& body = frame->getBody();
        if( body.size() > 0 ) {
            out->write( &body[0], 0, body.size() );
        }

        if( ( frame->getBodyLength() == 0 ) ||
            ( frame->getProperty( StompCommandConstants::HEADER_CONTENTLENGTH ) != "" ) ) {

            out->writeByte( '\0' );
        }

        out->writeByte( '\n' );

        // Flush the stream.
        out->flush();
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> StompWireFormat::unmarshal( decaf::io::DataInputStream* in )
    throw ( decaf::io::IOException ) {

    Pointer<StompFrame> frame;

    try{

        // Create a new Frame for reading to.
        frame.reset( new StompFrame() );

        // Read the command header.
        readStompCommandHeader( frame, in );

        // Read the headers.
        readStompHeaders( frame, in );

        // Read the body.
        readStompBody( frame, in );

        // Return the Command.
        return marshaler.marshal( frame );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<transport::Transport> StompWireFormat::createNegotiator(
    const Pointer<transport::Transport>& transport AMQCPP_UNUSED )
        throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    throw UnsupportedOperationException( __FILE__, __LINE__,
        "No Negotiator is required to use this WireFormat." );
}

////////////////////////////////////////////////////////////////////////////////
void StompWireFormat::readStompCommandHeader( Pointer<StompFrame>& frame, decaf::io::DataInputStream* in )
   throw ( decaf::io::IOException ) {

    try{

        while( true ) {

            // The command header is formatted
            // just like any other stomp header.
            readStompHeaderLine( in );

            // Ignore all white space before the command.
            long long offset = -1;
            for( size_t ix = 0; ix < buffer.size()-1; ++ix ) {

                // Find the first non whitespace character
                if( !Character::isWhitespace( buffer[ix] ) ){
                    offset = (long long)ix;
                    break;
                }
            }

            if( offset >= 0 ) {
                // Set the command in the frame - copy the memory.
                frame->setCommand( reinterpret_cast<char*>( &buffer[(size_t)offset] ) );
                break;
            }
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void StompWireFormat::readStompHeaders( Pointer<StompFrame>& frame, decaf::io::DataInputStream* in )
    throw ( decaf::io::IOException ) {

    try{

        // Read the command;
        bool endOfHeaders = false;

        while( !endOfHeaders ) {

            // Read in the next header line.
            std::size_t numChars = readStompHeaderLine( in );

            if( numChars == 0 ) {

                // should never get here
                throw decaf::io::IOException(
                    __FILE__, __LINE__,
                    "StompWireFormat::readStompHeaders: no characters read" );
            }

            // Check for an empty line to demark the end of the header section.
            // if its not the end then we have a header to process, so parse it.
            if( numChars == 1 && buffer[0] == '\0' ) {

                endOfHeaders = true;

            } else {

                // Search through this line to separate the key/value pair.
                for( size_t ix = 0; ix < buffer.size(); ++ix ) {

                    // If found the key/value separator...
                    if( buffer[ix] == ':' ) {

                        // Null-terminate the key.
                        buffer[ix] = '\0';

                        const char* key = reinterpret_cast<char*>( &buffer[0] );
                        const char* value = reinterpret_cast<char*>( &buffer[ix+1] );

                        // Assign the header key/value pair.
                        frame->getProperties().setProperty( key, value );

                        // Break out of the for loop.
                        break;
                    }
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t StompWireFormat::readStompHeaderLine( decaf::io::DataInputStream* in )
    throw ( decaf::io::IOException ) {

    try{

        // Clear any data from the buffer.
        buffer.clear();

        std::size_t count = 0;

        while( true ) {

            // Read the next char from the stream.
            buffer.push_back( in->readByte() );

            // Increment the position pointer.
            count++;

            // If we reached the line terminator, return the total number
            // of characters read.
            if( buffer[count-1] == '\n' ) {
                // Overwrite the line feed with a null character.
                buffer[count-1] = '\0';
                return count;
            }
        }

        // If we get here something bad must have happened.
        throw decaf::io::IOException(
            __FILE__, __LINE__,
            "StompWireFormat::readStompHeaderLine: "
            "Unrecoverable, error condition");
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void StompWireFormat::readStompBody( Pointer<StompFrame>& frame, decaf::io::DataInputStream* in )
   throw ( decaf::io::IOException ) {

    try{

        // Clear any data from the buffer.
        buffer.clear();

        unsigned int content_length = 0;

        if( frame->hasProperty( StompCommandConstants::HEADER_CONTENTLENGTH ) ) {
            string length = frame->getProperty( StompCommandConstants::HEADER_CONTENTLENGTH );
            content_length = (unsigned int)Integer::parseInt( length );
         }

         if( content_length != 0 ) {
            // For this case its assumed that content length indicates how
            // much to read.  We reserve space in the buffer for it to
            // minimize the number of reallocs that might occur.  We are
            // assuming that content length doesn't count the trailing null
            // that indicates the end of frame.  The reserve won't do anything
            // if the buffer already has that much capacity.  The resize call
            // basically sets the end iterator to the correct location since
            // this is a char vector and we already reserve enough space.
            // Resize doesn't realloc the vector smaller if content_length
            // is less than capacity of the buffer, it just move the end
            // iterator.  Reserve adds the benefit that the mem is set to
            // zero.  Over time as larger messages come in this will cause
            // us to adapt to that size so that future messages that are
            // around that size won't alloc any new memory.

            buffer.reserve( (std::size_t)content_length );
            buffer.resize( (std::size_t)content_length );

            // Read the Content Length now
            in->read( &buffer[0], 0, content_length );

            // Content Length read, now pop the end terminator off (\0\n).
            if( in->readByte() != '\0' ) {

                throw decaf::io::IOException(
                    __FILE__, __LINE__,
                    "StompWireFormat::readStompBody: "
                    "Read Content Length, and no trailing null");
            }

        } else {

            // Content length was either zero, or not set, so we read until the
            // first null is encountered.
            while( true ) {

                char byte = in->readByte();

                buffer.push_back(byte);

                content_length++;

                if( byte != '\0' ) {
                    continue;
                }

                break;  // Read null and newline we are done.
            }
        }

        if( content_length != 0 ) {
            // Set the body contents in the frame - copy the memory
            frame->getBody() = buffer;
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}
