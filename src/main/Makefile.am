# ---------------------------------------------------------------------------
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ---------------------------------------------------------------------------

cc_sources = 

h_sources = cms/DeliveryMode.h \
    cms/TemporaryQueue.h \
    cms/MapMessage.h \
    cms/Destination.h \
    cms/Connection.h \
    cms/TemporaryTopic.h \
    cms/Stoppable.h \
    cms/Topic.h \
    cms/BytesMessage.h \
    cms/CMSException.h \
    cms/Session.h \
    cms/Startable.h \
    cms/Closeable.h \
    cms/MessageListener.h \
    cms/ObjectMessage.h \
    cms/TextMessage.h \
    cms/ExceptionListener.h \
    cms/Queue.h \
    cms/ConnectionFactory.h \
    cms/MessageProducer.h \
    cms/Message.h \
    cms/MessageConsumer.h

##
## Compiler / Linker Info
##

lib_LIBRARIES=libcms.a
libcms_a_SOURCES= $(h_sources) $(cc_sources)

##
## Uncomment to build a shared lib
##
##lib_LTLIBRARIES= libactivemq-cpp.la
##libcms_la_SOURCES= $(h_sources) $(cc_sources)
##libcms_la_LDFLAGS= -version-info $(CMS_LIBRARY_VERSION) -release $(CMS_VERSION)
##libcms_la_LIBADD=-ldl


##
## Packaging Info
##
library_includedir=$(includedir)/$(CMS_LIBRARY_NAME)-$(CMS_VERSION)
nobase_library_include_HEADERS = $(h_sources)
