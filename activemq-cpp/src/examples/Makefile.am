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

main_example_sources = main.cpp

##
## Compiler / Linker Info
##

## Main Example
bin_PROGRAMS = example 
example_SOURCES = $(main_example_sources)
example_LDADD= $(AMQ_TEST_LIBS)
example_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Simple Async Consumer
simple_async_consumer_sources = ./consumers/SimpleAsyncConsumer.cpp
noinst_PROGRAMS = simple_async_consumer 
simple_async_consumer_SOURCES = $(simple_async_consumer_sources)
simple_async_consumer_LDADD= $(AMQ_TEST_LIBS)
simple_async_consumer_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Simple Producer
simple_producer_sources = ./producers/SimpleProducer.cpp
noinst_PROGRAMS += simple_producer 
simple_producer_SOURCES = $(simple_producer_sources)
simple_producer_LDADD= $(AMQ_TEST_LIBS)
simple_producer_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Chat Sample
chat_sources = ./topics/chat/Chat.cpp
noinst_PROGRAMS += chat 
chat_SOURCES = $(chat_sources)
chat_LDADD= $(AMQ_TEST_LIBS)
chat_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## CMSTemplate Message send Sample
cmstemplate_sender_sources = ./cmstemplate/CMSTemplateSender.cpp
noinst_PROGRAMS += cmstemplate_sender
cmstemplate_sender_SOURCES = $(cmstemplate_sender_sources)
cmstemplate_sender_LDADD= $(AMQ_TEST_LIBS)
cmstemplate_sender_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## CMSTemplate Message receive Sample
cmstemplate_receiver_sources = ./cmstemplate/CMSTemplateReceiver.cpp
noinst_PROGRAMS += cmstemplate_receiver
cmstemplate_receiver_SOURCES = $(cmstemplate_receiver_sources)
cmstemplate_receiver_LDADD= $(AMQ_TEST_LIBS)
cmstemplate_receiver_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Advisory Producer Example
advisory_producer_sources = advisories/AdvisoryProducer.cpp \
                            advisories/AdvisoryProducerMain.cpp
noinst_PROGRAMS += advisory_producer 
advisory_producer_SOURCES = $(advisory_producer_sources)
advisory_producer_LDADD= $(AMQ_TEST_LIBS)
advisory_producer_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Advisory Consumer Example
advisory_consumer_sources = advisories/AdvisoryConsumer.cpp \
                            advisories/AdvisoryConsumerMain.cpp
noinst_PROGRAMS += advisory_consumer
advisory_consumer_SOURCES = $(advisory_consumer_sources)
advisory_consumer_LDADD= $(AMQ_TEST_LIBS)
advisory_consumer_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Temp Destination Advisory Consumer Example
tempdest_advisory_consumer_sources = advisories/TempDestinationAdvisoryConsumer.cpp \
                                     advisories/TempDestinationAdvisoryConsumerMain.cpp
noinst_PROGRAMS += tempdest_advisory_consumer
tempdest_advisory_consumer_SOURCES = $(tempdest_advisory_consumer_sources)
tempdest_advisory_consumer_LDADD= $(AMQ_TEST_LIBS)
tempdest_advisory_consumer_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## Temp Destination Advisory Producer Example
tempdest_advisory_producer_sources = advisories/TempDestinationAdvisoryProducerMain.cpp
noinst_PROGRAMS += tempdest_advisory_producer
tempdest_advisory_producer_SOURCES = $(tempdest_advisory_producer_sources)
tempdest_advisory_producer_LDADD= $(AMQ_TEST_LIBS)
tempdest_advisory_producer_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## CMS Template Stress Test Example
cmstemplate_stress_sources = cmstemplate-stress/TestSenderAndReceiver.cpp \
                             cmstemplate-stress/Sender.cpp \
                             cmstemplate-stress/Receiver.cpp \
                             cmstemplate-stress/MessagingTask.cpp \
                             cmstemplate-stress/ConnectionFactoryMgr.cpp \
                             cmstemplate-stress/CMSTestMultipleSendersReceivers.cpp \
                             cmstemplate-stress/CmsMessageCreator.cpp
noinst_PROGRAMS += cmstemplate_stress
cmstemplate_stress_SOURCES = $(cmstemplate_stress_sources)
cmstemplate_stress_LDADD= $(AMQ_TEST_LIBS)
cmstemplate_stress_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main

## CMS Template Stress Test Example
stress_stress_sources = stress-test/TestSenderAndReceiver.cpp \
                        stress-test/Sender.cpp \
                        stress-test/Receiver.cpp \
                        stress-test/MessagingTask.cpp \
                        stress-test/ConnectionFactoryMgr.cpp \
                        stress-test/CmsStress.cpp \
                        stress-test/BrokerMonitor.cpp \
                        stress-test/CmsMessageCreator.cpp
noinst_PROGRAMS += stress_test
stress_test_SOURCES = $(stress_stress_sources)
stress_test_LDADD= $(AMQ_TEST_LIBS)
stress_test_CXXFLAGS = $(AMQ_TEST_CXXFLAGS) -I$(srcdir)/../main
