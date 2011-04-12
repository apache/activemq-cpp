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

#ifndef _DECAF_UTI_ZIP_DEFLATER_H_
#define _DECAF_UTI_ZIP_DEFLATER_H_

#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

#include <vector>

namespace decaf {
namespace util {
namespace zip {

    class DeflaterData;

    /**
     * This class compresses data using the <i>DEFLATE</i> algorithm (see <a
     * href="http://www.gzip.org/algorithm.txt">specification</a>).
     *
     * Basically this class is part of the API to the stream based ZLIB compression
     * library and is used as such by DeflaterOutputStream and its
     * descendants.
     *
     * The typical usage of a Deflater instance outside this package consists of a
     * specific call to one of its constructors before being passed to an instance
     * of DeflaterOutputStream.
     *
     * @see DeflaterOutputStream
     * @see Inflater
     * @since 1.0
     */
    class DECAF_API Deflater {
    public:

        /**
         * Compression level for fastest compression.
         */
        static const int BEST_SPEED;

        /**
         * Compression level for best compression.
         */
        static const int BEST_COMPRESSION;

        /**
         * Default compression level.
         */
        static const int DEFAULT_COMPRESSION;

        /**
         * Compression method for the deflate algorithm (the only one currently supported).
         */
        static const int DEFLATED;

        /**
         * Compression level for no compression. .
         */
        static const int NO_COMPRESSION;

        /**
         * Compression strategy best used for data consisting mostly of small values
         * with a somewhat random distribution. Forces more Huffman coding and less
         * string matching.
         */
        static const int FILTERED;

        /**
         * Compression strategy for Huffman coding only.
         */
        static const int HUFFMAN_ONLY;

        /**
         * Default compression strategy.
         */
        static const int DEFAULT_STRATEGY;

    private:

        // Class internal data used during compression.
        DeflaterData* data;

    private:

        Deflater( const Deflater& );
        Deflater operator= ( const Deflater& );

    public:

        /**
         * Creates a new compressor using the specified compression level. If 'nowrap' is true then
         * the ZLIB header and checksum fields will not be used in order to support the compression
         * format used in both GZIP and PKZIP.
         *
         * @param level
         *      The compression level to use (0-9).
         * @param nowrap
         *      If true uses GZip compatible compression (defaults to false).
         */
        Deflater( int level, bool nowrap = false );

        /**
         * Creates a new compressor with the default compression level. Compressed data will be
         * generated in ZLIB format.
         */
        Deflater();

        virtual ~Deflater();

        /**
         * Sets input data for compression. This should be called whenever needsInput() returns
         * true indicating that more input data is required.
         *
         * @param buffer
         *      The Buffer to read in for compression.
         * @param size
         *      The size in bytes of the buffer passed.
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
         * Sets input data for compression. This should be called whenever needsInput() returns
         * true indicating that more input data is required.
         *
         * @param buffer
         *      The Buffer to read in for compression.
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
         * Sets input data for compression. This should be called whenever needsInput() returns
         * true indicating that more input data is required.
         *
         * @param buffer
         *      The Buffer to read in for compression.
         *
         * @throws IllegalStateException if in the end state.
         */
        void setInput( const std::vector<unsigned char>& buffer );

        /**
         * Sets preset dictionary for compression. A preset dictionary is used when the
         * history buffer can be predetermined. When the data is later uncompressed with
         * Inflater.inflate(), Inflater.getAdler() can be called in order to get the
         * Adler-32 value of the dictionary required for decompression.
         *
         * @param buffer
         *      The buffer containing the preset dictionary.
         * @param size
         *      The size of the passed dictionary buffer.
         * @param offset
         *      The position in the Buffer to start reading from.
         * @param length
         *      The number of bytes to read from the input buffer.
         *
         * @throws NullPointerException if buffer is NULL.
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         */
        void setDictionary( const unsigned char* buffer, int size, int offset, int length );

        /**
         * Sets preset dictionary for compression. A preset dictionary is used when the
         * history buffer can be predetermined. When the data is later uncompressed with
         * Inflater.inflate(), Inflater.getAdler() can be called in order to get the
         * Adler-32 value of the dictionary required for decompression.
         *
         * @param buffer
         *      The buffer containing the preset dictionary.
         * @param offset
         *      The position in the Buffer to start reading from.
         * @param length
         *      The number of bytes to read from the input buffer.
         *
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         */
        void setDictionary( const std::vector<unsigned char>& buffer, int offset, int length );

        /**
         * Sets preset dictionary for compression. A preset dictionary is used when the
         * history buffer can be predetermined. When the data is later uncompressed with
         * Inflater.inflate(), Inflater.getAdler() can be called in order to get the
         * Adler-32 value of the dictionary required for decompression.
         *
         * @param buffer
         *      The buffer containing the preset dictionary.
         *
         * @throws IllegalStateException if in the end state.
         */
        void setDictionary( const std::vector<unsigned char>& buffer );

        /**
         * Sets the compression strategy to the specified value.
         *
         * @param strategy
         *      The new Compression strategy to use.
         *
         * @throws IllegalArgumentException if the strategy value is invalid.
         * @throws IllegalStateException if in the end state.
         */
        void setStrategy( int strategy );

        /**
         * Sets the compression level to the specified value.
         *
         * @param level
         *      The new Compression level to use.
         *
         * @throws IllegalArgumentException if the level value is invalid.
         * @throws IllegalStateException if in the end state.
         */
        void setLevel( int level );

        /**
         * @return true if the input data buffer is empty and setInput() should be called in
         *         order to provide more input
         */
        bool needsInput() const;

        /**
         * When called, indicates that compression should end with the current contents of
         * the input buffer.
         */
        void finish();

        /**
         * @return true if the end of the compressed data output stream has been reached.
         */
        bool finished() const;

        /**
         * Fills specified buffer with compressed data. Returns actual number of bytes of
         * compressed data. A return value of 0 indicates that needsInput() should be called
         * in order to determine if more input data is required.
         *
         * @param buffer
         *      The Buffer to write the compressed data to.
         * @param size
         *      The size of the passed buffer.
         * @param offset
         *      The position in the Buffer to start writing at.
         * @param length
         *      The maximum number of byte of data to write.
         *
         * @returns the actual number of bytes of compressed data.
         *
         * @throws NullPointerException if buffer is NULL.
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         */
        int deflate( unsigned char* buffer, int size, int offset, int length );

        /**
         * Fills specified buffer with compressed data. Returns actual number of bytes of
         * compressed data. A return value of 0 indicates that needsInput() should be called
         * in order to determine if more input data is required.
         *
         * @param buffer
         *      The Buffer to write the compressed data to.
         * @param offset
         *      The position in the Buffer to start writing at.
         * @param length
         *      The maximum number of byte of data to write.
         *
         * @returns the actual number of bytes of compressed data.
         *
         * @throws IndexOutOfBoundsException if the offset + length > size of the buffer.
         * @throws IllegalStateException if in the end state.
         */
        int deflate( std::vector<unsigned char>& buffer, int offset, int length );

        /**
         * Fills specified buffer with compressed data. Returns actual number of bytes of
         * compressed data. A return value of 0 indicates that needsInput() should be called
         * in order to determine if more input data is required.
         *
         * @param buffer
         *      The Buffer to write the compressed data to.
         *
         * @returns the actual number of bytes of compressed data.
         *
         * @throws IllegalStateException if in the end state.
         */
        int deflate( std::vector<unsigned char>& buffer );

        /**
         * @returns the ADLER-32 value of the uncompressed data.
         *
         * @throws IllegalStateException if in the end state.
         */
        long long getAdler() const;

        /**
         * @returns the total number of uncompressed bytes input so far.
         *
         * @throws IllegalStateException if in the end state.
         */
        long long getBytesRead() const;

        /**
         * @return the total number of compressed bytes output so far.
         *
         * @throws IllegalStateException if in the end state.
         */
        long long getBytesWritten() const;

        /**
         * Resets deflater so that a new set of input data can be processed. Keeps current compression
         * level and strategy settings.
         *
         * @throws IllegalStateException if in the end state.
         */
        void reset();

        /**
         * Closes the compressor and discards any unprocessed input. This method should be called
         * when the compressor is no longer being used, but will also be called automatically by the
         * destructor. Once this method is called, the behavior of the Deflater object is undefined.
         */
        void end();

    };

}}}

#endif /* _DECAF_UTI_ZIP_DEFLATER_H_ */
