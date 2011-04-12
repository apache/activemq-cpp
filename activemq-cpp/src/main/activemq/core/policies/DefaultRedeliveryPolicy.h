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

#ifndef _ACTIVEMQ_CORE_POLICIES_DEFAULTREDELIVERYPOLICY_H_
#define _ACTIVEMQ_CORE_POLICIES_DEFAULTREDELIVERYPOLICY_H_

#include <activemq/util/Config.h>

#include <activemq/core/RedeliveryPolicy.h>

namespace activemq {
namespace core {
namespace policies {

    class AMQCPP_API DefaultRedeliveryPolicy : public RedeliveryPolicy {
    private:

        double backOffMultiplier;
        double collisionAvoidanceFactor;
        long long initialRedeliveryDelay;
        int maximumRedeliveries;
        bool useCollisionAvoidance;
        bool useExponentialBackOff;
        long long redeliveryDelay;

    private:

        DefaultRedeliveryPolicy( const DefaultRedeliveryPolicy& );
        DefaultRedeliveryPolicy& operator= ( const DefaultRedeliveryPolicy& );

    public:

        DefaultRedeliveryPolicy();

        virtual ~DefaultRedeliveryPolicy();

        virtual double getBackOffMultiplier() const {
            return this->backOffMultiplier;
        }

        virtual void setBackOffMultiplier( double value ) {
            this->backOffMultiplier = value;
        }

        virtual short getCollisionAvoidancePercent() const;

        virtual void setCollisionAvoidancePercent( short value );

        virtual long long getInitialRedeliveryDelay() const {
            return this->initialRedeliveryDelay;
        }

        virtual void setInitialRedeliveryDelay( long long value ) {
            this->initialRedeliveryDelay = value;
        }

        virtual long long getRedeliveryDelay() const {
            return this->redeliveryDelay;
        }

        virtual void setRedeliveryDelay( long long value ) {
            this->redeliveryDelay = value;
        }

        virtual int getMaximumRedeliveries() const {
            return this->maximumRedeliveries;
        }

        virtual void setMaximumRedeliveries( int value ) {
            this->maximumRedeliveries = value;
        }

        virtual bool isUseCollisionAvoidance() const {
            return this->useCollisionAvoidance;
        }

        virtual void setUseCollisionAvoidance( bool value ) {
            this->useCollisionAvoidance = value;
        }

        virtual bool isUseExponentialBackOff() const {
            return this->useExponentialBackOff;
        }

        virtual void setUseExponentialBackOff( bool value ) {
            this->useExponentialBackOff = value;
        }

        virtual long long getNextRedeliveryDelay( long long previousDelay );

        virtual RedeliveryPolicy* clone() const;

    };

}}}

#endif /* _ACTIVEMQ_CORE_POLICIES_DEFAULTREDELIVERYPOLICY_H_ */
