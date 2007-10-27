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
#include "LogWriter.h"

#include <iostream>
#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
concurrent::Mutex LogWriter::mutex;

////////////////////////////////////////////////////////////////////////////////
LogWriter::LogWriter(void)
{
}

////////////////////////////////////////////////////////////////////////////////
LogWriter::~LogWriter(void)
{
}

////////////////////////////////////////////////////////////////////////////////
void LogWriter::log(const std::string& file DECAF_UNUSED,
                    const int          line DECAF_UNUSED,
                    const std::string& prefix,
                    const std::string& message)
{
   synchronized(&mutex)
   {
      cout << prefix  << " "
           << message << " - tid: " << Thread::getId() << endl;
   }
}

////////////////////////////////////////////////////////////////////////////////
void LogWriter::log(const std::string& message)
{
   synchronized(&mutex)
   {
      cout << message << " - tid: " << Thread::getId() << endl;
   }
}

////////////////////////////////////////////////////////////////////////////////
LogWriter& LogWriter::getInstance(void)
{
    // This one instance
    static LogWriter instance;

    return instance;
}

