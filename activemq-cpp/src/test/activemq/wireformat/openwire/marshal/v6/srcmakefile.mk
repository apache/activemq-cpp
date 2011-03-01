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
    activemq/wireformat/openwire/marshal/v6/ActiveMQBlobMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQBytesMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMapMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQObjectMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQStreamMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempQueueMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTextMessageMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTopicMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/BrokerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/BrokerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionErrorMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConnectionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConsumerControlMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConsumerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ConsumerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ControlCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/DataArrayResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/DataResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/DestinationInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/DiscoveryEventMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ExceptionResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/FlushCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/IntegerResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalQueueAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalTopicAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalTraceMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/JournalTransactionMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/KeepAliveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/LastPartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/LocalTransactionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchNotificationMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/MessageIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/MessagePullMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/NetworkBridgeFilterMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/PartialCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ProducerAckMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ProducerIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ProducerInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/RemoveInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/RemoveSubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ReplayCommandMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ResponseMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/SessionIdMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/SessionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/ShutdownInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/SubscriptionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/TransactionInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/WireFormatInfoMarshallerTest.cpp \
    activemq/wireformat/openwire/marshal/v6/XATransactionIdMarshallerTest.cpp

h_sources += \
    activemq/wireformat/openwire/marshal/v6/ActiveMQBlobMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQBytesMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMapMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQObjectMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQStreamMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempQueueMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTempTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTextMessageMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ActiveMQTopicMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/BrokerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/BrokerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionErrorMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConnectionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConsumerControlMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConsumerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ConsumerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ControlCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/DataArrayResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/DataResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/DestinationInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/DiscoveryEventMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ExceptionResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/FlushCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/IntegerResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/JournalQueueAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/JournalTopicAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/JournalTraceMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/JournalTransactionMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/KeepAliveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/LastPartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/LocalTransactionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/MessageAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/MessageDispatchNotificationMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/MessageIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/MessagePullMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/NetworkBridgeFilterMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/PartialCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ProducerAckMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ProducerIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ProducerInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/RemoveInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/RemoveSubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ReplayCommandMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ResponseMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/SessionIdMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/SessionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/ShutdownInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/SubscriptionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/TransactionInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/WireFormatInfoMarshallerTest.h \
    activemq/wireformat/openwire/marshal/v6/XATransactionIdMarshallerTest.h
