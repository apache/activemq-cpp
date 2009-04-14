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
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
WireFormatInfo::WireFormatInfo()
{
    // Init to our prefered version
    this->version = 2;

    // Initialize the MAGIC buffer.
    magic.push_back( 'A' );
    magic.push_back( 'c' );
    magic.push_back( 't' );
    magic.push_back( 'i' );
    magic.push_back( 'v' );
    magic.push_back( 'e' );
    magic.push_back( 'M' );
    magic.push_back( 'Q' );
}

////////////////////////////////////////////////////////////////////////////////
WireFormatInfo::~WireFormatInfo() {
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
        throw NullPointerException(
            __FILE__, __LINE__,
            "WireFormatInfo::copyDataStructure - src is NULL or invalid" );
    }

    this->properties.copy( srcPtr->getProperties() );
    this->version = srcPtr->getVersion();
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
    stream << " Value of stackTraceEnabled = " << isStackTraceEnabled() << std::endl;
    stream << " Value of tcpNoDelayEnabled = " << isTcpNoDelayEnabled() << std::endl;
    stream << " Value of cacheEnabled = " << isCacheEnabled() << std::endl;
    stream << " Value of cacheSize = " << getCacheSize() << std::endl;
    stream << " Value of tightEncodingEnabled = " << isTightEncodingEnabled() << std::endl;
    stream << " Value of sizePrefixDisabled = " << isSizePrefixDisabled() << std::endl;
    stream << " Value of maxInactivityDuration = " << getMaxInactivityDuration() << std::endl;
    stream << " Value of maxInactivityDuration = " << getMaxInactivityDurationInitalDelay() << std::endl;

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
    if( isStackTraceEnabled() != wireFormatInfo->isStackTraceEnabled() ) {
        return false;
    }
    if( isTcpNoDelayEnabled() != wireFormatInfo->isTcpNoDelayEnabled() ) {
        return false;
    }
    if( isCacheEnabled() != wireFormatInfo->isCacheEnabled() ) {
        return false;
    }
    if( getCacheSize() != wireFormatInfo->getCacheSize() ) {
        return false;
    }
    if( isTightEncodingEnabled() != wireFormatInfo->isTightEncodingEnabled() ) {
        return false;
    }
    if( isSizePrefixDisabled() != wireFormatInfo->isSizePrefixDisabled() ) {
        return false;
    }
    if( getMaxInactivityDuration() != wireFormatInfo->getMaxInactivityDuration() ) {
        return false;
    }
    if( getMaxInactivityDurationInitalDelay() != wireFormatInfo->getMaxInactivityDurationInitalDelay() ) {
        return false;
    }
    if( !BaseCommand<transport::Command>::equals( value ) ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isValid() const {

    if( magic.size() != std::string( "ActiveMQ" ).size() ) {
        return false;
    }

    if( magic[0] != 'A' ||
        magic[1] != 'c' ||
        magic[2] != 't' ||
        magic[3] != 'i' ||
        magic[4] != 'v' ||
        magic[5] != 'e' ||
        magic[6] != 'M' ||
        magic[7] != 'Q' ) {

        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isStackTraceEnabled() const {

    try {
        return properties.getBool( "StackTraceEnabled" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setStackTraceEnabled( bool stackTraceEnabled ) {

    try {
        properties.setBool( "StackTraceEnabled", stackTraceEnabled );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isTcpNoDelayEnabled() const {

    try {
        return properties.getBool( "TcpNoDelayEnabled" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setTcpNoDelayEnabled( bool tcpNoDelayEnabled ) {

    try {
        properties.setBool( "TcpNoDelayEnabled", tcpNoDelayEnabled );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isCacheEnabled() const {

    try {
        return properties.getBool( "CacheEnabled" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setCacheEnabled( bool cacheEnabled ) {

    try {
        properties.setBool( "CacheEnabled", cacheEnabled );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isTightEncodingEnabled() const {

    try {
        return properties.getBool( "TightEncodingEnabled" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setTightEncodingEnabled( bool tightEncodingEnabled ) {

    try {
        properties.setBool( "TightEncodingEnabled", tightEncodingEnabled );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool WireFormatInfo::isSizePrefixDisabled() const {

    try {
        return properties.getBool( "SizePrefixDisabled" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setSizePrefixDisabled( bool sizePrefixDisabled ) {

    try {
        properties.setBool( "SizePrefixDisabled", sizePrefixDisabled );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
long long WireFormatInfo::getMaxInactivityDuration() const {
    try {
        return properties.getLong( "MaxInactivityDuration" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return 0LL;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setMaxInactivityDuration( long long maxInactivityDuration ) {
    try {
        properties.setLong( "MaxInactivityDuration", maxInactivityDuration );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
long long WireFormatInfo::getMaxInactivityDurationInitalDelay() const {
    try {
        return properties.getLong( "MaxInactivityDurationInitalDelay" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return 0LL;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setMaxInactivityDurationInitalDelay( long long maxInactivityDurationInitalDelay ) {
    try {
        properties.setLong( "MaxInactivityDurationInitalDelay", maxInactivityDurationInitalDelay );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int WireFormatInfo::getCacheSize() const {
    try {
        return properties.getInt( "CacheSize" );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::setCacheSize( int value ) {
    try {
        properties.setInt( "CacheSize", value );
    }
    AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}
