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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONDATA_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONDATA_H_

#include <activemq/util/Config.h>
#include <activemq/connector/Connector.h>
#include <activemq/transport/Transport.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/Properties.h>

namespace activemq{
namespace core{

    /**
     * Container of the Data that is needed when creating a new connection
     * object.  Each ActiveMQConnection owns one of these objects.  This
     * object knows how to clean up the Connection Dependencies correctly
     */
    class AMQCPP_API ActiveMQConnectionData {
    private:

        // Connector Object
        connector::Connector* connector;

        // Transport we are using
        transport::Transport* transport;

        // Properties used to configure this connection.
        decaf::util::Properties* properties;

    public:

        /**
         * Creates a new Connection Data object, passing it the data that
         * it will manage for the parent connection object.
         * @param connector an connector instance
         * @param transport an <code>Transport</code> instance
         * @param properties an IPoperties instance
         * @throw IllegalArgumentException if an element is NULL
         */
        ActiveMQConnectionData( connector::Connector* connector,
                                transport::Transport* transport,
                                decaf::util::Properties* properties )
        {
            if( connector  == NULL ||
                transport  == NULL ||
                properties == NULL )
            {
                throw decaf::lang::exceptions::IllegalArgumentException(
                    __FILE__, __LINE__,
                    "ActiveMQConnectionData::ActiveMQConnectionData - "
                    "Required Parameter was NULL.");
            }

            this->connector = connector;
            this->transport = transport;
            this->properties = properties;
        }

        virtual ~ActiveMQConnectionData(){
            close();
        }

        virtual void close() throw (exceptions::ActiveMQException) {

            bool hasException = false;
            exceptions::ActiveMQException e;

            try
            {
                if( connector != NULL ){

                    try{
                        connector->close();
                    }catch( exceptions::ActiveMQException& ex ){
                        if( !hasException ){
                            hasException = true;
                            ex.setMark(__FILE__, __LINE__ );
                            e = ex;
                        }
                    }

                    try{
                        delete connector;
                    }catch( exceptions::ActiveMQException& ex ){
                        if( !hasException ){
                            hasException = true;
                            ex.setMark(__FILE__, __LINE__ );
                            e = ex;
                        }
                    }
                    connector = NULL;
                }

                if( transport != NULL ){

                    try{
                        transport->close();
                    }catch( exceptions::ActiveMQException& ex ){
                        if( !hasException ){
                            hasException = true;
                            ex.setMark(__FILE__, __LINE__ );
                            e = ex;
                        }
                    }

                    try{
                        delete transport;
                    }catch( exceptions::ActiveMQException& ex ){
                        if( !hasException ){
                            hasException = true;
                            ex.setMark(__FILE__, __LINE__ );
                            e = ex;
                        }
                    }
                    transport = NULL;
                }

                if( properties != NULL ){
                    delete properties;
                    properties = NULL;
                }

                // If we encountered an exception - throw the first
                // one we encountered.
                if( hasException ){
                    throw e;
                }
            }
            AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
            AMQ_CATCHALL_NOTHROW( )
        }

        /**
         * Get the Connector that this Connection Data object holds
         * @return Connector Pointer
         */
        virtual connector::Connector* getConnector(){
            return connector;
        }

        /**
         * Get the Connector that this Connection Data object holds
         * @return Connector Pointer
         */
        virtual transport::Transport* getTransport(){
            return transport;
        }

        /**
         * Gets a reference to the properties that were used to configure
         * this Connection.
         * @return Properties object reference.
         */
        virtual const decaf::util::Properties& getProperties() const {
            return *properties;
        }

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONDATA_H_*/
