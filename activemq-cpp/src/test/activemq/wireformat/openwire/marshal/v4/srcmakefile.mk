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
    activemq/wireformat/openwire/marshal/v4/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/BrokerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/BrokerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionErrorMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConnectionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConsumerControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConsumerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ConsumerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ControlCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/DataArrayResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/DataResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/DestinationInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/DiscoveryEventMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ExceptionResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/FlushCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/IntegerResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalQueueAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalTopicAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalTraceMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/JournalTransactionMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/KeepAliveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/LastPartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/LocalTransactionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/MessageIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/MessagePullMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/PartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ProducerAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ProducerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ProducerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/RemoveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ReplayCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/SessionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/SessionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/ShutdownInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/SubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/TransactionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/WireFormatInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v4/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v4/ActiveMQBlobMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQBytesMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMapMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQObjectMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQStreamMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTempTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTextMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ActiveMQTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/BrokerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/BrokerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionErrorMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConnectionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConsumerControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConsumerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ConsumerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ControlCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/DataArrayResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/DataResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/DestinationInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/DiscoveryEventMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ExceptionResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/FlushCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/IntegerResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/JournalQueueAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/JournalTopicAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/JournalTraceMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/JournalTransactionMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/KeepAliveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/LastPartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/LocalTransactionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/MessageAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/MessageDispatchNotificationMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/MessageIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/MessagePullMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/NetworkBridgeFilterMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/PartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ProducerAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ProducerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ProducerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/RemoveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ReplayCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/SessionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/SessionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/ShutdownInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/SubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/TransactionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/WireFormatInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v4/XATransactionIdMarshallerTest.h
