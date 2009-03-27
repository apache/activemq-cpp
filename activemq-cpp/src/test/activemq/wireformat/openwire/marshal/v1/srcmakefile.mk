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
    activemq/wireformat/openwire/marshal/v1/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/BrokerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/BrokerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionErrorMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConnectionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConsumerControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConsumerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ConsumerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ControlCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/DataArrayResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/DataResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/DestinationInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/DiscoveryEventMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ExceptionResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/FlushCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/IntegerResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalQueueAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalTopicAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalTraceMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/JournalTransactionMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/KeepAliveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/LastPartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/LocalTransactionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/MessageIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/MessagePullMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/PartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ProducerAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ProducerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ProducerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/RemoveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ReplayCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/SessionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/SessionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/ShutdownInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/SubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/TransactionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/WireFormatInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v1/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v1/ActiveMQBlobMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQBytesMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMapMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQObjectMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQStreamMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTempTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTextMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ActiveMQTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/BrokerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/BrokerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionErrorMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConnectionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConsumerControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConsumerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ConsumerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ControlCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/DataArrayResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/DataResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/DestinationInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/DiscoveryEventMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ExceptionResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/FlushCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/IntegerResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/JournalQueueAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/JournalTopicAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/JournalTraceMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/JournalTransactionMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/KeepAliveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/LastPartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/LocalTransactionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/MessageAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/MessageDispatchNotificationMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/MessageIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/MessagePullMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/NetworkBridgeFilterMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/PartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ProducerAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ProducerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ProducerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/RemoveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ReplayCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/SessionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/SessionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/ShutdownInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/SubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/TransactionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/WireFormatInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v1/XATransactionIdMarshallerTest.h
