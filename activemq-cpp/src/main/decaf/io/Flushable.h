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

#ifndef _DECAF_IO_FLUSHABLE_H_
#define _DECAF_IO_FLUSHABLE_H_

#include <decaf/util/Config.h>

#include <decaf/io/IOException.h>

namespace decaf {
namespace io {

    /**
     * A Flushable is a destination of data that can be flushed. The flush method is
     * invoked to write any buffered output to the underlying stream.
     *
     * @since 1.0
     */
    class DECAF_API Flushable {
    public:

        virtual ~Flushable() {}

        /**
         * Flushes this stream by writing any buffered output to the underlying stream.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void flush() = 0;

    };

}}

#endif /* _DECAF_IO_FLUSHABLE_H_ */
