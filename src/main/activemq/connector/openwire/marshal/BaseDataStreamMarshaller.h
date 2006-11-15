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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLER_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLER_H_

#include <activemq/connector/openwire/DataStreamMarshaller.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace marshal{

    class BaseDataStreamMarshaller : public DataStreamMarshaller
    {
    public:
    
    	BaseDataStreamMarshaller();
    	virtual ~BaseDataStreamMarshaller();

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties 
         * @param command -  the object to Marshal
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshal1( OpenWireFormat* format, 
                                   commands::DataStructure* command, 
                                   utils::BooleanStream* bs ) 
                                       throw ( io::IOException ) { return 0; };

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties 
         * @param command -  the object to Marshal
         * @param ds - the DataOutputStream to Marshal to
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshal2( OpenWireFormat* format, 
                                    commands::DataStructure* command, 
                                    io::DataOutputStream* ds, 
                                    utils::BooleanStream* bs ) 
                                        throw ( io::IOException ) {};

        /**
         * Tight Un-marhsal to the given stream
         * @param format - The OpenwireFormat properties 
         * @param command -  the object to Un-Marshal
         * @param dis - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @throws IOException if an error occurs.
         */
        virtual void tightUnmarshal( OpenWireFormat* format, 
                                     commands::DataStructure* command, 
                                     io::DataInputStream* dis, 
                                     utils::BooleanStream* bs ) 
                                        throw ( io::IOException ) {};
    
        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties 
         * @param command -  the object to Marshal
         * @param ds - DataOutputStream to marshal to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshal( OpenWireFormat* format, 
                                   commands::DataStructure* command, 
                                   io::DataOutputStream* ds ) 
                                       throw ( io::IOException ) {};

        /**
         * Loose Un-marhsal to the given stream
         * @param format - The OpenwireFormat properties 
         * @param command -  the object to Un-Marshal
         * @param dis - the DataInputStream to Un-Marshal from
         * @throws IOException if an error occurs.
         */
        virtual void looseUnmarshal( OpenWireFormat* format, 
                                     commands::DataStructure* command, 
                                     io::DataInputStream* dis ) 
                                        throw ( io::IOException ) {};

    protected:

        /**
         * Tight Unmarshal an array of char
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @returns the unmarshalled vector of chars.
         */
        virtual std::vector<char> tightUnmarshalByteArray( 
            io::DataInputStream* dataIn, 
            utils::BooleanStream* bs ) 
                { return std::vector<char>(); }

        /**
         * Loose Unmarshal an array of char
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @returns the unmarshalled vector of chars.
         */
        virtual std::vector<char> looseUnmarshalByteArray( 
            io::DataInputStream* dataIn )    
            { return std::vector<char>(); }

        /**
         * Tight Unmarshal a fixed size array from that data input stream
         * and return an stl vector of char as the resultant.
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @param size - size of the const array to unmarshal
         * @returns the unmarshalled vector of chars.
         */
        virtual std::vector<char> tightUnmarshalConstByteArray( 
            io::DataInputStream* dataIn, 
            utils::BooleanStream* bs, 
            int size ) { return std::vector<char>(); }

        /**
         * Tight Unmarshal a fixed size array from that data input stream
         * and return an stl vector of char as the resultant.
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @param size - size of the const array to unmarshal
         * @returns the unmarshalled vector of chars.
         */
        virtual std::vector<char> looseUnmarshalConstByteArray( 
            io::DataInputStream* dataIn, 
            int size ) { return std::vector<char>(); }

        /**
         * Performs Tight Unmarshalling of String Objects
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @returns the unmarshalled string.
         */
        virtual std::string tightUnmarshalString( io::DataInputStream* dataIn, 
                                                  utils::BooleanStream* bs ) {}


        /**
         * Tight Marshals the String to a Booleans Stream Object, returns
         * the marshalled size.
         * @param value - string to marshal
         * @param bs - BooleanStream to use.
         * @returns size of marshalled string. 
         */
        virtual int tightMarshalString1( const std::string& value, 
                                         utils::BooleanStream* bs ) {}

        /**
         * Tight Marshals the passed string to the streams passed
         * @param value - string to marshal
         * @param dataOut - the DataOutputStream to Marshal to
         * @param bs - boolean stream to marshal to.
         */ 
        virtual void tightMarshalString2( const std::string& value, 
                                          io::DataOutputStream* dataOut, 
                                          utils::BooleanStream* bs ) {}

        /**
         * Loose Marshal the String to the DataOuputStream passed
         * @param value - string to marshal
         * @param dataOut - stream to write marshalled form to
         */
        virtual void looseMarshalString( const std::string value, 
                                         io::DataOutputStream* dataOut ) {}
                                         
        /**
         * Loose Un-Marshal the String to the DataOuputStream passed
         * @param dataIn - stream to read marshalled form from
         * @return the unmarshalled string
         */
        virtual std::string looseUnmarshalString( io::DataInputStream* dataIn ) {}

        /**
         * Tightly marshal the long long to the BooleanStream passed.
         * @param wireFormat - The OpenwireFormat properties 
         * @param value - long long to marshal
         * @param bs - boolean stream to marshal to.
         * @returns size of data written.
         */ 
        virtual int tightMarshalLong1( OpenWireFormat* wireFormat, 
                                       long long value,
                                       utils::BooleanStream* bs ) {}

        /**
         * Tightly marshal the long long to the Streams passed.
         * @param wireFormat - The OpenwireFormat properties 
         * @param value - long long to marshal
         * @param dataOut - stream to write marshalled form to
         * @param bs - boolean stream to marshal to.
         */ 
        virtual void tightMarshalLong2( OpenWireFormat* wireFormat, 
                                        long long value, 
                                        io::DataOutputStream* dataOut, 
                                        utils::BooleanStream* bs ) {}

        /**
         * Tight marshal the long long type.  
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns the unmarshalled long long 
         */
        virtual long long tightUnmarshalLong( OpenWireFormat* wireFormat, 
                                              io::DataInputStream* dataIn, 
                                              utils::BooleanStream* bs ) {}

        
        /**
         * Tightly marshal the long long to the BooleanStream passed.
         * @param wireFormat - The OpenwireFormat properties 
         * @param value - long long to marshal
         * @param dataOut - DataOutputStream to marshal to.
         */ 
        virtual void looseMarshalLong( OpenWireFormat* wireFormat, 
                                       long long value, 
                                       io::DataOutputStream* dataOut ) {}
        
        /**
         * Loose marshal the long long type.  
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @returns the unmarshalled long long 
         */
        virtual long long looseUnmarshalLong( OpenWireFormat* wireFormat, 
                                              io::DataInputStream* dataIn ) { return 0; }
        
        /**
         * Tight Unmarshall the cached object
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns pointer to a new DataStructure Object
         */
        virtual commands::DataStructure* tightUnmarshalCachedObject(
            OpenWireFormat* wireFormat, 
            io::DataInputStream* dataIn,
            utils::BooleanStream* bs ) {}

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * BooleanStream returning the size of the data marshalled
         * @param wireFormat - The OpenwireFormat properties
         * @param data - DataStructure Object Pointer to marshal 
         * @param bs - boolean stream to marshal to.
         * @returns size of data written.
         */
        virtual int tightMarshalCachedObject1( OpenWireFormat* wireFormat, 
                                               commands::DataStructure* data, 
                                               utils::BooleanStream* bs ) { return 0; }

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * streams returning nothing
         * @param wireFormat - The OpenwireFormat properties
         * @param data - DataStructure Object Pointer to marshal 
         * @param bs - boolean stream to marshal to.
         * @param dataOut - stream to write marshalled data to
         */
        virtual void tightMarshalCachedObject2( OpenWireFormat* wireFormat, 
                                                commands::DataStructure* data, 
                                                io::DataOutputStream* dataOut, 
                                                utils::BooleanStream* bs ) {}


        /**
         * Loosely marshalls the passed DataStructure based object to the passed
         * stream returning nothing
         * @param wireFormat - The OpenwireFormat properties
         * @param data - DataStructure Object Pointer to marshal 
         * @param bs - boolean stream to marshal to.
         * @param dataOut - stream to write marshalled data to
         */
        virtual void looseMarshalCachedObject( OpenWireFormat* wireFormat, 
                                               commands::DataStructure* data, 
                                               io::DataOutputStream* dataOut ) {}
        
        /**
         * Loose Unmarshall the cached object
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @returns pointer to a new DataStructure Object
         */
        virtual commands::DataStructure* looseUnmarshalCachedObject( 
            OpenWireFormat* wireFormat, 
            io::DataInputStream* dataIn ) { return NULL; }

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * BooleanStream returning the size of the data marshalled
         * @param wireFormat - The OpenwireFormat properties
         * @param object - DataStructure Object Pointer to marshal 
         * @param bs - boolean stream to marshal to.
         * @returns size of data written.
         */
        virtual int tightMarshalNestedObject1( OpenWireFormat* wireFormat, 
                                               commands::DataStructure* object, 
                                               utils::BooleanStream* bs ) { return 0; } 

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * streams returning nothing
         * @param wireFormat - The OpenwireFormat properties
         * @param object - DataStructure Object Pointer to marshal 
         * @param bs - boolean stream to marshal to.
         * @param dataOut - stream to write marshalled data to
         */
        virtual void tightMarshalNestedObject2( OpenWireFormat* wireFormat, 
                                                commands::DataStructure* object, 
                                                io::DataOutputStream* dataOut, 
                                                utils::BooleanStream* bs ) {}

        /**
         * Tight Unmarshall the nested object
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns pointer to a new DataStructure Object
         */
        virtual commands::DataStructure* tightUnmarshalNestedObject(
            OpenWireFormat* wireFormat,
            io::DataInputStream* dataIn,
            utils::BooleanStream* bs ) { return NULL; }

        /**
         * Loose Unmarshall the nested object
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @returns pointer to a new DataStructure Object
         */
        virtual commands::DataStructure* looseUnmarshalNestedObject( 
            OpenWireFormat* wireFormat,
            io::DataInputStream* dataIn ) { return NULL; }

        /**
         * Loose marshall the nested object
         * @param wireFormat - The OpenwireFormat properties 
         * @param object - DataStructure Object Pointer to marshal 
         * @param dataOut - stream to write marshalled data to
         */
        virtual void looseMarshalNestedObject( OpenWireFormat* wireFormat, 
                                               commands::DataStructure* object, 
                                               io::DataOutputStream* dataOut ) {}

        /**
         * Tight Unarshall the Error object
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns pointer to a new DataStructure Object
         */
        virtual commands::DataStructure* tightUnmarshalBrokerError( 
            OpenWireFormat* wireFormat, 
            io::DataInputStream* dataIn, 
            utils::BooleanStream* bs ) { return NULL; }
            
        /**
         * Tight Marshall the Error object
         * @param wireFormat - The OpenwireFormat properties 
         * @param error - Error to Marshal
         * @param bs - boolean stream to marshal to.
         * @returns size of the marshalled data
         */
        virtual int tightMarshalBrokerError1( OpenWireFormat* wireFormat, 
                                              commands::DataStructure* error, 
                                              utils::BooleanStream* bs ) { return 0; }
                                              
        /**
         * Tight Marshall the Error object
         * @param wireFormat - The OpenwireFormat properties 
         * @param error - Error to Marshal
         * @param dataOut - stream to write marshalled data to
         * @param bs - boolean stream to marshal to.
         */
        virtual void tightMarshalBrokerError2( OpenWireFormat* wireFormat, 
                                               commands::DataStructure* error, 
                                               io::DataOutputStream* dataOut, 
                                               utils::BooleanStream* bs ) {}
                                               
        /**
         * Loose Unarshall the Error object
         * @param wireFormat - The OpenwireFormat properties 
         * @param dataIn - stream to read marshalled form from
         * @returns pointer to a new DataStructure Object
         */
        virtual commands::DataStructure* looseUnmarshalBrokerError( 
            OpenWireFormat* wireFormat, 
            io::DataInputStream* dataIn ) { return NULL; }
            
        /**
         * Tight Marshall the Error object
         * @param wireFormat - The OpenwireFormat properties 
         * @param error - Error to Marshal
         * @param dataOut - stream to write marshalled data to
         */
        virtual void looseMarshalBrokerError( OpenWireFormat* wireFormat, 
                                              commands::DataStructure* error, 
                                              io::DataOutputStream* dataOut ) {}
        
    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLER_H_*/
