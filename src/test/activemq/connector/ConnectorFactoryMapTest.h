/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ACTIVEMQ_CONNECTOR_CONNECTORFACTORYMAPTEST_H_
#define ACTIVEMQ_CONNECTOR_CONNECTORFACTORYMAPTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/ConnectorFactoryMap.h>
#include <activemq/connector/SessionInfo.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/util/Config.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>

namespace activemq{
namespace connector{
	
	class ConnectorFactoryMapTest : public CppUnit::TestFixture {
		
	  CPPUNIT_TEST_SUITE( ConnectorFactoryMapTest );
	  CPPUNIT_TEST( test );
	  CPPUNIT_TEST_SUITE_END();
	  
	public:
	
		class testConnector : public Connector
		{
		public:
		
		      virtual SessionInfo* createSessionInfo(void) throw( ConnectorException )
		      { return NULL; };
		
		      virtual cms::Topic* createTopic(const std::string& name AMQCPP_UNUSED, SessionInfo* session AMQCPP_UNUSED)
		          throw ( ConnectorException )
		      { return NULL; };
		      virtual cms::Queue* createQueue(const std::string& name AMQCPP_UNUSED, SessionInfo* session AMQCPP_UNUSED)
		         throw ( ConnectorException )
		      { return NULL; };
		          
		      virtual cms::TemporaryTopic* createTemporaryTopic(const std::string& name AMQCPP_UNUSED, 
		                                                    SessionInfo*       session AMQCPP_UNUSED)
		         throw ( ConnectorException )
		     { return NULL; };
		         
		       virtual cms::TemporaryQueue* createTemporaryQueue(const std::string& name AMQCPP_UNUSED, 
		                                                    SessionInfo*       session AMQCPP_UNUSED)
		          throw ( ConnectorException )
		       { return NULL; };
		          
		      virtual void Send(cms::Message* message AMQCPP_UNUSED) throw ( ConnectorException ) {};
		      virtual void Send(std::list<cms::Message*>& messages AMQCPP_UNUSED) 
		         throw ( ConnectorException ) {};
            virtual void Acknowledge(cms::Message* message AMQCPP_UNUSED) throw ( ConnectorException ) {};
		      virtual TransactionInfo* startTransaction(SessionInfo* session AMQCPP_UNUSED) 
		         throw ( ConnectorException ) { return NULL; };
		      virtual void commit(TransactionInfo* transaction AMQCPP_UNUSED, SessionInfo* session AMQCPP_UNUSED)
		         throw ( ConnectorException ) {};
		      virtual void rollback(TransactionInfo* transaction AMQCPP_UNUSED, SessionInfo* session AMQCPP_UNUSED)
		         throw ( ConnectorException ) {};
		
		      virtual cms::BytesMessage* createByteMessage(SessionInfo*     session AMQCPP_UNUSED,
		                                             TransactionInfo* transaction AMQCPP_UNUSED)
		                                               throw ( ConnectorException )
		      { return NULL; };
		      virtual cms::TextMessage* createTextMessage(SessionInfo*     session AMQCPP_UNUSED,
		                                             TransactionInfo* transaction AMQCPP_UNUSED)
		                                               throw ( ConnectorException )
		      { return NULL; };
		      virtual void subscribe(cms::Destination* destination AMQCPP_UNUSED, SessionInfo* session AMQCPP_UNUSED)
		         throw ( ConnectorException ) {};
		      virtual void unsubscribe(cms::Destination* destination AMQCPP_UNUSED, SessionInfo* session AMQCPP_UNUSED)
		         throw ( ConnectorException ) {};
		      virtual void addMessageListener(cms::MessageListener* listener AMQCPP_UNUSED) {};
		      virtual void removeMessageListener(cms::MessageListener* listener AMQCPP_UNUSED) {};
            virtual void addExceptionListener(cms::ExceptionListener* listener AMQCPP_UNUSED) {};
            virtual void removeExceptionListener(cms::ExceptionListener* listener AMQCPP_UNUSED) {};
		
		};
		
	public:
    
        virtual ~ConnectorFactoryMapTest(){}
	
		class TestConnectoryFactory : public ConnectorFactory
		{
		public:
		
		   virtual Connector* createConnector(
		      const decaf::util::Properties& properties AMQCPP_UNUSED,
            activemq::transport::Transport*   transport AMQCPP_UNUSED) { return NULL; };
		};
		
		void test(){
			
			ConnectorFactoryMap* factMap = ConnectorFactoryMap::getInstance();
			CPPUNIT_ASSERT( factMap != NULL );
			
			TestConnectoryFactory testFactory;
			
			factMap->registerConnectorFactory( "test", &testFactory );
			
			CPPUNIT_ASSERT( factMap->lookup( "test" ) == &testFactory );
			
			std::vector<std::string> names;
			CPPUNIT_ASSERT( factMap->getFactoryNames( names ) >= 1 );
			
            bool found = false;
            for( unsigned int i = 0; i < names.size(); ++i )
            {
                if( names[i] == "test" )
                {
                    found = true;
                    break;
                }
            }
			CPPUNIT_ASSERT( found );
			
			factMap->unregisterConnectorFactory( "test" );
			CPPUNIT_ASSERT( factMap->lookup( "test" ) == NULL );			
		}
	};
	
}}

#endif /*ACTIVEMQ_CONNECTOR_CONNECTORFACTORYMAPTEST_H_*/
