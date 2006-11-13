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
 
#include "ByteArrayInputStream.h"
#include <algorithm>

using namespace activemq::io;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream()
{
    pos = buffer.end();
    closing = false;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream( const unsigned char* buffer,
                                            int bufferSize )
{
    closing = false;
    setByteArray( buffer, bufferSize );
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::~ByteArrayInputStream(void)
{
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::setByteArray( const unsigned char* lbuffer,
                                         int lbufferSize )
{
    synchronized( this ){
        
        // Remove old data
        this->buffer.clear();
       
        // Copy data to internal buffer.
        for( int ix = 0; ix < lbufferSize; ++ix )
        {
            this->buffer.push_back(lbuffer[ix]);
        }
       
        // Begin at the Beginning.
        pos = this->buffer.begin();
        
        // Notify any listening threads that there
        // is now data available.
        notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::close() throw( cms::CMSException ){
    
    synchronized( this ){
     
        // Indicate that this stream is closing.
        closing = true;
           
        // Close the delegate stream.
        buffer.clear();
        
        // Notify that this stream is shutting down.
        notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ByteArrayInputStream::read() throw ( IOException )
{
    synchronized( this ){
        
        while( !closing ){
            
            if(pos != buffer.end())
            {
                return *(pos++);
            }                        
            
            // Wait for data to come in.
            wait();
        }
        
        throw IOException( __FILE__, __LINE__, "close occurred during a read" );
    }
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::read( unsigned char* buffer, 
                                const int bufferSize ) 
                                   throw ( IOException )
{
    synchronized( this ){
        
        int ix = 0;
        
        for( ; ix < bufferSize && !closing; ++ix, ++pos)
        {
            if(pos == this->buffer.end())
            {   
                // We don't have the requested data yet -
                // wait for it.     
                wait();
            }
          
            buffer[ix] = *(pos);
        }
        
        if( closing ){
            throw IOException( __FILE__, __LINE__, "close occurred during a read" );
        }
       
        return ix;
    }
}
