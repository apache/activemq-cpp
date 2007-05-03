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
 
#include "ByteArrayOutputStream.h"
#include <algorithm>

using namespace activemq::io;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream()
{
    activeBuffer = &defaultBuffer;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream( vector<unsigned char>& buffer)
{
    setBuffer( buffer );
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::setBuffer( vector<unsigned char>& buffer)
{
    activeBuffer = &buffer;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::clear() throw ( IOException )
{
    // Empty the contents of the buffer to the output stream.
    activeBuffer->clear();
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::write( unsigned char c ) 
   throw ( IOException )
{
    activeBuffer->push_back( c );  
}

////////////////////////////////////////////////////////////////////////////////    
void ByteArrayOutputStream::write( const unsigned char* buffer, 
                                   std::size_t len ) 
   throw ( IOException )
{     
    // Iterate until all the data is written.
    for( std::size_t ix = 0; ix < len; ++ix)
    {
        activeBuffer->push_back( buffer[ix] );
    }  
}

