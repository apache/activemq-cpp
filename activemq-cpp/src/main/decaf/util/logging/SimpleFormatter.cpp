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

#include "SimpleFormatter.h"

#include <decaf/util/logging/Level.h>

#include <decaf/util/Date.h>

#include <sstream>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
SimpleFormatter::SimpleFormatter() : Formatter() {
}

////////////////////////////////////////////////////////////////////////////////
SimpleFormatter::~SimpleFormatter() {
}

////////////////////////////////////////////////////////////////////////////////
std::string SimpleFormatter::format( const LogRecord& record DECAF_UNUSED ) const {

    ostringstream stream;

    stream << Date( record.getTimestamp() ).toString();
    stream << " ";
    stream << record.getSourceFile();
    stream << ":";
    stream << record.getSourceLine();
    stream << std::endl;

    stream << record.getLevel().getName();
    stream << ": ";
    stream << this->formatMessage( record );
    stream << std::endl;

    if( record.getThrown() != NULL ) {
        stream << "Throwable occurred: ";
        stream << std::endl;

        // TODO write Stack Trace.
    }
//    if (null != r.getThrown()) {
//        sb.append("Throwable occurred: "); //$NON-NLS-1$
//        Throwable t = r.getThrown();
//        PrintWriter pw = null;
//        try {
//            StringWriter sw = new StringWriter();
//            pw = new PrintWriter(sw);
//            t.printStackTrace(pw);
//            sb.append(sw.toString());
//        } finally {
//            if (pw != null) {
//                try {
//                    pw.close();
//                } catch (Exception e) {
//                    // ignore
//                }
//            }
//        }
//    }

    return stream.str();
}
