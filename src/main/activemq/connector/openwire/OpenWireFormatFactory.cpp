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

#include <activemq/connector/openwire/OpenWireFormatFactory.h>
#include <activemq/connector/openwire/OpenWireFormat.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::wireformat;
using namespace activemq::connector;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
WireFormat* OpenWireFormatFactory::createWireFormat(
    const decaf::util::Properties& properties )
        throw ( decaf::lang::exceptions::IllegalStateException ) {

    try{

        WireFormatInfo* info = new WireFormatInfo();

        // Configure the version to use
        info->setVersion( Integer::parseInt(
                properties.getProperty( "wireFormat.version", "3" ) ) );

        // parse params out of the properties
        info->setStackTraceEnabled( Boolean::parseBoolean(
            properties.getProperty( "wireFormat.stackTraceEnabled",
                                    "true" ) ) );
        info->setCacheEnabled( Boolean::parseBoolean(
            properties.getProperty( "wireFormat.cacheEnabled",
                                    "false" ) ) );
        info->setCacheSize( Integer::parseInt(
            properties.getProperty( "wireFormat.cacheSize",
                                    "1024" ) ) );
        info->setTcpNoDelayEnabled( Boolean::parseBoolean(
            properties.getProperty( "wireFormat.tcpNoDelayEnabled",
                                    "true" ) ) );
        info->setTightEncodingEnabled( Boolean::parseBoolean(
            properties.getProperty( "wireFormat.tightEncodingEnabled",
                                    "false" ) ) );
        info->setSizePrefixDisabled( Boolean::parseBoolean(
            properties.getProperty( "wireFormat.sizePrefixDisabled",
                                    "false" ) ) );
        info->setMaxInactivityDuration( Long::parseLong(
            properties.getProperty( "wireFormat.MaxInactivityDuration",
                                    "30000" ) ) );
        info->setMaxInactivityDurationInitalDelay( Long::parseLong(
            properties.getProperty( "wireFormat.MaxInactivityDurationInitalDelay",
                                    "10000" ) ) );

        // Create the Openwire Format Object
        OpenWireFormat* f = new OpenWireFormat( properties );

        // give the format object the ownership
        f->setPreferedWireFormatInfo( info );

        return f;
    }
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCHALL_THROW( IllegalStateException )
}
