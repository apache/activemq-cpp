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
    activemq/connector/openwire/marshal/v2/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQQueueMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ActiveMQTopicMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/BaseCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/BrokerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/BrokerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionControlMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionErrorMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConnectionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConsumerControlMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConsumerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ConsumerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ControlCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/DataArrayResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/DataResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/DestinationInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/DiscoveryEventMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ExceptionResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/FlushCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/IntegerResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/JournalQueueAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/JournalTopicAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/JournalTraceMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/JournalTransactionMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/KeepAliveInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/LastPartialCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/LocalTransactionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/MessageAckMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/MessageDispatchMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/MessageIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/MessagePullMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/PartialCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ProducerIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ProducerInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/RemoveInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ReplayCommandMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ResponseMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/SessionIdMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/SessionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/ShutdownInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/SubscriptionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/TransactionInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/WireFormatInfoMarshallerTest.cpp \
    activemq/connector/openwire/marshal/v2/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/connector/openwire/marshal/v2/ActiveMQBytesMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQMapMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQObjectMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQQueueMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQStreamMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTempQueueMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTempTopicMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTextMessageMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ActiveMQTopicMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/BaseCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/BrokerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/BrokerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConnectionControlMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConnectionErrorMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConnectionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConnectionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConsumerControlMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConsumerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ConsumerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ControlCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/DataArrayResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/DataResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/DestinationInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/DiscoveryEventMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ExceptionResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/FlushCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/IntegerResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/JournalQueueAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/JournalTopicAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/JournalTraceMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/JournalTransactionMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/KeepAliveInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/LastPartialCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/LocalTransactionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/MessageAckMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/MessageDispatchMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/MessageDispatchNotificationMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/MessageIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/MessagePullMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/NetworkBridgeFilterMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/PartialCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ProducerIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ProducerInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/RemoveInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ReplayCommandMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ResponseMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/SessionIdMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/SessionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/ShutdownInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/SubscriptionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/TransactionInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/WireFormatInfoMarshallerTest.h \
    activemq/connector/openwire/marshal/v2/XATransactionIdMarshallerTest.h
