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

#ifndef _DECAF_UTIL_ZIP_CHECKEDINPUTSTREAM_H_
#define _DECAF_UTIL_ZIP_CHECKEDINPUTSTREAM_H_

#include <decaf/util/Config.h>
#include <decaf/util/zip/Checksum.h>
#include <decaf/io/FilterInputStream.h>

namespace decaf {
namespace util {
namespace zip {

    /**
     * An implementation of a FilterInputStream that will maintain a Checksum of the
     * bytes read, the Checksum can then be used to verify the integrity of the
     * input stream.
     *
     * @since 1.0
     */
    class DECAF_API CheckedInputStream : public decaf::io::FilterInputStream {
    private:

        // Pointer to the Checksum instance to use in this stream, ownership
        // of the Checksum pointer is not taken by this class.
        Checksum* sum;

    private:

        CheckedInputStream( const CheckedInputStream& );
        CheckedInputStream& operator= ( const CheckedInputStream& );

    public:

        /**
         * Create a new instance of a CheckedInputStream
         *
         * @param inputStream
         *      The InputStream instance to Wrap.
         * @param sum
         *      The Checksum instance to use (does not take ownership of the Pointer).
         * @param own
         *      Indicates if this filer should take ownership of the InputStream.
         *
         * @throws NullPointerException if the Checksum pointer is NULL.
         */
        CheckedInputStream( InputStream* inputStream, Checksum* sum, bool own = false );

        virtual ~CheckedInputStream();

        /**
         * Returns a Pointer to the Checksum that is in use by this CheckedInputStream.
         *
         * @return the pointer to the Checksum instance that is in use by this object.
         */
        Checksum* getChecksum() const {
            return this->sum;
        }

        /**
         * {@inheritDoc}
         *
         * Adds the skipped bytes into the Checksum.
         */
        virtual long long skip( long long num );

    protected:

        virtual int doReadByte();

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length );

    };

}}}

#endif /* _DECAF_UTIL_ZIP_CHECKEDINPUTSTREAM_H_ */
