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

#include "ResponseCorrelator.h"
#include <algorithm>

#include <decaf/util/ArrayList.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/HashMap.h>

#include <activemq/commands/Response.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/transport/FutureResponse.h>

using namespace std;
using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::correlator;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ResponseFinalizer {
    private:

        ResponseFinalizer(const ResponseFinalizer&);
        ResponseFinalizer operator=(const ResponseFinalizer&);

    private:

        Mutex* mutex;
        int commandId;
        HashMap<unsigned int, Pointer<FutureResponse> >* map;

    public:

        ResponseFinalizer(Mutex* mutex, int commandId, HashMap<unsigned int, Pointer<FutureResponse> >* map) :
            mutex(mutex), commandId(commandId), map(map) {
        }

        ~ResponseFinalizer() {
            synchronized(mutex){
                try {
                    map->remove(commandId);
                } catch (...) {}
            }
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace transport{
namespace correlator{

    class CorrelatorData {
    public:

        // The next command id for sent commands.
        decaf::util::concurrent::atomic::AtomicInteger nextCommandId;

        // Map of request ids to future response objects.
        HashMap<unsigned int, Pointer<FutureResponse> > requestMap;

        // Sync object for accessing the request map.
        decaf::util::concurrent::Mutex mapMutex;

        // Indicates that an the filter is now unusable from some error.
        Pointer<Exception> priorError;

    public:

        CorrelatorData() : nextCommandId(1), requestMap(), mapMutex(), priorError(NULL) {}

    };

}}}

////////////////////////////////////////////////////////////////////////////////
ResponseCorrelator::ResponseCorrelator(Pointer<Transport> next) : TransportFilter(next), impl(new CorrelatorData) {
}

////////////////////////////////////////////////////////////////////////////////
ResponseCorrelator::~ResponseCorrelator() {

    // Close the transport and destroy it.
    try {
        close();
    }
    AMQ_CATCHALL_NOTHROW()

    delete this->impl;
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::oneway(const Pointer<Command> command) {

    try {

        checkClosed();

        command->setCommandId(this->impl->nextCommandId.getAndIncrement());
        command->setResponseRequired(false);

        next->oneway(command);
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<FutureResponse> ResponseCorrelator::asyncRequest(const Pointer<Command> command, const Pointer<ResponseCallback> responseCallback) {

    try {

        checkClosed();

        command->setCommandId(this->impl->nextCommandId.getAndIncrement());
        command->setResponseRequired(true);

        // Add a future response object to the map indexed by this command id.
        Pointer<FutureResponse> futureResponse(new FutureResponse(responseCallback));
        Pointer<Exception> priorError;

        synchronized(&this->impl->mapMutex) {
            priorError = this->impl->priorError;
            if (priorError == NULL) {
                this->impl->requestMap.put((unsigned int) command->getCommandId(), futureResponse);
            }
        }

        if (priorError != NULL) {

            Pointer<commands::BrokerError> exception(new commands::BrokerError(priorError));
            Pointer<commands::ExceptionResponse> response(new commands::ExceptionResponse);
            response->setException(exception);

            futureResponse->setResponse(response);

            throw IOException(__FILE__, __LINE__, this->impl->priorError->getMessage().c_str());
        }

        // Send the request.
        try {
            next->oneway(command);
        } catch (Exception &ex) {
            // We have to ensure this gets cleaned out otherwise we can consume memory over time.
            this->impl->requestMap.remove(command->getCommandId());
            throw;
        }

        return futureResponse;
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> ResponseCorrelator::request(const Pointer<Command> command) {

    try {

        checkClosed();

        command->setCommandId(this->impl->nextCommandId.getAndIncrement());
        command->setResponseRequired(true);

        // Add a future response object to the map indexed by this command id.
        Pointer<FutureResponse> futureResponse(new FutureResponse());
        Pointer<Exception> priorError;

        synchronized(&this->impl->mapMutex) {
            priorError = this->impl->priorError;
            if (priorError == NULL) {
                this->impl->requestMap.put((unsigned int) command->getCommandId(), futureResponse);
            }
        }

        if (priorError != NULL) {
            throw IOException(__FILE__, __LINE__, this->impl->priorError->getMessage().c_str());
        }

        // The finalizer will cleanup the map even if an exception is thrown.
        ResponseFinalizer finalizer(&this->impl->mapMutex, command->getCommandId(), &this->impl->requestMap);

        // Wait to be notified of the response via the futureResponse object.
        Pointer<commands::Response> response;

        // Send the request.
        next->oneway(command);

        // Get the response.
        response = futureResponse->getResponse();

        if (response == NULL) {
            throw IOException(__FILE__, __LINE__,
                "No valid response received for command: %s, check broker.", command->toString().c_str());
        }

        return response;
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> ResponseCorrelator::request(const Pointer<Command> command, unsigned int timeout) {

    try {

        checkClosed();

        command->setCommandId(this->impl->nextCommandId.getAndIncrement());
        command->setResponseRequired(true);

        // Add a future response object to the map indexed by this command id.
        Pointer<FutureResponse> futureResponse(new FutureResponse());
        Pointer<Exception> priorError;

        synchronized(&this->impl->mapMutex) {
            priorError = this->impl->priorError;
            if (priorError == NULL) {
                this->impl->requestMap.put((unsigned int) command->getCommandId(), futureResponse);
            }
        }

        if (priorError != NULL) {
            throw IOException(__FILE__, __LINE__, this->impl->priorError->getMessage().c_str());
        }

        // The finalizer will cleanup the map even if an exception is thrown.
        ResponseFinalizer finalizer(&this->impl->mapMutex, command->getCommandId(), &this->impl->requestMap);

        // Wait to be notified of the response via the futureResponse object.
        Pointer<commands::Response> response;

        // Send the request.
        next->oneway(command);

        // Get the response.
        response = futureResponse->getResponse(timeout);

        if (response == NULL) {
            throw IOException(__FILE__, __LINE__,
                "No valid response received for command: %s, check broker.", command->toString().c_str());
        }

        return response;
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::onCommand(const Pointer<Command> command) {

    // Let's see if the incoming command is a response, if not we just pass it along
    // and allow outstanding requests to keep waiting without stalling control commands.
    if (!command->isResponse()) {
        TransportFilter::onCommand(command);
        return;
    }

    Pointer<Response> response = command.dynamicCast<Response>();

    // It is a response - let's correlate ...
    synchronized(&this->impl->mapMutex) {

        Pointer<FutureResponse> futureResponse;
        try {
            futureResponse = this->impl->requestMap.remove(response->getCorrelationId());
        } catch (NoSuchElementException& ex) {
            return;
        }

        // Set the response property in the future response.
        futureResponse->setResponse(response);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::doClose() {
    try {
        dispose(Pointer<Exception>(new IOException(__FILE__, __LINE__, "Transport Stopped")));
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::onException(const decaf::lang::Exception& ex) {
    dispose(Pointer<Exception>(ex.clone()));
    TransportFilter::onException(ex);
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelator::dispose(Pointer<Exception> error) {

    ArrayList<Pointer<FutureResponse> > requests;
    synchronized(&this->impl->mapMutex) {
        if (this->impl->priorError == NULL) {
            this->impl->priorError = error;
            requests.ensureCapacity((int)this->impl->requestMap.size());
            requests.copy(this->impl->requestMap.values());
            this->impl->requestMap.clear();
        }
    }

    if (!requests.isEmpty()) {
        Pointer<commands::BrokerError> exception(new commands::BrokerError);
        exception->setExceptionClass("java.io.IOException");
        exception->setMessage(error->getMessage());

        Pointer<commands::ExceptionResponse> errorResponse(new commands::ExceptionResponse);
        errorResponse->setException(exception);

        Pointer<Iterator<Pointer<FutureResponse> > > iter(requests.iterator());
        while (iter->hasNext()) {
            Pointer<FutureResponse> response = iter->next();
            response->setResponse(errorResponse);
        }
    }
}
