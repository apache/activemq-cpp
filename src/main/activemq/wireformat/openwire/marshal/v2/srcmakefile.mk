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
    activemq/wireformat/openwire/marshal/v2/ActiveMQBlobMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQBytesMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMapMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQObjectMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQStreamMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTextMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/BaseCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/BrokerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/BrokerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionErrorMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConsumerControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConsumerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ConsumerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ControlCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/DataArrayResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/DataResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/DestinationInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/DiscoveryEventMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ExceptionResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/FlushCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/IntegerResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalQueueAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalTopicAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalTraceMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalTransactionMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/KeepAliveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/LastPartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/LocalTransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/MarshallerFactory.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchNotificationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/MessagePullMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/NetworkBridgeFilterMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/PartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ProducerAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ProducerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ProducerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/RemoveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ReplayCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/SessionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/SessionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/ShutdownInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/SubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/TransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/TransactionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/WireFormatInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v2/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v2/ActiveMQBlobMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQBytesMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMapMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQObjectMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQStreamMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTextMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/BaseCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/BrokerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/BrokerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionErrorMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConsumerControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConsumerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ConsumerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ControlCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/DataArrayResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/DataResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/DestinationInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/DiscoveryEventMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ExceptionResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/FlushCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/IntegerResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/JournalQueueAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/JournalTopicAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/JournalTraceMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/JournalTransactionMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/KeepAliveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/LastPartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/LocalTransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/MarshallerFactory.h \
    activemq/wireformat/openwire/marshal/v2/MessageAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchNotificationMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/MessageIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/MessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/MessagePullMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/NetworkBridgeFilterMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/PartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ProducerAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ProducerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ProducerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/RemoveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/RemoveSubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ReplayCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/SessionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/SessionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/ShutdownInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/SubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/TransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/TransactionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/WireFormatInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v2/XATransactionIdMarshaller.h
