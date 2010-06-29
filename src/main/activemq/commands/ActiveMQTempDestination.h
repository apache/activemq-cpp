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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQTEMPDESTINATION_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQTEMPDESTINATION_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <cms/Closeable.h>
#include <vector>
#include <string>

namespace activemq{
namespace core{
    class ActiveMQConnection;
}
namespace commands{

    class AMQCPP_API ActiveMQTempDestination : public ActiveMQDestination,
                                               public cms::Closeable {
    protected:

        /**
         * Connection that we call back on close to allow this resource to
         * be cleaned up correctly at this end and at the Broker End.
         */
        core::ActiveMQConnection* connection;

    public:

        const static unsigned char ID_ACTIVEMQTEMPDESTINATION = 0;

    private:

        ActiveMQTempDestination( const ActiveMQTempDestination& );
        ActiveMQTempDestination& operator= ( const ActiveMQTempDestination& );

    public:

        ActiveMQTempDestination();
        ActiveMQTempDestination( const std::string& name );
        virtual ~ActiveMQTempDestination() throw();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQTempDestination* cloneDataStructure() const {
            return NULL;
        }

        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQDestination::copyDataStructure( src );
        }

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const {
            return ActiveMQDestination::equals( value );
        }

        virtual void close();

        /**
         * Sets the Parent Connection that is notified when this destination is
         * destroyed.
         *
         * @param connection
         *      The parent connection to be used to destroy this destination if closed..
         */
        void setConnection( core::ActiveMQConnection* connection ) {
            this->connection = connection;
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQTEMPDESTINATION_H_*/
