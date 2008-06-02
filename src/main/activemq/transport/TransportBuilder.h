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

#ifndef _ACTIVEMQ_TRANSPORT_TRANSPORTBUILDER_H_
#define _ACTIVEMQ_TRANSPORT_TRANSPORTBUILDER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Transport.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace activemq{
namespace transport{

    /**
     * This class controls the creation of transports based on configuration
     * options set in a properties object.
     */
    class AMQCPP_API TransportBuilder {
    public:

        TransportBuilder() {}
        virtual ~TransportBuilder() {}

    public:

        /**
         * Builds a transport from the passed URL, filling in the properties
         * object that its passed with any properties that it finds in the
         * URL.  Any properties that the caller sets before calling this method
         * are preserved.
         * @param url - URL of the Broker
         * @param properties - Properties object to fill
         * @returns pointer to a new Transport, caller owns.
         * @throws CMSException on failure.
         */
        virtual Transport* buildTransport( const std::string& url,
                                           decaf::util::Properties& properties )
                                               throw ( cms::CMSException );

    protected:

        /**
         * Given a Transport Name and the properties it should use to configure
         * itself, create it.  If the name cannot be linked to a transport
         * factory then an exception is thrown.
         * @param name - Name of the Transport to Create
         * @param next - Next Transport in the chain.
         * @param properties - Properties to configure the transport
         * @returns a newly created transport.
         * @throws CMSException if an error occurs during creation.
         */
        virtual Transport* createTransport( const std::string& name,
                                            const decaf::util::Properties& properties,
                                            Transport* next = NULL )
                                                throw ( cms::CMSException );

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_TRANSPORTBUILDER_H_*/
