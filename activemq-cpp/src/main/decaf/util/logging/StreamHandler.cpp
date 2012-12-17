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
#include <decaf/util/logging/Formatter.h>
#include <decaf/util/logging/ErrorManager.h>
#include <decaf/util/logging/Filter.h>
#include <decaf/io/OutputStream.h>
#include <decaf/io/OutputStreamWriter.h>
#include <decaf/io/Writer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
StreamHandler::StreamHandler() : Handler(), stream(NULL), writer(NULL), writerNotInitialized(true) {
}

////////////////////////////////////////////////////////////////////////////////
StreamHandler::StreamHandler( OutputStream* stream, Formatter* formatter ) :
    Handler(), stream(stream), writer(NULL), writerNotInitialized(true) {

    if( stream == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "OutputStream cannot be NULL." );
    }

    if( formatter == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Formatter cannot be NULL." );
    }

    setFormatter( formatter );
}

////////////////////////////////////////////////////////////////////////////////
StreamHandler::~StreamHandler() {

    try {
        this->close(true);
    }
    DECAF_CATCH_NOTHROW( lang::Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::close() {
    this->close( true );
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::flush() {

    if( this->stream != NULL ) {
        try {
            if( this->writer != NULL ) {
                this->writer->flush();
            } else {
                this->stream->flush();
            }
        } catch( Exception& e ) {
            this->getErrorManager()->error(
                "Failed to flush the output stream", &e, ErrorManager::FLUSH_FAILURE );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::publish( const LogRecord& record ) {

    try {

        if( this->isLoggable( record ) ) {

            if( this->writerNotInitialized ) {
                initializeWritter();
            }

            std::string msg = "";
            try {
                msg = getFormatter()->format( record );
            } catch( Exception& e ) {
                this->getErrorManager()->error(
                    "Failed to format the LogRecord", &e, ErrorManager::FORMAT_FAILURE );
            }

            this->write( msg );
        }
    } catch( Exception& e ) {
        this->getErrorManager()->error(
            "Failed to publish the LogRecord", &e, ErrorManager::GENERIC_FAILURE );
    }
}

////////////////////////////////////////////////////////////////////////////////
bool StreamHandler::isLoggable( const LogRecord& record ) const {

    if( this->stream != NULL && Handler::isLoggable( record ) ) {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::setOuputStream( decaf::io::OutputStream* stream ) {

    if( stream == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "OutputStream cannot be set to NULL." );
    }

    this->close( true );
    this->stream = stream;
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::close( bool closeStream ) {

    if( this->stream != NULL ) {

        if( this->writerNotInitialized ) {
            initializeWritter();
        }

        this->write( getFormatter()->getTail( this ) );

        try {

            this->writer->flush();

            if( closeStream ) {
                this->writer->close();
                this->writer = NULL;
                this->stream = NULL;
            }

        } catch( Exception& e ) {
            this->getErrorManager()->error(
                "Failed to close the OutputStream", &e, ErrorManager::CLOSE_FAILURE );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::write( const std::string& value ) {

    try{
        this->stream->write( (const unsigned char*)value.c_str(), (int)value.length(), 0, (int)value.length() );
    } catch( Exception& e ) {
        this->getErrorManager()->error(
            "Failed to write to the OutputStream", &e, ErrorManager::WRITE_FAILURE );
    }
}

////////////////////////////////////////////////////////////////////////////////
void StreamHandler::initializeWritter() {

    this->writerNotInitialized = false;
    this->writer = new OutputStreamWriter( this->stream );

    // Start the Document by writing out the Head element.
    this->write( getFormatter()->getHead( this ) );
}
