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

#include <activemq/commands/Message.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/wireformat/openwire/marshal/PrimitiveTypesMarshaller.h>
#include <decaf/lang/System.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command code for OpenWire format for Message
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
Message::Message() :
    BaseCommand(), producerId(NULL), destination(NULL), transactionId(NULL), originalDestination(NULL), messageId(NULL), originalTransactionId(NULL), 
      groupID(""), groupSequence(0), correlationId(""), persistent(false), expiration(0), priority(0), replyTo(NULL), timestamp(0), 
      type(""), content(), marshalledProperties(), dataStructure(NULL), targetConsumerId(NULL), compressed(false), redeliveryCounter(0), 
      brokerPath(), arrival(0), userID(""), recievedByDFBridge(false), droppable(false), cluster(), brokerInTime(0), brokerOutTime(0), ackHandler(NULL), properties(), readOnlyProperties(false), readOnlyBody(false), connection(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
Message::~Message() {
}

////////////////////////////////////////////////////////////////////////////////
Message* Message::cloneDataStructure() const {
    std::auto_ptr<Message> message(new Message());

    // Copy the data from the base class or classes
    message->copyDataStructure(this);

    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void Message::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const Message* srcPtr = dynamic_cast<const Message*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "Message::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setProducerId(srcPtr->getProducerId());
    this->setDestination(srcPtr->getDestination());
    this->setTransactionId(srcPtr->getTransactionId());
    this->setOriginalDestination(srcPtr->getOriginalDestination());
    this->setMessageId(srcPtr->getMessageId());
    this->setOriginalTransactionId(srcPtr->getOriginalTransactionId());
    this->setGroupID(srcPtr->getGroupID());
    this->setGroupSequence(srcPtr->getGroupSequence());
    this->setCorrelationId(srcPtr->getCorrelationId());
    this->setPersistent(srcPtr->isPersistent());
    this->setExpiration(srcPtr->getExpiration());
    this->setPriority(srcPtr->getPriority());
    this->setReplyTo(srcPtr->getReplyTo());
    this->setTimestamp(srcPtr->getTimestamp());
    this->setType(srcPtr->getType());
    this->setContent(srcPtr->getContent());
    this->setMarshalledProperties(srcPtr->getMarshalledProperties());
    this->setDataStructure(srcPtr->getDataStructure());
    this->setTargetConsumerId(srcPtr->getTargetConsumerId());
    this->setCompressed(srcPtr->isCompressed());
    this->setRedeliveryCounter(srcPtr->getRedeliveryCounter());
    this->setBrokerPath(srcPtr->getBrokerPath());
    this->setArrival(srcPtr->getArrival());
    this->setUserID(srcPtr->getUserID());
    this->setRecievedByDFBridge(srcPtr->isRecievedByDFBridge());
    this->setDroppable(srcPtr->isDroppable());
    this->setCluster(srcPtr->getCluster());
    this->setBrokerInTime(srcPtr->getBrokerInTime());
    this->setBrokerOutTime(srcPtr->getBrokerOutTime());
    this->properties.copy(srcPtr->properties);
    this->setAckHandler(srcPtr->getAckHandler());
    this->setReadOnlyBody(srcPtr->isReadOnlyBody());
    this->setReadOnlyProperties(srcPtr->isReadOnlyProperties());
    this->setConnection(srcPtr->getConnection());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char Message::getDataStructureType() const {
    return Message::ID_MESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
std::string Message::toString() const {

    ostringstream stream;

    stream << "Message { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "ProducerId = ";
    if (this->getProducerId() != NULL) {
        stream << this->getProducerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "TransactionId = ";
    if (this->getTransactionId() != NULL) {
        stream << this->getTransactionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "OriginalDestination = ";
    if (this->getOriginalDestination() != NULL) {
        stream << this->getOriginalDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "MessageId = ";
    if (this->getMessageId() != NULL) {
        stream << this->getMessageId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "OriginalTransactionId = ";
    if (this->getOriginalTransactionId() != NULL) {
        stream << this->getOriginalTransactionId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "GroupID = " << this->getGroupID();
    stream << ", ";
    stream << "GroupSequence = " << this->getGroupSequence();
    stream << ", ";
    stream << "CorrelationId = " << this->getCorrelationId();
    stream << ", ";
    stream << "Persistent = " << this->isPersistent();
    stream << ", ";
    stream << "Expiration = " << this->getExpiration();
    stream << ", ";
    stream << "Priority = " << (int)this->getPriority();
    stream << ", ";
    stream << "ReplyTo = ";
    if (this->getReplyTo() != NULL) {
        stream << this->getReplyTo()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Timestamp = " << this->getTimestamp();
    stream << ", ";
    stream << "Type = " << this->getType();
    stream << ", ";
    stream << "Content = ";
    if (this->getContent().size() > 0) {
        stream << "[size=" << this->getContent().size() << "]";
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "MarshalledProperties = ";
    if (this->getMarshalledProperties().size() > 0) {
        stream << "[size=" << this->getMarshalledProperties().size() << "]";
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "DataStructure = ";
    if (this->getDataStructure() != NULL) {
        stream << this->getDataStructure()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "TargetConsumerId = ";
    if (this->getTargetConsumerId() != NULL) {
        stream << this->getTargetConsumerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Compressed = " << this->isCompressed();
    stream << ", ";
    stream << "RedeliveryCounter = " << this->getRedeliveryCounter();
    stream << ", ";
    stream << "BrokerPath = ";
    if (this->getBrokerPath().size() > 0) {
        stream << "[";
        for (size_t ibrokerPath = 0; ibrokerPath < this->getBrokerPath().size(); ++ibrokerPath) {
            if (this->getBrokerPath()[ibrokerPath] != NULL) {
                stream << this->getBrokerPath()[ibrokerPath]->toString() << ", ";
            } else {
                stream << "NULL" << ", ";
            }
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Arrival = " << this->getArrival();
    stream << ", ";
    stream << "UserID = " << this->getUserID();
    stream << ", ";
    stream << "RecievedByDFBridge = " << this->isRecievedByDFBridge();
    stream << ", ";
    stream << "Droppable = " << this->isDroppable();
    stream << ", ";
    stream << "Cluster = ";
    if (this->getCluster().size() > 0) {
        stream << "[";
        for (size_t icluster = 0; icluster < this->getCluster().size(); ++icluster) {
            if (this->getCluster()[icluster] != NULL) {
                stream << this->getCluster()[icluster]->toString() << ", ";
            } else {
                stream << "NULL" << ", ";
            }
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "BrokerInTime = " << this->getBrokerInTime();
    stream << ", ";
    stream << "BrokerOutTime = " << this->getBrokerOutTime();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool Message::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const Message* valuePtr = dynamic_cast<const Message*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getProducerId() != NULL) {
        if (!this->getProducerId()->equals( valuePtr->getProducerId().get())) {
            return false;
        }
    } else if (valuePtr->getProducerId() != NULL) {
        return false;
    }
    if (this->getDestination() != NULL) {
        if (!this->getDestination()->equals( valuePtr->getDestination().get())) {
            return false;
        }
    } else if (valuePtr->getDestination() != NULL) {
        return false;
    }
    if (this->getTransactionId() != NULL) {
        if (!this->getTransactionId()->equals( valuePtr->getTransactionId().get())) {
            return false;
        }
    } else if (valuePtr->getTransactionId() != NULL) {
        return false;
    }
    if (this->getOriginalDestination() != NULL) {
        if (!this->getOriginalDestination()->equals( valuePtr->getOriginalDestination().get())) {
            return false;
        }
    } else if (valuePtr->getOriginalDestination() != NULL) {
        return false;
    }
    if (this->getMessageId() != NULL) {
        if (!this->getMessageId()->equals( valuePtr->getMessageId().get())) {
            return false;
        }
    } else if (valuePtr->getMessageId() != NULL) {
        return false;
    }
    if (this->getOriginalTransactionId() != NULL) {
        if (!this->getOriginalTransactionId()->equals( valuePtr->getOriginalTransactionId().get())) {
            return false;
        }
    } else if (valuePtr->getOriginalTransactionId() != NULL) {
        return false;
    }
    if (this->getGroupID() != valuePtr->getGroupID()) {
        return false;
    }
    if (this->getGroupSequence() != valuePtr->getGroupSequence()) {
        return false;
    }
    if (this->getCorrelationId() != valuePtr->getCorrelationId()) {
        return false;
    }
    if (this->isPersistent() != valuePtr->isPersistent()) {
        return false;
    }
    if (this->getExpiration() != valuePtr->getExpiration()) {
        return false;
    }
    if (this->getPriority() != valuePtr->getPriority()) {
        return false;
    }
    if (this->getReplyTo() != NULL) {
        if (!this->getReplyTo()->equals( valuePtr->getReplyTo().get())) {
            return false;
        }
    } else if (valuePtr->getReplyTo() != NULL) {
        return false;
    }
    if (this->getTimestamp() != valuePtr->getTimestamp()) {
        return false;
    }
    if (this->getType() != valuePtr->getType()) {
        return false;
    }
    for (size_t icontent = 0; icontent < this->getContent().size(); ++icontent) {
        if (this->getContent()[icontent] != valuePtr->getContent()[icontent]) {
            return false;
        }
    }
    for (size_t imarshalledProperties = 0; imarshalledProperties < this->getMarshalledProperties().size(); ++imarshalledProperties) {
        if (this->getMarshalledProperties()[imarshalledProperties] != valuePtr->getMarshalledProperties()[imarshalledProperties]) {
            return false;
        }
    }
    if (this->getDataStructure() != NULL) {
        if (!this->getDataStructure()->equals( valuePtr->getDataStructure().get())) {
            return false;
        }
    } else if (valuePtr->getDataStructure() != NULL) {
        return false;
    }
    if (this->getTargetConsumerId() != NULL) {
        if (!this->getTargetConsumerId()->equals( valuePtr->getTargetConsumerId().get())) {
            return false;
        }
    } else if (valuePtr->getTargetConsumerId() != NULL) {
        return false;
    }
    if (this->isCompressed() != valuePtr->isCompressed()) {
        return false;
    }
    if (this->getRedeliveryCounter() != valuePtr->getRedeliveryCounter()) {
        return false;
    }
    for (size_t ibrokerPath = 0; ibrokerPath < this->getBrokerPath().size(); ++ibrokerPath) {
        if (this->getBrokerPath()[ibrokerPath] != NULL ) {
            if (!this->getBrokerPath()[ibrokerPath]->equals( valuePtr->getBrokerPath()[ibrokerPath].get())) {
                return false;
            }
        } else if (valuePtr->getBrokerPath()[ibrokerPath] != NULL) {
            return false;
        }
    }
    if (this->getArrival() != valuePtr->getArrival()) {
        return false;
    }
    if (this->getUserID() != valuePtr->getUserID()) {
        return false;
    }
    if (this->isRecievedByDFBridge() != valuePtr->isRecievedByDFBridge()) {
        return false;
    }
    if (this->isDroppable() != valuePtr->isDroppable()) {
        return false;
    }
    for (size_t icluster = 0; icluster < this->getCluster().size(); ++icluster) {
        if (this->getCluster()[icluster] != NULL ) {
            if (!this->getCluster()[icluster]->equals( valuePtr->getCluster()[icluster].get())) {
                return false;
            }
        } else if (valuePtr->getCluster()[icluster] != NULL) {
            return false;
        }
    }
    if (this->getBrokerInTime() != valuePtr->getBrokerInTime()) {
        return false;
    }
    if (this->getBrokerOutTime() != valuePtr->getBrokerOutTime()) {
        return false;
    }
    if (ackHandler != valuePtr->getAckHandler()){
        return false;
    }

    if (!properties.equals(valuePtr->properties)) {
        return false;
    }

    if (readOnlyBody != valuePtr->isReadOnlyBody()){
        return false;
    }

    if (readOnlyProperties != valuePtr->isReadOnlyProperties()){
        return false;
    }

    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ProducerId>& Message::getProducerId() const {
    return producerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ProducerId>& Message::getProducerId() {
    return producerId;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setProducerId(const decaf::lang::Pointer<ProducerId>& producerId) {
    this->producerId = producerId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& Message::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& Message::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<TransactionId>& Message::getTransactionId() const {
    return transactionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<TransactionId>& Message::getTransactionId() {
    return transactionId;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setTransactionId(const decaf::lang::Pointer<TransactionId>& transactionId) {
    this->transactionId = transactionId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& Message::getOriginalDestination() const {
    return originalDestination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& Message::getOriginalDestination() {
    return originalDestination;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setOriginalDestination(const decaf::lang::Pointer<ActiveMQDestination>& originalDestination) {
    this->originalDestination = originalDestination;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<MessageId>& Message::getMessageId() const {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageId>& Message::getMessageId() {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setMessageId(const decaf::lang::Pointer<MessageId>& messageId) {
    this->messageId = messageId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<TransactionId>& Message::getOriginalTransactionId() const {
    return originalTransactionId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<TransactionId>& Message::getOriginalTransactionId() {
    return originalTransactionId;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setOriginalTransactionId(const decaf::lang::Pointer<TransactionId>& originalTransactionId) {
    this->originalTransactionId = originalTransactionId;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& Message::getGroupID() const {
    return groupID;
}

////////////////////////////////////////////////////////////////////////////////
std::string& Message::getGroupID() {
    return groupID;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setGroupID(const std::string& groupID) {
    this->groupID = groupID;
}

////////////////////////////////////////////////////////////////////////////////
int Message::getGroupSequence() const {
    return groupSequence;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setGroupSequence(int groupSequence) {
    this->groupSequence = groupSequence;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& Message::getCorrelationId() const {
    return correlationId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& Message::getCorrelationId() {
    return correlationId;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setCorrelationId(const std::string& correlationId) {
    this->correlationId = correlationId;
}

////////////////////////////////////////////////////////////////////////////////
bool Message::isPersistent() const {
    return persistent;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setPersistent(bool persistent) {
    this->persistent = persistent;
}

////////////////////////////////////////////////////////////////////////////////
long long Message::getExpiration() const {
    return expiration;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setExpiration(long long expiration) {
    this->expiration = expiration;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char Message::getPriority() const {
    return priority;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setPriority(unsigned char priority) {
    this->priority = priority;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& Message::getReplyTo() const {
    return replyTo;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& Message::getReplyTo() {
    return replyTo;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setReplyTo(const decaf::lang::Pointer<ActiveMQDestination>& replyTo) {
    this->replyTo = replyTo;
}

////////////////////////////////////////////////////////////////////////////////
long long Message::getTimestamp() const {
    return timestamp;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setTimestamp(long long timestamp) {
    this->timestamp = timestamp;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& Message::getType() const {
    return type;
}

////////////////////////////////////////////////////////////////////////////////
std::string& Message::getType() {
    return type;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setType(const std::string& type) {
    this->type = type;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<unsigned char>& Message::getContent() const {
    return content;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char>& Message::getContent() {
    return content;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setContent(const std::vector<unsigned char>& content) {
    this->content = content;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<unsigned char>& Message::getMarshalledProperties() const {
    return marshalledProperties;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char>& Message::getMarshalledProperties() {
    return marshalledProperties;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setMarshalledProperties(const std::vector<unsigned char>& marshalledProperties) {
    this->marshalledProperties = marshalledProperties;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<DataStructure>& Message::getDataStructure() const {
    return dataStructure;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<DataStructure>& Message::getDataStructure() {
    return dataStructure;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setDataStructure(const decaf::lang::Pointer<DataStructure>& dataStructure) {
    this->dataStructure = dataStructure;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConsumerId>& Message::getTargetConsumerId() const {
    return targetConsumerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConsumerId>& Message::getTargetConsumerId() {
    return targetConsumerId;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setTargetConsumerId(const decaf::lang::Pointer<ConsumerId>& targetConsumerId) {
    this->targetConsumerId = targetConsumerId;
}

////////////////////////////////////////////////////////////////////////////////
bool Message::isCompressed() const {
    return compressed;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setCompressed(bool compressed) {
    this->compressed = compressed;
}

////////////////////////////////////////////////////////////////////////////////
int Message::getRedeliveryCounter() const {
    return redeliveryCounter;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setRedeliveryCounter(int redeliveryCounter) {
    this->redeliveryCounter = redeliveryCounter;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector< decaf::lang::Pointer<BrokerId> >& Message::getBrokerPath() const {
    return brokerPath;
}

////////////////////////////////////////////////////////////////////////////////
std::vector< decaf::lang::Pointer<BrokerId> >& Message::getBrokerPath() {
    return brokerPath;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setBrokerPath(const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath) {
    this->brokerPath = brokerPath;
}

////////////////////////////////////////////////////////////////////////////////
long long Message::getArrival() const {
    return arrival;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setArrival(long long arrival) {
    this->arrival = arrival;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& Message::getUserID() const {
    return userID;
}

////////////////////////////////////////////////////////////////////////////////
std::string& Message::getUserID() {
    return userID;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setUserID(const std::string& userID) {
    this->userID = userID;
}

////////////////////////////////////////////////////////////////////////////////
bool Message::isRecievedByDFBridge() const {
    return recievedByDFBridge;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setRecievedByDFBridge(bool recievedByDFBridge) {
    this->recievedByDFBridge = recievedByDFBridge;
}

////////////////////////////////////////////////////////////////////////////////
bool Message::isDroppable() const {
    return droppable;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setDroppable(bool droppable) {
    this->droppable = droppable;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector< decaf::lang::Pointer<BrokerId> >& Message::getCluster() const {
    return cluster;
}

////////////////////////////////////////////////////////////////////////////////
std::vector< decaf::lang::Pointer<BrokerId> >& Message::getCluster() {
    return cluster;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setCluster(const std::vector< decaf::lang::Pointer<BrokerId> >& cluster) {
    this->cluster = cluster;
}

////////////////////////////////////////////////////////////////////////////////
long long Message::getBrokerInTime() const {
    return brokerInTime;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setBrokerInTime(long long brokerInTime) {
    this->brokerInTime = brokerInTime;
}

////////////////////////////////////////////////////////////////////////////////
long long Message::getBrokerOutTime() const {
    return brokerOutTime;
}

////////////////////////////////////////////////////////////////////////////////
void Message::setBrokerOutTime(long long brokerOutTime) {
    this->brokerOutTime = brokerOutTime;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> Message::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processMessage(this);
}

////////////////////////////////////////////////////////////////////////////////
bool Message::isExpired() const {
    long long expireTime = this->getExpiration();
    long long currentTime = decaf::lang::System::currentTimeMillis();
    if (expireTime > 0 && currentTime > expireTime) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int Message::getSize() const {

    unsigned int size = DEFAULT_MESSAGE_SIZE;

    size += (unsigned int)this->getContent().size();
    size += (unsigned int)this->getMarshalledProperties().size();

    return size;
}

////////////////////////////////////////////////////////////////////////////////
void Message::beforeMarshal(wireformat::WireFormat* wireFormat AMQCPP_UNUSED) {

    try {
        marshalledProperties.clear();
        if (!properties.isEmpty()) {
            wireformat::openwire::marshal::PrimitiveTypesMarshaller::marshal(
                &properties, marshalledProperties );
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
void Message::afterUnmarshal(wireformat::WireFormat* wireFormat AMQCPP_UNUSED) {

    try {
        wireformat::openwire::marshal::PrimitiveTypesMarshaller::unmarshal(
            &properties, marshalledProperties);
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

