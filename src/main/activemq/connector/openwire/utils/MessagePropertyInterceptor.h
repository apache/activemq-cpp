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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_MESSAGEPROPERTYINTERCEPTOR_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_MESSAGEPROPERTYINTERCEPTOR_H_

#include <activemq/util/Config.h>
#include <activemq/connector/openwire/commands/Message.h>
#include <activemq/util/PrimitiveMap.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/exceptions/NullPointerException.h>

namespace activemq {
namespace connector {
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
        MessagePropertyInterceptor( commands::Message* message, util::PrimitiveMap* properties )
            throw( decaf::lang::exceptions::NullPointerException );

        virtual ~MessagePropertyInterceptor();

        /**
         * Gets a boolean property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual bool getBooleanProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a byte property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual unsigned char getByteProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a double property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual double getDoubleProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a float property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual float getFloatProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a int property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual int getIntProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a long property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual long long getLongProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a short property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual short getShortProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Gets a string property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual std::string getStringProperty( const std::string& name ) const
            throw( exceptions::ActiveMQException );

        /**
         * Sets a boolean property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setBooleanProperty( const std::string& name,
                                         bool value )
                                            throw( exceptions::ActiveMQException );
        /**
         * Sets a byte property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setByteProperty( const std::string& name,
                                      unsigned char value )
                                        throw( exceptions::ActiveMQException );

        /**
         * Sets a double property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setDoubleProperty( const std::string& name,
                                        double value )
                                            throw( exceptions::ActiveMQException );

        /**
         * Sets a float property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setFloatProperty( const std::string& name,
                                       float value )
                                        throw( exceptions::ActiveMQException );

        /**
         * Sets a int property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setIntProperty( const std::string& name,
                                     int value )
                                        throw( exceptions::ActiveMQException );

        /**
         * Sets a long property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setLongProperty( const std::string& name,
                                      long long value )
                                        throw( exceptions::ActiveMQException );

        /**
         * Sets a short property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setShortProperty( const std::string& name,
                                       short value )
                                        throw( exceptions::ActiveMQException );

        /**
         * Sets a string property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setStringProperty( const std::string& name,
                                        const std::string& value )
                                            throw( exceptions::ActiveMQException );

    };

}}}}

#endif /* _ACTIVEMQ_CONNECTOR_OPENWIRE_UTILS_MESSAGEPROPERTYINTERCEPTOR_H_ */
