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
#include "SSLSocketFactory.h"
#include "SSLSocket.h"
#include "SocketException.h"

#include <activemq/util/Properties.h>
#include <activemq/util/Random.h>
#include <activemq/concurrent/Thread.h>

#include <stdio.h>

#ifdef AMQ_HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/rand.h>
#endif /* AMQ_HAVE_OPENSSL */

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::network;
using namespace activemq::exceptions;
using namespace activemq::concurrent;

#ifdef AMQ_HAVE_OPENSSL

Mutex *SSLSocketFactory::locks = 0;

void SSLSocketFactory::locking_cb( int mode, int type, const char *, int )
{
    if( mode & CRYPTO_LOCK )
	locks[type].lock ();
    else
	locks[type].unlock ();
}

#endif /* AMQ_HAVE_OPENSSL */

////////////////////////////////////////////////////////////////////////////////

SSLSocketFactory::SSLSocketFactory()
{
#ifdef AMQ_HAVE_OPENSSL
    manageLocks = false;

    // Initialize only if locking callback is not yet set.
    if( !CRYPTO_get_locking_callback() ){

	// General library initialization.
	CRYPTO_malloc_init();
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	// Set up threading.
	if( !locks ){
	    manageLocks = true;
	    locks = new Mutex[CRYPTO_num_locks()];
	    CRYPTO_set_id_callback( &Thread::getId );
	    CRYPTO_set_locking_callback( &locking_cb );
	}
    }

    // Seed the random number generator.
    // Not really safe, but what can you do if the system does not have a good
    // random source..
    if( !RAND_status() ){
	std::vector<unsigned char> data (64);
	Random().nextBytes( data );
#ifdef WIN32
	RAND_seed( static_cast<const void *>( data._Myfirst ), (int)data.size() );
#else
	RAND_seed( static_cast<const void *>( data.data() ), data.size() );
#endif
    }

#endif /* AMQ_HAVE_OPENSSL */
}

SSLSocketFactory::~SSLSocketFactory()
{
#ifdef AMQ_HAVE_OPENSSL

  if( manageLocks ){
      CRYPTO_set_locking_callback( 0 );
      delete [] locks;
      locks = 0;
  }

#endif /* AMQ_HAVE_OPENSSL */
}

TcpSocket* SSLSocketFactory::createTcpSocket(
    const util::Properties &properties )
    throw ( SocketException )
{
#ifdef AMQ_HAVE_OPENSSL
    std::string ca_file
	= properties.getProperty( "sslCAFile", "" );
    std::string ca_path
	= properties.getProperty( "sslCAPath", "" );

    std::string cert_file
	= properties.getProperty( "sslCertFile", "" );
    std::string key_file
	= properties.getProperty( "sslKeyFile", cert_file );

    std::string password
	= properties.getProperty( "sslPassword", "" );

    bool verify_peer
	= properties.getProperty( "sslVerifyPeer", "false" ) == "true";
    std::string verify_name
	= properties.getProperty( "sslVerifyName", "" );

    std::string ciphers
	= properties.getProperty( "sslCiphers", "" );

    SSLSocket *socket = new SSLSocket();

    try {
	try {
	    socket->setPassword( password );
	    socket->setCAFilePath( ca_file, ca_path );
	    socket->setCertFile( cert_file, key_file );
	    socket->setVerifyPeer( verify_peer, verify_name );
	    socket->setCiphers( ciphers );
	}
        catch ( SocketException& ex ) {
	    ex.setMark( __FILE__, __LINE__ );
	    try{
		delete socket;
	    } catch( SocketException& ex2 ){ /* Absorb */ }

	    throw ex;
	}

        return socket;
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, SocketException )
    AMQ_CATCHALL_THROW( SocketException )

#else /* not AMQ_HAVE_OPENSSL */

    throw SocketException( __FILE__, __LINE__, "SSL is not available");

#endif /* not AMQ_HAVE_OPENSSL */
}
