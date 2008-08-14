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
    activemq/connector/openwire/marshal/v3/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQQueueMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ActiveMQTopicMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/BrokerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/BrokerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionControlMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionErrorMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConnectionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConsumerControlMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConsumerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ConsumerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ControlCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/DataArrayResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/DataResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/DestinationInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/DiscoveryEventMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ExceptionResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/FlushCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/IntegerResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/JournalQueueAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/JournalTopicAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/JournalTraceMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/JournalTransactionMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/KeepAliveInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/LastPartialCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/LocalTransactionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/MessageAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/MessageDispatchMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/MessageIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/MessagePullMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/PartialCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ProducerAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ProducerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ProducerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/RemoveInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ReplayCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/SessionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/SessionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/ShutdownInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/SubscriptionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/TransactionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/WireFormatInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v3/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/connector/openwire/marshal/v3/ActiveMQBlobMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQBytesMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQMapMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQObjectMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQQueueMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQStreamMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTempQueueMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTempTopicMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTextMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ActiveMQTopicMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/BrokerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/BrokerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConnectionControlMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConnectionErrorMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConnectionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConnectionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConsumerControlMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConsumerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ConsumerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ControlCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/DataArrayResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/DataResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/DestinationInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/DiscoveryEventMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ExceptionResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/FlushCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/IntegerResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/JournalQueueAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/JournalTopicAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/JournalTraceMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/JournalTransactionMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/KeepAliveInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/LastPartialCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/LocalTransactionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/MessageAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/MessageDispatchMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/MessageDispatchNotificationMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/MessageIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/MessagePullMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/NetworkBridgeFilterMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/PartialCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ProducerAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ProducerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ProducerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/RemoveInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ReplayCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/SessionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/SessionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/ShutdownInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/SubscriptionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/TransactionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/WireFormatInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v3/XATransactionIdMarshallerTest.h
