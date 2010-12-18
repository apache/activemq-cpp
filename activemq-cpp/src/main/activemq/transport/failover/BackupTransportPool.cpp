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

#include "BackupTransportPool.h"

#include <memory>

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/transport/TransportRegistry.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
BackupTransportPool::BackupTransportPool( const Pointer<CompositeTaskRunner>& taskRunner,
                                          const Pointer<CloseTransportsTask>& closeTask,
                                          const Pointer<URIPool>& uriPool ) : backups(),
                                                                              taskRunner(taskRunner),
                                                                              closeTask(closeTask),
                                                                              uriPool(uriPool),
                                                                              backupPoolSize(1),
                                                                              enabled(false),
                                                                              pending(false) {

    if( taskRunner == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "TaskRunner passed is NULL" );
    }

    if( uriPool == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "URIPool passed is NULL" );
    }

    if( closeTask == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Close Transport Task passed is NULL" );
    }

    // Add this instance as a Task so that we can create backups when nothing else is
    // going on.
    this->taskRunner->addTask( this );
}

////////////////////////////////////////////////////////////////////////////////
BackupTransportPool::BackupTransportPool( int backupPoolSize,
                                          const Pointer<CompositeTaskRunner>& taskRunner,
                                          const Pointer<CloseTransportsTask>& closeTask,
                                          const Pointer<URIPool>& uriPool ) : backups(),
                                                                              taskRunner(taskRunner),
                                                                              closeTask(closeTask),
                                                                              uriPool(uriPool),
                                                                              backupPoolSize(backupPoolSize),
                                                                              enabled(false),
                                                                              pending(false) {

    if( taskRunner == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "TaskRunner passed is NULL" );
    }

    if( uriPool == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "URIPool passed is NULL" );
    }

    if( closeTask == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Close Transport Task passed is NULL" );
    }

    // Add this instance as a Task so that we can create backups when nothing else is
    // going on.
    this->taskRunner->addTask( this );
}

////////////////////////////////////////////////////////////////////////////////
BackupTransportPool::~BackupTransportPool() {
    this->taskRunner->removeTask( this );
}

////////////////////////////////////////////////////////////////////////////////
void BackupTransportPool::setEnabled( bool value ) {
    this->enabled = value;

    if( enabled == true ) {
        this->taskRunner->wakeup();
    } else {
        synchronized( &backups ) {
            this->backups.clear();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<BackupTransport> BackupTransportPool::getBackup() {

    if( !isEnabled() ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "The Backup Pool is not enabled." );
    }

    Pointer<BackupTransport> result;

    synchronized( &backups ) {
        if( !backups.isEmpty() ) {
            result = backups.removeAt( 0 );
        }
    }

    // Flag as pending so the task gets run again and new backups are created.
    this->pending = true;
    this->taskRunner->wakeup();

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool BackupTransportPool::isPending() const {

    if( this->isEnabled() ) {
        return this->pending;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool BackupTransportPool::iterate() {

    LinkedList<URI> failures;

    synchronized( &backups ) {

        while( isEnabled() && (int)backups.size() < backupPoolSize ) {

            URI connectTo;

            // Try for a URI, if one isn't free return and indicate this task
            // is done for now, the next time a backup is requested this task
            // will become pending again and we will attempt to fill the pool.
            try{
                connectTo = uriPool->getURI();
            } catch( NoSuchElementException& ex ) {
                break;
            }

            Pointer<BackupTransport> backup( new BackupTransport( this ) );
            backup->setUri( connectTo );

            try{
                Pointer<Transport> transport = createTransport( connectTo );
                transport->setTransportListener( backup.get() );
                transport->start();
                backup->setTransport( transport );
                backups.add( backup );
            } catch(...) {
                // Store it in the list of URIs that didn't work, once done we
                // return those to the pool.
                failures.add( connectTo );
            }

        }
    }

    // return all failures to the URI Pool, we can try again later.
    uriPool->addURIs( failures );
    this->pending = false;

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void BackupTransportPool::onBackupTransportFailure( BackupTransport* failedTransport ) {

    synchronized( &backups ) {

        std::auto_ptr< Iterator< Pointer<BackupTransport> > > iter( backups.iterator() );

        while( iter->hasNext() ) {
            if( iter->next() == failedTransport ) {
                iter->remove();
            }

            this->uriPool->addURI( failedTransport->getUri() );
            this->closeTask->add( failedTransport->getTransport() );
            this->taskRunner->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> BackupTransportPool::createTransport( const URI& location ) const {

    try{

        TransportFactory* factory =
            TransportRegistry::getInstance().findFactory( location.getScheme() );

        if( factory == NULL ) {
            throw new IOException(
                __FILE__, __LINE__, "Invalid URI specified, no valid Factory Found.");
        }

        Pointer<Transport> transport( factory->createComposite( location ) );

        return transport;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}
