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
 
#include "LoggingInputStream.h"
#include <sstream>
#include <iomanip>

using namespace std;
using namespace activemq;
using namespace activemq::io;

LOGCMS_INITIALIZE( logger, LoggingInputStream, "activemq.io.LoggingInputStream")

////////////////////////////////////////////////////////////////////////////////
LoggingInputStream::LoggingInputStream( InputStream* inputStream, bool own )
 : FilterInputStream( inputStream, own )
{
}

////////////////////////////////////////////////////////////////////////////////
LoggingInputStream::~LoggingInputStream()
{
}

////////////////////////////////////////////////////////////////////////////////
unsigned char LoggingInputStream::read() throw ( IOException ) {
    try {
        unsigned char c = FilterInputStream::read();
        
        log( &c, 1 );
        
        return c;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t LoggingInputStream::read( unsigned char* buffer, std::size_t bufferSize ) 
    throw ( IOException )
{
    try {
        std::size_t numRead = FilterInputStream::read( buffer, bufferSize );
        
        log( buffer, numRead );
        
        return numRead;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void LoggingInputStream::log( const unsigned char* buffer, size_t len ) {
        
    ostringstream ostream;
    
    ostream << "TCP Trace: Reading: " << endl << "[";
    
    for( size_t ix=0; ix<len; ++ix ){
        ostream << setw(2) << setfill('0') << std::hex << (int)buffer[ix];
        
        if( ((ix+1) % 2) == 0 ){
            ostream << ' ';
        }
    }
    
    ostream << "] len: " << std::dec << len << " bytes";
        
    // Log the data
    LOGCMS_INFO( logger, ostream.str() )
}
