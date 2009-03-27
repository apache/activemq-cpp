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
    activemq/wireformat/openwire/marshal/v3/ActiveMQBlobMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQBytesMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMapMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQObjectMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQStreamMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTextMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/BaseCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/BrokerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/BrokerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionErrorMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConsumerControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConsumerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ConsumerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ControlCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/DataArrayResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/DataResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/DestinationInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/DiscoveryEventMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ExceptionResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/FlushCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/IntegerResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalQueueAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalTopicAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalTraceMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalTransactionMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/KeepAliveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/LastPartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/LocalTransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/MarshallerFactory.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchNotificationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/MessagePullMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/NetworkBridgeFilterMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/PartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ProducerAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ProducerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ProducerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/RemoveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ReplayCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/SessionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/SessionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/ShutdownInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/SubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/TransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/TransactionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/WireFormatInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v3/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v3/ActiveMQBlobMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQBytesMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMapMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQObjectMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQStreamMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTextMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/BaseCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/BrokerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/BrokerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionErrorMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConsumerControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConsumerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ConsumerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ControlCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/DataArrayResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/DataResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/DestinationInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/DiscoveryEventMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ExceptionResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/FlushCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/IntegerResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/JournalQueueAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/JournalTopicAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/JournalTraceMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/JournalTransactionMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/KeepAliveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/LastPartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/LocalTransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/MarshallerFactory.h \
    activemq/wireformat/openwire/marshal/v3/MessageAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchNotificationMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/MessageIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/MessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/MessagePullMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/NetworkBridgeFilterMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/PartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ProducerAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ProducerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ProducerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/RemoveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/RemoveSubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ReplayCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/SessionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/SessionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/ShutdownInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/SubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/TransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/TransactionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/WireFormatInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v3/XATransactionIdMarshaller.h
