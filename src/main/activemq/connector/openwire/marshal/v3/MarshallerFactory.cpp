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
#include <activemq/connector/openwire/marshal/v3/MarshallerFactory.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQBlobMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQBytesMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQMapMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQObjectMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQQueueMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQStreamMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQTempQueueMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQTempTopicMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQTextMessageMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ActiveMQTopicMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/BrokerIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/BrokerInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConnectionControlMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConnectionErrorMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConnectionIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConnectionInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConsumerControlMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConsumerIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ConsumerInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ControlCommandMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/DataArrayResponseMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/DataResponseMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/DestinationInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/DiscoveryEventMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ExceptionResponseMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/FlushCommandMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/IntegerResponseMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/JournalQueueAckMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/JournalTopicAckMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/JournalTraceMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/JournalTransactionMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/KeepAliveInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/LastPartialCommandMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/LocalTransactionIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/MessageAckMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/MessageDispatchMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/MessageDispatchNotificationMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/MessageIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/MessagePullMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/NetworkBridgeFilterMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/PartialCommandMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ProducerAckMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ProducerIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ProducerInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/RemoveInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/RemoveSubscriptionInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ReplayCommandMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ResponseMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/SessionIdMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/SessionInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/ShutdownInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/SubscriptionInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/TransactionInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/WireFormatInfoMarshaller.h>
#include <activemq/connector/openwire/marshal/v3/XATransactionIdMarshaller.h>

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
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::marshal::v3;

///////////////////////////////////////////////////////////////////////////////
void MarshallerFactory::configure( OpenWireFormat* format ) {

    format->addMarshaller( new ActiveMQBlobMessageMarshaller() );
    format->addMarshaller( new ActiveMQBytesMessageMarshaller() );
    format->addMarshaller( new ActiveMQMapMessageMarshaller() );
    format->addMarshaller( new ActiveMQMessageMarshaller() );
    format->addMarshaller( new ActiveMQObjectMessageMarshaller() );
    format->addMarshaller( new ActiveMQQueueMarshaller() );
    format->addMarshaller( new ActiveMQStreamMessageMarshaller() );
    format->addMarshaller( new ActiveMQTempQueueMarshaller() );
    format->addMarshaller( new ActiveMQTempTopicMarshaller() );
    format->addMarshaller( new ActiveMQTextMessageMarshaller() );
    format->addMarshaller( new ActiveMQTopicMarshaller() );
    format->addMarshaller( new BrokerIdMarshaller() );
    format->addMarshaller( new BrokerInfoMarshaller() );
    format->addMarshaller( new ConnectionControlMarshaller() );
    format->addMarshaller( new ConnectionErrorMarshaller() );
    format->addMarshaller( new ConnectionIdMarshaller() );
    format->addMarshaller( new ConnectionInfoMarshaller() );
    format->addMarshaller( new ConsumerControlMarshaller() );
    format->addMarshaller( new ConsumerIdMarshaller() );
    format->addMarshaller( new ConsumerInfoMarshaller() );
    format->addMarshaller( new ControlCommandMarshaller() );
    format->addMarshaller( new DataArrayResponseMarshaller() );
    format->addMarshaller( new DataResponseMarshaller() );
    format->addMarshaller( new DestinationInfoMarshaller() );
    format->addMarshaller( new DiscoveryEventMarshaller() );
    format->addMarshaller( new ExceptionResponseMarshaller() );
    format->addMarshaller( new FlushCommandMarshaller() );
    format->addMarshaller( new IntegerResponseMarshaller() );
    format->addMarshaller( new JournalQueueAckMarshaller() );
    format->addMarshaller( new JournalTopicAckMarshaller() );
    format->addMarshaller( new JournalTraceMarshaller() );
    format->addMarshaller( new JournalTransactionMarshaller() );
    format->addMarshaller( new KeepAliveInfoMarshaller() );
    format->addMarshaller( new LastPartialCommandMarshaller() );
    format->addMarshaller( new LocalTransactionIdMarshaller() );
    format->addMarshaller( new MessageAckMarshaller() );
    format->addMarshaller( new MessageDispatchMarshaller() );
    format->addMarshaller( new MessageDispatchNotificationMarshaller() );
    format->addMarshaller( new MessageIdMarshaller() );
    format->addMarshaller( new MessagePullMarshaller() );
    format->addMarshaller( new NetworkBridgeFilterMarshaller() );
    format->addMarshaller( new PartialCommandMarshaller() );
    format->addMarshaller( new ProducerAckMarshaller() );
    format->addMarshaller( new ProducerIdMarshaller() );
    format->addMarshaller( new ProducerInfoMarshaller() );
    format->addMarshaller( new RemoveInfoMarshaller() );
    format->addMarshaller( new RemoveSubscriptionInfoMarshaller() );
    format->addMarshaller( new ReplayCommandMarshaller() );
    format->addMarshaller( new ResponseMarshaller() );
    format->addMarshaller( new SessionIdMarshaller() );
    format->addMarshaller( new SessionInfoMarshaller() );
    format->addMarshaller( new ShutdownInfoMarshaller() );
    format->addMarshaller( new SubscriptionInfoMarshaller() );
    format->addMarshaller( new TransactionInfoMarshaller() );
    format->addMarshaller( new WireFormatInfoMarshaller() );
    format->addMarshaller( new XATransactionIdMarshaller() );
}

