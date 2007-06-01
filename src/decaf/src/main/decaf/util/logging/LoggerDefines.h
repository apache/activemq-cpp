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
#ifndef _DECAF_UTIL_LOGGING_LOGGERDEFINES_H_
#define _DECAF_UTIL_LOGGING_LOGGERDEFINES_H_

#include <decaf/util/logging/SimpleLogger.h>
#include <sstream>

#define LOGDECAF_DECLARE(loggerName)                                  \
   static decaf::util::logging::SimpleLogger loggerName;

#define LOGDECAF_INITIALIZE(loggerName, className, loggerFamily)      \
   decaf::util::logging::SimpleLogger className::loggerName(loggerFamily);

#define LOGDECAF_DECLARE_LOCAL(loggerName)                            \
   decaf::util::logging::Logger loggerName;

#define LOGDECAF_DEBUG(logger, message)                               \
   logger.debug(__FILE__, __LINE__, message);

#define LOGDECAF_DEBUG_1(logger, message, value);                     \
   {                                                                  \
      std::ostringstream ostream;                                     \
      ostream << message << value;                                    \
      logger.debug(__FILE__, __LINE__, ostream.str());                \
   }

#define LOGDECAF_INFO(logger, message)                                \
   logger.info(__FILE__, __LINE__, message);

#define LOGDECAF_ERROR(logger, message)                               \
   logger.error(__FILE__, __LINE__, message);

#define LOGDECAF_WARN(logger, message)                                \
   logger.warn(__FILE__, __LINE__, message);

#define LOGDECAF_FATAL(logger, message)                               \
   logger.fatal(__FILE__, __LINE__, message);


#endif
