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

#include "CMSListener.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
CMSListener::CMSListener( cms::Session* session ) : session( session ) {
    this->reset();
}

////////////////////////////////////////////////////////////////////////////////
CMSListener::~CMSListener() {
}

////////////////////////////////////////////////////////////////////////////////
void CMSListener::reset() {
    this->numReceived = 0;
}

////////////////////////////////////////////////////////////////////////////////
void CMSListener::asyncWaitForMessages( unsigned int count ) {

    try {

        synchronized( &mutex ) {
            int stopAtZero = count + 5;

            while( numReceived < count ) {
                mutex.wait( 500 );

                if( --stopAtZero == 0 ) {
                    break;
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CMSListener::onMessage( const cms::Message* message ) {

    if( session->getAcknowledgeMode() == cms::Session::CLIENT_ACKNOWLEDGE ) {
        try {
            message->acknowledge();
        } catch( CMSException& ex ) {
            CPPUNIT_ASSERT_MESSAGE(ex.getStackTraceString(), false );
        }
    }

    // Got a text message.
    const cms::TextMessage* txtMsg =
        dynamic_cast<const cms::TextMessage*>( message );

    if( txtMsg != NULL ) {
        numReceived++;

        // Signal that we got one
        synchronized( &mutex ) {
            mutex.notifyAll();
        }

        return;
    }

    // Got a bytes msg.
    const cms::BytesMessage* bytesMsg =
        dynamic_cast<const cms::BytesMessage*>( message );

    if( bytesMsg != NULL ) {

        numReceived++;

        // Signal that we got one
        synchronized( &mutex ) {
            mutex.notifyAll();
        }

        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CMSListener::onException( const cms::CMSException& error ) {
    CPPUNIT_ASSERT_MESSAGE( error.getStackTraceString(), false );
}
