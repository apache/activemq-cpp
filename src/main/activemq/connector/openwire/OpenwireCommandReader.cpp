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
 
#include "OpenwireCommandReader.h"

#include <activemq/concurrent/Thread.h>

using namespace std;
using namespace activemq;
using namespace activemq::concurrent;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::transport;
using namespace activemq::io;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
OpenwireCommandReader::OpenwireCommandReader(void)
{
    inputStream = NULL;
}

////////////////////////////////////////////////////////////////////////////////
OpenwireCommandReader::OpenwireCommandReader( InputStream* is )
{
    inputStream = is;
}

////////////////////////////////////////////////////////////////////////////////
Command* OpenwireCommandReader::readCommand(void) 
    throw ( CommandIOException )
{
    try
    {
        return NULL;
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
int OpenwireCommandReader::read( unsigned char* buffer, int count ) 
   throw( io::IOException )
{
    try
    {
        if( inputStream == NULL )
        {
            throw IOException( 
                __FILE__, __LINE__, 
                "OpenwireCommandReader::read(char*,int) - input stream is NULL" );
        }
       
        int head = 0;
       
        // We call the read(buffer, size) version asking for one
        // byte, if this returns zero, then there wasn't anything 
        // on the stream to read, so we try again after a short 
        // pause in hopes that some more data will show up.
        while( true )
        {
            head += inputStream->read( &buffer[head], count - head );
          
            if( head == count )
            {
                return count;
            }
          
            // Got here, so we wait a bit and try again.
            Thread::sleep( 10 );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
 
////////////////////////////////////////////////////////////////////////////////
unsigned char OpenwireCommandReader::readByte(void) throw( io::IOException )
{
    try
    {
        if( inputStream == NULL )
        {
            throw IOException( 
                __FILE__, __LINE__, 
                "OpenwireCommandReader::read(char*,int) - "
                "input stream is NULL" );
        }
       
        unsigned char c = 0;
        inputStream->read( &c, 1 );
        return c;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
