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

#include <decaf/util/BooleanBenchmark.h>

#include <decaf/util/PropertiesBenchmark.h>
#include <decaf/util/QueueBenchmark.h>
#include <decaf/util/SetBenchmark.h>
#include <decaf/util/MapBenchmark.h>

#include <decaf/io/DataInputStreamBenchmark.h>
#include <decaf/io/DataOutputStreamBenchmark.h>

CPPUNIT_TEST_SUITE_REGISTRATION( decaf::lang::BooleanBenchmark );

CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::PropertiesBenchmark );
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::QueueBenchmark );
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::SetBenchmark );
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::util::MapBenchmark );

CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::DataInputStreamBenchmark );
CPPUNIT_TEST_SUITE_REGISTRATION( decaf::io::DataOutputStreamBenchmark );
