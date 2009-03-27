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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYMESSAGE_H_
#define ACTIVEMQ_CMSUTIL_DUMMYMESSAGE_H_

#include <cms/Message.h>

namespace activemq{
namespace cmsutil{
   
    class DummyMessage : public cms::Message
    {         
    public:
   
        virtual ~DummyMessage(){}
      
        virtual Message* clone() const { return NULL; }               
        
        virtual void acknowledge() const throw( cms::CMSException ){}
      
        virtual void clearBody(){}
        
        virtual void clearProperties(){}
  
        virtual std::vector<std::string> getPropertyNames() const {
            return std::vector<std::string>();
        }
        
        virtual bool propertyExists( const std::string& name ) const {
            return false;
        }
        
        virtual bool getBooleanProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return false;
        }
        
        virtual unsigned char getByteProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return (unsigned char)0;
        }
        virtual double getDoubleProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return 0.0;
        }
        
        virtual float getFloatProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return 0.0f;
        }
            
        virtual int getIntProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return 0;
        }
        virtual long long getLongProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return 0LL;
        }
          
        virtual short getShortProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return 0;
        }
            
        virtual std::string getStringProperty( const std::string& name ) const 
            throw( cms::CMSException ) {
            return "";
        }
        
        virtual void setBooleanProperty( const std::string& name,
            bool value ) throw( cms::CMSException ) {}
            
        virtual void setByteProperty( const std::string& name,
            unsigned char value ) throw( cms::CMSException ) {}
            
        virtual void setDoubleProperty( const std::string& name,
            double value ) throw( cms::CMSException ) {}
            
        virtual void setFloatProperty( const std::string& name,
            float value ) throw( cms::CMSException ) {}
            
        
        virtual void setIntProperty( const std::string& name,
            int value ) throw( cms::CMSException ) {}
            
        virtual void setLongProperty( const std::string& name,
            long long value ) throw( cms::CMSException ) {}
            
        virtual void setShortProperty( const std::string& name,
            short value ) throw( cms::CMSException ) {}
            
        virtual void setStringProperty( const std::string& name,
            const std::string& value ) throw( cms::CMSException ) {}
      
        virtual std::string getCMSCorrelationID() const {return "";}

        virtual void setCMSCorrelationID( const std::string& correlationId ) {}

        virtual int getCMSDeliveryMode() const { return 0; }

        virtual void setCMSDeliveryMode( int mode ) {}
      
        virtual const cms::Destination* getCMSDestination() const { return NULL; }
      
        virtual void setCMSDestination( const cms::Destination* destination ) {}
              
        virtual long long getCMSExpiration() const { return 0LL; }
      
        virtual void setCMSExpiration( long long expireTime ) {}
              
        virtual std::string getCMSMessageID() const { return ""; }
        
        virtual void setCMSMessageID( const std::string& id ) {}
      
        virtual int getCMSPriority() const { return 0; }
      
        virtual void setCMSPriority( int priority ) {}

        virtual bool getCMSRedelivered() const { return false; }
      
        virtual void setCMSRedelivered( bool redelivered ) {}

        virtual const cms::Destination* getCMSReplyTo() const { return NULL; }
      
        virtual void setCMSReplyTo( const cms::Destination* destination ) {}
        
        virtual long long getCMSTimestamp() const { return 0LL; }
      
        virtual void setCMSTimestamp( long long timeStamp ) {}

        virtual std::string getCMSType() const { return ""; }
      
        virtual void setCMSType( const std::string& type ) {}
        
    };
}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYMESSAGE_H_*/
