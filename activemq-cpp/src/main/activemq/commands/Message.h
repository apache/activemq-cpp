/*
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

#ifndef _ACTIVEMQ_COMMANDS_MESSAGE_H_
#define _ACTIVEMQ_COMMANDS_MESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/BrokerId.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/DataStructure.h>
#include <activemq/commands/MessageId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/util/Config.h>
#include <activemq/util/PrimitiveMap.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace core{
    class ActiveMQAckHandler;
}
namespace commands{

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for Message
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API Message : public BaseCommand {
    private:

        // Used to allow a client to call Message::acknowledge when in the Client
        // Ack mode.
        Pointer<core::ActiveMQAckHandler> ackHandler;

        // Message properties, these are Marshaled and Unmarshaled from the Message
        // Command's marshaledProperties vector.
        activemq::util::PrimitiveMap properties;

        // Indicates if the Message Properties are Read Only
        bool readOnlyProperties;

        // Indicates if the Message Body are Read Only
        bool readOnlyBody;

    protected:

        static const unsigned int DEFAULT_MESSAGE_SIZE = 1024;

    protected:

        Pointer<ProducerId> producerId;
        Pointer<ActiveMQDestination> destination;
        Pointer<TransactionId> transactionId;
        Pointer<ActiveMQDestination> originalDestination;
        Pointer<MessageId> messageId;
        Pointer<TransactionId> originalTransactionId;
        std::string groupID;
        int groupSequence;
        std::string correlationId;
        bool persistent;
        long long expiration;
        unsigned char priority;
        Pointer<ActiveMQDestination> replyTo;
        long long timestamp;
        std::string type;
        std::vector<unsigned char> content;
        std::vector<unsigned char> marshalledProperties;
        Pointer<DataStructure> dataStructure;
        Pointer<ConsumerId> targetConsumerId;
        bool compressed;
        int redeliveryCounter;
        std::vector< decaf::lang::Pointer<BrokerId> > brokerPath;
        long long arrival;
        std::string userID;
        bool recievedByDFBridge;
        bool droppable;
        std::vector< decaf::lang::Pointer<BrokerId> > cluster;
        long long brokerInTime;
        long long brokerOutTime;

    public:

        const static unsigned char ID_MESSAGE = 0;

    protected:

        Message( const Message& ) : BaseCommand() {};
        Message& operator= ( const Message& ) { return *this; };

    public:

        Message();

        virtual ~Message();

        /**
         * Get the unique identifier that this object and its own
         * Marshaler share.
         * @returns new DataStructure type copy.
         */
        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual Message* cloneDataStructure() const;

        /**
         * Copy the contents of the passed object into this object's
         * members, overwriting any existing data.
         * @param src - Source Object
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
         * Handles the marshaling of the objects properties into the
         * internal byte array before the object is marshaled to the
         * wire
         * @param wireFormat - the wireformat controller
         */
        virtual void beforeMarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException );

        /**
         * Called after unmarshaling is started to cleanup the object being
         * unmarshaled.
         * @param wireFormat - the wireformat object to control unmarshaling
         */
        virtual void afterUnmarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException );

        /**
         * Indicates that this command is aware of Marshaling, and needs
         * to have its Marshaling methods invoked.
         * @returns boolean indicating desire to be in marshaling stages
         */
        virtual bool isMarshalAware() const {
            return true;
        }

        /**
         * Sets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @param handler ActiveMQAckHandler to call
         */
        virtual void setAckHandler( const Pointer<core::ActiveMQAckHandler>& handler ) {
            this->ackHandler = handler;
        }

        /**
         * Gets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @returns handler ActiveMQAckHandler to call or NULL if not set
         */
        virtual Pointer<core::ActiveMQAckHandler> getAckHandler() const {
            return this->ackHandler;
        }

        /**
         * Returns the Size of this message in Bytes.
         * @returns number of bytes this message equates to.
         */
        virtual unsigned int getSize() const;

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const;

        /**
         * Allows derived Message classes to perform tasks before a message is sent.
         */
        virtual void onSend() {}

        /**
         * Gets a reference to the Message's Properties object, allows the derived
         * classes to get and set their own specific properties.
         *
         * @return a reference to the Primitive Map that holds message properties.
         */
        util::PrimitiveMap& getMessageProperties() {
            return this->properties;
        }
        const util::PrimitiveMap& getMessageProperties() const {
            return this->properties;
        }

        /**
         * Returns if the Message Properties Are Read Only
         * @return true if Message Properties are Read Only.
         */
        bool isReadOnlyProperties() const {
            return this->readOnlyProperties;
        }

        /**
         * Set the Read Only State of the Message Properties.
         * @param value - true if Properties should be read only.
         */
        void setReadOnlyProperties( bool value ) {
            this->readOnlyProperties = value;
        }

        /**
         * Returns if the Message Body is Read Only
         * @return true if Message Content is Read Only.
         */
        bool isReadOnlyBody() const {
            return this->readOnlyBody;
        }

        /**
         * Set the Read Only State of the Message Content.
         * @param value - true if Content should be read only.
         */
        void setReadOnlyBody( bool value ) {
            this->readOnlyBody = value;
        }

        virtual const Pointer<ProducerId>& getProducerId() const;
        virtual Pointer<ProducerId>& getProducerId();
        virtual void setProducerId( const Pointer<ProducerId>& producerId );

        virtual const Pointer<ActiveMQDestination>& getDestination() const;
        virtual Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination( const Pointer<ActiveMQDestination>& destination );

        virtual const Pointer<TransactionId>& getTransactionId() const;
        virtual Pointer<TransactionId>& getTransactionId();
        virtual void setTransactionId( const Pointer<TransactionId>& transactionId );

        virtual const Pointer<ActiveMQDestination>& getOriginalDestination() const;
        virtual Pointer<ActiveMQDestination>& getOriginalDestination();
        virtual void setOriginalDestination( const Pointer<ActiveMQDestination>& originalDestination );

        virtual const Pointer<MessageId>& getMessageId() const;
        virtual Pointer<MessageId>& getMessageId();
        virtual void setMessageId( const Pointer<MessageId>& messageId );

        virtual const Pointer<TransactionId>& getOriginalTransactionId() const;
        virtual Pointer<TransactionId>& getOriginalTransactionId();
        virtual void setOriginalTransactionId( const Pointer<TransactionId>& originalTransactionId );

        virtual const std::string& getGroupID() const;
        virtual std::string& getGroupID();
        virtual void setGroupID( const std::string& groupID );

        virtual int getGroupSequence() const;
        virtual void setGroupSequence( int groupSequence );

        virtual const std::string& getCorrelationId() const;
        virtual std::string& getCorrelationId();
        virtual void setCorrelationId( const std::string& correlationId );

        virtual bool isPersistent() const;
        virtual void setPersistent( bool persistent );

        virtual long long getExpiration() const;
        virtual void setExpiration( long long expiration );

        virtual unsigned char getPriority() const;
        virtual void setPriority( unsigned char priority );

        virtual const Pointer<ActiveMQDestination>& getReplyTo() const;
        virtual Pointer<ActiveMQDestination>& getReplyTo();
        virtual void setReplyTo( const Pointer<ActiveMQDestination>& replyTo );

        virtual long long getTimestamp() const;
        virtual void setTimestamp( long long timestamp );

        virtual const std::string& getType() const;
        virtual std::string& getType();
        virtual void setType( const std::string& type );

        virtual const std::vector<unsigned char>& getContent() const;
        virtual std::vector<unsigned char>& getContent();
        virtual void setContent( const std::vector<unsigned char>& content );

        virtual const std::vector<unsigned char>& getMarshalledProperties() const;
        virtual std::vector<unsigned char>& getMarshalledProperties();
        virtual void setMarshalledProperties( const std::vector<unsigned char>& marshalledProperties );

        virtual const Pointer<DataStructure>& getDataStructure() const;
        virtual Pointer<DataStructure>& getDataStructure();
        virtual void setDataStructure( const Pointer<DataStructure>& dataStructure );

        virtual const Pointer<ConsumerId>& getTargetConsumerId() const;
        virtual Pointer<ConsumerId>& getTargetConsumerId();
        virtual void setTargetConsumerId( const Pointer<ConsumerId>& targetConsumerId );

        virtual bool isCompressed() const;
        virtual void setCompressed( bool compressed );

        virtual int getRedeliveryCounter() const;
        virtual void setRedeliveryCounter( int redeliveryCounter );

        virtual const std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath() const;
        virtual std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath();
        virtual void setBrokerPath( const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath );

        virtual long long getArrival() const;
        virtual void setArrival( long long arrival );

        virtual const std::string& getUserID() const;
        virtual std::string& getUserID();
        virtual void setUserID( const std::string& userID );

        virtual bool isRecievedByDFBridge() const;
        virtual void setRecievedByDFBridge( bool recievedByDFBridge );

        virtual bool isDroppable() const;
        virtual void setDroppable( bool droppable );

        virtual const std::vector< decaf::lang::Pointer<BrokerId> >& getCluster() const;
        virtual std::vector< decaf::lang::Pointer<BrokerId> >& getCluster();
        virtual void setCluster( const std::vector< decaf::lang::Pointer<BrokerId> >& cluster );

        virtual long long getBrokerInTime() const;
        virtual void setBrokerInTime( long long brokerInTime );

        virtual long long getBrokerOutTime() const;
        virtual void setBrokerOutTime( long long brokerOutTime );

        /**
         * @return an answer of true to the isMessage() query.
         */
        virtual bool isMessage() const {
            return true;
        }

        /**
         * Allows a Visitor to visit this command and return a response to the
         * command based on the command type being visited.  The command will call
         * the proper processXXX method in the visitor.
         * 
         * @return a Response to the visitor being called or NULL if no response.
         */
        virtual Pointer<Command> visit( activemq::state::CommandVisitor* visitor )
            throw( exceptions::ActiveMQException );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_MESSAGE_H_*/
