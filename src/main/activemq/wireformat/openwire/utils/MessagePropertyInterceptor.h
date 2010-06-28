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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_MESSAGEPROPERTYINTERCEPTOR_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_MESSAGEPROPERTYINTERCEPTOR_H_

#include <activemq/util/Config.h>
#include <activemq/commands/Message.h>
#include <activemq/util/PrimitiveMap.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/exceptions/NullPointerException.h>

namespace activemq {
namespace wireformat {
namespace openwire {
namespace utils {

    /**
     * Used the base ActiveMQMessage class to intercept calls to get and set properties
     * in order to capture the calls that use the reserved JMS properties and get and
     * set them in the OpenWire Message properties.
     *
     * Currently the only properties that are intercepted and handled are:
     *
     * Name                | Conversion Supported
     * ------------------------------------------------------
     * JMSXDeliveryCount   | Int, Long, String
     * JMSXGroupID         | String
     * JMSXGroupSeq        | Int, Long, String
     *
     */
    class AMQCPP_API MessagePropertyInterceptor {
    private:

        commands::Message* message;
        util::PrimitiveMap* properties;

    private:

        MessagePropertyInterceptor( const MessagePropertyInterceptor& );
        MessagePropertyInterceptor& operator= ( const MessagePropertyInterceptor& );

    public:

        /**
         * Constructor, accepts the Message that will be used to store JMS reserved
         * property values, and the PrimitiveMap to get and set the rest to.
         *
         * @param message - The Message to store reserved property data in
         * @param properties - The PrimitiveMap to store the rest of the properties in.
         *
         * @throws NullPointerException if either param is NULL
         */
        MessagePropertyInterceptor( commands::Message* message, util::PrimitiveMap* properties );

        virtual ~MessagePropertyInterceptor() throw();

        /**
         * Gets a boolean property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual bool getBooleanProperty( const std::string& name ) const;

        /**
         * Gets a byte property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual unsigned char getByteProperty( const std::string& name ) const;

        /**
         * Gets a double property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual double getDoubleProperty( const std::string& name ) const;

        /**
         * Gets a float property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual float getFloatProperty( const std::string& name ) const;

        /**
         * Gets a int property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual int getIntProperty( const std::string& name ) const;

        /**
         * Gets a long property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual long long getLongProperty( const std::string& name ) const;

        /**
         * Gets a short property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual short getShortProperty( const std::string& name ) const;

        /**
         * Gets a string property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         */
        virtual std::string getStringProperty( const std::string& name ) const;

        /**
         * Sets a boolean property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setBooleanProperty( const std::string& name, bool value );

        /**
         * Sets a byte property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setByteProperty( const std::string& name, unsigned char value );

        /**
         * Sets a double property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setDoubleProperty( const std::string& name, double value );

        /**
         * Sets a float property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setFloatProperty( const std::string& name, float value );

        /**
         * Sets a int property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setIntProperty( const std::string& name, int value );

        /**
         * Sets a long property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setLongProperty( const std::string& name, long long value );

        /**
         * Sets a short property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setShortProperty( const std::string& name, short value );

        /**
         * Sets a string property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         */
        virtual void setStringProperty( const std::string& name, const std::string& value );

    };

}}}}

#endif /* _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_MESSAGEPROPERTYINTERCEPTOR_H_ */
