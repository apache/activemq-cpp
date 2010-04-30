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

#ifndef _DECAF_IO_FILEDESCRIPTOR_H_
#define _DECAF_IO_FILEDESCRIPTOR_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace io {

    /**
     * This class servers as an opaque wrapper around an underlying OS level resource that can
     * be used as a source / sink for byte level data such as sockets and files.
     *
     * @since 1.0
     */
    class DECAF_API FileDescriptor {
    public:

        /**
         * A handle to the standard input stream. Usually, this file descriptor is not used
         * directly, but rather via the input stream known as System::in.
         */
        static FileDescriptor in;

        /**
         * A handle to the standard output stream. Usually, this file descriptor is not used
         * directly, but rather via the output stream known as System::out.
         */
        static FileDescriptor out;

        /**
         * A handle to the standard error stream. Usually, this file descriptor is not used
         * directly, but rather via the output stream known as System::err.
         */
        static FileDescriptor err;

    protected:

        long descriptor;
        bool readonly;

    protected:

        FileDescriptor( long value, bool readonly );

    public:

        FileDescriptor();

        virtual ~FileDescriptor();

        /**
         * Force any/all buffered data for this FileDescriptor to be flushed to the underlying device.
         *
         * This method blocks until all data is flushed to the underlying device and is used to place
         * the device into a known state.  In the case of data that is buffered at a higher level such
         * as a BufferedOutputStream the stream must first be flushed before this method can force
         * the data to be sent to the output device.
         */
        void sync();

        /**
         * Indicates whether the File Descriptor is valid.
         *
         * @return true for a valid descriptor such as open socket or file, false otherwise.
         */
        bool valid();

    };

}}

#endif /* _DECAF_IO_FILEDESCRIPTOR_H_ */
