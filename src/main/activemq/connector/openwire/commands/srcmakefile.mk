# ---------------------------------------------------------------------------
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ---------------------------------------------------------------------------

cc_sources += \
    activemq/connector/openwire/commands/ActiveMQBlobMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQBytesMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQDestination.cpp \
    activemq/connector/openwire/commands/ActiveMQMapMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQObjectMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQQueue.cpp \
    activemq/connector/openwire/commands/ActiveMQStreamMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQTempDestination.cpp \
    activemq/connector/openwire/commands/ActiveMQTempQueue.cpp \
    activemq/connector/openwire/commands/ActiveMQTempTopic.cpp \
    activemq/connector/openwire/commands/ActiveMQTextMessage.cpp \
    activemq/connector/openwire/commands/ActiveMQTopic.cpp \
    activemq/connector/openwire/commands/BrokerId.cpp \
    activemq/connector/openwire/commands/BrokerInfo.cpp \
    activemq/connector/openwire/commands/ConnectionControl.cpp \
    activemq/connector/openwire/commands/ConnectionError.cpp \
    activemq/connector/openwire/commands/ConnectionId.cpp \
    activemq/connector/openwire/commands/ConnectionInfo.cpp \
    activemq/connector/openwire/commands/ConsumerControl.cpp \
    activemq/connector/openwire/commands/ConsumerId.cpp \
    activemq/connector/openwire/commands/ConsumerInfo.cpp \
    activemq/connector/openwire/commands/ControlCommand.cpp \
    activemq/connector/openwire/commands/DataArrayResponse.cpp \
    activemq/connector/openwire/commands/DataResponse.cpp \
    activemq/connector/openwire/commands/DestinationInfo.cpp \
    activemq/connector/openwire/commands/DiscoveryEvent.cpp \
    activemq/connector/openwire/commands/ExceptionResponse.cpp \
    activemq/connector/openwire/commands/FlushCommand.cpp \
    activemq/connector/openwire/commands/IntegerResponse.cpp \
    activemq/connector/openwire/commands/JournalQueueAck.cpp \
    activemq/connector/openwire/commands/JournalTopicAck.cpp \
    activemq/connector/openwire/commands/JournalTrace.cpp \
    activemq/connector/openwire/commands/JournalTransaction.cpp \
    activemq/connector/openwire/commands/KeepAliveInfo.cpp \
    activemq/connector/openwire/commands/LastPartialCommand.cpp \
    activemq/connector/openwire/commands/LocalTransactionId.cpp \
    activemq/connector/openwire/commands/Message.cpp \
    activemq/connector/openwire/commands/MessageAck.cpp \
    activemq/connector/openwire/commands/MessageDispatch.cpp \
    activemq/connector/openwire/commands/MessageDispatchNotification.cpp \
    activemq/connector/openwire/commands/MessageId.cpp \
    activemq/connector/openwire/commands/MessagePull.cpp \
    activemq/connector/openwire/commands/NetworkBridgeFilter.cpp \
    activemq/connector/openwire/commands/PartialCommand.cpp \
    activemq/connector/openwire/commands/ProducerAck.cpp \
    activemq/connector/openwire/commands/ProducerId.cpp \
    activemq/connector/openwire/commands/ProducerInfo.cpp \
    activemq/connector/openwire/commands/RemoveInfo.cpp \
    activemq/connector/openwire/commands/RemoveSubscriptionInfo.cpp \
    activemq/connector/openwire/commands/ReplayCommand.cpp \
    activemq/connector/openwire/commands/Response.cpp \
    activemq/connector/openwire/commands/SessionId.cpp \
    activemq/connector/openwire/commands/SessionInfo.cpp \
    activemq/connector/openwire/commands/ShutdownInfo.cpp \
    activemq/connector/openwire/commands/SubscriptionInfo.cpp \
    activemq/connector/openwire/commands/TransactionId.cpp \
    activemq/connector/openwire/commands/TransactionInfo.cpp \
    activemq/connector/openwire/commands/WireFormatInfo.cpp \
    activemq/connector/openwire/commands/XATransactionId.cpp

h_sources += \
    activemq/connector/openwire/commands/ActiveMQBlobMessage.h \
    activemq/connector/openwire/commands/ActiveMQBytesMessage.h \
    activemq/connector/openwire/commands/ActiveMQDestination.h \
    activemq/connector/openwire/commands/ActiveMQMapMessage.h \
    activemq/connector/openwire/commands/ActiveMQMessage.h \
    activemq/connector/openwire/commands/ActiveMQMessageBase.h \
    activemq/connector/openwire/commands/ActiveMQObjectMessage.h \
    activemq/connector/openwire/commands/ActiveMQQueue.h \
    activemq/connector/openwire/commands/ActiveMQStreamMessage.h \
    activemq/connector/openwire/commands/ActiveMQTempDestination.h \
    activemq/connector/openwire/commands/ActiveMQTempQueue.h \
    activemq/connector/openwire/commands/ActiveMQTempTopic.h \
    activemq/connector/openwire/commands/ActiveMQTextMessage.h \
    activemq/connector/openwire/commands/ActiveMQTopic.h \
    activemq/connector/openwire/commands/BaseCommand.h \
    activemq/connector/openwire/commands/BaseDataStructure.h \
    activemq/connector/openwire/commands/BooleanExpression.h \
    activemq/connector/openwire/commands/BrokerError.h \
    activemq/connector/openwire/commands/BrokerId.h \
    activemq/connector/openwire/commands/BrokerInfo.h \
    activemq/connector/openwire/commands/ConnectionControl.h \
    activemq/connector/openwire/commands/ConnectionError.h \
    activemq/connector/openwire/commands/ConnectionId.h \
    activemq/connector/openwire/commands/ConnectionInfo.h \
    activemq/connector/openwire/commands/ConsumerControl.h \
    activemq/connector/openwire/commands/ConsumerId.h \
    activemq/connector/openwire/commands/ConsumerInfo.h \
    activemq/connector/openwire/commands/ControlCommand.h \
    activemq/connector/openwire/commands/DataArrayResponse.h \
    activemq/connector/openwire/commands/DataResponse.h \
    activemq/connector/openwire/commands/DataStructure.h \
    activemq/connector/openwire/commands/DestinationInfo.h \
    activemq/connector/openwire/commands/DiscoveryEvent.h \
    activemq/connector/openwire/commands/ExceptionResponse.h \
    activemq/connector/openwire/commands/FlushCommand.h \
    activemq/connector/openwire/commands/IntegerResponse.h \
    activemq/connector/openwire/commands/JournalQueueAck.h \
    activemq/connector/openwire/commands/JournalTopicAck.h \
    activemq/connector/openwire/commands/JournalTrace.h \
    activemq/connector/openwire/commands/JournalTransaction.h \
    activemq/connector/openwire/commands/KeepAliveInfo.h \
    activemq/connector/openwire/commands/LastPartialCommand.h \
    activemq/connector/openwire/commands/LocalTransactionId.h \
    activemq/connector/openwire/commands/Message.h \
    activemq/connector/openwire/commands/MessageAck.h \
    activemq/connector/openwire/commands/MessageDispatch.h \
    activemq/connector/openwire/commands/MessageDispatchNotification.h \
    activemq/connector/openwire/commands/MessageId.h \
    activemq/connector/openwire/commands/MessagePull.h \
    activemq/connector/openwire/commands/NetworkBridgeFilter.h \
    activemq/connector/openwire/commands/PartialCommand.h \
    activemq/connector/openwire/commands/ProducerAck.h \
    activemq/connector/openwire/commands/ProducerId.h \
    activemq/connector/openwire/commands/ProducerInfo.h \
    activemq/connector/openwire/commands/RemoveInfo.h \
    activemq/connector/openwire/commands/RemoveSubscriptionInfo.h \
    activemq/connector/openwire/commands/ReplayCommand.h \
    activemq/connector/openwire/commands/Response.h \
    activemq/connector/openwire/commands/SessionId.h \
    activemq/connector/openwire/commands/SessionInfo.h \
    activemq/connector/openwire/commands/ShutdownInfo.h \
    activemq/connector/openwire/commands/SubscriptionInfo.h \
    activemq/connector/openwire/commands/TransactionId.h \
    activemq/connector/openwire/commands/TransactionInfo.h \
    activemq/connector/openwire/commands/WireFormatInfo.h \
    activemq/connector/openwire/commands/XATransactionId.h
