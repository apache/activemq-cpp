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

#ifndef ACTIVEMQ_CMSUTIL_CACHEDPRODUCER_H_
#define ACTIVEMQ_CMSUTIL_CACHEDPRODUCER_H_

#include <cms/MessageProducer.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    /**
     * A cached message producer contained within a pooled session.
     */
    class AMQCPP_API CachedProducer : public cms::MessageProducer {
    private:

        cms::MessageProducer* producer;

    private:

        CachedProducer( const CachedProducer& );
        CachedProducer& operator= ( const CachedProducer& );

    public:

        CachedProducer( cms::MessageProducer* producer ) : producer( producer ) {
        }

        virtual ~CachedProducer() throw() {}

        /**
         * Does nothing - the real producer resource will be closed
         * by the lifecycle manager.
         */
        virtual void close() {
            // Do nothing.
        }

        virtual void send( cms::Message* message ) {
            producer->send( message );
        }

        virtual void send( cms::Message* message, int deliveryMode,
                           int priority, long long timeToLive ) {

            producer->send( message, deliveryMode, priority, timeToLive );
        }


        virtual void send( const cms::Destination* destination,
                           cms::Message* message ) {

            producer->send( destination, message );
        }

        virtual void send( const cms::Destination* destination,
                           cms::Message* message, int deliveryMode,
                           int priority, long long timeToLive ) {

            producer->send( destination, message, deliveryMode, priority, timeToLive );
        }

        virtual void setDeliveryMode( int mode ) {
            producer->setDeliveryMode( mode );
        }

        virtual int getDeliveryMode() const {
            return producer->getDeliveryMode();
        }

        virtual void setDisableMessageID( bool value ) {
            producer->setDisableMessageID( value );
        }

        virtual bool getDisableMessageID() const {
            return producer->getDisableMessageID();
        }

        virtual void setDisableMessageTimeStamp( bool value ) {
            producer->setDisableMessageTimeStamp( value );
        }

        virtual bool getDisableMessageTimeStamp() const {
            return producer->getDisableMessageTimeStamp();
        }

        virtual void setPriority( int priority ) {
            producer->setPriority( priority );
        }

        virtual int getPriority() const {
            return producer->getPriority();
        }

        virtual void setTimeToLive( long long time ) {
            producer->setTimeToLive( time );
        }

        virtual long long getTimeToLive() const {
            return producer->getTimeToLive();
        }

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_CACHEDPRODUCER_H_*/
