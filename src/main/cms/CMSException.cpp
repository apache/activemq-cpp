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

#include <cms/CMSException.h>

#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace cms;

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException() throw() : std::exception() {
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException( const CMSException& ex ) throw() : std::exception() {

    this->cause.reset( ex.cause.release() );
    this->message = ex.message;
    this->stackTrace = ex.stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException( const std::string& message,
                            const std::exception* cause ) throw() : std::exception() {

    this->cause.reset( cause );
    this->message = message;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException( const std::string& message,
                            const std::exception* cause,
                            const std::vector< std::pair< std::string, int> >& stackTrace )
                                throw() : std::exception() {

    this->cause.reset( cause );
    this->message = message;
    this->stackTrace = stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::~CMSException() throw() {
}

////////////////////////////////////////////////////////////////////////////////
void CMSException::setMark( const char* file, const int lineNumber ) {

    // Add this mark to the end of the stack trace.
    stackTrace.push_back( std::make_pair( (std::string)file, (int)lineNumber ) );

    std::ostringstream stream;
    stream << "\tFILE: " << stackTrace[stackTrace.size()-1].first;
    stream << ", LINE: " << stackTrace[stackTrace.size()-1].second;
}

////////////////////////////////////////////////////////////////////////////////
void CMSException::printStackTrace() const {
    printStackTrace( std::cerr );
}

////////////////////////////////////////////////////////////////////////////////
void CMSException::printStackTrace( std::ostream& stream ) const {
    stream << getStackTraceString();
}

////////////////////////////////////////////////////////////////////////////////
std::string CMSException::getStackTraceString() const {

    // Create the output stream.
    std::ostringstream stream;

    // Write the message and each stack entry.
    stream << message << std::endl;
    for( unsigned int ix=0; ix<stackTrace.size(); ++ix ){
        stream << "\tFILE: " << stackTrace[ix].first;
        stream << ", LINE: " << stackTrace[ix].second;
        stream << std::endl;
    }

    // Return the string from the output stream.
    return stream.str();
}
