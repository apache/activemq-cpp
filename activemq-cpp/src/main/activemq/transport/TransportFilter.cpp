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

#include "TransportFilter.h"
#include <decaf/io/IOException.h>

using namespace activemq;
using namespace activemq::transport;
using namespace decaf::lang;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
TransportFilter::TransportFilter( const Pointer<Transport>& next ) :
    next( next ), listener( NULL ) {

    // Observe the nested transport for events.
    next->setTransportListener( this );
}

////////////////////////////////////////////////////////////////////////////////
TransportFilter::~TransportFilter() {

}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::onCommand( const Pointer<Command>& command ){
    fire( command );
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::onException( const decaf::lang::Exception& ex ) {
    fire( ex );
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::fire( const decaf::lang::Exception& ex ) {

    if( listener != NULL ){
        try{
            listener->onException( ex );
        }catch( ... ){}
    }
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::fire( const Pointer<Command>& command ) {
    try{
        if( listener != NULL ){
            listener->onCommand( command );
        }
    }catch( ... ){}
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::transportInterrupted() {
    try{
        if( listener != NULL ){
            listener->transportInterrupted();
        }
    }catch( ... ){}
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::transportResumed() {
    try{
        if( listener != NULL ){
            listener->transportResumed();
        }
    }catch( ... ){}
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::start() {

    if( listener == NULL ){
        throw decaf::io::IOException( __FILE__, __LINE__, "exceptionListener is invalid" );
    }

    // Start the delegate transport object.
    next->start();
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::stop() {
    next->stop();
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::close() {

    if( next != NULL ) {
        next->close();
        next.reset( NULL );
    }
}

////////////////////////////////////////////////////////////////////////////////
Transport* TransportFilter::narrow( const std::type_info& typeId ) {
    if( typeid( *this ) == typeId ) {
        return this;
    } else if( this->next != NULL ) {
        return this->next->narrow( typeId );
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void TransportFilter::reconnect( const decaf::net::URI& uri ) {

    try{
        next->reconnect( uri );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
