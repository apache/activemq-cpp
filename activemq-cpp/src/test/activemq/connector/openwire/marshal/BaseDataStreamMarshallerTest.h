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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLERTEST_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/connector/openwire/commands/DataStructure.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace marshal{
    
    class BaseDataStreamMarshallerTest : public CppUnit::TestFixture {
        
        CPPUNIT_TEST_SUITE( BaseDataStreamMarshallerTest );
        CPPUNIT_TEST( testLooseMarshal );
        CPPUNIT_TEST_SUITE_END();                 
        
    public:
    
        class SimpleDataStructure : public commands::BaseDataStructure {
            
        public:
        
            bool boolValue;
            char charValue;
            short shortValue;
            int intValue;
            long long longValue;
            float floatValue;
            double doubleValue;
            std::string stringValue;
                        
        public:
        
            const static unsigned char TYPE = 0xFF;
            
            virtual ~SimpleDataStructure(){}
            
            virtual unsigned char getDataStructureType() const {
                return TYPE;
            }
            
            virtual DataStructure* cloneDataStructure() const {
                SimpleDataStructure* s = new SimpleDataStructure();
                s->copyDataStructure( this );
                return s;
            }

            virtual void copyDataStructure( const DataStructure* src ){
                const SimpleDataStructure* srcObj = dynamic_cast<const SimpleDataStructure*>(src);
                if( srcObj == NULL ){
                    return;
                }
                boolValue = srcObj->boolValue;
                charValue = srcObj->charValue;
                shortValue = srcObj->shortValue;
                intValue = srcObj->intValue;
                longValue = srcObj->longValue;
                floatValue = srcObj->floatValue;
                doubleValue = srcObj->doubleValue;
                stringValue = srcObj->stringValue;
            }
        };
        
        class SimpleDataStructureMarshaller : public BaseDataStreamMarshaller{
        public:
        
            virtual ~SimpleDataStructureMarshaller(){}
         
            virtual commands::DataStructure* createObject() const {
                return new SimpleDataStructure();
            }
            
            virtual unsigned char getDataStructureType() const {
                return SimpleDataStructure::TYPE;
            }
            
            virtual void tightUnmarshal( OpenWireFormat* wireFormat,
                                     commands::DataStructure* dataStructure,
                                     io::DataInputStream* dataIn,
                                     utils::BooleanStream* bs ) throw( io::IOException ) 
             {
                BaseDataStreamMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

                SimpleDataStructure* info =
                    dynamic_cast<SimpleDataStructure*>( dataStructure );
                    
                info->boolValue = bs->readBoolean();
                info->charValue = dataIn->readChar();
                info->shortValue = dataIn->readShort();
                info->intValue = dataIn->readInt();
                info->longValue = tightUnmarshalLong( wireFormat, dataIn, bs );
                info->floatValue = dataIn->readFloat();
                info->doubleValue = dataIn->readDouble();
                info->stringValue = tightUnmarshalString( dataIn, bs );
             }

            virtual int tightMarshal1( OpenWireFormat* wireFormat,
                                       commands::DataStructure* dataStructure,
                                       utils::BooleanStream* bs ) throw( io::IOException )
            {
                SimpleDataStructure* info =
                    dynamic_cast<SimpleDataStructure*>( dataStructure );
            
                int rc = BaseDataStreamMarshaller::tightMarshal1( wireFormat, dataStructure, bs );
                bs->writeBoolean( info->boolValue );
                rc += tightMarshalLong1( wireFormat, info->longValue, bs );
                rc += tightMarshalString1( info->stringValue, bs );
            
                return 19 + rc;
            }
    
            virtual void tightMarshal2( OpenWireFormat* wireFormat,
                                        commands::DataStructure* dataStructure,
                                        io::DataOutputStream* dataOut,
                                        utils::BooleanStream* bs ) throw( io::IOException )
            {
                BaseDataStreamMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

                SimpleDataStructure* info =
                    dynamic_cast<SimpleDataStructure*>( dataStructure );
                    
                bs->readBoolean();
                dataOut->writeChar( info->charValue );
                dataOut->writeShort( info->shortValue );
                dataOut->writeInt( info->intValue );                
                tightMarshalLong2(wireFormat, info->longValue, dataOut, bs);
                dataOut->writeFloat( info->floatValue );
                dataOut->writeDouble( info->doubleValue );
                tightMarshalString2( info->stringValue, dataOut, bs );
                
            }
    
            virtual void looseUnmarshal( OpenWireFormat* wireFormat,
                                         commands::DataStructure* dataStructure,
                                         io::DataInputStream* dataIn ) throw( io::IOException )
            {
                BaseDataStreamMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
                SimpleDataStructure* info =
                    dynamic_cast<SimpleDataStructure*>( dataStructure );
                    
                info->boolValue = dataIn->readBoolean();
                info->charValue = dataIn->readChar();
                info->shortValue = dataIn->readShort();
                info->intValue = dataIn->readInt();
                info->longValue = looseUnmarshalLong( wireFormat, dataIn );
                info->floatValue = dataIn->readFloat();
                info->doubleValue = dataIn->readDouble();
                info->stringValue = looseUnmarshalString( dataIn );
            }
    
            virtual void looseMarshal( OpenWireFormat* wireFormat,
                                       commands::DataStructure* dataStructure,
                                       io::DataOutputStream* dataOut ) throw( io::IOException )
            {
                SimpleDataStructure* info =
                    dynamic_cast<SimpleDataStructure*>( dataStructure );
                BaseDataStreamMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );
    
                dataOut->writeBoolean( info->boolValue );
                dataOut->writeChar( info->charValue );
                dataOut->writeShort( info->shortValue );
                dataOut->writeInt( info->intValue );
                looseMarshalLong( wireFormat, info->longValue, dataOut );
                dataOut->writeFloat( info->floatValue );
                dataOut->writeDouble( info->doubleValue );
                looseMarshalString( info->stringValue, dataOut );
            }
        };
        
        class ComplexDataStructure : public commands::BaseDataStructure {
            
        public:
        
            bool boolValue;
            SimpleDataStructure* cachedChild;
                        
        public:
        
            const static unsigned char TYPE = 0xFE;
            
            ComplexDataStructure(){
                cachedChild = NULL;
            }
            virtual ~ComplexDataStructure(){
                setCachedChild( NULL );
            }
            
            void setCachedChild( SimpleDataStructure* child ) {
                if( cachedChild != NULL ) {
                    delete cachedChild;
                    cachedChild = NULL;
                }
                
                if( child != NULL ) {
                    cachedChild = child;
                }
            }
            
            virtual unsigned char getDataStructureType() const {
                return TYPE;
            }
            
            virtual DataStructure* cloneDataStructure() const {
                ComplexDataStructure* s = new ComplexDataStructure();
                s->copyDataStructure( this );
                return s;
            }

            virtual void copyDataStructure( const DataStructure* src ){
                const ComplexDataStructure* srcObj = dynamic_cast<const ComplexDataStructure*>(src);
                if( srcObj == NULL ){
                    return;
                }
                boolValue = srcObj->boolValue;
                
                if( cachedChild != NULL ) {
                    delete cachedChild;
                    cachedChild = NULL;
                }
                
                if( srcObj->cachedChild != NULL ) {
                    cachedChild = dynamic_cast<SimpleDataStructure*>(srcObj->cachedChild->cloneDataStructure());
                }             
            }
        };
        
        class ComplexDataStructureMarshaller : public BaseDataStreamMarshaller{
        public:
        
            virtual ~ComplexDataStructureMarshaller(){}
         
            virtual commands::DataStructure* createObject() const {
                return new ComplexDataStructure();
            }
            
            virtual unsigned char getDataStructureType() const {
                return ComplexDataStructure::TYPE;
            }
            
            virtual void tightUnmarshal( OpenWireFormat* wireFormat,
                                     commands::DataStructure* dataStructure,
                                     io::DataInputStream* dataIn,
                                     utils::BooleanStream* bs ) throw( io::IOException ) 
             {
                BaseDataStreamMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

                ComplexDataStructure* info =
                    dynamic_cast<ComplexDataStructure*>( dataStructure );
                    
                info->boolValue = bs->readBoolean();
                info->setCachedChild( dynamic_cast< SimpleDataStructure* >(
                    tightUnmarshalCachedObject( wireFormat, dataIn, bs ) ) );
             }

            virtual int tightMarshal1( OpenWireFormat* wireFormat,
                                       commands::DataStructure* dataStructure,
                                       utils::BooleanStream* bs ) throw( io::IOException )
            {
                ComplexDataStructure* info =
                    dynamic_cast<ComplexDataStructure*>( dataStructure );
            
                int rc = BaseDataStreamMarshaller::tightMarshal1( wireFormat, dataStructure, bs );
                bs->writeBoolean( info->boolValue );
                rc += tightMarshalCachedObject1( wireFormat, info->cachedChild, bs );
            
                return rc;
            }
    
            virtual void tightMarshal2( OpenWireFormat* wireFormat,
                                        commands::DataStructure* dataStructure,
                                        io::DataOutputStream* dataOut,
                                        utils::BooleanStream* bs ) throw( io::IOException )
            {
                BaseDataStreamMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

                ComplexDataStructure* info =
                    dynamic_cast<ComplexDataStructure*>( dataStructure );
                    
                bs->readBoolean();
                tightMarshalCachedObject2( wireFormat, info->cachedChild, dataOut, bs );
                
            }
    
            virtual void looseUnmarshal( OpenWireFormat* wireFormat,
                                         commands::DataStructure* dataStructure,
                                         io::DataInputStream* dataIn ) throw( io::IOException )
            {
                BaseDataStreamMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
                ComplexDataStructure* info =
                    dynamic_cast<ComplexDataStructure*>( dataStructure );
                    
                info->boolValue = dataIn->readBoolean();
                info->setCachedChild( dynamic_cast< SimpleDataStructure* >( 
                    looseUnmarshalCachedObject( wireFormat, dataIn ) ) );
            }
    
            virtual void looseMarshal( OpenWireFormat* wireFormat,
                                       commands::DataStructure* dataStructure,
                                       io::DataOutputStream* dataOut ) throw( io::IOException )
            {
                ComplexDataStructure* info =
                    dynamic_cast<ComplexDataStructure*>( dataStructure );
                BaseDataStreamMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );
    
                dataOut->writeBoolean( info->boolValue );
                looseMarshalCachedObject( wireFormat, info->cachedChild, dataOut );
            }
        };
        
    private:
    
        ComplexDataStructure* dataStructure;

    public:
    
        BaseDataStreamMarshallerTest() {}
        virtual ~BaseDataStreamMarshallerTest() {}
        
        virtual void setUp(){
            dataStructure = new ComplexDataStructure();
            dataStructure->boolValue = true;
            dataStructure->setCachedChild( new SimpleDataStructure() );
            dataStructure->cachedChild->boolValue = true;
            dataStructure->cachedChild->charValue = 'a';
            dataStructure->cachedChild->shortValue = 1000;
            dataStructure->cachedChild->intValue = 100000;
            dataStructure->cachedChild->longValue = 1000000;
            dataStructure->cachedChild->floatValue = 10.3f;
            dataStructure->cachedChild->doubleValue = 20.1;
            dataStructure->cachedChild->stringValue = "hello world";
        } 
        
        virtual void tearDown(){
            
            if( dataStructure != NULL ) {
                delete dataStructure;
                dataStructure = NULL;
            }
        }
    
        void testLooseMarshal();
        
    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_BASEDATASTREAMMARSHALLERTEST_H_*/
