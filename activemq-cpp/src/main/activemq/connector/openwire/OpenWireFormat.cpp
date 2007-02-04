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

#include <activemq/util/Boolean.h>
#include <activemq/util/Integer.h>
#include <activemq/util/Long.h>
#include <activemq/util/Guid.h>
#include <activemq/util/Math.h>
#include <activemq/io/ByteArrayOutputStream.h>
#include <activemq/connector/openwire/utils/BooleanStream.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/connector/openwire/commands/DataStructure.h>
#include <activemq/connector/openwire/marshal/MarshalAware.h>
#include <activemq/connector/openwire/DataStreamMarshaller.h>
#include <activemq/connector/openwire/marshal/v2/MarshallerFactory.h>

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::io;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::marshal::v2;
using namespace activemq::connector::openwire::utils;

////////////////////////////////////////////////////////////////////////////////
const unsigned char OpenWireFormat::NULL_TYPE = 0;

////////////////////////////////////////////////////////////////////////////////
OpenWireFormat::OpenWireFormat( const activemq::util::Properties& properties ) {

    // init
    this->preferedWireFormatInfo = NULL;
    
    // Copy config data
    this->properties.copy( &properties );

    // Fill in that DataStreamMarshallers collection
    dataMarshallers.resize( 255 );
    MarshallerFactory().configure( this );

    // Generate an ID
    this->id = Guid::createGUIDString();

    // parse params out of the properties
    stackTraceEnabled = Boolean::parseBoolean( 
        properties.getProperty( "wireFormat.stackTraceEnabled", 
                                "1" ) );
    cacheEnabled = Boolean::parseBoolean(
        properties.getProperty( "wireFormat.cacheEnabled", 
                                "1" ) );
    tcpNoDelayEnabled = Boolean::parseBoolean( 
        properties.getProperty( "wireFormat.tcpNoDelayEnabled", 
                                "1" ) );
    tightEncodingEnabled = Boolean::parseBoolean( 
        properties.getProperty( "wireFormat.tightEncodingEnabled", 
                                "1" ) );
    sizePrefixDisabled = Boolean::parseBoolean( 
        properties.getProperty( "wireFormat.sizePrefixDisabled", 
                                "0" ) );
}

////////////////////////////////////////////////////////////////////////////////
OpenWireFormat::~OpenWireFormat()
{
    try {
        
        for( size_t i = 0; i < dataMarshallers.size(); ++i )
        {
            delete dataMarshallers[i];
        }
        
        delete preferedWireFormatInfo;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::addMarshaller( DataStreamMarshaller* marshaller )
{
    unsigned char type = marshaller->getDataStructureType();
    dataMarshallers[type & 0xFF] = marshaller;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::setPreferedWireFormatInfo( 
    commands::WireFormatInfo* info ) throw ( IllegalStateException ) {
    
    this->preferedWireFormatInfo = info;

    try {
        preferedWireFormatInfo->setStackTraceEnabled( stackTraceEnabled );
        preferedWireFormatInfo->setCacheEnabled( cacheEnabled );
        preferedWireFormatInfo->setTcpNoDelayEnabled( tcpNoDelayEnabled );
        preferedWireFormatInfo->setTightEncodingEnabled( tightEncodingEnabled );
        preferedWireFormatInfo->setSizePrefixDisabled( sizePrefixDisabled );
    } catch( ActiveMQException& e ) {
        throw IllegalStateException(
            __FILE__, __LINE__,
            "OpenWireFormat::initialize - "
            "Could not configure WireFormatInfo" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::marshal( transport::Command* command, 
                              io::DataOutputStream* dataOut ) 
    throw ( io::IOException ) {

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
                BooleanStream* bs = new BooleanStream();
                size += dsm->tightMarshal1( this, dataStructure, bs );
                size += bs->marshalledSize(); 
    
                if( !sizePrefixDisabled ) {
                    dataOut->writeInt( size );
                }
    
                dataOut->writeByte( type );
                bs->marshal( dataOut );
                dsm->tightMarshal2( this, dataStructure, dataOut, bs );
    
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
                    dataOut->writeInt( (int)baos->getByteArraySize() );
                    dataOut->write( baos->getByteArray(), 
                                    baos->getByteArraySize() );
                                    
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
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
transport::Command* OpenWireFormat::unmarshal( io::DataInputStream* dis ) 
    throw ( io::IOException ) {

    try{
        
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
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int OpenWireFormat::tightMarshalNestedObject1( commands::DataStructure* object, 
                                               utils::BooleanStream* bs )
    throw ( io::IOException ) {

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
        
        DataStreamMarshaller* dsm = 
            dynamic_cast< DataStreamMarshaller* >( dataMarshallers[type & 0xFF] );
        
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
            ds->write( &sequence[0], sequence.size() );
            
        } else {
            
            DataStreamMarshaller* dsm = 
                dynamic_cast< DataStreamMarshaller* >( dataMarshallers[type & 0xFF] );
            
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
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* OpenWireFormat::tightUnmarshalNestedObject( DataInputStream* dis, 
                                                           BooleanStream* bs) 
    throw ( io::IOException ) {

    try {
        
        if( bs->readBoolean() ) {
            
            const unsigned char dataType = dis->readByte();
            
            DataStreamMarshaller* dsm = 
                dynamic_cast< DataStreamMarshaller* >( 
                    dataMarshallers[dataType & 0xFF] );
                    
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
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* OpenWireFormat::looseUnmarshalNestedObject( io::DataInputStream* dis ) 
    throw ( IOException ) {

    try{
        
        if( dis->readBoolean() ) {
            
            unsigned char dataType = dis->readByte();
            
            DataStreamMarshaller* dsm = 
                dynamic_cast< DataStreamMarshaller* >( 
                    dataMarshallers[dataType & 0xFF] );
                    
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
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::looseMarshalNestedObject( commands::DataStructure* o, 
                                               io::DataOutputStream* dataOut ) 
    throw ( io::IOException ) {
        
    try{

        dataOut->writeBoolean( o != NULL );
        if( o != NULL ) {
            
            unsigned char dataType = o->getDataStructureType();
            
            dataOut->writeByte( dataType );
            
            DataStreamMarshaller* dsm = 
                dynamic_cast< DataStreamMarshaller* >( 
                    dataMarshallers[dataType & 0xFF] );

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
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormat::renegotiateWireFormat( WireFormatInfo* info ) 
    throw ( exceptions::IllegalStateException ) {
    
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
    
}
