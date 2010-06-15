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
#include <activemq/wireformat/openwire/marshal/universal/MarshallerFactory.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQBlobMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQBytesMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQMapMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQObjectMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQQueueMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQStreamMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQTempQueueMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQTempTopicMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQTextMessageMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ActiveMQTopicMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/BrokerIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/BrokerInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConnectionControlMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConnectionErrorMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConnectionIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConnectionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConsumerControlMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConsumerIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ConsumerInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ControlCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/DataArrayResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/DataResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/DestinationInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/DiscoveryEventMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ExceptionResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/FlushCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/IntegerResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/JournalQueueAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/JournalTopicAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/JournalTraceMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/JournalTransactionMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/KeepAliveInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/LastPartialCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/LocalTransactionIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/MessageAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/MessageDispatchMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/MessageDispatchNotificationMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/MessageIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/MessagePullMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/NetworkBridgeFilterMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/PartialCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ProducerAckMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ProducerIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ProducerInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/RemoveInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/RemoveSubscriptionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ReplayCommandMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ResponseMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/SessionIdMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/SessionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/ShutdownInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/SubscriptionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/TransactionInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/WireFormatInfoMarshaller.h>
#include <activemq/wireformat/openwire/marshal/universal/XATransactionIdMarshaller.h>

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
using namespace activemq::wireformat::openwire::marshal::universal;

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

