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

#ifndef _DECAF_IO_BYTEARRAYINPUTSTREAMBENCHMARK_H_
#define _DECAF_IO_BYTEARRAYINPUTSTREAMBENCHMARK_H_

#include <benchmark/BenchmarkBase.h>
#include <decaf/io/ByteArrayInputStream.h>

namespace decaf {
namespace io {

    class ByteArrayInputStreamBenchmark :
    public benchmark::BenchmarkBase<
        decaf::io::ByteArrayInputStreamBenchmark, ByteArrayInputStream >
    {
    private:

        unsigned char* buffer;
        std::vector<unsigned char> stlBuffer;
        static const int bufferSize;

    private:

        ByteArrayInputStreamBenchmark( const ByteArrayInputStreamBenchmark& );
        ByteArrayInputStreamBenchmark& operator= ( const ByteArrayInputStreamBenchmark& );

    public:

        ByteArrayInputStreamBenchmark();
        virtual ~ByteArrayInputStreamBenchmark();

        virtual void setUp();
        virtual void tearDown();
        virtual void run();

    };

}}

#endif /* _DECAF_IO_BYTEARRAYINPUTSTREAMBENCHMARK_H_ */
