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
    activemq/wireformat/openwire/marshal/v3/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/BrokerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/BrokerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionErrorMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConnectionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConsumerControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConsumerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ConsumerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ControlCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/DataArrayResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/DataResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/DestinationInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/DiscoveryEventMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ExceptionResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/FlushCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/IntegerResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalQueueAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalTopicAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalTraceMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/JournalTransactionMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/KeepAliveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/LastPartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/LocalTransactionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/MessageIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/MessagePullMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/PartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ProducerAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ProducerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ProducerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/RemoveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ReplayCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/SessionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/SessionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/ShutdownInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/SubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/TransactionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/WireFormatInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v3/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v3/ActiveMQBlobMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQBytesMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMapMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQObjectMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQStreamMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTempTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTextMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ActiveMQTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/BrokerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/BrokerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionErrorMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConnectionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConsumerControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConsumerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ConsumerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ControlCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/DataArrayResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/DataResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/DestinationInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/DiscoveryEventMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ExceptionResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/FlushCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/IntegerResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/JournalQueueAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/JournalTopicAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/JournalTraceMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/JournalTransactionMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/KeepAliveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/LastPartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/LocalTransactionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/MessageAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/MessageDispatchNotificationMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/MessageIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/MessagePullMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/NetworkBridgeFilterMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/PartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ProducerAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ProducerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ProducerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/RemoveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ReplayCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/SessionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/SessionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/ShutdownInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/SubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/TransactionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/WireFormatInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v3/XATransactionIdMarshallerTest.h
