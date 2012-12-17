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

#ifndef _DECAF_UTI_ZIP_INFLATER_H_
#define _DECAF_UTI_ZIP_INFLATER_H_

#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

#include <decaf/util/zip/DataFormatException.h>

#include <vector>

namespace decaf {
namespace util {
namespace zip {

    class InflaterData;

    /**
     * This class uncompresses data that was compressed using the <i>DEFLATE</i>
     * algorithm (see <a href="http://www.gzip.org/algorithm.txt">specification</a>).
     * <p>
     * Basically this class is part of the API to the stream based ZLIB compression
     * library and is used as such by InflaterInputStream and its descendants.
     * <p>
     * The typical usage of a Inflater outside this package consists of a specific
     * call to one of its constructors before being passed to an instance of
     * InflaterInputStream.
     *
     * @see InflaterInputStream
     * @see Deflater
     *
     * @since 1.0
     */
    class DECAF_API Inflater {
    private:

        InflaterData* data;

    private:

        Inflater( const Inflater& );
        Inflater& operator=( const Inflater& );

    public:

        /**
         * Creates a new decompressor.  This constructor defaults the inflater to use the ZLIB
         * header and checksum fields.
         */
        Inflater();

        /**
         * Creates a new decompressor. If the parameter 'nowrap' is true then the ZLIB header
         * and checksum fields will not be used. This provides compatibility with the compression
         * format used by both GZIP and PKZIP.
         *
         * Note: When using the 'nowrap' option it is also necessary to provide an extra "dummy"
         * byte as input. This is required by the ZLIB native library in order to support
         * certain optimizations.
         */
        Inflater( bool nowrap );

        virtual ~Inflater();

        /**
         * Sets input data for decompression. This should be called whenever needsInput() returns
         * true indicating that more input data is required.
         *
         * @param buffer
         *      The Buffer to read in for decompression.
         * @param size
         *      The size of the buffer passed in.
         * @param offset
         *      The position in the Buffer to start reading from.
         * @param length
         *      The number of bytes to read from the input buffer.
         *
         * @throws NullPointerException if buffer is NULL.
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         */
        void setInput( const unsigned char* buffer, int size, int offset, int length );

        /**
         * Sets input data for decompression. This should be called whenever needsInput() returns
         * true indicating that more input data is required.
         *
         * @param buffer
         *      The Buffer to read in for decompression.
         * @param offset
         *      The position in the Buffer to start reading from.
         * @param length
         *      The number of bytes to read from the input buffer.
         *
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         */
        void setInput( const std::vector<unsigned char>& buffer, int offset, int length );

        /**
         * Sets input data for decompression. This should be called whenever needsInput() returns
         * true indicating that more input data is required.
         *
         * @param buffer
         *      The Buffer to read in for decompression.
         *
         * @throws IllegalStateException if in the end state.
         */
        void setInput( const std::vector<unsigned char>& buffer );

        /**
         * Returns the total number of bytes remaining in the input buffer. This can be used to
         * find out what bytes still remain in the input buffer after decompression has finished.
         *
         * @returns the total number of bytes remaining in the input buffer
         */
        int getRemaining() const;

        /**
         * Sets the preset dictionary to the given array of bytes. Should be called when inflate()
         * returns 0 and needsDictionary() returns true indicating that a preset dictionary is
         * required. The method getAdler() can be used to get the Adler-32 value of the dictionary
         * needed.
         *
         * @param buffer
         *      The Buffer to read in for decompression.
         * @param size
         *      The size of the buffer passed in.
         * @param offset
         *      The position in the Buffer to start reading from.
         * @param length
         *      The number of bytes to read from the input buffer.
         *
         * @throws NullPointerException if buffer is NULL.
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         * @throws IllegalArgumentException if the given dictionary doesn't match thre required
         *         dictionaries checksum value.
         */
        void setDictionary( const unsigned char* buffer, int size, int offset, int length );

        /**
         * Sets the preset dictionary to the given array of bytes. Should be called when inflate()
         * returns 0 and needsDictionary() returns true indicating that a preset dictionary is
         * required. The method getAdler() can be used to get the Adler-32 value of the dictionary
         * needed.
         *
         * @param buffer
         *      The Buffer to read in for decompression.
         * @param offset
         *      The position in the Buffer to start reading from.
         * @param length
         *      The number of bytes to read from the input buffer.
         *
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         * @throws IllegalArgumentException if the given dictionary doesn't match thre required
         *         dictionaries checksum value.
         */
        void setDictionary( const std::vector<unsigned char>& buffer, int offset, int length );

        /**
         * Sets the preset dictionary to the given array of bytes. Should be called when inflate()
         * returns 0 and needsDictionary() returns true indicating that a preset dictionary is
         * required. The method getAdler() can be used to get the Adler-32 value of the dictionary
         * needed.
         *
         * @param buffer
         *      The Buffer to read in for decompression.
         *
         * @throws IllegalStateException if in the end state.
         * @throws IllegalArgumentException if the given dictionary doesn't match the required
         *         dictionaries checksum value.
         */
        void setDictionary( const std::vector<unsigned char>& buffer );

        /**
         * @return true if the input data buffer is empty and setInput() should be called in
         *         order to provide more input
         */
        bool needsInput() const;

        /**
         * @returns true if a preset dictionary is needed for decompression.
         */
        bool needsDictionary() const;

        /**
         * When called, indicates that decompression should end with the current contents of
         * the input buffer.
         */
        void finish();

        /**
         * @return true if the end of the compressed data output stream has been reached.
         */
        bool finished() const;

        /**
         * Uncompresses bytes into specified buffer. Returns actual number of bytes uncompressed.
         * A return value of 0 indicates that needsInput() or needsDictionary() should be called
         * in order to determine if more input data or a preset dictionary is required. In the
         * latter case, getAdler() can be used to get the Adler-32 value of the dictionary required.
         *
         * @param buffer
         *      The Buffer to write the compressed data to.
         * @param size
         *      The size of the buffer passed in.
         * @param offset
         *      The position in the Buffer to start writing at.
         * @param length
         *      The maximum number of byte of data to write.
         *
         * @throws NullPointerException if buffer is NULL.
         * @throws IllegalStateException if in the end state.
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws DataFormatException if the compressed data format is invalid.
         */
        int inflate( unsigned char* buffer, int size, int offset, int length );

        /**
         * Uncompresses bytes into specified buffer. Returns actual number of bytes uncompressed.
         * A return value of 0 indicates that needsInput() or needsDictionary() should be called
         * in order to determine if more input data or a preset dictionary is required. In the
         * latter case, getAdler() can be used to get the Adler-32 value of the dictionary required.
         *
         * @param buffer
         *      The Buffer to write the compressed data to.
         * @param offset
         *      The position in the Buffer to start writing at.
         * @param length
         *      The maximum number of byte of data to write.
         *
         * @throws IllegalStateException if in the end state.
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws DataFormatException if the compressed data format is invalid.
         */
        int inflate( std::vector<unsigned char>& buffer, int offset, int length );

        /**
         * Uncompresses bytes into specified buffer. Returns actual number of bytes uncompressed.
         * A return value of 0 indicates that needsInput() or needsDictionary() should be called
         * in order to determine if more input data or a preset dictionary is required. In the
         * latter case, getAdler() can be used to get the Adler-32 value of the dictionary required.
         *
         * @param buffer
         *      The Buffer to write the compressed data to.
         *
         * @throws IllegalStateException if in the end state.
         * @throws DataFormatException if the compressed data format is invalid.
         */
        int inflate( std::vector<unsigned char>& buffer );

        /**
         * @returns the ADLER-32 value of the uncompressed data.
         *
         * @throws IllegalStateException if in the end state.
         */
        long long getAdler() const;

        /**
         * @returns the total number of compressed bytes input so far.
         *
         * @throws IllegalStateException if in the end state.
         */
        long long getBytesRead() const;

        /**
         * @return the total number of decompressed bytes output so far.
         *
         * @throws IllegalStateException if in the end state.
         */
        long long getBytesWritten() const;

        /**
         * Resets deflater so that a new set of input data can be processed. Keeps current decompression
         * level and strategy settings.
         *
         * @throws IllegalStateException if in the end state.
         */
        void reset();

        /**
         * Closes the decompressor and discards any unprocessed input. This method should be called
         * when the decompressor is no longer being used, but will also be called automatically by the
         * destructor. Once this method is called, the behavior of the Inflater object is undefined.
         */
        void end();

    };

}}}

#endif /* _DECAF_UTI_ZIP_INFLATER_H_ */
