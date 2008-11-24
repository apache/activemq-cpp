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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_SUBSCRIBECOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_SUBSCRIBECOMMAND_H_

#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/transport/Command.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Boolean.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Command sent to the broker to subscribe to a topic
     * or queue.
     */
    class AMQCPP_API SubscribeCommand : public AbstractCommand< transport::Command > {
    public:

        SubscribeCommand() :
            AbstractCommand<transport::Command>() {
                initialize( getFrame() );
        }

        SubscribeCommand( StompFrame* frame ) :
            AbstractCommand< transport::Command >( frame ) {
                validate( getFrame() );
        }

        virtual ~SubscribeCommand(void) {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return new SubscribeCommand( getFrame().clone() );
        }

        /**
         * Get the destination
         * @returns the destination Name String
         */
        virtual std::string getDestination() const{
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_DESTINATION ), "" );
        }

        /**
         * Set the destination
         * @param destination the destination Name String
         */
        virtual void setDestination( const std::string& destination ){
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_DESTINATION ),
                destination );
        }

        /**
         * Set the Ack Mode of this Subscription
         * @param mode Ack mode setting.
         */
        virtual void setAckMode( const CommandConstants::AckMode mode ){
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_ACK ),
                CommandConstants::toString( mode ) );
        }

        /**
         * Get the Ack Mode of this Subscription
         * @return mode setting.
         */
        virtual CommandConstants::AckMode getAckMode() const{
            return CommandConstants::toAckMode(
                getPropertyValue(
                    CommandConstants::toString(
                        CommandConstants::HEADER_ACK ) ) );
        }

        /**
         * Sets the Message Selector that is associated with this
         * subscribe request
         * @param selector Destination selector string
         */
        virtual void setMessageSelector( const std::string& selector ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_SELECTOR ),
                selector );
        }

        /**
         * Gets the Message Selector that is associated with this
         * subscribe request
         * @returns the selector string
         */
        virtual std::string getMessageSelector() const{
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_SELECTOR ), "" );
        }

        /**
         * Sets the Subscription Name that is associated with this
         * subscribe request
         * @param name Subscription Name
         */
        virtual void setSubscriptionName( const std::string& name ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_SUBSCRIPTIONNAME ),
                name );
        }

        /**
         * Gets the Subscription Name that is associated with this
         * subscribe request
         * @returns the Subscription Name
         */
        virtual std::string getSubscriptionName() const{
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_SUBSCRIPTIONNAME ), "" );
        }

        /**
         * Gets whether or not locally sent messages should be ignored for
         * subscriptions. Set to true to filter out locally sent messages
         * @return NoLocal value
         */
        virtual bool getNoLocal(void) const {
            return decaf::lang::Boolean::parseBoolean( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_NOLOCAL ),
                "false" ) );
        }

        /**
         * Sets whether or not locally sent messages should be ignored for
         * subscriptions. Set to true to filter out locally sent messages
         * @param noLocal No Local delivery value
         */
        virtual void setNoLocal( bool noLocal ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_NOLOCAL ),
                decaf::lang::Boolean::toString( noLocal ) );
        }

        /**
         * Sets whether or not the broker is to dispatch messages in an
         * asynchronous manner. Set to true if you want Async.
         * @return true if in dispatch async mode
         */
        virtual bool getDispatchAsync() const {
            return decaf::lang::Boolean::parseBoolean( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_DISPATCH_ASYNC ),
                "false" ) );
        }

        /**
         * Sets whether or not the broker is to dispatch messages in an
         * asynchronous manner. Set to true if you want Async.
         * @param dispatchAsync true for async dispatch mode
         */
        virtual void setDispatchAsync( bool dispatchAsync ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_DISPATCH_ASYNC ),
                decaf::lang::Boolean::toString( dispatchAsync ) );
        }

        /**
         * Gets whether or not this consumer is an exclusive consumer for
         * this destination.
         * @return true for exclusive mode
         */
        virtual bool getExclusive() const {
            return decaf::lang::Boolean::parseBoolean( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_EXCLUSIVE ),
                "false" ) );
        }

        /**
         * Sets whether or not this consumer is an exclusive consumer for
         * this destination.
         * @param exclusive true if in exclusive mode
         */
        virtual void setExclusive( bool exclusive ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_EXCLUSIVE ),
                decaf::lang::Boolean::toString( exclusive ) );
        }

        /**
         * Get the max number of pending messages on a destination
         * For Slow Consumer Handlingon non-durable topics by dropping old
         * messages - we can set a maximum pending limit which once a slow
         * consumer backs up to this high water mark we begin to discard
         * old messages
         * @return Max value
         */
        virtual int getMaxPendingMsgLimit() const {
            return decaf::lang::Integer::parseInt( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MAXPENDINGMSGLIMIT ),
                "0" ) );
        }

        /**
         * Set the max number of pending messages on a destination
         * For Slow Consumer Handlingon non-durable topics by dropping old
         * messages - we can set a maximum pending limit which once a slow
         * consumer backs up to this high water mark we begin to discard
         * old messages
         * @param limit Max Pending value
         */
        virtual void setMaxPendingMsgLimit( int limit ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MAXPENDINGMSGLIMIT ),
                decaf::lang::Integer::toString( limit ) );
        }

        /**
         * Get the maximum number of pending messages that will be
         * dispatched to the client. Once this maximum is reached no more
         * messages are dispatched until the client acknowledges a message.
         * Set to 1 for very fair distribution of messages across consumers
         * where processing messages can be slow
         * @return prefetch size value
         */
        virtual int getPrefetchSize() const {
            return decaf::lang::Integer::parseInt( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_PREFETCHSIZE ),
                "1000" ) );
        }

        /**
         * Set the maximum number of pending messages that will be
         * dispatched to the client. Once this maximum is reached no more
         * messages are dispatched until the client acknowledges a message.
         * Set to 1 for very fair distribution of messages across consumers
         * where processing messages can be slow
         * @param size prefetch size value
         */
        virtual void setPrefetchSize( int size ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_PREFETCHSIZE ),
                decaf::lang::Integer::toString( size ) );
        }

        /**
         * Gets the priority of the consumer so that dispatching can be
         * weighted in priority order
         * @return priority level
         */
        virtual int getPriority() const {
            return decaf::lang::Integer::parseInt( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_CONSUMERPRIORITY ),
                "0" ) );
        }

        /**
         * Sets the priority of the consumer so that dispatching can be
         * weighted in priority order
         * @param priority message prioirty level
         */
        virtual void setPriority( int priority ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_CONSUMERPRIORITY ),
                decaf::lang::Integer::toString( priority ) );
        }

        /**
         * Get For non-durable topics if this subscription is set to be
         * retroactive
         * @return true for retroactive mode
         */
        virtual bool getRetroactive() const {
            return decaf::lang::Boolean::parseBoolean( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_RETROACTIVE ),
                "false" ) );
        }

        /**
         * Set For non-durable topics if this subscription is set to be
         * retroactive
         * @param retroactive true if in retroactive mode
         */
        virtual void setRetroactive( bool retroactive ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_RETROACTIVE ),
                decaf::lang::Boolean::toString( retroactive ) );
        }

    protected:

        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame ) {

            frame.setCommand( CommandConstants::toString(
                CommandConstants::SUBSCRIBE ) );

            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_ACK ),
                CommandConstants::toString(
                    CommandConstants::ACK_AUTO ) );
        }

        /**
         * Inheritors are required to override this method to validate
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns frame true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const {

            if((frame.getCommand() ==
                CommandConstants::toString( CommandConstants::SUBSCRIBE )) &&
               (frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_DESTINATION ) ) ) ) {

                return true;
            }

            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_SUBSCRIBECOMMAND_H_*/
