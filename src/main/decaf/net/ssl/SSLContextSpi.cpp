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

#include "SSLContextSpi.h"

#include <decaf/net/ssl/SSLSocket.h>
#include <decaf/net/ssl/SSLSocketFactory.h>
#include <decaf/net/ssl/SSLParameters.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <memory>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
SSLContextSpi::~SSLContextSpi() {

}

////////////////////////////////////////////////////////////////////////////////
SSLParameters* SSLContextSpi::providerGetSupportedSSLParameters() {

    SocketFactory* factory = NULL;

    std::auto_ptr<SSLParameters> params( new SSLParameters() );
    std::auto_ptr<SSLSocket> socket;

    try{

        factory = SSLSocketFactory::getDefault();

        socket.reset( dynamic_cast<SSLSocket*>( factory->createSocket() ) );

        if( socket.get() == NULL ) {
            return NULL;
        }

        params->setCipherSuites( socket->getSupportedCipherSuites() );
        params->setProtocols( socket->getSupportedProtocols() );

        return params.release();
    }
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnsupportedOperationException )
    DECAF_CATCHALL_THROW( UnsupportedOperationException )
}

////////////////////////////////////////////////////////////////////////////////
SSLParameters* SSLContextSpi::providerGetDefaultSSLParameters() {

    std::auto_ptr<SSLParameters> params( new SSLParameters() );
    std::auto_ptr<SocketFactory> factory;
    std::auto_ptr<SSLSocket> socket;

    try{

        factory.reset( SSLSocketFactory::getDefault() );

        socket.reset( dynamic_cast<SSLSocket*>( factory->createSocket() ) );

        if( socket.get() == NULL ) {
            return NULL;
        }

        params->setCipherSuites( socket->getEnabledCipherSuites() );
        params->setProtocols( socket->getEnabledProtocols() );

        return params.release();
    }
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnsupportedOperationException )
    DECAF_CATCHALL_THROW( UnsupportedOperationException )
}
