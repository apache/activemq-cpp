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
    activemq/connector/openwire/marshal/v2/ActiveMQBytesMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQDestinationMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQMapMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQObjectMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQQueueMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQStreamMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTempDestinationMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTempQueueMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTempTopicMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTextMessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTopicMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/BaseCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/BrokerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/BrokerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionControlMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionErrorMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConsumerControlMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConsumerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ConsumerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ControlCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/DataArrayResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/DataResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/DestinationInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/DiscoveryEventMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ExceptionResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/FlushCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/IntegerResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/JournalQueueAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/JournalTopicAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/JournalTraceMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/JournalTransactionMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/KeepAliveInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/LastPartialCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/LocalTransactionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/MarshallerFactory.cpp \
    activemq/connector/openwire/marshal/v2/MessageAckMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/MessageDispatchMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/MessageDispatchNotificationMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/MessageIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/MessageMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/MessagePullMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/NetworkBridgeFilterMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/PartialCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ProducerIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ProducerInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/RemoveInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ReplayCommandMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ResponseMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/SessionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/SessionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/ShutdownInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/SubscriptionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/TransactionIdMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/TransactionInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/WireFormatInfoMarshaller.cpp \
    activemq/connector/openwire/marshal/v2/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/connector/openwire/marshal/v2/ActiveMQBytesMessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQDestinationMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQMapMessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQMessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQObjectMessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQQueueMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQStreamMessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTempDestinationMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTempQueueMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTempTopicMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTextMessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTopicMarshaller.h \
    activemq/connector/openwire/marshal/v2/BaseCommandMarshaller.h \
    activemq/connector/openwire/marshal/v2/BrokerIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/BrokerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConnectionControlMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConnectionErrorMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConnectionIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConnectionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConsumerControlMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConsumerIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/ConsumerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/ControlCommandMarshaller.h \
    activemq/connector/openwire/marshal/v2/DataArrayResponseMarshaller.h \
    activemq/connector/openwire/marshal/v2/DataResponseMarshaller.h \
    activemq/connector/openwire/marshal/v2/DestinationInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/DiscoveryEventMarshaller.h \
    activemq/connector/openwire/marshal/v2/ExceptionResponseMarshaller.h \
    activemq/connector/openwire/marshal/v2/FlushCommandMarshaller.h \
    activemq/connector/openwire/marshal/v2/IntegerResponseMarshaller.h \
    activemq/connector/openwire/marshal/v2/JournalQueueAckMarshaller.h \
    activemq/connector/openwire/marshal/v2/JournalTopicAckMarshaller.h \
    activemq/connector/openwire/marshal/v2/JournalTraceMarshaller.h \
    activemq/connector/openwire/marshal/v2/JournalTransactionMarshaller.h \
    activemq/connector/openwire/marshal/v2/KeepAliveInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/LastPartialCommandMarshaller.h \
    activemq/connector/openwire/marshal/v2/LocalTransactionIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/MarshallerFactory.h \
    activemq/connector/openwire/marshal/v2/MessageAckMarshaller.h \
    activemq/connector/openwire/marshal/v2/MessageDispatchMarshaller.h \
    activemq/connector/openwire/marshal/v2/MessageDispatchNotificationMarshaller.h \
    activemq/connector/openwire/marshal/v2/MessageIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/MessageMarshaller.h \
    activemq/connector/openwire/marshal/v2/MessagePullMarshaller.h \
    activemq/connector/openwire/marshal/v2/NetworkBridgeFilterMarshaller.h \
    activemq/connector/openwire/marshal/v2/PartialCommandMarshaller.h \
    activemq/connector/openwire/marshal/v2/ProducerIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/ProducerInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/RemoveInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/RemoveSubscriptionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/ReplayCommandMarshaller.h \
    activemq/connector/openwire/marshal/v2/ResponseMarshaller.h \
    activemq/connector/openwire/marshal/v2/SessionIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/SessionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/ShutdownInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/SubscriptionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/TransactionIdMarshaller.h \
    activemq/connector/openwire/marshal/v2/TransactionInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/WireFormatInfoMarshaller.h \
    activemq/connector/openwire/marshal/v2/XATransactionIdMarshaller.h
