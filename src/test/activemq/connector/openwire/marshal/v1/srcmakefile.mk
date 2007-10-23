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
    activemq/connector/openwire/marshal/v1/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQQueueMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ActiveMQTopicMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/BrokerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/BrokerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionControlMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionErrorMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConnectionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConsumerControlMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConsumerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ConsumerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ControlCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/DataArrayResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/DataResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/DestinationInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/DiscoveryEventMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ExceptionResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/FlushCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/IntegerResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/JournalQueueAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/JournalTopicAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/JournalTraceMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/JournalTransactionMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/KeepAliveInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/LastPartialCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/LocalTransactionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/MessageAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/MessageDispatchMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/MessageIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/MessagePullMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/PartialCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ProducerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ProducerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/RemoveInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ReplayCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/SessionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/SessionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/ShutdownInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/SubscriptionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/TransactionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/WireFormatInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v1/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/connector/openwire/marshal/v1/ActiveMQBytesMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQMapMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQObjectMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQQueueMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQStreamMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTempQueueMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTempTopicMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTextMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ActiveMQTopicMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/BrokerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/BrokerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConnectionControlMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConnectionErrorMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConnectionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConnectionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConsumerControlMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConsumerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ConsumerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ControlCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/DataArrayResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/DataResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/DestinationInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/DiscoveryEventMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ExceptionResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/FlushCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/IntegerResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/JournalQueueAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/JournalTopicAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/JournalTraceMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/JournalTransactionMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/KeepAliveInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/LastPartialCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/LocalTransactionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/MessageAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/MessageDispatchMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/MessageDispatchNotificationMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/MessageIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/MessagePullMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/NetworkBridgeFilterMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/PartialCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ProducerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ProducerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/RemoveInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ReplayCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/SessionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/SessionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/ShutdownInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/SubscriptionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/TransactionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/WireFormatInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v1/XATransactionIdMarshallerTest.h
