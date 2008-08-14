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
    activemq/connector/openwire/marshal/v3/ActiveMQBlobMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQBytesMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQDestinationMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQMapMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQObjectMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQQueueMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQStreamMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTempDestinationMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTempQueueMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTempTopicMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTextMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTopicMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/BaseCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/BrokerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/BrokerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionControlMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionErrorMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConsumerControlMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConsumerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ConsumerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ControlCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/DataArrayResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/DataResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/DestinationInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/DiscoveryEventMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ExceptionResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/FlushCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/IntegerResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/JournalQueueAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/JournalTopicAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/JournalTraceMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/JournalTransactionMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/KeepAliveInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/LastPartialCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/LocalTransactionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/MarshallerFactory.cpp \
    activemq/connector/openwire/marshal/v3/MessageAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/MessageDispatchMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/MessageDispatchNotificationMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/MessageIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/MessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/MessagePullMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/NetworkBridgeFilterMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/PartialCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ProducerAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ProducerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ProducerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/RemoveInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ReplayCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/SessionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/SessionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/ShutdownInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/SubscriptionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/TransactionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/TransactionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/WireFormatInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v3/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/connector/openwire/marshal/v3/ActiveMQBlobMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQBytesMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQDestinationMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQMapMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQObjectMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQQueueMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQStreamMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTempDestinationMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTempQueueMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTempTopicMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTextMessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTopicMarshaller.h \
    activemq/connector/openwire/marshal/v3/BaseCommandMarshaller.h \
    activemq/connector/openwire/marshal/v3/BrokerIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/BrokerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConnectionControlMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConnectionErrorMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConnectionIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConnectionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConsumerControlMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConsumerIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/ConsumerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/ControlCommandMarshaller.h \
    activemq/connector/openwire/marshal/v3/DataArrayResponseMarshaller.h \
    activemq/connector/openwire/marshal/v3/DataResponseMarshaller.h \
    activemq/connector/openwire/marshal/v3/DestinationInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/DiscoveryEventMarshaller.h \
    activemq/connector/openwire/marshal/v3/ExceptionResponseMarshaller.h \
    activemq/connector/openwire/marshal/v3/FlushCommandMarshaller.h \
    activemq/connector/openwire/marshal/v3/IntegerResponseMarshaller.h \
    activemq/connector/openwire/marshal/v3/JournalQueueAckMarshaller.h \
    activemq/connector/openwire/marshal/v3/JournalTopicAckMarshaller.h \
    activemq/connector/openwire/marshal/v3/JournalTraceMarshaller.h \
    activemq/connector/openwire/marshal/v3/JournalTransactionMarshaller.h \
    activemq/connector/openwire/marshal/v3/KeepAliveInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/LastPartialCommandMarshaller.h \
    activemq/connector/openwire/marshal/v3/LocalTransactionIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/MarshallerFactory.h \
    activemq/connector/openwire/marshal/v3/MessageAckMarshaller.h \
    activemq/connector/openwire/marshal/v3/MessageDispatchMarshaller.h \
    activemq/connector/openwire/marshal/v3/MessageDispatchNotificationMarshaller.h \
    activemq/connector/openwire/marshal/v3/MessageIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/MessageMarshaller.h \
    activemq/connector/openwire/marshal/v3/MessagePullMarshaller.h \
    activemq/connector/openwire/marshal/v3/NetworkBridgeFilterMarshaller.h \
    activemq/connector/openwire/marshal/v3/PartialCommandMarshaller.h \
    activemq/connector/openwire/marshal/v3/ProducerAckMarshaller.h \
    activemq/connector/openwire/marshal/v3/ProducerIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/ProducerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/RemoveInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/RemoveSubscriptionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/ReplayCommandMarshaller.h \
    activemq/connector/openwire/marshal/v3/ResponseMarshaller.h \
    activemq/connector/openwire/marshal/v3/SessionIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/SessionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/ShutdownInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/SubscriptionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/TransactionIdMarshaller.h \
    activemq/connector/openwire/marshal/v3/TransactionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/WireFormatInfoMarshaller.h \
    activemq/connector/openwire/marshal/v3/XATransactionIdMarshaller.h
