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

#include "StompFrame.h"

#include <string>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/wireformat/stomp/StompCommandConstants.h>

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StompFrame* StompFrame::clone() const {
    StompFrame* frame = new StompFrame();
    frame->copy( this );
    return frame;
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::copy( const StompFrame* src ) {

    this->setCommand( src->getCommand() );
    this->properties.copy( &( src->getProperties() ) );
    // Use the Vectors assignment operator.
    this->body = src->getBody();
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::setBody( const unsigned char* bytes, std::size_t numBytes ) {

    // Remove old data
    body.clear();

    // Copy data to internal buffer.
    std::back_insert_iterator< std::vector<unsigned char> > iter( body );
    std::copy( bytes, bytes + numBytes, iter );
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::toStream( decaf::io::OutputStream* stream ) const {

    if( stream == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Stream Passed is Null" );
    }

    // Write the command.
    const string& cmdString = this->getCommand();
    stream->write( (unsigned char*)cmdString.c_str(), 0, cmdString.length() );
    stream->write( '\n' );

    // Write all the headers.
    vector< pair<string,string> > headers = this->getProperties().toArray();
    for( std::size_t ix=0; ix < headers.size(); ++ix ) {
        string& name = headers[ix].first;
        string& value = headers[ix].second;

        stream->write( (unsigned char*)name.c_str(), 0, name.length() );
        stream->write( ':' );
        stream->write( (unsigned char*)value.c_str(), 0, value.length() );
        stream->write( '\n' );
    }

    // Finish the header section with a form feed.
    stream->write( '\n' );

    // Write the body.
    const std::vector<unsigned char>& body = this->getBody();
    if( body.size() > 0 ) {
        stream->write( &body[0], 0, body.size() );
    }

    if( ( this->getBodyLength() == 0 ) ||
        ( this->getProperty( StompCommandConstants::HEADER_CONTENTLENGTH ) != "" ) ) {

        stream->write( '\0' );
    }

    stream->write( '\n' );

    // Flush the stream.
    stream->flush();
}
