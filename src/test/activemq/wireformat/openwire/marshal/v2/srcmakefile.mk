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
    activemq/wireformat/openwire/marshal/v2/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/BrokerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/BrokerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionErrorMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConnectionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConsumerControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConsumerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ConsumerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ControlCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/DataArrayResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/DataResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/DestinationInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/DiscoveryEventMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ExceptionResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/FlushCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/IntegerResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalQueueAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalTopicAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalTraceMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/JournalTransactionMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/KeepAliveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/LastPartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/LocalTransactionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/MessageIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/MessagePullMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/PartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ProducerAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ProducerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ProducerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/RemoveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ReplayCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/SessionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/SessionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/ShutdownInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/SubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/TransactionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/WireFormatInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v2/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v2/ActiveMQBlobMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQBytesMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMapMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQObjectMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQStreamMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTempTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTextMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ActiveMQTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/BrokerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/BrokerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionErrorMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConnectionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConsumerControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConsumerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ConsumerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ControlCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/DataArrayResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/DataResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/DestinationInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/DiscoveryEventMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ExceptionResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/FlushCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/IntegerResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/JournalQueueAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/JournalTopicAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/JournalTraceMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/JournalTransactionMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/KeepAliveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/LastPartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/LocalTransactionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/MessageAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/MessageDispatchNotificationMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/MessageIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/MessagePullMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/NetworkBridgeFilterMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/PartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ProducerAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ProducerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ProducerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/RemoveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ReplayCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/SessionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/SessionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/ShutdownInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/SubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/TransactionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/WireFormatInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v2/XATransactionIdMarshallerTest.h
