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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREFORMAT_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREFORMAT_H_

#include <activemq/util/Config.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/connector/openwire/commands/DataStructure.h>
#include <activemq/connector/openwire/utils/BooleanStream.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace activemq{
namespace connector{
namespace openwire{

    class DataStreamMarshaller;

    class AMQCPP_API OpenWireFormat : public wireformat::WireFormat {
    public:

        /**
         * Constructs a new OpenWireFormat object
         * @param properties - can contain optional config params.
         */
        OpenWireFormat( const decaf::util::Properties& properties );

        virtual ~OpenWireFormat();

        /**
         * Allows an external source to add marshallers to this object for
         * types that may be marshalled or unmarhsalled.
         * @param marshaller - the Marshaller to add to the collection.
         */
        void addMarshaller( DataStreamMarshaller* marshaller );

        /**
         * Stream based marshaling
         * @param command - The Command to Marshal
         * @param out - the output stream to write the command to.
         * @throws IOException
         */
        virtual void marshal( transport::Command* command,
                              decaf::io::DataOutputStream* dataOut )
            throw ( decaf::io::IOException );

        /**
         * Packet based un-marshaling
         * @param dis - the input stream to read the command from.
         * @returns the newly marshaled Command, caller owns the pointer
         * @throws IOException
         */
        virtual transport::Command* unmarshal( decaf::io::DataInputStream* dis )
            throw ( decaf::io::IOException );

        /**
         * Utility method for Tight Marshalling the given object to the boolean
         * stream passed.
         * @param object - The DataStructure to marshal
         * @param bs - the BooleanStream to write to
         * @returns size of the data returned.
         */
        virtual int tightMarshalNestedObject1( commands::DataStructure* object,
                                               utils::BooleanStream* bs )
            throw ( decaf::io::IOException );

        /**
         * Utility method that will Tight marshall some internally nested object
         * that implements the DataStructure interface.  Writes the data to the
         * Data Ouput Stream provided.
         * @param o - DataStructure object
         * @param ds - DataOuputStream for writing
         * @param bs - BooleanStream
         * @throws IOException if an error occurs.
         */
        void tightMarshalNestedObject2( commands::DataStructure* o,
                                        decaf::io::DataOutputStream* ds,
                                        utils::BooleanStream* bs )
            throw ( decaf::io::IOException );

        /**
         * Utility method used to Unmarshal a Nested DataStructure type object
         * from the given DataInputStream.  The DataStructure instance that is
         * returned is now the property of the caller.
         * @param ds - DataInputStream to read from
         * @param bs - BooleanStream to read from
         * @returns Newly allocated DataStructure Object
         * @throws IOException if an error occurs.
         */
        commands::DataStructure* tightUnmarshalNestedObject( decaf::io::DataInputStream* dis,
                                                             utils::BooleanStream* bs )
            throw ( decaf::io::IOException );

        /**
         * Utility method to unmarshal an DataStructure object from an
         * DataInputStream using the Loose Unmarshalling format.  Will read
         * the Data and construct a new DataStructure based Object, the
         * pointer to the Object returned is now owned by the caller.
         * @param dis - the DataInputStream to read the data from
         * @returns a new DataStructure derived Object pointer
         * @throws IOException if an error occurs.
         */
        commands::DataStructure* looseUnmarshalNestedObject(
            decaf::io::DataInputStream* dis)
                throw ( decaf::io::IOException );

        /**
         * Utility method to loosely Marshal an object that is derived from the
         * DataStrucutre interface.  The marshalled data is written to the
         * passed in DataOutputStream.
         * @param o - DataStructure derived Object to Marshal
         * @param dataOut - DataOutputStream to write the data to
         * @throw IOException if an error occurs.
         */
        void looseMarshalNestedObject( commands::DataStructure* o,
                                       decaf::io::DataOutputStream* dataOut )
                                           throw ( decaf::io::IOException );

        /**
         * Called to renegotiate the settings for the WireFormatInfo, these
         * determine how the client and broker communicate.
         * @param info - The new Wireformat Info settings
         * @throws IllegalStateException is the params can't be negotiated.
         */
        void renegotiateWireFormat( commands::WireFormatInfo* info )
            throw ( decaf::lang::exceptions::IllegalStateException );

        /**
         * Configures this object using the provieded WireformatInfo object
         * @param info - a WireFormatInfo object, takes ownership.
         */
        virtual void setPreferedWireFormatInfo( commands::WireFormatInfo* info )
            throw ( decaf::lang::exceptions::IllegalStateException );

        /**
         * Gets the Preferend WireFormatInfo object that this class holds
         * @return pointer to a prefered WireFormatInfo object
         */
        virtual commands::WireFormatInfo* getPreferedWireFormatInfo() const {
            return this->preferedWireFormatInfo;
        }

        /**
         * Checks if the stackTraceEnabled flag is on
         * @return true if the flag is on.
         */
        bool isStackTraceEnabled() const {
            return stackTraceEnabled;
        }

        /**
         * Sets if the stackTraceEnabled flag is on
         * @param stackTraceEnabled - ture to turn flag is on
         */
        void setStackTraceEnabled( bool stackTraceEnabled ) {
            this->stackTraceEnabled = stackTraceEnabled;
        }

        /**
         * Checks if the tcpNoDelayEnabled flag is on
         * @return true if the flag is on.
         */
        bool isTcpNoDelayEnabled() const {
            return tcpNoDelayEnabled;
        }

        /**
         * Sets if the tcpNoDelayEnabled flag is on
         * @param tcpNoDelayEnabled - ture to turn flag is on
         */
        void setTcpNoDelayEnabled( bool tcpNoDelayEnabled ) {
            this->tcpNoDelayEnabled = tcpNoDelayEnabled;
        }

        /**
         * Get the current Wireformat Version
         * @return int that identifies the version
         */
        int getVersion() const {
            return version;
        }

        /**
         * Set the current Wireformat Version
         * @param version - int that identifies the version
         */
        void setVersion( int version ) throw ( decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Checks if the cacheEnabled flag is on
         * @return true if the flag is on.
         */
        bool isCacheEnabled() const {
            return cacheEnabled;
        }

        /**
         * Sets if the cacheEnabled flag is on
         * @param cacheEnabled - ture to turn flag is on
         */
        void setCacheEnabled( bool cacheEnabled ) {
            this->cacheEnabled = cacheEnabled;
        }

        /**
         * Returns the currently set Cache size.
         * @return the current value of the broker's cache size.
         */
        int getCacheSize() const {
            return cacheSize;
        }

        /**
         * Sets the current Cache size.
         * @param value - the value to send as the broker's cache size.
         */
        void setCacheSize( int value ) {
            this->cacheSize = value;
        }

        /**
         * Checks if the tightEncodingEnabled flag is on
         * @return true if the flag is on.
         */
        bool isTightEncodingEnabled() const {
            return tightEncodingEnabled;
        }

        /**
         * Sets if the tightEncodingEnabled flag is on
         * @param tightEncodingEnabled - ture to turn flag is on
         */
        void setTightEncodingEnabled( bool tightEncodingEnabled ) {
            this->tightEncodingEnabled = tightEncodingEnabled;
        }

        /**
         * Checks if the sizePrefixDisabled flag is on
         * @return true if the flag is on.
         */
        bool isSizePrefixDisabled() const {
            return sizePrefixDisabled;
        }

        /**
         * Sets if the sizePrefixDisabled flag is on
         * @param sizePrefixDisabled - ture to turn flag is on
         */
        void setSizePrefixDisabled( bool sizePrefixDisabled ) {
            this->sizePrefixDisabled = sizePrefixDisabled;
        }

        /**
         * Gets the MaxInactivityDuration setting.
         * @return maximum inactivity duration value in milliseconds.
         */
        long long getMaxInactivityDuration() const {
            return this->maxInactivityDuration;
        }

        /**
         * Sets the MaxInactivityDuration setting.
         * @param value - the Max inactivity duration value in milliseconds.
         */
        void setMaxInactivityDuration( long long value ) {
            this->maxInactivityDuration = value;
        }

        /**
         * Gets the MaxInactivityDurationInitialDelay setting.
         * @return maximum inactivity duration initial delay value in milliseconds.
         */
        long long getMaxInactivityDurationInitialDelay() const {
            return this->maxInactivityDurationInitialDelay;
        }

        /**
         * Sets the MaxInactivityDurationInitialDelay setting.
         * @param value - the Max inactivity Initial Delay duration value in milliseconds.
         */
        void setMaxInactivityDurationInitialDelay( long long value ) {
            this->maxInactivityDurationInitialDelay = value;
        }

    protected:

        /**
         * Perform the actual unmarshal of data from the given DataInputStream
         * return the unmarshalled DataStrucutre object once done, caller takes
         * ownership of this object.  This method can return null if the type
         * of the object to unmarshal is NULL, empty data.
         * @param dis - DataInputStream to read from
         * @returns new DataStructure* that the caller owns
         * @throws IOException if an error occurs during the unmarshal
         */
        commands::DataStructure* doUnmarshal( decaf::io::DataInputStream* dis )
            throw ( decaf::io::IOException );

        /**
         * Cleans up all registered Marshallers and empties the dataMarshallers
         * vector.  This should be called before a reconfiguration of the version
         * marshallers, or on destruction of this object
         */
        void destroyMarshalers();

    protected:

        // Declared here to make life easier.
        static const unsigned char NULL_TYPE;

        // V1 if the default version we start at.
        static const int DEFAULT_VERSION = 1;

    private:

        // This object config data
        decaf::util::Properties properties;

        // Prefered WireFormatInfo
        commands::WireFormatInfo* preferedWireFormatInfo;

        // Marshallers
        std::vector< DataStreamMarshaller* > dataMarshallers;

        // Uniquely Generated ID, initialize in the Ctor
        std::string id;

        // WireFormat Data
        int version;
        bool stackTraceEnabled;
        bool tcpNoDelayEnabled;
        bool cacheEnabled;
        int cacheSize;
        bool tightEncodingEnabled;
        bool sizePrefixDisabled;
        long long maxInactivityDuration;
        long long maxInactivityDurationInitialDelay;

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREFORMAT_H_*/
