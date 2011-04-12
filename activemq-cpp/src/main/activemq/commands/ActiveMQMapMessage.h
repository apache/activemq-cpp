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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQMAPMESSAGE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQMAPMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQMessageTemplate.h>
#include <activemq/util/PrimitiveMap.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <cms/MapMessage.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQMapMessage :
        public ActiveMQMessageTemplate<cms::MapMessage> {

    private:

        // Map Structure to hold unmarshaled Map Data
        mutable std::auto_ptr<util::PrimitiveMap> map;

    public:

        const static unsigned char ID_ACTIVEMQMAPMESSAGE = 25;

    private:

        ActiveMQMapMessage( const ActiveMQMapMessage& );
        ActiveMQMapMessage& operator= ( const ActiveMQMapMessage& );

    public:

        ActiveMQMapMessage();

        virtual ~ActiveMQMapMessage() throw();

        virtual unsigned char getDataStructureType() const;

        virtual bool isMarshalAware() const {
            return true;
        }

        virtual ActiveMQMapMessage* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual void beforeMarshal( wireformat::WireFormat* wireFormat );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual void clearBody() throw( cms::CMSException );

    public:   // CMS Message

        virtual cms::MapMessage* clone() const {
            return dynamic_cast<cms::MapMessage*>( this->cloneDataStructure() );
        }

    public:   // CMS MapMessage

        /**
         * {@inheritDoc}
         */
        virtual bool isEmpty() const;

        /**
         * {@inheritDoc}
         */
        virtual std::vector< std::string > getMapNames() const;

        /**
         * {@inheritDoc}
         */
        virtual bool itemExists( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual bool getBoolean( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setBoolean( const std::string& name, bool value );

        /**
         * {@inheritDoc}
         */
        virtual unsigned char getByte( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setByte( const std::string& name, unsigned char value );

        /**
         * {@inheritDoc}
         */
        virtual std::vector<unsigned char> getBytes( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setBytes( const std::string& name, const std::vector<unsigned char>& value );

        /**
         * {@inheritDoc}
         */
        virtual char getChar( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setChar( const std::string& name, char value );

        /**
         * {@inheritDoc}
         */
        virtual double getDouble( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setDouble( const std::string& name, double value );

        /**
         * {@inheritDoc}
         */
        virtual float getFloat( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setFloat( const std::string& name, float value );

        /**
         * {@inheritDoc}
         */
        virtual int getInt( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setInt( const std::string& name, int value );

        /**
         * {@inheritDoc}
         */
        virtual long long getLong( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setLong( const std::string& name, long long value );

        /**
         * {@inheritDoc}
         */
        virtual short getShort( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setShort( const std::string& name, short value );

        /**
         * {@inheritDoc}
         */
        virtual std::string getString( const std::string& name ) const;

        /**
         * {@inheritDoc}
         */
        virtual void setString( const std::string& name, const std::string& value );

    protected:

        /**
         * Fetches a reference to this objects PrimitiveMap, if one needs
         * to be created or unmarshaled, this will perform the correct steps.
         *
         * @returns reference to a PrimtiveMap;
         *
         * @throws NullPointerException if the internal Map is Null.
         */
        util::PrimitiveMap& getMap();
        const util::PrimitiveMap& getMap() const;

        /**
         * Performs the unmarshal on the Map if needed, otherwise just returns
         *
         * @throws NullPointerException if the internal Map is Null.
         */
        virtual void checkMapIsUnmarshalled() const;

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQMAPMESSAGE_H_*/
