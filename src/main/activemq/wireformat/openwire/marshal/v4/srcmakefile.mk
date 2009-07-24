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
    activemq/wireformat/openwire/marshal/v4/ActiveMQBlobMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQBytesMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMapMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQObjectMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQStreamMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTextMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/BaseCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/BrokerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/BrokerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionErrorMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConsumerControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConsumerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ConsumerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ControlCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/DataArrayResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/DataResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/DestinationInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/DiscoveryEventMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ExceptionResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/FlushCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/IntegerResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalQueueAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalTopicAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalTraceMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalTransactionMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/KeepAliveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/LastPartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/LocalTransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/MarshallerFactory.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchNotificationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/MessagePullMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/NetworkBridgeFilterMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/PartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ProducerAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ProducerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ProducerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/RemoveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ReplayCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/SessionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/SessionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/ShutdownInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/SubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/TransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/TransactionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/WireFormatInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v4/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v4/ActiveMQBlobMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQBytesMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMapMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQObjectMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQStreamMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTextMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/BaseCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/BrokerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/BrokerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionErrorMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConsumerControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConsumerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ConsumerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ControlCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/DataArrayResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/DataResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/DestinationInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/DiscoveryEventMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ExceptionResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/FlushCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/IntegerResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/JournalQueueAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/JournalTopicAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/JournalTraceMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/JournalTransactionMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/KeepAliveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/LastPartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/LocalTransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/MarshallerFactory.h \
    activemq/wireformat/openwire/marshal/v4/MessageAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchNotificationMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/MessageIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/MessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/MessagePullMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/NetworkBridgeFilterMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/PartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ProducerAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ProducerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ProducerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/RemoveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/RemoveSubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ReplayCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/SessionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/SessionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/ShutdownInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/SubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/TransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/TransactionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/WireFormatInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v4/XATransactionIdMarshaller.h
