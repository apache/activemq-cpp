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

#include <activemq/connector/openwire/marshal/DataStreamMarshaller.h>
#include <activemq/connector/openwire/commands/MessageId.h>
#include <activemq/connector/openwire/commands/ProducerId.h>
#include <activemq/connector/openwire/commands/TransactionId.h>
#include <activemq/connector/openwire/utils/HexTable.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace marshal{

    /**
     * Base class for all Marshallers that marshal DataStructures to and
     * from the wire using the OpenWire protocal.
     */
    class AMQCPP_API BaseDataStreamMarshaller : public DataStreamMarshaller {
    private:

        // Table for converting bytes to Hex Strings.
        static utils::HexTable hexTable;

    public:

        virtual ~BaseDataStreamMarshaller() {}

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Marshal
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshal1( OpenWireFormat* format AMQCPP_UNUSED,
                                   commands::DataStructure* command AMQCPP_UNUSED,
                                   utils::BooleanStream* bs AMQCPP_UNUSED )
                                       throw ( decaf::io::IOException ) { return 0; }

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Marshal
         * @param ds - the DataOutputStream to Marshal to
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshal2( OpenWireFormat* format AMQCPP_UNUSED,
                                    commands::DataStructure* command AMQCPP_UNUSED,
                                    decaf::io::DataOutputStream* ds AMQCPP_UNUSED,
                                    utils::BooleanStream* bs AMQCPP_UNUSED )
                                        throw ( decaf::io::IOException ) {}

        /**
         * Tight Un-marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Un-Marshal
         * @param dis - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @throws IOException if an error occurs.
         */
        virtual void tightUnmarshal( OpenWireFormat* format AMQCPP_UNUSED,
                                     commands::DataStructure* command AMQCPP_UNUSED,
                                     decaf::io::DataInputStream* dis AMQCPP_UNUSED,
                                     utils::BooleanStream* bs AMQCPP_UNUSED )
                                        throw ( decaf::io::IOException ) {}

        /**
         * Tight Marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Marshal
         * @param ds - DataOutputStream to marshal to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshal( OpenWireFormat* format AMQCPP_UNUSED,
                                   commands::DataStructure* command AMQCPP_UNUSED,
                                   decaf::io::DataOutputStream* ds AMQCPP_UNUSED )
                                       throw ( decaf::io::IOException ) {}

        /**
         * Loose Un-marhsal to the given stream
         * @param format - The OpenwireFormat properties
         * @param command -  the object to Un-Marshal
         * @param dis - the DataInputStream to Un-Marshal from
         * @throws IOException if an error occurs.
         */
        virtual void looseUnmarshal( OpenWireFormat* format AMQCPP_UNUSED,
                                     commands::DataStructure* command AMQCPP_UNUSED,
                                     decaf::io::DataInputStream* dis AMQCPP_UNUSED )
                                        throw ( decaf::io::IOException ) {}

    public:      // Statics

        /**
         * Converts the object to a String
         * @param id - MessageId pointer
         * @returns string representing the id
         */
        static std::string toString( const commands::MessageId* id );

        /**
         * Converts the object to a String
         * @param id - ProducerId pointer
         * @returns string representing the id
         */
        static std::string toString( const commands::ProducerId* id );

        /**
         * Converts the given transaction ID into a String
         * @param txnId - TransactionId poitner
         * @returns string representation of the id
         */
        static std::string toString( const commands::TransactionId* txnId );

        /**
         * given an array of bytes, convert that array to a Hexidecimal
         * coded string that represents that data.
         * @param data - unsigned char data array pointer
         * @returns a string coded in hex that represents the data
         */
        static std::string toHexFromBytes(
            const std::vector<unsigned char>& data );

    protected:

        /**
         * Tight Unmarshall the cached object
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns pointer to a new DataStructure Object
         * @throws IOException if an error occurs.
         */
        virtual commands::DataStructure* tightUnmarshalCachedObject(
            OpenWireFormat* wireFormat,
            decaf::io::DataInputStream* dataIn,
            utils::BooleanStream* bs ) throw ( decaf::io::IOException );

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * BooleanStream returning the size of the data marshalled
         * @param wireFormat - The OpenwireFormat properties
         * @param data - DataStructure Object Pointer to marshal
         * @param bs - boolean stream to marshal to.
         * @returns size of data written.
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshalCachedObject1( OpenWireFormat* wireFormat,
                                               commands::DataStructure* data,
                                               utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * streams returning nothing
         * @param wireFormat - The OpenwireFormat properties
         * @param data - DataStructure Object Pointer to marshal
         * @param bs - boolean stream to marshal to.
         * @param dataOut - stream to write marshalled data to
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshalCachedObject2( OpenWireFormat* wireFormat,
                                                commands::DataStructure* data,
                                                decaf::io::DataOutputStream* dataOut,
                                                utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Loosely marshalls the passed DataStructure based object to the passed
         * stream returning nothing
         * @param wireFormat - The OpenwireFormat properties
         * @param data - DataStructure Object Pointer to marshal
         * @param bs - boolean stream to marshal to.
         * @param dataOut - stream to write marshalled data to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshalCachedObject( OpenWireFormat* wireFormat,
                                               commands::DataStructure* data,
                                               decaf::io::DataOutputStream* dataOut )
                                                    throw ( decaf::io::IOException );

        /**
         * Loose Unmarshall the cached object
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @returns pointer to a new DataStructure Object
         * @throws IOException if an error occurs.
         */
        virtual commands::DataStructure* looseUnmarshalCachedObject(
            OpenWireFormat* wireFormat,
            decaf::io::DataInputStream* dataIn ) throw ( decaf::io::IOException );

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * BooleanStream returning the size of the data marshalled
         * @param wireFormat - The OpenwireFormat properties
         * @param object - DataStructure Object Pointer to marshal
         * @param bs - boolean stream to marshal to.
         * @returns size of data written.
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshalNestedObject1( OpenWireFormat* wireFormat,
                                               commands::DataStructure* object,
                                               utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Tightly marshalls the passed DataStructure based object to the passed
         * streams returning nothing
         * @param wireFormat - The OpenwireFormat properties
         * @param object - DataStructure Object Pointer to marshal
         * @param bs - boolean stream to marshal to.
         * @param dataOut - stream to write marshalled data to
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshalNestedObject2( OpenWireFormat* wireFormat,
                                                commands::DataStructure* object,
                                                decaf::io::DataOutputStream* dataOut,
                                                utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Tight Unmarshall the nested object
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns pointer to a new DataStructure Object
         * @throws IOException if an error occurs.
         */
        virtual commands::DataStructure* tightUnmarshalNestedObject(
            OpenWireFormat* wireFormat,
            decaf::io::DataInputStream* dataIn,
            utils::BooleanStream* bs ) throw ( decaf::io::IOException );

        /**
         * Loose Unmarshall the nested object
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @returns pointer to a new DataStructure Object
         * @throws IOException if an error occurs.
         */
        virtual commands::DataStructure* looseUnmarshalNestedObject(
            OpenWireFormat* wireFormat,
            decaf::io::DataInputStream* dataIn ) throw ( decaf::io::IOException );

        /**
         * Loose marshall the nested object
         * @param wireFormat - The OpenwireFormat properties
         * @param object - DataStructure Object Pointer to marshal
         * @param dataOut - stream to write marshalled data to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshalNestedObject( OpenWireFormat* wireFormat,
                                               commands::DataStructure* object,
                                               decaf::io::DataOutputStream* dataOut )
                                                    throw ( decaf::io::IOException );

        /**
         * Performs Tight Unmarshalling of String Objects
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @returns the unmarshalled string.
         * @throws IOException if an error occurs.
         */
        virtual std::string tightUnmarshalString( decaf::io::DataInputStream* dataIn,
                                                  utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Tight Marshals the String to a Booleans Stream Object, returns
         * the marshalled size.
         * @param value - string to marshal
         * @param bs - BooleanStream to use.
         * @returns size of marshalled string.
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshalString1( const std::string& value,
                                         utils::BooleanStream* bs )
                                            throw ( decaf::io::IOException );

        /**
         * Tight Marshals the passed string to the streams passed
         * @param value - string to marshal
         * @param dataOut - the DataOutputStream to Marshal to
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshalString2( const std::string& value,
                                          decaf::io::DataOutputStream* dataOut,
                                          utils::BooleanStream* bs )
                                                throw ( decaf::io::IOException );

        /**
         * Loose Marshal the String to the DataOuputStream passed
         * @param value - string to marshal
         * @param dataOut - stream to write marshalled form to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshalString( const std::string value,
                                         decaf::io::DataOutputStream* dataOut )
                                            throw ( decaf::io::IOException );

        /**
         * Loose Un-Marshal the String to the DataOuputStream passed
         * @param dataIn - stream to read marshalled form from
         * @return the unmarshalled string
         * @throws IOException if an error occurs.
         */
        virtual std::string looseUnmarshalString( decaf::io::DataInputStream* dataIn )
            throw ( decaf::io::IOException );

        /**
         * Tightly marshal the long long to the BooleanStream passed.
         * @param wireFormat - The OpenwireFormat properties
         * @param value - long long to marshal
         * @param bs - boolean stream to marshal to.
         * @returns size of data written.
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshalLong1( OpenWireFormat* wireFormat,
                                       long long value,
                                       utils::BooleanStream* bs )
                                            throw ( decaf::io::IOException );

        /**
         * Tightly marshal the long long to the Streams passed.
         * @param wireFormat - The OpenwireFormat properties
         * @param value - long long to marshal
         * @param dataOut - stream to write marshalled form to
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshalLong2( OpenWireFormat* wireFormat,
                                        long long value,
                                        decaf::io::DataOutputStream* dataOut,
                                        utils::BooleanStream* bs )
                                            throw ( decaf::io::IOException );

        /**
         * Tight marshal the long long type.
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns the unmarshalled long long
         * @throws IOException if an error occurs.
         */
        virtual long long tightUnmarshalLong( OpenWireFormat* wireFormat,
                                              decaf::io::DataInputStream* dataIn,
                                              utils::BooleanStream* bs )
                                                throw ( decaf::io::IOException );

        /**
         * Tightly marshal the long long to the BooleanStream passed.
         * @param wireFormat - The OpenwireFormat properties
         * @param value - long long to marshal
         * @param dataOut - DataOutputStream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshalLong( OpenWireFormat* wireFormat,
                                       long long value,
                                       decaf::io::DataOutputStream* dataOut )
                                            throw ( decaf::io::IOException );

        /**
         * Loose marshal the long long type.
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @returns the unmarshalled long long
         * @throws IOException if an error occurs.
         */
        virtual long long looseUnmarshalLong( OpenWireFormat* wireFormat,
                                              decaf::io::DataInputStream* dataIn )
                                                throw ( decaf::io::IOException );

        /**
         * Tight Unmarshal an array of char
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @returns the unmarshalled vector of chars.
         * @throws IOException if an error occurs.
         */
        virtual std::vector<unsigned char> tightUnmarshalByteArray(
            decaf::io::DataInputStream* dataIn,
            utils::BooleanStream* bs ) throw ( decaf::io::IOException );

        /**
         * Loose Unmarshal an array of char
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @returns the unmarshalled vector of chars.
         * @throws IOException if an error occurs.
         */
        virtual std::vector<unsigned char> looseUnmarshalByteArray(
            decaf::io::DataInputStream* dataIn ) throw ( decaf::io::IOException );

        /**
         * Tight Unmarshal a fixed size array from that data input stream
         * and return an stl vector of char as the resultant.
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @param size - size of the const array to unmarshal
         * @returns the unmarshalled vector of chars.
         * @throws IOException if an error occurs.
         */
        virtual std::vector<unsigned char> tightUnmarshalConstByteArray(
            decaf::io::DataInputStream* dataIn,
            utils::BooleanStream* bs,
            int size ) throw ( decaf::io::IOException );

        /**
         * Tight Unmarshal a fixed size array from that data input stream
         * and return an stl vector of char as the resultant.
         * @param dataIn - the DataInputStream to Un-Marshal from
         * @param bs - boolean stream to unmarshal from.
         * @param size - size of the const array to unmarshal
         * @returns the unmarshalled vector of chars.
         * @throws IOException if an error occurs.
         */
        virtual std::vector<unsigned char> looseUnmarshalConstByteArray(
            decaf::io::DataInputStream* dataIn,
            int size ) throw ( decaf::io::IOException );

        /**
         * Tight Unarshall the Error object
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @param bs - boolean stream to marshal to.
         * @returns pointer to a new DataStructure Object
         * @throws IOException if an error occurs.
         */
        virtual commands::DataStructure* tightUnmarshalBrokerError(
            OpenWireFormat* wireFormat,
            decaf::io::DataInputStream* dataIn,
            utils::BooleanStream* bs ) throw ( decaf::io::IOException );

        /**
         * Tight Marshall the Error object
         * @param wireFormat - The OpenwireFormat properties
         * @param data - Error to Marshal
         * @param bs - boolean stream to marshal to.
         * @returns size of the marshalled data
         * @throws IOException if an error occurs.
         */
        virtual int tightMarshalBrokerError1( OpenWireFormat* wireFormat,
                                              commands::DataStructure* data,
                                              utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Tight Marshall the Error object
         * @param wireFormat - The OpenwireFormat properties
         * @param data - Error to Marshal
         * @param dataOut - stream to write marshalled data to
         * @param bs - boolean stream to marshal to.
         * @throws IOException if an error occurs.
         */
        virtual void tightMarshalBrokerError2( OpenWireFormat* wireFormat,
                                               commands::DataStructure* data,
                                               decaf::io::DataOutputStream* dataOut,
                                               utils::BooleanStream* bs )
                                                    throw ( decaf::io::IOException );

        /**
         * Loose Unarshall the Error object
         * @param wireFormat - The OpenwireFormat properties
         * @param dataIn - stream to read marshalled form from
         * @returns pointer to a new DataStructure Object
         * @throws IOException if an error occurs.
         */
        virtual commands::DataStructure* looseUnmarshalBrokerError(
            OpenWireFormat* wireFormat,
            decaf::io::DataInputStream* dataIn ) throw ( decaf::io::IOException );

        /**
         * Tight Marshall the Error object
         * @param wireFormat - The OpenwireFormat properties
         * @param data - Error to Marshal
         * @param dataOut - stream to write marshalled data to
         * @throws IOException if an error occurs.
         */
        virtual void looseMarshalBrokerError( OpenWireFormat* wireFormat,
                                              commands::DataStructure* data,
                                              decaf::io::DataOutputStream* dataOut )
                                                  throw ( decaf::io::IOException );

        /**
         * Tightly Marshal an array of DataStructure objects to the provided
         * boolean stream, and return the size that the tight marshalling is
         * going to take.
         * @param wireFormat - The OpenwireFormat properties
         * @param objects - array of DataStructure object pointers.
         * @param bs - boolean stream to marshal to.
         * @returns size of the marshalled data
         * @throws IOException if an error occurs.
         */
        template<typename T>
        int tightMarshalObjectArray1( OpenWireFormat* wireFormat,
                                      std::vector<T*> objects,
                                      utils::BooleanStream* bs )
                                        throw ( decaf::io::IOException )
        {
            try{
                if( !objects.empty() )
                {
                    int rc = 0;
                    bs->writeBoolean( true );
                    rc += 2;
                    for( std::size_t i = 0; i < objects.size(); ++i ) {
                        rc += tightMarshalNestedObject1(
                                wireFormat, objects[i], bs );
                    }
                    return rc;

                } else {
                    bs->writeBoolean( false );
                    return 0;
                }
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

        /**
         * Tightly Marshal an array of DataStructure objects to the provided
         * boolean stream and data output stream
         * @param wireFormat - The OpenwireFormat properties
         * @param objects - array of DataStructure object pointers.
         * @param dataOut - stream to write marshalled data to
         * @param bs - boolean stream to marshal to.
         * @returns size of the marshalled data
         * @throws IOException if an error occurs.
         */
        template<typename T>
        void tightMarshalObjectArray2( OpenWireFormat* wireFormat,
                                       std::vector<T*> objects,
                                       decaf::io::DataOutputStream* dataOut,
                                       utils::BooleanStream* bs )
                                        throw ( decaf::io::IOException ) {

            try {

                if( bs->readBoolean() ) {

                    dataOut->writeShort( (short)objects.size() );
                    for( std::size_t i = 0; i < objects.size(); ++i ) {
                        tightMarshalNestedObject2(
                            wireFormat, objects[i], dataOut, bs );
                    }
                }
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

        /**
         * Loosely Marshal an array of DataStructure objects to the provided
         * boolean stream and data output stream
         * @param wireFormat - The OpenwireFormat properties
         * @param objects - array of DataStructure object pointers.
         * @param dataOut - stream to write marshalled data to
         * @returns size of the marshalled data
         * @throws IOException if an error occurs.
         */
        template<typename T>
        void looseMarshalObjectArray( OpenWireFormat* wireFormat,
                                      std::vector<T*> objects,
                                      decaf::io::DataOutputStream* dataOut )
                                        throw ( decaf::io::IOException ) {

            try {

                dataOut->writeBoolean( !objects.empty() );
                if( !objects.empty() ) {

                    dataOut->writeShort( (short)objects.size() );
                    for( std::size_t i = 0; i < objects.size(); ++i ) {
                        looseMarshalNestedObject(
                            wireFormat, objects[i], dataOut );
                    }
                }
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

    protected:

        /**
         * Given an DataInputStream read a know ASCII formatted string from
         * the input and return that string
         * @param dataIn - DataInputStream to read from
         * @return string value read from stream
         */
        virtual std::string readAsciiString( decaf::io::DataInputStream* dataIn )
            throw ( decaf::io::IOException );

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLER_H_*/
