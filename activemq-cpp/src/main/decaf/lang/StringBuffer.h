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

#ifndef _DECAF_LANG_STRINGBUFFER_H_
#define _DECAF_LANG_STRINGBUFFER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/AbstractStringBuilder.h>
#include <decaf/lang/String.h>
#include <decaf/lang/CharSequence.h>
#include <decaf/lang/Appendable.h>
#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace lang {

    /**
     * StringBuffer is a variable size contiguous indexable array of characters. The
     * length of the StringBuffer is the number of characters it contains. The capacity
     * of the StringBuffer is the number of characters it can hold.
     *
     * Characters may be inserted at any position up to the length of the StringBuffer,
     * increasing the length of the StringBuffer. Characters at any position in the
     * StringBuffer may be replaced, which does not affect the StringBuffer length.
     *
     * The capacity of a StringBuffer may be specified when the StringBuffer is
     * created. If the capacity of the StringBuffer is exceeded, the capacity is
     * increased.
     *
     * StringBuffer objects are safe for use by multiple threads. The methods are
     * synchronized where necessary so that all the operations on any particular instance
     * behave as if they occur in some serial order that is consistent with the order of
     * the method calls made by each of the individual threads involved.
     *
     * @see String
     * @see StringBuilder
     *
     * @since 1.0
     */
    class DECAF_API StringBuffer : public AbstractStringBuilder {
    public:

        /**
         * Creates an empty StringBuffer instance with a capacity of 16.
         */
        StringBuffer();

        /**
         * Creates an empty StringBuffer instance with the given capacity
         *
         * @param capacity
         *      The initial capacity to give this new instance.
         *
         * @throws NegativeArraySizeException if the given capacity is less than zero.
         */
        StringBuffer(int capacity);

        /**
         * Constructs a string buffer initialized to the contents of the specified string.
         * The initial capacity of the string builder is 16 plus the length of the string
         * argument.
         *
         * @param source
         *      The String whose contents are to be copied into this StringBuffer.
         */
        StringBuffer(const String& source);

        /**
         * Constructs a string buffer initialized to the contents of the specified string.
         * The initial capacity of the string builder is 16 plus the length of the string
         * argument.
         *
         * @param source
         *      The CharSequence whose contents are to be copied into this StringBuffer.
         *
         * @throws NullPointerException if the CharSequence pointer is NULL.
         */
        StringBuffer(const CharSequence* source);

        virtual ~StringBuffer();

    public:

        virtual int capacity() const;

    };

}}

#endif /* _DECAF_LANG_STRINGBUFFER_H_ */
