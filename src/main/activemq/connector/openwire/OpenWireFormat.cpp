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
#include <activemq/connector/openwire/utils/BooleanStream.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/connector/openwire/commands/DataStructure.h>
#include <activemq/connector/openwire/marshal/MarshalAware.h>
#include <activemq/connector/openwire/marshal/DataStreamMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/MarshallerFactory.h>
#include <activemq/connector/openwire/marshal/v2/MarshallerFactory.h>
#include <activemq/connector/openwire/marshal/v1/MarshallerFactory.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::utils;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const unsigned char OpenWireFormat::NULL_TYPE = 0;

////////////////////////////////////////////////////////////////////////////////
OpenWireFormat::OpenWireFormat( const decaf::util::Properties& properties ) {

    // Copy config data
    this->properties.copy( &properties );
    this->preferedWireFormatInfo = NULL;

    // Fill in that DataStreamMarshallers collection
    dataMarshallers.resize( 256 );

    // Generate an ID
    this->id = UUID::randomUUID().toString();
    // Set defaults for initial WireFormat negotiation
    this->version = 0;
    this->stackTraceEnabled = true;
    this->cacheEnabled = true;
    this->cacheSize = 1024;
    this->tcpNoDelayEnabled = true;
    this->tightEncodingEnabled = false;
    this->sizePrefixDisabled = false;
    this->maxInactivityDuration = 30000;
    this->maxInactivityDurationInitialDelay = 10000;

    // Set to Default as lowest common denominator, then we will try
    // and move up to the prefered when the wireformat is negotiated.
    this->setVersion( DEFAULT_VERSION );
}

////////////////////////////////////////////////////////////////////////////////
OpenWireFormat::~OpenWireFormat() {
    try {
        this->destroyMarshalers();
        delete preferedWireFormatInfo;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::destroyMarshalers() {
    try {
        for( size_t i = 0; i < dataMarshallers.size(); ++i ) {
            delete dataMarshallers[i];
            dataMarshallers[i] = NULL;
        }
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::setVersion( int version ) throw ( IllegalArgumentException ) {

    try{
        if( version == this->getVersion() ){
            return;
        }

        // Clear old marshalers in preparation for the new set.
        this->destroyMarshalers();
        this->version = version;

        switch( this->version ){
        case 1:
            v1::MarshallerFactory().configure( this );
            break;
        case 2:
            v2::MarshallerFactory().configure( this );
            break;
        case 3:
            v3::MarshallerFactory().configure( this );
            break;
        default:
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "OpenWireFormat::setVersion - "
                "Given Version: %d , is not supported", version );
        }
    }
    AMQ_CATCH_RETHROW( IllegalArgumentException )
    AMQ_CATCHALL_THROW( IllegalArgumentException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::addMarshaller( DataStreamMarshaller* marshaller ) {
    unsigned char type = marshaller->getDataStructureType();
    dataMarshallers[type & 0xFF] = marshaller;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::setPreferedWireFormatInfo(
    commands::WireFormatInfo* info ) throw ( IllegalStateException ) {

    delete preferedWireFormatInfo;
    this->preferedWireFormatInfo = info;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::marshal( transport::Command* command,
                              decaf::io::DataOutputStream* dataOut )
    throw ( decaf::io::IOException ) {

    try {

        int size = 1;

        if( command != NULL ) {

            DataStructure* dataStructure =
                dynamic_cast< DataStructure* >( command );

            unsigned char type = dataStructure->getDataStructureType();

            DataStreamMarshaller* dsm = dataMarshallers[type & 0xFF];

            if( dsm == NULL ) {
                throw IOException(
                    __FILE__, __LINE__,
                    ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                    Integer::toString( type ) ).c_str() );
            }

            if( tightEncodingEnabled ) {
                BooleanStream bs;
                size += dsm->tightMarshal1( this, dataStructure, &bs );
                size += bs.marshalledSize();

                if( !sizePrefixDisabled ) {
                    dataOut->writeInt( size );
                }

                dataOut->writeByte( type );
                bs.marshal( dataOut );
                dsm->tightMarshal2( this, dataStructure, dataOut, &bs );

            } else {
                DataOutputStream* looseOut = dataOut;
                ByteArrayOutputStream* baos = NULL;

                if( !sizePrefixDisabled ) {
                    baos = new ByteArrayOutputStream();
                    looseOut = new DataOutputStream( baos );
                }

                looseOut->writeByte( type );
                dsm->looseMarshal( this, dataStructure, looseOut );

                if( !sizePrefixDisabled ) {
                    looseOut->close();
                    dataOut->writeInt( (int)baos->size() );
                    dataOut->write( baos->toByteArray(), 0, baos->size() );

                    // Delete allocated resource
                    delete baos;
                    delete looseOut;
                }
            }
        } else {
            dataOut->writeInt( size );
            dataOut->writeByte( NULL_TYPE );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
transport::Command* OpenWireFormat::unmarshal( decaf::io::DataInputStream* dis )
    throw ( decaf::io::IOException ) {

    try {

        if( !sizePrefixDisabled ) {
            dis->readInt();
        }

        // Get the unmarshalled DataStructure
        DataStructure* data = doUnmarshal( dis );

        if( data == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "OpenWireFormat::doUnmarshal - "
                "Failed to unmarshal an Object" );
        }

        // Now all unmarshals from this level should result in an object
        // that is a transport::Command type, if its not then we throw an
        // exception.
        transport::Command* command =
            dynamic_cast< transport::Command* >( data );

        if( command == NULL ) {
            delete data;

            throw IOException(
                __FILE__, __LINE__,
                "OpenWireFormat::doUnmarshal - "
                "Unmarshalled a non Command Type" );
        }

        return command;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
commands::DataStructure* OpenWireFormat::doUnmarshal( DataInputStream* dis )
    throw ( IOException ) {

    try {

        unsigned char dataType = dis->readByte();

        if( dataType != NULL_TYPE ) {

            DataStreamMarshaller* dsm =
                dynamic_cast< DataStreamMarshaller* >(
                    dataMarshallers[dataType & 0xFF] );

            if( dsm == NULL ) {
                throw IOException(
                    __FILE__, __LINE__,
                    ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                    Integer::toString( dataType ) ).c_str() );
            }

            // Ask the DataStreamMarshaller to create a new instance of its
            // command so that we can fill in its data.
            DataStructure* data = dsm->createObject();

            if( this->tightEncodingEnabled ) {
                BooleanStream bs;
                bs.unmarshal( dis );
                dsm->tightUnmarshal( this, data, dis, &bs );
            } else {
                dsm->looseUnmarshal( this, data, dis );
            }

            return data;
        }

        return NULL;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int OpenWireFormat::tightMarshalNestedObject1( commands::DataStructure* object,
                                               utils::BooleanStream* bs )
    throw ( decaf::io::IOException ) {

    try {

        bs->writeBoolean( object != NULL );
        if( object == NULL ) {
            return 0;
        }

        if( object->isMarshalAware() ) {

            std::vector<unsigned char> sequence =
                object->getMarshaledForm(this);
            bs->writeBoolean( !sequence.empty() );
            if( !sequence.empty() ) {
                return (int)(1 + sequence.size());
            }
        }

        unsigned char type = object->getDataStructureType();
        if( type == 0 ) {
            throw IOException(
                __FILE__, __LINE__,
                "No valid data structure type for object of this type");
        }

        DataStreamMarshaller* dsm = dataMarshallers[type & 0xFF];

        if( dsm == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                Integer::toString( type ) ).c_str() );
        }

        return 1 + dsm->tightMarshal1( this, object, bs );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::tightMarshalNestedObject2( DataStructure* o,
                                                DataOutputStream* ds,
                                                BooleanStream* bs )
                                                    throw ( IOException ) {

    try {

        if( !bs->readBoolean() ) {
            return;
        }

        unsigned char type = o->getDataStructureType();

        ds->writeByte(type);

        if( o->isMarshalAware() && bs->readBoolean() ) {

            MarshalAware* ma = dynamic_cast< MarshalAware* >( o );
            vector<unsigned char> sequence = ma->getMarshaledForm( this );
            ds->write( &sequence[0], 0, sequence.size() );

        } else {

            DataStreamMarshaller* dsm = dataMarshallers[type & 0xFF];

            if( dsm == NULL ) {
                throw IOException(
                    __FILE__, __LINE__,
                    ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                    Integer::toString( type ) ).c_str() );
            }

            dsm->tightMarshal2( this, o, ds, bs );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* OpenWireFormat::tightUnmarshalNestedObject( DataInputStream* dis,
                                                           BooleanStream* bs )
    throw ( decaf::io::IOException ) {

    try {

        if( bs->readBoolean() ) {

            const unsigned char dataType = dis->readByte();

            DataStreamMarshaller* dsm = dataMarshallers[dataType & 0xFF];

            if( dsm == NULL ) {
                throw IOException(
                    __FILE__, __LINE__,
                    ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                    Integer::toString( dataType ) ).c_str() );
            }

            DataStructure* data = dsm->createObject();

            if( data->isMarshalAware() && bs->readBoolean() ) {

                dis->readInt();
                dis->readByte();

                BooleanStream bs2;
                bs2.unmarshal( dis );
                dsm->tightUnmarshal( this, data, dis, &bs2 );

            } else {
                dsm->tightUnmarshal( this, data, dis, bs );
            }

            return data;
        } else {
            return NULL;
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* OpenWireFormat::looseUnmarshalNestedObject( decaf::io::DataInputStream* dis )
    throw ( IOException ) {

    try{

        if( dis->readBoolean() ) {

            unsigned char dataType = dis->readByte();

            DataStreamMarshaller* dsm = dataMarshallers[dataType & 0xFF];

            if( dsm == NULL ) {
                throw IOException(
                    __FILE__, __LINE__,
                    ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                    Integer::toString( dataType ) ).c_str() );
            }

            DataStructure* data = dsm->createObject();
            dsm->looseUnmarshal( this, data, dis );

            return data;

        } else {
            return NULL;
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::looseMarshalNestedObject( commands::DataStructure* o,
                                               decaf::io::DataOutputStream* dataOut )
    throw ( decaf::io::IOException ) {

    try{

        dataOut->writeBoolean( o != NULL );
        if( o != NULL ) {

            unsigned char dataType = o->getDataStructureType();

            dataOut->writeByte( dataType );

            DataStreamMarshaller* dsm = dataMarshallers[dataType & 0xFF];

            if( dsm == NULL ) {
                throw IOException(
                    __FILE__, __LINE__,
                    ( string( "OpenWireFormat::marshal - Unknown data type: " ) +
                    Integer::toString( dataType ) ).c_str() );
            }

            dsm->looseMarshal( this, o, dataOut );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::renegotiateWireFormat( WireFormatInfo* info )
    throw ( IllegalStateException ) {

    if( preferedWireFormatInfo == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__,
            "OpenWireFormat::renegotiateWireFormat - "
            "Wireformat cannot not be renegotiated." );
    }

    this->setVersion( Math::min( preferedWireFormatInfo->getVersion(),
                                 info->getVersion() ) );
    this->stackTraceEnabled = info->isStackTraceEnabled() &&
                              preferedWireFormatInfo->isStackTraceEnabled();
    this->tcpNoDelayEnabled = info->isTcpNoDelayEnabled() &&
                              preferedWireFormatInfo->isTcpNoDelayEnabled();
    this->cacheEnabled = info->isCacheEnabled() &&
                         preferedWireFormatInfo->isCacheEnabled();
    this->tightEncodingEnabled = info->isTightEncodingEnabled() &&
                                 preferedWireFormatInfo->isTightEncodingEnabled();
    this->sizePrefixDisabled = info->isSizePrefixDisabled() &&
                               preferedWireFormatInfo->isSizePrefixDisabled();
    this->cacheSize = min( info->getCacheSize(), preferedWireFormatInfo->getCacheSize() );
    this->maxInactivityDuration = min( info->getMaxInactivityDuration(),
                                       preferedWireFormatInfo->getMaxInactivityDuration() );
    this->maxInactivityDurationInitialDelay = min( info->getMaxInactivityDurationInitalDelay(),
                                                   preferedWireFormatInfo->getMaxInactivityDurationInitalDelay() );
}
