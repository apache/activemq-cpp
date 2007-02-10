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

#include "PrimitiveMapMarshaller.h"

#include <activemq/io/ByteArrayInputStream.h>
#include <activemq/io/ByteArrayOutputStream.h>
#include <activemq/io/DataInputStream.h>
#include <activemq/io/DataOutputStream.h>
#include <activemq/connector/openwire/utils/OpenwireStringSupport.h>

using namespace activemq;
using namespace activemq::io;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::marshal;

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::marshal( const util::PrimitiveMap* map,
                                      std::vector<unsigned char>& dest )
                                        throw ( cms::CMSException )
{
    try {

        ByteArrayOutputStream bytesOut( dest );
        DataOutputStream dataOut( &bytesOut );

        if( map == NULL )
        {
            dataOut.writeInt( -1 );
        }
        else
        {
            dataOut.writeInt( (int)map->size() );

            std::vector<std::string> keys = map->getKeys();
            std::vector<std::string>::const_iterator iter = keys.begin();

            for(; iter != keys.end(); ++iter ) {

                OpenwireStringSupport::writeString( dataOut, &(*iter) );
                PrimitiveMap::ValueNode value = map->getValue( *iter );
                marshalPrimitive( dataOut, value );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
PrimitiveMap* PrimitiveMapMarshaller::unmarshal(
    const std::vector<unsigned char>& src )
        throw ( cms::CMSException )
{
    try{

        ByteArrayInputStream bytesIn( src );
        DataInputStream dataIn( &bytesIn );

        int size = dataIn.readInt();

        if( size > 0 )
        {
            PrimitiveMap* map = new PrimitiveMap;

            for( int i=0; i < size; i++ )
            {
                std::string key = OpenwireStringSupport::readString( dataIn );
                unmarshalPrimitive( dataIn, key, *map );
            }

            return map;
        }

        return NULL;

    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::unmarshal(
    util::PrimitiveMap* map,
    const std::vector<unsigned char>& src ) throw ( cms::CMSException ) {

    try {

        if( map == NULL ) {
            return;
        }

        // Clear old data
        map->clear();

        ByteArrayInputStream bytesIn( src );
        DataInputStream dataIn( &bytesIn );

        int size = dataIn.readInt();

        if( size > 0 )
        {
            for( int i=0; i < size; i++ )
            {
                std::string key = OpenwireStringSupport::readString( dataIn );
                unmarshalPrimitive( dataIn, key, *map );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::marshalPrimitive( io::DataOutputStream& dataOut,
                                               util::PrimitiveMap::ValueNode& value )
                                                    throw ( cms::CMSException ) {

    try {

        if( value.getValueType() == PrimitiveMap::BOOLEAN_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::BOOLEAN_TYPE );
            dataOut.writeBoolean( value.getBool() );
        }
        else if( value.getValueType() == PrimitiveMap::BYTE_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::BYTE_TYPE );
            dataOut.writeByte( value.getByte() );
        }
        else if( value.getValueType() == PrimitiveMap::CHAR_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::CHAR_TYPE );
            dataOut.writeChar( value.getChar() );
        }
        else if( value.getValueType() == PrimitiveMap::SHORT_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::SHORT_TYPE );
            dataOut.writeShort( value.getShort() );
        }
        else if( value.getValueType() == PrimitiveMap::INTEGER_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::INTEGER_TYPE );
            dataOut.writeInt( value.getInt() );
        }
        else if( value.getValueType() == PrimitiveMap::LONG_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::LONG_TYPE );
            dataOut.writeLong( value.getLong() );
        }
        else if( value.getValueType() == PrimitiveMap::FLOAT_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::FLOAT_TYPE );
            dataOut.writeFloat( value.getFloat() );
        }
        else if( value.getValueType() == PrimitiveMap::DOUBLE_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::DOUBLE_TYPE );
            dataOut.writeDouble( value.getDouble() );
        }
        else if( value.getValueType() == PrimitiveMap::BYTE_ARRAY_TYPE )
        {
            dataOut.writeByte( PrimitiveMap::BYTE_ARRAY_TYPE );

            std::vector<unsigned char> data = value.getByteArray();

            dataOut.writeInt( (int)data.size() );
            dataOut.write( data );
        }
        else if( value.getValueType() == PrimitiveMap::STRING_TYPE )
        {
            std::string data = value.getString();

            // is the string big??
            if( data.size() > 8191 ) {
                dataOut.writeByte( PrimitiveMap::BIG_STRING_TYPE );
            } else {
                dataOut.writeByte( PrimitiveMap::STRING_TYPE );
            }

            OpenwireStringSupport::writeString( dataOut, &data );
        }
//        else if( value is IDictionary )
//        {
//            dataOut.Write( MAP_TYPE );
//            MarshalPrimitiveMap((IDictionary) value, dataOut);
//        }
//        else if( value is IList )
//        {
//            dataOut.Write( LIST_TYPE );
//            MarshalPrimitiveList((IList) value, dataOut);
//        }
        else
        {
            throw IOException(
                __FILE__,
                __LINE__,
                "Object is not a primitive: ");
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::unmarshalPrimitive( io::DataInputStream& dataIn,
                                                 const std::string& key,
                                                 util::PrimitiveMap& map )
                                                    throw ( cms::CMSException ) {

    try {

        unsigned char type = dataIn.readByte();

        switch( type )
        {
            case PrimitiveMap::BYTE_TYPE:
                map.setByte( key, dataIn.readByte() );
                break;
            case PrimitiveMap::BOOLEAN_TYPE:
                map.setBool( key, dataIn.readBoolean() );
                break;
            case PrimitiveMap::CHAR_TYPE:
                map.setChar( key, dataIn.readChar() );
                break;
            case PrimitiveMap::SHORT_TYPE:
                map.setShort( key, dataIn.readShort() );
                break;
            case PrimitiveMap::INTEGER_TYPE:
                map.setInt( key, dataIn.readInt() );
                break;
            case PrimitiveMap::LONG_TYPE:
                map.setLong( key, dataIn.readLong() );
                break;
            case PrimitiveMap::FLOAT_TYPE:
                map.setFloat( key, dataIn.readFloat() );
                break;
            case PrimitiveMap::DOUBLE_TYPE:
                map.setDouble( key, dataIn.readDouble() );
                break;
            case PrimitiveMap::BYTE_ARRAY_TYPE:
            {
                int size = dataIn.readInt();
                std::vector<unsigned char> data;
                data.resize( size );
                dataIn.readFully( data );
                map.setByteArray( key, data );
                break;
            }
            case PrimitiveMap::STRING_TYPE:
            case PrimitiveMap::BIG_STRING_TYPE:
                map.setString(
                    key,
                    OpenwireStringSupport::readString( dataIn ) );
                break;
//            case PrimitiveMap::MAP_TYPE:
//                value = UnmarshalPrimitiveMap(dataIn);
//                break;
//            case PrimitiveMap::LIST_TYPE:
//                value = UnmarshalPrimitiveList(dataIn);
//                break;
            default:
                throw IOException(
                    __FILE__,
                    __LINE__,
                    "PrimitiveMapMarshaller::unmarshalPrimitive - "
                    "Unsupported data type: ");
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

/*

        public static void MarshalPrimitiveList(IList list, BinaryWriter dataOut)
        {
            dataOut.Write((int) list.Count);
            foreach (Object element in list)
            {
                MarshalPrimitive(dataOut, element);
            }
        }

        public static IList UnmarshalPrimitiveList(BinaryReader dataIn)
        {
            int size = dataIn.ReadInt32();
            IList answer = new ArrayList(size);
            while (size-- > 0) {
                answer.Add(UnmarshalPrimitive(dataIn));
            }
            return answer;
        }


*/
