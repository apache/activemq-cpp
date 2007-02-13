/**
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

#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
std::vector<char> WireFormatInfo::MAGIC;

////////////////////////////////////////////////////////////////////////////////
WireFormatInfo::WireFormatInfo()
{
    // Initialize the static MAGIC buffer.
    if( MAGIC.empty() ) {
        MAGIC.push_back( 'A' );
        MAGIC.push_back( 'c' );
        MAGIC.push_back( 't' );
        MAGIC.push_back( 'i' );
        MAGIC.push_back( 'v' );
        MAGIC.push_back( 'e' );
        MAGIC.push_back( 'M' );
        MAGIC.push_back( 'Q' );
    }

    // Initialize member magic buffer to the static buffer.
    magic.resize( MAGIC.size() );
    for( std::size_t ix=0; ix<MAGIC.size(); ++ix ){
        magic[ix] = MAGIC[ix];
    }
}

////////////////////////////////////////////////////////////////////////////////
WireFormatInfo::~WireFormatInfo()
{
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* WireFormatInfo::cloneDataStructure() const {
    WireFormatInfo* wireFormatInfo = new WireFormatInfo();

    // Copy the data from the base class or classes
    wireFormatInfo->copyDataStructure( this );

    return wireFormatInfo;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::copyDataStructure( const DataStructure* src ) {

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure( src );

    const WireFormatInfo* srcPtr = dynamic_cast<const WireFormatInfo*>( src );

    if( srcPtr == NULL || src == NULL ) {

        throw exceptions::NullPointerException(
            __FILE__, __LINE__,
            "WireFormatInfo::copyDataStructure - src is NULL or invalid" );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char WireFormatInfo::getDataStructureType() const {
    return WireFormatInfo::ID_WIREFORMATINFO;
}

////////////////////////////////////////////////////////////////////////////////
std::string WireFormatInfo::toString() const {
    std::ostringstream stream;

    stream << "Begin Class = WireFormatInfo" << std::endl;

    std::vector<unsigned char> magic;
    std::vector<unsigned char> marshalledProperties;

    for( size_t imagic = 0; imagic < magic.size(); ++imagic ) {
        stream << " Value of magic[" << imagic << "] = " << magic[imagic] << std::endl;
    }

    for( size_t imarshalledProperties = 0; imarshalledProperties < marshalledProperties.size(); ++imarshalledProperties ) {
        stream << " Value of marshalledProperties[" << imarshalledProperties << "] = "
               << marshalledProperties[imarshalledProperties] << std::endl;
    }

    stream << " Value of properties = " << properties.toString() << std::endl;
    stream << " Value of version = " << version << std::endl;
    stream << " Value of stackTraceEnabled = " << stackTraceEnabled << std::endl;
    stream << " Value of tcpNoDelayEnabled = " << tcpNoDelayEnabled << std::endl;
    stream << " Value of cacheEnabled = " << cacheEnabled << std::endl;
    stream << " Value of tightEncodingEnabled = " << tightEncodingEnabled << std::endl;
    stream << " Value of sizePrefixDisabled = " << sizePrefixDisabled << std::endl;
    stream << " Value of maxInactivityDuration = " << maxInactivityDuration << std::endl;

    stream << BaseCommand<transport::Command>::toString();
    stream << "End Class = WireFormatInfo" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::equals( const DataStructure* value ) const {

    const WireFormatInfo* wireFormatInfo =
        dynamic_cast<const WireFormatInfo*>( value );

    if( value == NULL || wireFormatInfo == NULL ) {
        return false;
    }

    if( magic != wireFormatInfo->getMagic() ) {
        return false;
    }
    if( marshalledProperties != wireFormatInfo->getMarshalledProperties() ) {
        return false;
    }
    if( !properties.equals( wireFormatInfo->properties ) ) {
        return false;
    }
    if( version != wireFormatInfo->getVersion() ) {
        return false;
    }
    if( stackTraceEnabled != wireFormatInfo->isStackTraceEnabled() ) {
        return false;
    }
    if( tcpNoDelayEnabled != wireFormatInfo->isTcpNoDelayEnabled() ) {
        return false;
    }
    if( cacheEnabled != wireFormatInfo->isCacheEnabled() ) {
        return false;
    }
    if( tightEncodingEnabled != wireFormatInfo->isTightEncodingEnabled() ) {
        return false;
    }
    if( sizePrefixDisabled != wireFormatInfo->isSizePrefixDisabled() ) {
        return false;
    }
    if( maxInactivityDuration != wireFormatInfo->getMaxInactivityDuration() ) {
        return false;
    }
    if( !BaseCommand<transport::Command>::equals( value ) ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isValid() const {

    if( magic.size() != MAGIC.size() ) {
        return false;
    }

    for( size_t i = 0; i < magic.size(); i++ ) {
        if( magic[i] != MAGIC[i] ) {
            return false;
        }
    }

    return true;
}
