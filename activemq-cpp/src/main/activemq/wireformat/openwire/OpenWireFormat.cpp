/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "OpenWireFormat.h"

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/util/UUID.h>
#include <decaf/lang/Math.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <activemq/wireformat/openwire/OpenWireFormatNegotiator.h>
#include <activemq/wireformat/openwire/utils/BooleanStream.h>
#include <activemq/wireformat/MarshalAware.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/DataStructure.h>
#include <activemq/wireformat/openwire/marshal/DataStreamMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/MarshallerFactory.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace activemq::wireformat::openwire::utils;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const unsigned char OpenWireFormat::NULL_TYPE = 0;
const int OpenWireFormat::DEFAULT_VERSION = 1;
const int OpenWireFormat::MAX_SUPPORTED_VERSION = 9;

////////////////////////////////////////////////////////////////////////////////
OpenWireFormat::OpenWireFormat(const decaf::util::Properties& properties) :
    properties(properties), preferedWireFormatInfo(), dataMarshallers(256),
    id(UUID::randomUUID().toString()), receiving(), version(0), stackTraceEnabled(true),
    tcpNoDelayEnabled(true), cacheEnabled(true), cacheSize(1024), tightEncodingEnabled(false),
    sizePrefixDisabled(false), maxInactivityDuration(30000), maxInactivityDurationInitialDelay(10000) {

    // initialize the universal marshalers, don't need to reset them again
    // after this so its safe to do this here.
    generated::MarshallerFactory().configure(this);

    // Set to Default as lowest common denominator, then we will try
    // and move up to the preferred when the wireformat is negotiated.
    this->setVersion(DEFAULT_VERSION);
}

////////////////////////////////////////////////////////////////////////////////
OpenWireFormat::~OpenWireFormat() {
    try {
        this->destroyMarshalers();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> OpenWireFormat::createNegotiator(const Pointer<Transport> transport) {

    try {
        return Pointer<Transport>(new OpenWireFormatNegotiator(this, transport));
    }
    AMQ_CATCH_RETHROW(UnsupportedOperationException)
    AMQ_CATCHALL_THROW(UnsupportedOperationException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::destroyMarshalers() {

    try {
        for (size_t i = 0; i < dataMarshallers.size(); ++i) {
            delete dataMarshallers[i];
            dataMarshallers[i] = NULL;
        }
    }
    AMQ_CATCH_NOTHROW(ActiveMQException)
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::setVersion(int version) {

    try {

        if (version == this->getVersion()) {
            return;
        }

        if (version > MAX_SUPPORTED_VERSION) {
            throw IllegalArgumentException(__FILE__, __LINE__, "OpenWireFormat::setVersion - "
                    "Given Version: %d , is not yet supported", version);
        }

        // Clear old marshalers in preparation for the new set.
        this->version = version;
    }
    AMQ_CATCH_RETHROW(IllegalArgumentException)
    AMQ_CATCHALL_THROW(IllegalArgumentException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::addMarshaller(DataStreamMarshaller* marshaller) {
    unsigned char type = marshaller->getDataStructureType();
    dataMarshallers[type & 0xFF] = marshaller;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::setPreferedWireFormatInfo(const Pointer<commands::WireFormatInfo> info) {
    this->preferedWireFormatInfo = info;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::marshal(const Pointer<commands::Command> command, const activemq::transport::Transport* transport, decaf::io::DataOutputStream* dataOut) {

    if (transport == NULL) {
        throw decaf::io::IOException(__FILE__, __LINE__, "Transport passed is NULL");
    }

    if (dataOut == NULL) {
        throw decaf::io::IOException(__FILE__, __LINE__, "DataOutputStream passed is NULL");
    }

    try {

        int size = 1;

        if (command != NULL) {

            DataStructure* dataStructure = dynamic_cast<DataStructure*>(command.get());

            unsigned char type = dataStructure->getDataStructureType();

            DataStreamMarshaller* dsm = dataMarshallers[type & 0xFF];

            if (dsm == NULL) {
                throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(type)).c_str());
            }

            if (tightEncodingEnabled) {
                BooleanStream bs;
                size += dsm->tightMarshal1(this, dataStructure, &bs);
                size += bs.marshalledSize();

                if (!sizePrefixDisabled) {
                    dataOut->writeInt(size);
                }

                dataOut->writeByte(type);
                bs.marshal(dataOut);
                dsm->tightMarshal2(this, dataStructure, dataOut, &bs);

            } else {

                if (sizePrefixDisabled) {
                    dataOut->writeByte(type);
                    dsm->looseMarshal(this, dataStructure, dataOut);
                } else {

                    ByteArrayOutputStream* baos = new ByteArrayOutputStream();
                    std::auto_ptr<DataOutputStream> looseOut(new DataOutputStream(baos, true));

                    looseOut->writeByte(type);
                    dsm->looseMarshal(this, dataStructure, looseOut.get());
                    looseOut->close();

                    // Now the data goes to the transport from out byte buffer.
                    dataOut->writeInt((int) baos->size());

                    if (baos->size() > 0) {
                        std::pair<unsigned char*, int> array = baos->toByteArray();

                        try {
                            dataOut->write(array.first, array.second);
                        } catch (Exception& ex) {
                            delete[] array.first;
                            throw;
                        }

                        delete[] array.first;
                    }
                }
            }
        } else {
            dataOut->writeInt(size);
            dataOut->writeByte(NULL_TYPE);
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::Command> OpenWireFormat::unmarshal(const activemq::transport::Transport* transport AMQCPP_UNUSED, decaf::io::DataInputStream* dis) {

    try {

        if (dis == NULL) {
            throw decaf::io::IOException(__FILE__, __LINE__, "DataInputStream passed is NULL");
        }

        if (!sizePrefixDisabled) {
            dis->readInt();
        }

        // Get the unmarshalled DataStructure
        Pointer<DataStructure> data(doUnmarshal(dis));

        if (data == NULL) {
            throw IOException(__FILE__, __LINE__, "OpenWireFormat::doUnmarshal - "
                    "Failed to unmarshal an Object");
        }

        // Now all unmarshals from this level should result in an object
        // that is a commands::Command type, if its not then the cast will
        // throw an ClassCastException.
        Pointer<Command> command = data.dynamicCast<Command>();

        return command;
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
commands::DataStructure* OpenWireFormat::doUnmarshal(DataInputStream* dis) {

    try {

        class Finally {
        private:

            decaf::util::concurrent::atomic::AtomicBoolean* state;

        private:

            Finally(const Finally&);
            Finally& operator=(const Finally&);

        public:

            Finally(decaf::util::concurrent::atomic::AtomicBoolean* state) : state(state) {
                state->set(true);
            }

            ~Finally() {
                state->set(false);
            }
        }

        finalizer(&(this->receiving));

        unsigned char dataType = dis->readByte();

        if (dataType != NULL_TYPE) {

            DataStreamMarshaller* dsm = dynamic_cast<DataStreamMarshaller*>(dataMarshallers[dataType & 0xFF]);

            if (dsm == NULL) {
                throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(dataType)).c_str());
            }

            // Ask the DataStreamMarshaller to create a new instance of its
            // command so that we can fill in its data.
            std::auto_ptr<DataStructure> data(dsm->createObject());

            if (this->tightEncodingEnabled) {
                BooleanStream bs;
                bs.unmarshal(dis);
                dsm->tightUnmarshal(this, data.get(), dis, &bs);
            } else {
                dsm->looseUnmarshal(this, data.get(), dis);
            }

            return data.release();
        }

        return NULL;
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int OpenWireFormat::tightMarshalNestedObject1(commands::DataStructure* object, utils::BooleanStream* bs) {

    try {

        bs->writeBoolean(object != NULL);
        if (object == NULL) {
            return 0;
        }

        if (object->isMarshalAware()) {

            std::vector<unsigned char> sequence = object->getMarshaledForm(this);
            bs->writeBoolean(!sequence.empty());
            if (!sequence.empty()) {
                return (int) (1 + sequence.size());
            }
        }

        unsigned char type = object->getDataStructureType();
        if (type == 0) {
            throw IOException(__FILE__, __LINE__, "No valid data structure type for object of this type");
        }

        DataStreamMarshaller* dsm = dataMarshallers[type & 0xFF];

        if (dsm == NULL) {
            throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(type)).c_str());
        }

        return 1 + dsm->tightMarshal1(this, object, bs);
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::tightMarshalNestedObject2(DataStructure* o, DataOutputStream* ds, BooleanStream* bs) {

    try {

        if (!bs->readBoolean()) {
            return;
        }

        unsigned char type = o->getDataStructureType();

        ds->writeByte(type);

        if (o->isMarshalAware() && bs->readBoolean()) {

            MarshalAware* ma = dynamic_cast<MarshalAware*>(o);
            vector<unsigned char> sequence = ma->getMarshaledForm(this);
            ds->write(&sequence[0], (int) sequence.size());

        } else {

            DataStreamMarshaller* dsm = dataMarshallers[type & 0xFF];

            if (dsm == NULL) {
                throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(type)).c_str());
            }

            dsm->tightMarshal2(this, o, ds, bs);
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* OpenWireFormat::tightUnmarshalNestedObject(DataInputStream* dis, BooleanStream* bs) {

    try {

        if (bs->readBoolean()) {

            const unsigned char dataType = dis->readByte();

            DataStreamMarshaller* dsm = dataMarshallers[dataType & 0xFF];

            if (dsm == NULL) {
                throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(dataType)).c_str());
            }

            std::auto_ptr<DataStructure> data(dsm->createObject());

            if (data->isMarshalAware() && bs->readBoolean()) {

                dis->readInt();
                dis->readByte();

                BooleanStream bs2;
                bs2.unmarshal(dis);
                dsm->tightUnmarshal(this, data.get(), dis, &bs2);
            } else {
                dsm->tightUnmarshal(this, data.get(), dis, bs);
            }

            return data.release();
        } else {
            return NULL;
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* OpenWireFormat::looseUnmarshalNestedObject(decaf::io::DataInputStream* dis) {

    try {

        if (dis->readBoolean()) {

            unsigned char dataType = dis->readByte();

            DataStreamMarshaller* dsm = dataMarshallers[dataType & 0xFF];

            if (dsm == NULL) {
                throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(dataType)).c_str());
            }

            std::auto_ptr<DataStructure> data(dsm->createObject());
            dsm->looseUnmarshal(this, data.get(), dis);
            return data.release();
        } else {
            return NULL;
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::looseMarshalNestedObject(commands::DataStructure* o, decaf::io::DataOutputStream* dataOut) {

    try {

        dataOut->writeBoolean(o != NULL);
        if (o != NULL) {

            unsigned char dataType = o->getDataStructureType();

            dataOut->writeByte(dataType);

            DataStreamMarshaller* dsm = dataMarshallers[dataType & 0xFF];

            if (dsm == NULL) {
                throw IOException(__FILE__, __LINE__, (string("OpenWireFormat::marshal - Unknown data type: ") + Integer::toString(dataType)).c_str());
            }

            dsm->looseMarshal(this, o, dataOut);
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(ActiveMQException, IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::renegotiateWireFormat(const WireFormatInfo& info) {

    if (preferedWireFormatInfo == NULL) {
        throw IllegalStateException(__FILE__, __LINE__,
            "OpenWireFormat::renegotiateWireFormat - Wireformat cannot not be renegotiated.");
    }

    this->setVersion(Math::min(preferedWireFormatInfo->getVersion(), info.getVersion()));
    this->stackTraceEnabled = info.isStackTraceEnabled() && preferedWireFormatInfo->isStackTraceEnabled();
    this->tcpNoDelayEnabled = info.isTcpNoDelayEnabled() && preferedWireFormatInfo->isTcpNoDelayEnabled();
    this->cacheEnabled = info.isCacheEnabled() && preferedWireFormatInfo->isCacheEnabled();
    this->tightEncodingEnabled = info.isTightEncodingEnabled() && preferedWireFormatInfo->isTightEncodingEnabled();
    this->sizePrefixDisabled = info.isSizePrefixDisabled() && preferedWireFormatInfo->isSizePrefixDisabled();
    this->cacheSize = min(info.getCacheSize(), preferedWireFormatInfo->getCacheSize());
    this->maxInactivityDuration = min(info.getMaxInactivityDuration(), preferedWireFormatInfo->getMaxInactivityDuration());
    this->maxInactivityDurationInitialDelay = min(info.getMaxInactivityDurationInitalDelay(), preferedWireFormatInfo->getMaxInactivityDurationInitalDelay());
}
