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

#include "SSLSocketOutputStream.h"
#include "SSLError.h"
#include "SocketError.h"

#ifdef AMQ_HAVE_OPENSSL

using namespace activemq::network;
using namespace activemq::io;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SSLSocketOutputStream::SSLSocketOutputStream( Socket::SocketHandle socket,
					      SSL* ssl)
  : SocketOutputStream( socket ),
    ssl( ssl )
{
}

////////////////////////////////////////////////////////////////////////////////
void SSLSocketOutputStream::write( const unsigned char* buffer, std::size_t len )
    throw (IOException)
{
    std::size_t remaining = len;

    while( remaining > 0 && !closed )
    {
        int len = SSL_write( ssl, buffer, remaining );

	switch (SSL_get_error( ssl, len ))
	  {
	  case SSL_ERROR_NONE:
	    buffer += len;
	    remaining -= len;
	    break;

	  case SSL_ERROR_WANT_READ:
	  case SSL_ERROR_WANT_WRITE:
	    // Repeat the operation.
	    break;

	  case SSL_ERROR_ZERO_RETURN:
            throw IOException( __FILE__, __LINE__,
			       "activemq::io::SSLSocketInputStream::write"
			       " - The connection is broken" );

	  case SSL_ERROR_SSL:
            throw IOException( __FILE__, __LINE__,
			       "activemq::io::SSLSocketInputStream::write"
			       " - %s",
			       SSLError::getErrorString().c_str());

	  case SSL_ERROR_SYSCALL:
	    if( SocketError::getErrorCode() != SocketError::INTERRUPTED )
		throw IOException( __FILE__, __LINE__,
				   "activemq::io::SSLSocketInputStream::write"
				   " - %s",
				   SocketError::getErrorString().c_str() );
	    break;
        }
    }
}

#endif /* AMQ_HAVE_OPENSSL */
