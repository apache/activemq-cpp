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
 
#include <activemq/connector/openwire/OpenwireFormatFactory.h>
#include <activemq/connector/openwire/OpenwireFormat.h>

using namespace std;
using namespace activemq;
using namespace activemq::wireformat;
using namespace activemq::connector;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
WireFormat* OpenWireFormatFactory::createWireFormat( 
    const util::Properties& properties )
        throw ( exceptions::IllegalStateException ) {

    WireFormatInfo* info = new WireFormatInfo();
    
    // Configure the version to use
    info->setVersion( 2 );
    
    // Create the Openwire Format Object
    OpenWireFormat* f = new OpenWireFormat( properties );
    
    // give the format object the ownership
    f->setPreferedWireFormatInfo( info );

    return f;
}

