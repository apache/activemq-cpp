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

#include "XMLFormatter.h"

#include <decaf/lang/Throwable.h>
#include <decaf/util/Date.h>

#include <sstream>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
namespace {

    void formatMessages( const LogRecord& record, ostringstream& out ) {

        std::string message = record.getMessage();

        if( message != "" ) {
            out << "   <message>" << message << "</message>" << std::endl;
        } else {
            out << "   <message/>" << std::endl;
        }
    }

    void formatThrowable( const LogRecord& record, ostringstream& out ) {

        Throwable* thrown = record.getThrown();
        if( thrown != NULL ) {
            out << "   <exception>" << std::endl
                << "      <message>" << thrown->getMessage() << "</message>" << std::endl;

            // format throwable's stack trace
            std::vector< std::pair< std::string, int> > stack = thrown->getStackTrace();
            std::vector< std::pair< std::string, int> >::const_iterator trace = stack.begin();

            for( ; trace != stack.end(); ++trace ) {
                out << "      <file>" << trace->first << "</file>" << std::endl;
                out << "      <line>" << trace->second << "</line>" << std::endl;
            }

            out << "   <exception>" << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
XMLFormatter::XMLFormatter() : Formatter() {
}

////////////////////////////////////////////////////////////////////////////////
XMLFormatter::~XMLFormatter() {
}

////////////////////////////////////////////////////////////////////////////////
std::string XMLFormatter::format( const LogRecord& record ) const {

    ostringstream out;

    out << "<record>" << std::endl;
    out << "   <date>" << Date( record.getTimestamp() ).toString() << "</date>" << std::endl;

    // TODO - Log Mills
    // TODO - Log Sequence Number
    //    sb.append(indent).append(("<sequence>")).append(r.getSequenceNumber())
    //            .append(("</sequence>")).append(lineSeperator);

    if( "" != record.getLoggerName() ) {
        out << "   <logger>" << record.getLoggerName() << "</logger>" << std::endl;
    }

    out << "   <level>" << record.getLevel().getName() << "</level>" << std::endl;

    if( "" != record.getSourceFile() ) {
        out << "   <file>" << record.getSourceFile() << "</file>" << std::endl;
        out << "   <line>" << record.getSourceLine() << "</line>" << std::endl;
    }

    if( "" != record.getSourceFunction() ) {
        out << "   <method>" << record.getSourceFunction() << "</method>" << std::endl;
    }

    // TODO -Store Thread ID and log it.
    //    sb.append(indent).append(("<thread>")).append(r.getThreadID()).append(
    //            ("</thread>")).append(lineSeperator);

    formatMessages( record, out );
    formatThrowable( record, out );

    out << "</record>" << std::endl;

    return out.str();
}

////////////////////////////////////////////////////////////////////////////////
std::string XMLFormatter::getHead( const Handler* handler DECAF_UNUSED ) {

    ostringstream out;

    out << "<?xml version=\"1.0\" encoding=UTF-8\""
        << "\" standalone=\"no\"?>"
        << std::endl;

    out << "<!DOCTYPE log SYSTEM \"logger.dtd\">" << std::endl;
    out << "<log>";

    return out.str();
}

////////////////////////////////////////////////////////////////////////////////
std::string XMLFormatter::getTail( const Handler* handler DECAF_UNUSED ) {
    return "</log>";
}
