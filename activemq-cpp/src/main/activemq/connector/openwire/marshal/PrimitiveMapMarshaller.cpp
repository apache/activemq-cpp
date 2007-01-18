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

using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;

///////////////////////////////////////////////////////////////////////////////
PrimitiveMapMarshaller::PrimitiveMapMarshaller()
{
}

///////////////////////////////////////////////////////////////////////////////
PrimitiveMapMarshaller::~PrimitiveMapMarshaller()
{
}

///////////////////////////////////////////////////////////////////////////////
void PrimitiveMapMarshaller::marshal( const util::PrimitiveMap* map, 
                                      std::vector<unsigned char>& dest ) 
                                        throw ( cms::CMSException ) 
{
//    ByteArrayOutputStream bytesOut( dest );
//    DataOutputStream dataOut( &bytesOut );
//
//    if( map == NULL )
//    {
//        dataOut.writeInt( -1 );
//    }
//    else
//    {
//        dataOut.write( map.count() );
//        
//        foreach (DictionaryEntry entry in map)
//        {
//            std::string name = (std::string) entry.Key;
//            dataOut.Write(name);
//            Object value = entry.Value;
//            MarshalPrimitive(dataOut, value);
//    }

}

///////////////////////////////////////////////////////////////////////////////
PrimitiveMap* PrimitiveMapMarshaller::unmarshal( 
    const std::vector<unsigned char>& src ) 
        throw ( cms::CMSException ) 
{
    return NULL;
}

/*


        /// <summary>
        /// Marshals the primitive type map to a byte array
        /// </summary>
        public static byte[] MarshalPrimitiveMap(IDictionary map)
        {
            if (map == null)
            {
                return null;
            }
            else
            {
                MemoryStream memoryStream = new MemoryStream();
                MarshalPrimitiveMap(map, new OpenWireBinaryWriter(memoryStream));
                return memoryStream.GetBuffer();
            }
        }
        
        public static void MarshalPrimitiveMap(IDictionary map, BinaryWriter dataOut)
        {
            if (map == null)
            {
                dataOut.Write((int)-1);
            }
            else
            {
                dataOut.Write(map.Count);
                foreach (DictionaryEntry entry in map)
                {
                    String name = (String) entry.Key;
                    dataOut.Write(name);
                    Object value = entry.Value;
                    MarshalPrimitive(dataOut, value);
                }
            }}
        
        
        
        /// <summary>
        /// Unmarshals the primitive type map from the given byte array
        /// </summary>
        public static  IDictionary UnmarshalPrimitiveMap(byte[] data)
        {
            if (data == null)
            {
                return new Hashtable();
            }
            else
            {
                return UnmarshalPrimitiveMap(new OpenWireBinaryReader(new MemoryStream(data)));
            }
        }
        
        public static  IDictionary UnmarshalPrimitiveMap(BinaryReader dataIn)
        {
            int size = dataIn.ReadInt32();
            if (size < 0)
            {
                return null;
            }
            else
            {
                IDictionary answer = new Hashtable(size);
                for (int i=0; i < size; i++)
                {
                    String name = dataIn.ReadString();
                    answer[name] = UnmarshalPrimitive(dataIn);
                }
                return answer;
            }
            
        }

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


        public static void MarshalPrimitive(BinaryWriter dataOut, Object value)
        {
            if (value == null)
            {
                dataOut.Write(NULL);
            }
            else if (value is bool)
            {
                dataOut.Write(BOOLEAN_TYPE);
                dataOut.Write((bool) value);
            }
            else if (value is byte)
            {
                dataOut.Write(BYTE_TYPE);
                dataOut.Write(((byte)value));
            }
            else if (value is char)
            {
                dataOut.Write(CHAR_TYPE);
                dataOut.Write((char) value);
            }
            else if (value is short)
            {
                dataOut.Write(SHORT_TYPE);
                dataOut.Write((short) value);
            }
            else if (value is int)
            {
                dataOut.Write(INTEGER_TYPE);
                dataOut.Write((int) value);
            }
            else if (value is long)
            {
                dataOut.Write(LONG_TYPE);
                dataOut.Write((long) value);
            }
            else if (value is float)
            {
                dataOut.Write(FLOAT_TYPE);
                dataOut.Write((float) value);
            }
            else if (value is double)
            {
                dataOut.Write(DOUBLE_TYPE);
                dataOut.Write((double) value);
            }
            else if (value is byte[])
            {
                byte[] data = (byte[]) value;
                dataOut.Write(BYTE_ARRAY_TYPE);
                dataOut.Write(data.Length);
                dataOut.Write(data);
            }
            else if (value is string)
            {
                string s = (string) value;
                // is the string big??
                if (s.Length > 8191)
                {
                    dataOut.Write(BIG_STRING_TYPE);
                    dataOut.Write(s);
                }
                else
                {
                    dataOut.Write(STRING_TYPE);
                    dataOut.Write(s);
                }
            }
            else if (value is IDictionary)
            {
                dataOut.Write(MAP_TYPE);
                MarshalPrimitiveMap((IDictionary) value, dataOut);
            }
            else if (value is IList)
            {
                dataOut.Write(LIST_TYPE);
                MarshalPrimitiveList((IList) value, dataOut);
            }
            else
            {
                throw new IOException("Object is not a primitive: " + value);
            }
        }
        
        public static Object UnmarshalPrimitive(BinaryReader dataIn)
        {
            Object value=null;
            byte type = dataIn.ReadByte();
            switch (type)
            {
                case BYTE_TYPE:
                    value = dataIn.ReadByte();
                    break;
                case BOOLEAN_TYPE:
                    value = dataIn.ReadBoolean();
                    break;
                case CHAR_TYPE:
                    value = dataIn.ReadChar();
                    break;
                case SHORT_TYPE:
                    value = dataIn.ReadInt16();
                    break;
                case INTEGER_TYPE:
                    value = dataIn.ReadInt32();
                    break;
                case LONG_TYPE:
                    value = dataIn.ReadInt64();
                    break;
                case FLOAT_TYPE:
                    value = dataIn.ReadSingle();
                    break;
                case DOUBLE_TYPE:
                    value = dataIn.ReadDouble();
                    break;
                case BYTE_ARRAY_TYPE:
                    int size = dataIn.ReadInt32();
                    byte[] data = new byte[size];
                    dataIn.Read(data, 0, size);
                    value = data;
                    break;
                case STRING_TYPE:
                    value = dataIn.ReadString();
                    break;
                case BIG_STRING_TYPE:
                    value = dataIn.ReadString();
                    break;
                case MAP_TYPE:
                    value = UnmarshalPrimitiveMap(dataIn);
                    break;

                case LIST_TYPE:
                    value = UnmarshalPrimitiveList(dataIn);
                    break;

                default:
                    throw new Exception("Unsupported data type: " + type);
            }
            return value;
        }


*/
