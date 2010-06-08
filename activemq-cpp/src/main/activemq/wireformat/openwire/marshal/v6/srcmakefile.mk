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
    activemq/wireformat/openwire/marshal/v6/ActiveMQBlobMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQBytesMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMapMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQObjectMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQStreamMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTextMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/BaseCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/BrokerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/BrokerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionErrorMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConsumerControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConsumerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ConsumerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ControlCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/DataArrayResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/DataResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/DestinationInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/DiscoveryEventMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ExceptionResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/FlushCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/IntegerResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalQueueAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalTopicAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalTraceMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalTransactionMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/KeepAliveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/LastPartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/LocalTransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/MarshallerFactory.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchNotificationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/MessagePullMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/NetworkBridgeFilterMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/PartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ProducerAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ProducerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ProducerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/RemoveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ReplayCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/SessionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/SessionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/ShutdownInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/SubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/TransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/TransactionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/WireFormatInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v6/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v6/ActiveMQBlobMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQBytesMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMapMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQObjectMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQStreamMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTextMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/BaseCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/BrokerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/BrokerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionErrorMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConsumerControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConsumerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ConsumerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ControlCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/DataArrayResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/DataResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/DestinationInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/DiscoveryEventMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ExceptionResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/FlushCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/IntegerResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/JournalQueueAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/JournalTopicAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/JournalTraceMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/JournalTransactionMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/KeepAliveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/LastPartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/LocalTransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/MarshallerFactory.h \
    activemq/wireformat/openwire/marshal/v6/MessageAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchNotificationMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/MessageIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/MessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/MessagePullMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/NetworkBridgeFilterMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/PartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ProducerAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ProducerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ProducerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/RemoveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/RemoveSubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ReplayCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/SessionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/SessionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/ShutdownInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/SubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/TransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/TransactionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/WireFormatInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v6/XATransactionIdMarshaller.h
