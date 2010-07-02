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

#ifndef _DECAF_UTIL_ZIP_CHECKEDOUTPUTSTREAM_H_
#define _DECAF_UTIL_ZIP_CHECKEDOUTPUTSTREAM_H_

#include <decaf/util/Config.h>
#include <decaf/util/zip/Checksum.h>
#include <decaf/io/FilterOutputStream.h>

namespace decaf {
namespace util {
namespace zip {

    /**
     * An implementation of a FilterOutputStream that will maintain a Checksum of the
     * bytes written, the Checksum can then be used to verify the integrity of the
     * output stream.
     *
     * @since 1.0
     */
    class DECAF_API CheckedOutputStream : public decaf::io::FilterOutputStream {
    private:

        // The Checksom instance used by this stream, not owned by the stream.
        Checksum* sum;

    private:

        CheckedOutputStream( const CheckedOutputStream& );
        CheckedOutputStream& operator= ( const CheckedOutputStream& );

    public:

        /**
         * Create a new instance of a CheckedOutputStream
         *
         * @param outputStream
         *      The OutputStream instance to Wrap.
         * @param sum
         *      The Checksum instance to use (does not take ownership of the Pointer).
         * @param own
         *      Indicates if this filer should take ownership of the InputStream.
         *
         * @throws NullPointerException if the Checksum pointer is NULL.
         */
        CheckedOutputStream( decaf::io::OutputStream* outputStream, Checksum* sum, bool own = false );

        virtual ~CheckedOutputStream();

        /**
         * @returns a pointer to the Checksum instance in use by this object.
         */
        Checksum* getChecksum() const {
            return this->sum;
        }

    protected:

        virtual void doWriteByte( unsigned char value );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length );

    };

}}}

#endif /* _DECAF_UTIL_ZIP_CHECKEDOUTPUTSTREAM_H_ */
