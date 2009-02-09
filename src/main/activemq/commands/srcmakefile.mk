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
    activemq/commands/ActiveMQBlobMessage.cpp \
    activemq/commands/ActiveMQBytesMessage.cpp \
    activemq/commands/ActiveMQDestination.cpp \
    activemq/commands/ActiveMQMapMessage.cpp \
    activemq/commands/ActiveMQMessage.cpp \
    activemq/commands/ActiveMQObjectMessage.cpp \
    activemq/commands/ActiveMQQueue.cpp \
    activemq/commands/ActiveMQStreamMessage.cpp \
    activemq/commands/ActiveMQTempDestination.cpp \
    activemq/commands/ActiveMQTempQueue.cpp \
    activemq/commands/ActiveMQTempTopic.cpp \
    activemq/commands/ActiveMQTextMessage.cpp \
    activemq/commands/ActiveMQTopic.cpp \
    activemq/commands/BrokerError.cpp \
    activemq/commands/BrokerId.cpp \
    activemq/commands/BrokerInfo.cpp \
    activemq/commands/ConnectionControl.cpp \
    activemq/commands/ConnectionError.cpp \
    activemq/commands/ConnectionId.cpp \
    activemq/commands/ConnectionInfo.cpp \
    activemq/commands/ConsumerControl.cpp \
    activemq/commands/ConsumerId.cpp \
    activemq/commands/ConsumerInfo.cpp \
    activemq/commands/ControlCommand.cpp \
    activemq/commands/DataArrayResponse.cpp \
    activemq/commands/DataResponse.cpp \
    activemq/commands/DestinationInfo.cpp \
    activemq/commands/DiscoveryEvent.cpp \
    activemq/commands/ExceptionResponse.cpp \
    activemq/commands/FlushCommand.cpp \
    activemq/commands/IntegerResponse.cpp \
    activemq/commands/JournalQueueAck.cpp \
    activemq/commands/JournalTopicAck.cpp \
    activemq/commands/JournalTrace.cpp \
    activemq/commands/JournalTransaction.cpp \
    activemq/commands/KeepAliveInfo.cpp \
    activemq/commands/LastPartialCommand.cpp \
    activemq/commands/LocalTransactionId.cpp \
    activemq/commands/Message.cpp \
    activemq/commands/MessageAck.cpp \
    activemq/commands/MessageDispatch.cpp \
    activemq/commands/MessageDispatchNotification.cpp \
    activemq/commands/MessageId.cpp \
    activemq/commands/MessagePull.cpp \
    activemq/commands/NetworkBridgeFilter.cpp \
    activemq/commands/PartialCommand.cpp \
    activemq/commands/ProducerAck.cpp \
    activemq/commands/ProducerId.cpp \
    activemq/commands/ProducerInfo.cpp \
    activemq/commands/RemoveInfo.cpp \
    activemq/commands/RemoveSubscriptionInfo.cpp \
    activemq/commands/ReplayCommand.cpp \
    activemq/commands/Response.cpp \
    activemq/commands/SessionId.cpp \
    activemq/commands/SessionInfo.cpp \
    activemq/commands/ShutdownInfo.cpp \
    activemq/commands/SubscriptionInfo.cpp \
    activemq/commands/TransactionId.cpp \
    activemq/commands/TransactionInfo.cpp \
    activemq/commands/WireFormatInfo.cpp \
    activemq/commands/XATransactionId.cpp

h_sources += \
    activemq/commands/ActiveMQBlobMessage.h \
    activemq/commands/ActiveMQBytesMessage.h \
    activemq/commands/ActiveMQDestination.h \
    activemq/commands/ActiveMQMapMessage.h \
    activemq/commands/ActiveMQMessage.h \
    activemq/commands/ActiveMQMessageTemplate.h \
    activemq/commands/ActiveMQObjectMessage.h \
    activemq/commands/ActiveMQQueue.h \
    activemq/commands/ActiveMQStreamMessage.h \
    activemq/commands/ActiveMQTempDestination.h \
    activemq/commands/ActiveMQTempQueue.h \
    activemq/commands/ActiveMQTempTopic.h \
    activemq/commands/ActiveMQTextMessage.h \
    activemq/commands/ActiveMQTopic.h \
    activemq/commands/BaseCommand.h \
    activemq/commands/BaseDataStructure.h \
    activemq/commands/BooleanExpression.h \
    activemq/commands/BrokerError.h \
    activemq/commands/BrokerId.h \
    activemq/commands/BrokerInfo.h \
    activemq/commands/Command.h \
    activemq/commands/ConnectionControl.h \
    activemq/commands/ConnectionError.h \
    activemq/commands/ConnectionId.h \
    activemq/commands/ConnectionInfo.h \
    activemq/commands/ConsumerControl.h \
    activemq/commands/ConsumerId.h \
    activemq/commands/ConsumerInfo.h \
    activemq/commands/ControlCommand.h \
    activemq/commands/DataArrayResponse.h \
    activemq/commands/DataResponse.h \
    activemq/commands/DataStructure.h \
    activemq/commands/DestinationInfo.h \
    activemq/commands/DiscoveryEvent.h \
    activemq/commands/ExceptionResponse.h \
    activemq/commands/FlushCommand.h \
    activemq/commands/IntegerResponse.h \
    activemq/commands/JournalQueueAck.h \
    activemq/commands/JournalTopicAck.h \
    activemq/commands/JournalTrace.h \
    activemq/commands/JournalTransaction.h \
    activemq/commands/KeepAliveInfo.h \
    activemq/commands/LastPartialCommand.h \
    activemq/commands/LocalTransactionId.h \
    activemq/commands/Message.h \
    activemq/commands/MessageAck.h \
    activemq/commands/MessageDispatch.h \
    activemq/commands/MessageDispatchNotification.h \
    activemq/commands/MessageId.h \
    activemq/commands/MessagePull.h \
    activemq/commands/NetworkBridgeFilter.h \
    activemq/commands/PartialCommand.h \
    activemq/commands/ProducerAck.h \
    activemq/commands/ProducerId.h \
    activemq/commands/ProducerInfo.h \
    activemq/commands/RemoveInfo.h \
    activemq/commands/RemoveSubscriptionInfo.h \
    activemq/commands/ReplayCommand.h \
    activemq/commands/Response.h \
    activemq/commands/SessionId.h \
    activemq/commands/SessionInfo.h \
    activemq/commands/ShutdownInfo.h \
    activemq/commands/SubscriptionInfo.h \
    activemq/commands/TransactionId.h \
    activemq/commands/TransactionInfo.h \
    activemq/commands/WireFormatInfo.h \
    activemq/commands/XATransactionId.h
