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

#ifndef _ACTIVEMQ_CORE_REDELIVERYPOLICY_H_
#define _ACTIVEMQ_CORE_REDELIVERYPOLICY_H_

#include <activemq/util/Config.h>

#include <decaf/util/Properties.h>

namespace activemq {
namespace core {

    /**
     * Interface for a RedeliveryPolicy object that controls how message Redelivery is
     * handled in ActiveMQ-CPP when a transaction is rolled back.
     *
     * @since 3.2.0
     */
    class AMQCPP_API RedeliveryPolicy {
    public:

        static const long long NO_MAXIMUM_REDELIVERIES;

    private:

        RedeliveryPolicy( const RedeliveryPolicy& );
        RedeliveryPolicy& operator= ( const RedeliveryPolicy& );

    protected:

        RedeliveryPolicy();

    public:

        virtual ~RedeliveryPolicy();

        /**
         * @returns The value of the Back-Off Multiplier for Message Redelivery.
         */
        virtual double getBackOffMultiplier() const = 0;

        /**
         * Sets the Back-Off Multiplier for Message Redelivery.
         *
         * @param value
         *      The new value for the back-off multiplier.
         */
        virtual void setBackOffMultiplier( double value ) = 0;

        /**
         * @returns the currently set Collision Avoidance percentage.
         */
        virtual short getCollisionAvoidancePercent() const = 0;

        /**
         * @param value
         *      The collision avoidance percentage setting.
         */
        virtual void setCollisionAvoidancePercent( short value ) = 0;

        /**
         * Gets the initial time that redelivery of messages is delayed.
         *
         * @returns the time in milliseconds that redelivery is delayed initially.
         */
        virtual long long getInitialRedeliveryDelay() const = 0;

        /**
         * Sets the initial time that redelivery will be delayed.
         *
         * @param value
         *      Time in Milliseconds to wait before starting redelivery.
         */
        virtual void setInitialRedeliveryDelay( long long value ) = 0;

        /**
         * Gets the time that redelivery of messages is delayed.
         *
         * @returns the time in milliseconds that redelivery is delayed.
         */
        virtual long long getRedeliveryDelay() const = 0;

        /**
         * Sets the time that redelivery will be delayed.
         *
         * @param value
         *      Time in Milliseconds to wait before the next redelivery.
         */
        virtual void setRedeliveryDelay( long long value ) = 0;

        /**
         * Gets the Maximum number of allowed redeliveries for a message before it will
         * be discarded by the consumer.
         *
         * @returns maximum allowed redeliveries for a message.
         */
        virtual int getMaximumRedeliveries() const = 0;

        /**
         * Sets the Maximum allowable redeliveries for a Message.
         *
         * @param maximumRedeliveries
         *      The maximum number of times that a message will be redelivered.
         */
        virtual void setMaximumRedeliveries( int maximumRedeliveries ) = 0;

        /**
         * Given the last used redelivery delay calculate the next value of the delay
         * based on the settings in this Policy instance.
         *
         * @param previousDelay
         *      The last delay that was used between message redeliveries.
         *
         * @return the new delay to use before attempting another redelivery.
         */
        virtual long long getNextRedeliveryDelay( long long previousDelay ) = 0;

        /**
         * @returns whether or not collision avoidance is enabled for this Policy.
         */
        virtual bool isUseCollisionAvoidance() const = 0;

        /**
         * @param value
         *      Enable or Disable collision avoidance for this Policy.
         */
        virtual void setUseCollisionAvoidance( bool value ) = 0;

        /**
         * @returns whether or not the exponential back off option is enabled.
         */
        virtual bool isUseExponentialBackOff() const = 0;

        /**
         * @param value
         *      Enable or Disable the exponential back off multiplier option.
         */
        virtual void setUseExponentialBackOff( bool value ) = 0;

        /**
         * Create a copy of this Policy and return it.
         *
         * @return pointer to a new RedeliveryPolicy that is a copy of this one.
         */
        virtual RedeliveryPolicy* clone() const = 0;

        /**
         * Checks the supplied properties object for properties matching the configurable
         * settings of this class.  The default implementation looks for properties named
         * with the prefix cms.RedeliveryPolicy.XXX where XXX is the name of a property with
         * a public setter method.  For instance cms.RedeliveryPolicy.useExponentialBackOff
         * will be used to set the value of the use exponential back off toggle.
         *
         * Subclasses can override this method to add more configuration options or to exclude
         * certain parameters from being set via the properties object.
         *
         * @param properties
         *      The Properties object used to configure this object.
         *
         * @throws NumberFormatException if a property that is numeric cannot be converted
         * @throws IllegalArgumentException if a property can't be converted to the correct type.
         */
        virtual void configure( const decaf::util::Properties& properties );

    };

}}

#endif /* _ACTIVEMQ_CORE_REDELIVERYPOLICY_H_ */
