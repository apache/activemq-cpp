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
    activemq/connector/openwire/marshal/v1/ActiveMQBytesMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQDestinationMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQMapMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQObjectMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQQueueMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQStreamMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTempDestinationMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTempQueueMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTempTopicMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTextMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTopicMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/BaseCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/BrokerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/BrokerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionControlMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionErrorMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConsumerControlMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConsumerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ConsumerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ControlCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/DataArrayResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/DataResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/DestinationInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/DiscoveryEventMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ExceptionResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/FlushCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/IntegerResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/JournalQueueAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/JournalTopicAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/JournalTraceMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/JournalTransactionMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/KeepAliveInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/LastPartialCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/LocalTransactionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/MarshallerFactory.cpp \
    activemq/connector/openwire/marshal/v1/MessageAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/MessageDispatchMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/MessageDispatchNotificationMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/MessageIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/MessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/MessagePullMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/NetworkBridgeFilterMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/PartialCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ProducerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ProducerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/RemoveInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ReplayCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/SessionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/SessionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/ShutdownInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/SubscriptionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/TransactionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/TransactionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/WireFormatInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v1/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/connector/openwire/marshal/v1/ActiveMQBytesMessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQDestinationMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQMapMessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQMessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQObjectMessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQQueueMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQStreamMessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTempDestinationMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTempQueueMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTempTopicMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTextMessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTopicMarshaller.h \
    activemq/connector/openwire/marshal/v1/BaseCommandMarshaller.h \
    activemq/connector/openwire/marshal/v1/BrokerIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/BrokerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConnectionControlMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConnectionErrorMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConnectionIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConnectionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConsumerControlMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConsumerIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/ConsumerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/ControlCommandMarshaller.h \
    activemq/connector/openwire/marshal/v1/DataArrayResponseMarshaller.h \
    activemq/connector/openwire/marshal/v1/DataResponseMarshaller.h \
    activemq/connector/openwire/marshal/v1/DestinationInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/DiscoveryEventMarshaller.h \
    activemq/connector/openwire/marshal/v1/ExceptionResponseMarshaller.h \
    activemq/connector/openwire/marshal/v1/FlushCommandMarshaller.h \
    activemq/connector/openwire/marshal/v1/IntegerResponseMarshaller.h \
    activemq/connector/openwire/marshal/v1/JournalQueueAckMarshaller.h \
    activemq/connector/openwire/marshal/v1/JournalTopicAckMarshaller.h \
    activemq/connector/openwire/marshal/v1/JournalTraceMarshaller.h \
    activemq/connector/openwire/marshal/v1/JournalTransactionMarshaller.h \
    activemq/connector/openwire/marshal/v1/KeepAliveInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/LastPartialCommandMarshaller.h \
    activemq/connector/openwire/marshal/v1/LocalTransactionIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/MarshallerFactory.h \
    activemq/connector/openwire/marshal/v1/MessageAckMarshaller.h \
    activemq/connector/openwire/marshal/v1/MessageDispatchMarshaller.h \
    activemq/connector/openwire/marshal/v1/MessageDispatchNotificationMarshaller.h \
    activemq/connector/openwire/marshal/v1/MessageIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/MessageMarshaller.h \
    activemq/connector/openwire/marshal/v1/MessagePullMarshaller.h \
    activemq/connector/openwire/marshal/v1/NetworkBridgeFilterMarshaller.h \
    activemq/connector/openwire/marshal/v1/PartialCommandMarshaller.h \
    activemq/connector/openwire/marshal/v1/ProducerIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/ProducerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/RemoveInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/RemoveSubscriptionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/ReplayCommandMarshaller.h \
    activemq/connector/openwire/marshal/v1/ResponseMarshaller.h \
    activemq/connector/openwire/marshal/v1/SessionIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/SessionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/ShutdownInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/SubscriptionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/TransactionIdMarshaller.h \
    activemq/connector/openwire/marshal/v1/TransactionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/WireFormatInfoMarshaller.h \
    activemq/connector/openwire/marshal/v1/XATransactionIdMarshaller.h
