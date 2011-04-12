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

#ifndef _DECAF_LANG_READABLE_H_
#define _DECAF_LANG_READABLE_H_

#include <decaf/util/Config.h>

#include <decaf/io/IOException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/nio/ReadOnlyBufferException.h>

namespace decaf {
namespace nio {
    class CharBuffer;
}
namespace lang {

    /**
     * A Readable is a source of characters. Characters from a Readable are made available
     * to callers of the read method via a CharBuffer.
     *
     * @since 1.0
     */
    class DECAF_API Readable {
    public:

        virtual ~Readable() {}

        /**
         * Attempts to read characters into the specified character buffer. The buffer is used
         * as a repository of characters as-is: the only changes made are the results of a put
         * operation.  No flipping or rewinding of the buffer is performed.
         *
         * @param charBuffer
         *      The Buffer to read Characters into.
         *
         * @return The number of char values added to the buffer, or -1 if this source of
         *         characters is at its end
         *
         * @throws IOException if an I/O error occurs.
         * @throws NullPointerException if buffer is NULL.
         * @throws ReadOnlyBufferException if charBuffer is a read only buffer.
         */
        virtual int read( decaf::nio::CharBuffer* charBuffer ) = 0;

    };

}}

#endif /* _DECAF_LANG_READABLE_H_ */
