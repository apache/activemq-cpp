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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_OPENWIREFORMAT_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_OPENWIREFORMAT_H_

#include <activemq/util/Config.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/DataStructure.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/wireformat/openwire/utils/BooleanStream.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <memory>

namespace activemq{
namespace wireformat{
namespace openwire{

namespace marshal {
    class DataStreamMarshaller;
}

    using decaf::lang::Pointer;

    class AMQCPP_API OpenWireFormat : public wireformat::WireFormat {
    protected:

        // Declared here to make life easier.
        static const unsigned char NULL_TYPE;

        // V1 if the default version we start at.
        static const int DEFAULT_VERSION;

        // Defines the maximum supported openwire version
        static const int MAX_SUPPORTED_VERSION;

    private:

        // Configuration parameters
        decaf::util::Properties properties;

        // Preferred WireFormatInfo
        Pointer<commands::WireFormatInfo> preferedWireFormatInfo;

        // Marshalers
        std::vector< marshal::DataStreamMarshaller* > dataMarshallers;

        // Uniquely Generated ID, initialize in the Ctor
        std::string id;

        // Indicates when we are in the doUnmarshal call
        decaf::util::concurrent::atomic::AtomicBoolean receiving;

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

    public:

        /**
         * Constructs a new OpenWireFormat object
         * @param properties - can contain optional config params.
         */
        OpenWireFormat( const decaf::util::Properties& properties );

        virtual ~OpenWireFormat();

        /**
         * {@inheritDoc}
         */
        virtual bool hasNegotiator() const {
            return true;
        }

        /**
         * {@inheritDoc}
         */
        virtual Pointer<transport::Transport> createNegotiator(
            const Pointer<transport::Transport>& transport );

        /**
         * Allows an external source to add marshalers to this object for
         * types that may be marshaled or unmarhsaled.
         * @param marshaler - the Marshaler to add to the collection.
         */
        void addMarshaller( marshal::DataStreamMarshaller* marshaler );

        /**
         * {@inheritDoc}
         */
        virtual void marshal( const Pointer<commands::Command>& command,
                              const activemq::transport::Transport* transport,
                              decaf::io::DataOutputStream* out );

        /**
         * {@inheritDoc}
         */
        virtual Pointer<commands::Command> unmarshal( const activemq::transport::Transport* transport,
                                                      decaf::io::DataInputStream* in );

    public:

        /**
         * Utility method for Tight Marshaling the given object to the boolean
         * stream passed.
         * @param object - The DataStructure to marshal
         * @param bs - the BooleanStream to write to
         * @returns size of the data returned.
         */
        virtual int tightMarshalNestedObject1( commands::DataStructure* object,
                                               utils::BooleanStream* bs );

        /**
         * Utility method that will Tight marshal some internally nested object
         * that implements the DataStructure interface.  Writes the data to the
         * Data Output Stream provided.
         * @param o - DataStructure object
         * @param ds - DataOuputStream for writing
         * @param bs - BooleanStream
         * @throws IOException if an error occurs.
         */
        void tightMarshalNestedObject2( commands::DataStructure* o,
                                        decaf::io::DataOutputStream* ds,
                                        utils::BooleanStream* bs );

        /**
         * Utility method used to Unmarshal a Nested DataStructure type object
         * from the given DataInputStream.  The DataStructure instance that is
         * returned is now the property of the caller.
         * @param dis - DataInputStream to read from
         * @param bs - BooleanStream to read from
         * @returns Newly allocated DataStructure Object
         * @throws IOException if an error occurs.
         */
        commands::DataStructure* tightUnmarshalNestedObject( decaf::io::DataInputStream* dis,
                                                             utils::BooleanStream* bs );

        /**
         * Utility method to unmarshal an DataStructure object from an
         * DataInputStream using the Loose Unmarshaling format.  Will read
         * the Data and construct a new DataStructure based Object, the
         * pointer to the Object returned is now owned by the caller.
         * @param dis - the DataInputStream to read the data from
         * @returns a new DataStructure derived Object pointer
         * @throws IOException if an error occurs.
         */
        commands::DataStructure* looseUnmarshalNestedObject( decaf::io::DataInputStream* dis );

        /**
         * Utility method to loosely Marshal an object that is derived from the
         * DataStrucutre interface.  The marshaled data is written to the
         * passed in DataOutputStream.
         * @param o - DataStructure derived Object to Marshal
         * @param dataOut - DataOutputStream to write the data to
         * @throw IOException if an error occurs.
         */
        void looseMarshalNestedObject( commands::DataStructure* o,
                                       decaf::io::DataOutputStream* dataOut );

        /**
         * Called to re-negotiate the settings for the WireFormatInfo, these
         * determine how the client and broker communicate.
         *
         * @param info
         *      The new Wireformat Info settings.
         *
         * @throws IllegalStateException is wire format can't be negotiated.
         */
        void renegotiateWireFormat( const commands::WireFormatInfo& info );

        /**
         * Configures this object using the provided WireformatInfo object
         *
         * @param info
         *      A WireFormatInfo object, takes ownership.
         *
         * @throws IllegalStateException if the WireFormat object has not been initialized.
         */
        void setPreferedWireFormatInfo( const Pointer<commands::WireFormatInfo>& info );

        /**
         * Gets the Preferred WireFormatInfo object that this class holds
         * @return pointer to a preferred WireFormatInfo object
         */
        const Pointer<commands::WireFormatInfo>& getPreferedWireFormatInfo() const {
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
         * @param stackTraceEnabled - true to turn flag is on
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
         * @param tcpNoDelayEnabled - true to turn flag is on
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
         *
         * @param version
         *      An int that identifies the version
         *
         * @throws IllegalArgumentException if the version given is not supported.
         */
        void setVersion( int version );

        /**
         * Is there a Message being unmarshaled?
         *
         * @return true while in the doUnmarshal method.
         */
        virtual bool inReceive() const {
            return this->receiving.get();
        }

        /**
         * Checks if the cacheEnabled flag is on
         * @return true if the flag is on.
         */
        bool isCacheEnabled() const {
            return cacheEnabled;
        }

        /**
         * Sets if the cacheEnabled flag is on
         * @param cacheEnabled - true to turn flag is on
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
         * @param tightEncodingEnabled - true to turn flag is on
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
         * @param sizePrefixDisabled - true to turn flag is on
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
         *
         * @param dis
         *      The DataInputStream to read from.
         *
         * @returns new DataStructure* that the caller owns.
         *
         * @throws IOException if an error occurs during the unmarshal.
         */
        commands::DataStructure* doUnmarshal( decaf::io::DataInputStream* dis );

        /**
         * Cleans up all registered Marshallers and empties the dataMarshallers
         * vector.  This should be called before a reconfiguration of the version
         * marshallers, or on destruction of this object
         */
        void destroyMarshalers();

    };

}}}

#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_OPENWIREFORMAT_H_*/
