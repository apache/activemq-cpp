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
    activemq/wireformat/openwire/marshal/v5/ActiveMQBlobMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQBytesMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMapMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQObjectMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQStreamMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempDestinationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempQueueMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTextMessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTopicMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/BaseCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/BrokerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/BrokerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionErrorMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConsumerControlMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConsumerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ConsumerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ControlCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/DataArrayResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/DataResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/DestinationInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/DiscoveryEventMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ExceptionResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/FlushCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/IntegerResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalQueueAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalTopicAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalTraceMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalTransactionMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/KeepAliveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/LastPartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/LocalTransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/MarshallerFactory.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchNotificationMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/MessagePullMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/NetworkBridgeFilterMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/PartialCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ProducerAckMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ProducerIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ProducerInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/RemoveInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/RemoveSubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ReplayCommandMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ResponseMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/SessionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/SessionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/ShutdownInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/SubscriptionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/TransactionIdMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/TransactionInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/WireFormatInfoMarshaller.cpp \
    activemq/wireformat/openwire/marshal/v5/XATransactionIdMarshaller.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v5/ActiveMQBlobMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQBytesMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMapMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQObjectMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQStreamMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempDestinationMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempQueueMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTextMessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTopicMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/BaseCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/BrokerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/BrokerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionErrorMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConsumerControlMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConsumerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ConsumerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ControlCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/DataArrayResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/DataResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/DestinationInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/DiscoveryEventMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ExceptionResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/FlushCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/IntegerResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/JournalQueueAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/JournalTopicAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/JournalTraceMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/JournalTransactionMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/KeepAliveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/LastPartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/LocalTransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/MarshallerFactory.h \
    activemq/wireformat/openwire/marshal/v5/MessageAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchNotificationMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/MessageIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/MessageMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/MessagePullMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/NetworkBridgeFilterMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/PartialCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ProducerAckMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ProducerIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ProducerInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/RemoveInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/RemoveSubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ReplayCommandMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ResponseMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/SessionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/SessionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/ShutdownInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/SubscriptionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/TransactionIdMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/TransactionInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/WireFormatInfoMarshaller.h \
    activemq/wireformat/openwire/marshal/v5/XATransactionIdMarshaller.h
