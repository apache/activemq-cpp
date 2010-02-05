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

#include "StreamHandler.h"

#include <decaf/util/logging/Level.h>
#include <decaf/io/OutputStream.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
StreamHandler::StreamHandler() : level( Level::SEVERE ) {

    this->stream = NULL;
    this->formatter = NULL;
    this->filter = NULL;
}

////////////////////////////////////////////////////////////////////////////////
StreamHandler::StreamHandler( OutputStream* stream, Formatter* formatter ) : level( Level::SEVERE ) {

    this->stream = stream;
    this->formatter = formatter;
    this->filter = NULL;
}

////////////////////////////////////////////////////////////////////////////////
StreamHandler::~StreamHandler() {

    try {
        this->close();
    }
    DECAF_CATCH_NOTHROW( lang::Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::close() throw ( decaf::io::IOException ) {
    if( stream ) {
        stream->flush();
        stream->close();
    }
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::flush() {
    if( stream ) {
        stream->flush();
    }
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::publish( const LogRecord& record ) {

    try {

        if( !stream ) {
            throw lang::exceptions::NullPointerException(
                __FILE__, __LINE__,
                "StreamHandler::publish - Stream not set.");
        }

        // Check if we should log this record
        if( isLoggable( record ) ) {

            std::string log = formatter->format( record );

            synchronized( stream ) {
                // Write the data to the stream
                stream->write( (const unsigned char*)log.c_str(), 0, log.length() );
            }
        }
    }
    DECAF_CATCH_RETHROW( lang::Exception )
    DECAF_CATCHALL_THROW( lang::Exception )
}

////////////////////////////////////////////////////////////////////////////////
bool StreamHandler::isLoggable( const LogRecord& record ) const {

    if( filter ) {
        // Allow for some filtering to occur
        return filter->isLoggable( record );
    }

    // By default we want everything that is greater than our
    // equal to the set level of this Handler.
    return record.getLevel().compareTo( this->level ) >= 0;
}
