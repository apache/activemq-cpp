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
    activemq/wireformat/openwire/marshal/v5/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/BrokerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/BrokerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionErrorMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConnectionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConsumerControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConsumerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ConsumerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ControlCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/DataArrayResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/DataResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/DestinationInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/DiscoveryEventMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ExceptionResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/FlushCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/IntegerResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalQueueAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalTopicAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalTraceMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/JournalTransactionMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/KeepAliveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/LastPartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/LocalTransactionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/MessageIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/MessagePullMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/PartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ProducerAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ProducerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ProducerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/RemoveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ReplayCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/SessionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/SessionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/ShutdownInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/SubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/TransactionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/WireFormatInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v5/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v5/ActiveMQBlobMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQBytesMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMapMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQObjectMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQStreamMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTempTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTextMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ActiveMQTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/BrokerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/BrokerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionErrorMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConnectionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConsumerControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConsumerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ConsumerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ControlCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/DataArrayResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/DataResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/DestinationInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/DiscoveryEventMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ExceptionResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/FlushCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/IntegerResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/JournalQueueAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/JournalTopicAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/JournalTraceMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/JournalTransactionMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/KeepAliveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/LastPartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/LocalTransactionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/MessageAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/MessageDispatchNotificationMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/MessageIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/MessagePullMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/NetworkBridgeFilterMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/PartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ProducerAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ProducerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ProducerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/RemoveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ReplayCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/SessionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/SessionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/ShutdownInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/SubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/TransactionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/WireFormatInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v5/XATransactionIdMarshallerTest.h
