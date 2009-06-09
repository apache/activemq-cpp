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

#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_OPENWIRESTRINGSUPPORTBENCHMARK_H_
#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_OPENWIRESTRINGSUPPORTBENCHMARK_H_

#include <benchmark/BenchmarkBase.h>

#include <activemq/wireformat/openwire/utils/OpenwireStringSupport.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>

#include <vector>

namespace activemq {
namespace wireformat {
namespace openwire {
namespace utils {

    class OpenWireStringSupportBenchmark : public benchmark::BenchmarkBase<
        activemq::wireformat::openwire::utils::OpenWireStringSupportBenchmark, OpenwireStringSupport > {

    private:

        decaf::io::ByteArrayInputStream bytesIn;
        decaf::io::ByteArrayOutputStream bytesOut;

        decaf::io::DataInputStream* dataIn;
        decaf::io::DataOutputStream* dataOut;

        std::string buffer;

    public:

        OpenWireStringSupportBenchmark();
        virtual ~OpenWireStringSupportBenchmark();

        void setUp();
        void tearDown();

        void run();
    };

}}}}

#endif /* _ACTIVEMQ_WIREFORMAT_OPENWIRE_UTILS_OPENWIRESTRINGSUPPORTBENCHMARK_H_ */
