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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_WIREFORMATINFO_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_WIREFORMATINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/openwire/commands/BaseCommand.h>
#include <activemq/connector/openwire/marshal/PrimitiveMapMarshaller.h>
#include <activemq/transport/Command.h>
#include <activemq/util/PrimitiveMap.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <vector>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class AMQCPP_API WireFormatInfo : public BaseCommand<transport::Command> {
    public:

        const static unsigned char ID_WIREFORMATINFO = 1;

    public:

        WireFormatInfo();

        virtual ~WireFormatInfo();

        /**
         * Get the unique identifier that this object and its own
         * Marshaller share.
         * @returns new DataStructure type copy.
         */
        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this obbject and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual DataStructure* cloneDataStructure() const;

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const;
        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const;

        /**
         * Indicates that this command is aware of Marshalling, and needs
         * to have its Marshalling methods invoked.
         * @returns boolean indicating desire to be in marshalling stages
         */
        virtual bool isMarshalAware() const {
            return true;
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
        void setVersion( int version ) {
            this->version = version;
        }

        /**
         * Returns the currently configured Max Inactivity duration.
         * @return the set inactivity duration value.
         */
        long long getMaxInactivityDuration() const;

        /**
         * Sets the Max inactivity duration value.
         * @param maxInactivityDuration - max time a client can be inactive.
         */
        void setMaxInactivityDuration( long long maxInactivityDuration );

        /**
         * Returns the currently configured Max Inactivity Intial Delay duration.
         * @return the set inactivity duration initial delay value.
         */
        long long getMaxInactivityDurationInitalDelay() const;

        /**
         * Sets the Max inactivity initial delay duration value.
         * @param maxInactivityDurationInitalDelay - time before the inactivity delay is checked.
         */
        void setMaxInactivityDurationInitalDelay( long long maxInactivityDurationInitalDelay );

        /**
         * Checks if the stackTraceEnabled flag is on
         * @return true if the flag is on.
         */
        bool isStackTraceEnabled() const;

        /**
         * Sets if the stackTraceEnabled flag is on
         * @param stackTraceEnabled - ture to turn flag is on
         */
        void setStackTraceEnabled( bool stackTraceEnabled );

        /**
         * Checks if the tcpNoDelayEnabled flag is on
         * @return true if the flag is on.
         */
        bool isTcpNoDelayEnabled() const;

        /**
         * Sets if the tcpNoDelayEnabled flag is on
         * @param tcpNoDelayEnabled - ture to turn flag is on
         */
        void setTcpNoDelayEnabled( bool tcpNoDelayEnabled );

        /**
         * Checks if the cacheEnabled flag is on
         * @return true if the flag is on.
         */
        bool isCacheEnabled() const;

        /**
         * Sets if the cacheEnabled flag is on
         * @param cacheEnabled - ture to turn flag is on
         */
        void setCacheEnabled( bool cacheEnabled );

        /**
         * Gets the Cache Size setting.
         * @return currently set cache size.
         */
        int getCacheSize() const;

        /**
         * Sets the Cache Size setting.
         * @param value - value to set to the cache size.
         */
        void setCacheSize( int value );

        /**
         * Checks if the tightEncodingEnabled flag is on
         * @return true if the flag is on.
         */
        bool isTightEncodingEnabled() const;

        /**
         * Sets if the tightEncodingEnabled flag is on
         * @param tightEncodingEnabled - ture to turn flag is on
         */
        void setTightEncodingEnabled( bool tightEncodingEnabled );

        /**
         * Checks if the sizePrefixDisabled flag is on
         * @return true if the flag is on.
         */
        bool isSizePrefixDisabled() const;

        /**
         * Sets if the sizePrefixDisabled flag is on
         * @param sizePrefixDisabled - ture to turn flag is on
         */
        void setSizePrefixDisabled( bool sizePrefixDisabled );

        /**
         * Get the Magic field
         * @return const reference to a std::vector<char>
         */
        const std::vector<unsigned char>& getMagic() const {
            return magic;
        }

        /**
         * Sets the value of the magic field
         * @param magic - const std::vector<char>
         */
        void setMagic( const std::vector<unsigned char>& magic ) {
            this->magic = magic;
        }

        /**
         * Get the marshalledProperties field
         * @return const reference to a std::vector<char>
         */
        const std::vector<unsigned char>& getMarshalledProperties() const {
            return marshalledProperties;
        }

        /**
         * Sets the value of the marshalledProperties field
         * @param magic - const std::vector<char>
         */
        void setMarshalledProperties( const std::vector<unsigned char>& marshalledProperties ) {
            this->marshalledProperties = marshalledProperties;
        }

        /**
         * Gets the Properties for this Command
         * @param reference to a PrimitiveMap
         * @throws cms::CMSException
         */
        virtual const util::PrimitiveMap& getProperties() const {
            return properties;
        }
        virtual util::PrimitiveMap& getProperties() {
            return properties;
        }

        /**
         * Sets the Properties for this Command
         * @param map - PrimtiveMap to copy
         */
        virtual void setProperties( const util::PrimitiveMap& map ) {
            this->properties.copy( map );
        }

        /**
         * Determines if we think this is a Valid WireFormatInfo command
         * @returns true if its valid.
         */
        bool isValid() const;

    public:

        /**
         * Handles the marshalling of the objects properties into the
         * internal byte array before the object is marshalled to the
         * wire
         * @param wireFormat - the wireformatting controller
         */
        virtual void beforeMarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException ) {

            try{

                marshalledProperties.clear();
                if( !properties.isEmpty() )
                {
                    marshal::PrimitiveMapMarshaller::marshal(
                        &properties, marshalledProperties );
                }
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

        /**
         * Called after unmarshaling is started to cleanup the object being
         * unmarshaled.
         * @param wireFormat - the wireformat object to control unmarshaling
         */
        virtual void afterUnmarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException ) {

            try{

                marshal::PrimitiveMapMarshaller::unmarshal(
                    &properties, marshalledProperties );
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

    private:

        std::vector<unsigned char> magic;
        std::vector<unsigned char> marshalledProperties;

        /**
         * WireFormatInfo Properties, unmarshalled from the marshalled
         * properties on use.
         */
        util::PrimitiveMap properties;

        /**
         * OpenWire Protocal Version
         */
        int version;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_WIREFORMATINFO_H_*/
