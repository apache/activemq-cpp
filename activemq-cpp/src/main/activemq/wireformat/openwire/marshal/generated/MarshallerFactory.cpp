/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/wireformat/openwire/marshal/generated/MarshallerFactory.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQBlobMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQBytesMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQMapMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQObjectMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQQueueMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQStreamMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQTempQueueMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQTempTopicMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQTextMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ActiveMQTopicMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/BrokerIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/BrokerInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConnectionControlMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConnectionErrorMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConnectionIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConnectionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConsumerControlMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConsumerIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ConsumerInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ControlCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/DataArrayResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/DataResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/DestinationInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/DiscoveryEventMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ExceptionResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/FlushCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/IntegerResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/JournalQueueAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/JournalTopicAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/JournalTraceMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/JournalTransactionMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/KeepAliveInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/LastPartialCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/LocalTransactionIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/MessageAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/MessageDispatchMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/MessageDispatchNotificationMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/MessageIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/MessagePullMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/NetworkBridgeFilterMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/PartialCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ProducerAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ProducerIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ProducerInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/RemoveInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/RemoveSubscriptionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ReplayCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/SessionIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/SessionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/ShutdownInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/SubscriptionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/TransactionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/WireFormatInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/generated/XATransactionIdMarshaller.h>

/*
 *
 *  Command and marshaling code for OpenWire format for MarshallerFactory
 *
 *
 *  NOTE!: This file is autogenerated - do not modify!
 *         if you need to make a change, please see the Java Classes
 *         in the activemq-openwire-generator module
 *
 */

using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace activemq::wireformat::openwire::marshal::generated;

///////////////////////////////////////////////////////////////////////////////
void MarshallerFactory::configure(OpenWireFormat* format) {

    format->addMarshaller(new ActiveMQBlobMessageMarshaller());
    format->addMarshaller(new ActiveMQBytesMessageMarshaller());
    format->addMarshaller(new ActiveMQMapMessageMarshaller());
    format->addMarshaller(new ActiveMQMessageMarshaller());
    format->addMarshaller(new ActiveMQObjectMessageMarshaller());
    format->addMarshaller(new ActiveMQQueueMarshaller());
    format->addMarshaller(new ActiveMQStreamMessageMarshaller());
    format->addMarshaller(new ActiveMQTempQueueMarshaller());
    format->addMarshaller(new ActiveMQTempTopicMarshaller());
    format->addMarshaller(new ActiveMQTextMessageMarshaller());
    format->addMarshaller(new ActiveMQTopicMarshaller());
    format->addMarshaller(new BrokerIdMarshaller());
    format->addMarshaller(new BrokerInfoMarshaller());
    format->addMarshaller(new ConnectionControlMarshaller());
    format->addMarshaller(new ConnectionErrorMarshaller());
    format->addMarshaller(new ConnectionIdMarshaller());
    format->addMarshaller(new ConnectionInfoMarshaller());
    format->addMarshaller(new ConsumerControlMarshaller());
    format->addMarshaller(new ConsumerIdMarshaller());
    format->addMarshaller(new ConsumerInfoMarshaller());
    format->addMarshaller(new ControlCommandMarshaller());
    format->addMarshaller(new DataArrayResponseMarshaller());
    format->addMarshaller(new DataResponseMarshaller());
    format->addMarshaller(new DestinationInfoMarshaller());
    format->addMarshaller(new DiscoveryEventMarshaller());
    format->addMarshaller(new ExceptionResponseMarshaller());
    format->addMarshaller(new FlushCommandMarshaller());
    format->addMarshaller(new IntegerResponseMarshaller());
    format->addMarshaller(new JournalQueueAckMarshaller());
    format->addMarshaller(new JournalTopicAckMarshaller());
    format->addMarshaller(new JournalTraceMarshaller());
    format->addMarshaller(new JournalTransactionMarshaller());
    format->addMarshaller(new KeepAliveInfoMarshaller());
    format->addMarshaller(new LastPartialCommandMarshaller());
    format->addMarshaller(new LocalTransactionIdMarshaller());
    format->addMarshaller(new MessageAckMarshaller());
    format->addMarshaller(new MessageDispatchMarshaller());
    format->addMarshaller(new MessageDispatchNotificationMarshaller());
    format->addMarshaller(new MessageIdMarshaller());
    format->addMarshaller(new MessagePullMarshaller());
    format->addMarshaller(new NetworkBridgeFilterMarshaller());
    format->addMarshaller(new PartialCommandMarshaller());
    format->addMarshaller(new ProducerAckMarshaller());
    format->addMarshaller(new ProducerIdMarshaller());
    format->addMarshaller(new ProducerInfoMarshaller());
    format->addMarshaller(new RemoveInfoMarshaller());
    format->addMarshaller(new RemoveSubscriptionInfoMarshaller());
    format->addMarshaller(new ReplayCommandMarshaller());
    format->addMarshaller(new ResponseMarshaller());
    format->addMarshaller(new SessionIdMarshaller());
    format->addMarshaller(new SessionInfoMarshaller());
    format->addMarshaller(new ShutdownInfoMarshaller());
    format->addMarshaller(new SubscriptionInfoMarshaller());
    format->addMarshaller(new TransactionInfoMarshaller());
    format->addMarshaller(new WireFormatInfoMarshaller());
    format->addMarshaller(new XATransactionIdMarshaller());
}

