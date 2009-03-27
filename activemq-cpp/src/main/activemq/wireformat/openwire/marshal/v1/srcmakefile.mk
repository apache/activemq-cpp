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
    activemq/wireformat/openwire/marshal/v1/ActiveMQBlobMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQBytesMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMapMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQObjectMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQStreamMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTextMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/BaseCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/BrokerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/BrokerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionErrorMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConsumerControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConsumerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ConsumerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ControlCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/DataArrayResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/DataResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/DestinationInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/DiscoveryEventMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ExceptionResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/FlushCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/IntegerResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalQueueAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalTopicAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalTraceMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalTransactionMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/KeepAliveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/LastPartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/LocalTransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/MarshallerFactory.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchNotificationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/MessagePullMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/NetworkBridgeFilterMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/PartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ProducerAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ProducerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ProducerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/RemoveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ReplayCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/SessionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/SessionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/ShutdownInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/SubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/TransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/TransactionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/WireFormatInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v1/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v1/ActiveMQBlobMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQBytesMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMapMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQObjectMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQStreamMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTextMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/BaseCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/BrokerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/BrokerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionErrorMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConsumerControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConsumerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ConsumerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ControlCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/DataArrayResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/DataResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/DestinationInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/DiscoveryEventMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ExceptionResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/FlushCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/IntegerResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/JournalQueueAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/JournalTopicAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/JournalTraceMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/JournalTransactionMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/KeepAliveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/LastPartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/LocalTransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/MarshallerFactory.h \
    activemq/wireformat/openwire/marshal/v1/MessageAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchNotificationMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/MessageIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/MessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/MessagePullMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/NetworkBridgeFilterMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/PartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ProducerAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ProducerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ProducerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/RemoveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/RemoveSubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ReplayCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/SessionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/SessionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/ShutdownInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/SubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/TransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/TransactionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/WireFormatInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v1/XATransactionIdMarshaller.h
