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

#ifndef _DECAF_INTERNAL_UTIL_BYTEARRAYADAPTER_H_
#define _DECAF_INTERNAL_UTIL_BYTEARRAYADAPTER_H_

#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>

namespace decaf{
namespace internal{
namespace util{

    /**
     * This class adapts primitive type arrays to a base byte array so that the
     * classes can inter-operate on the same base byte array without copying data.
     * All the array types are mapped down to a byte array and methods are
     * supplied for accessing the data in any of the primitive type forms.
     *
     * Methods in this class that do not return a specific value return a
     * reference to this object so that calls can be chained.
     *
     * @since 1.0
     */
    class DECAF_API ByteArrayAdapter {
    private:

        // Used to allow this adapter to view its array as the different
        // types it supports
        union Array {
            unsigned char* bytes;
            char* chars;
            short* shorts;
            int* ints;
            long long* longs;
            float* floats;
            double* doubles;
        };

        // Buffer to read and write to, may be shared with other instances.
        Array array;

        // Size of the Buffer
        int size;

        // Whether this object owns the buffer
        bool own;

    public:

        /**
         * Creates a byte array object that is allocated internally and is then owned
         * and deleted when this object is deleted.  The array is initially created
         * with all elements initialized to zero.
         *
         * @param size
         *      The size of the array, this is the limit we read and write to.
         *
         * @throws IllegalArgumentException if size is negative.
         */
        ByteArrayAdapter( int size );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( unsigned char* array, int size, bool own = false );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( char* array, int size, bool own = false );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( double* array, int size, bool own = false );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( float* array, int size, bool own = false );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( long long* array, int size, bool own = false );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( int* array, int size, bool own = false );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         *
         * @param array
         *      The physical array to wrap.
         * @param size
         *      The size of the array, this is the limit we read and write to.
         * @param own
         *      Indicates if this class is now the owner of the pointer.
         *
         * @throws NullPointerException if buffer is NULL
         * @throws IndexOutOfBoundsException if the size is negative.
         */
        ByteArrayAdapter( short* array, int size, bool own = false );

        virtual ~ByteArrayAdapter();

        /**
         * Gets the size of the underlying array.
         * @return the size the array.
         */
        virtual int getCapacity() const {
            return this->size;
        }

        /**
         * Gets the size of the underlying array as if it contains chars
         * @return the size the array.
         */
        virtual int getCharCapacity() const {
            return this->size;
        }

        /**
         * Gets the size of the underlying array as if it contains doubles
         * @return the size the array.
         */
        virtual int getDoubleCapacity() const {
            return this->size / (int)sizeof( double );
        }

        /**
         * Gets the size of the underlying array as if it contains doubles
         * @return the size the array.
         */
        virtual int getFloatCapacity() const {
            return this->size / (int)sizeof( float );
        }

        /**
         * Gets the size of the underlying array as if it contains doubles
         * @return the size the array.
         */
        virtual int getLongCapacity() const {
            return this->size / (int)sizeof( long long );
        }

        /**
         * Gets the size of the underlying array as if it contains ints
         * @return the size the array.
         */
        virtual int getIntCapacity() const {
            return this->size / (int)sizeof( int );
        }

        /**
         * Gets the size of the underlying array as if it contains shorts
         * @return the size the array.
         */
        virtual int getShortCapacity() const {
            return this->size / (int)sizeof( short );
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an unsigned char* pointer to the array this object wraps.
         */
        virtual unsigned char* getByteArray() {
            return this->array.bytes;
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an char* pointer to the array this object wraps.
         */
        virtual char* getCharArray() {
            return this->array.chars;
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an short* pointer to the array this object wraps.
         */
        virtual short* getShortArray() {
            return this->array.shorts;
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an int* pointer to the array this object wraps.
         */
        virtual int* getIntArray() {
            return this->array.ints;
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an long long* pointer to the array this object wraps.
         */
        virtual long long* getLongArray() {
            return this->array.longs;
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an double* pointer to the array this object wraps.
         */
        virtual double* getDoubleArray() {
            return this->array.doubles;
        }

        /**
         * Gets the pointer to the array we are wrapping.  Changes to the data in this
         * array are reflected by all ByteArrayAdapter objects that point to this array.
         * @returns an float* pointer to the array this object wraps.
         */
        virtual float* getFloatArray() {
            return this->array.floats;
        }

        /**
         * Reads from the Byte array starting at the specified offset and reading
         * the specified length.  If the length is greater than the size of this
         * underlying byte array then an BufferUnderflowException is thrown.
         *
         * @param buffer
         *      The buffer to read data from this array into.
         * @param size
         *      The size of the buffer passed.
         * @param offset
         *      The position in this array to start reading from.
         * @param length
         *      The amount of data to read from this array.
         *
         * @throws IndexOutOfBoundsException if the offset + length exceeds the size.
         * @throws NullPointerException if buffer is null
         * @throws BufferUnderflowException if there is not enough data to read
         *         because the offset or the length is greater than the size of this array.
         */
        virtual void read( unsigned char* buffer, int size, int offset, int length ) const;

        /**
         * Writes from the Byte array given, starting at the specified offset and writing
         * the specified amount of data into this objects internal array..  If the length
         * is greater than the size of this underlying byte array then an
         * BufferOverflowException is thrown.
         *
         * @param buffer
         *      The buffer to read data from this array into.
         * @param size
         *      The size of the buffer passed.
         * @param offset
         *      The position in this array to start reading from.
         * @param length
         *      The amount of data to read from this array.
         *
         * @throws IndexOutOfBoundsException if the offset + length exceeds the size.
         * @throws NullPointerException if buffer is null
         * @throws BufferOverflowException if the amount of data to be written to this
         * array or the offset given are larger than this array's size.
         */
        virtual void write( unsigned char* buffer, int size, int offset, int length );

        /**
         * Resizes the underlying array to the new given size, preserving all the
         * Data that was previously in the array, unless the resize is smaller than the
         * current size in which case only the data that will fit into the new array is
         * preserved.
         *
         * A ByteArrayAdapter can only be resized when it owns the underlying array, if it does
         * not then it will throw an InvalidStateException.
         *
         * @param size
         *      The new size of the array.
         *
         * @throws IllegalArgumentException if the size parameter is negative.
         * @throws InvalidStateException if this object does not own the buffer.
         */
        virtual void resize( int size );

        /**
         * Clear all data from that Array, setting the underlying bytes to zero.
         */
        virtual void clear() throw();

        /**
         * Allows the ByteArrayAdapter to be indexed as a standard array.  calling the
         * non constant version allows the user to change the value at index
         *
         * @param index
         *      The position in the array to access, if the value is negative or
         *      greater than the size of the underlying array an IndexOutOfBoundsException
         *      is thrown.
         *
         * @throws IndexOutOfBoundsException if the preconditions of index are not met.
         */
        unsigned char& operator[]( int index );
        const unsigned char& operator[]( int index ) const;

        /**
         * Absolute get method. Reads the byte at the given index.
         *
         * @param index
         *      The index in the Buffer where the byte is to be read.
         *
         * @returns the byte that is located at the given index.
         *
         * @throws IndexOutOfBoundsException If index is not smaller than the
         *         buffer's limit or is negative.
         */
        virtual unsigned char get( int index ) const;

        /**
         * Reads one byte at the given index and returns it.
         *
         * @param index
         *      The index in the Buffer where the byte is to be read.
         *
         * @returns the byte that is located at the given index.
         *
         * @throws IndexOutOfBoundsException If index is not smaller than the
         *         buffer's limit or is negative.
         */
        virtual char getChar( int index ) const;

        /**
         * Reads eight bytes at the given index and returns it.  The index is a
         * relative to the size of the type to be read, in other words when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read.
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual double getDouble( int index ) const;

        /**
         * Reads eight bytes at the given byte index and returns it.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual double getDoubleAt( int index ) const;

        /**
         * Reads four bytes at the given index and returns it. The index is a
         * relative to the size of the type to be read, in other words when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read.
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual float getFloat( int index ) const;

        /**
         * Reads four bytes at the given byte index and returns it
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual float getFloatAt( int index ) const;

        /**
         * Reads eight bytes at the given index and returns it.  The index is a
         * relative to the size of the type to be read, in other words when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read.
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual long long getLong( int index ) const;

        /**
         * Reads eight bytes at the given byte index and returns it.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual long long getLongAt( int index ) const;

        /**
         * Reads four bytes at the given index and returns it.  The index is a
         * relative to the size of the type to be read, in other words when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read.
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual int getInt( int index ) const;

        /**
         * Reads four bytes at the given byte index and returns it.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual int getIntAt( int index ) const;

        /**
         * Reads two bytes at the given index and returns it. The index is a
         * relative to the size of the type to be read, in other words when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read.
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual short getShort( int index ) const;

        /**
         * Reads two bytes at the given byte index and returns it.
         *
         * @param index
         *      The index in the Buffer where the bytes are to be read
         *
         * @returns the value at the given index in the buffer.
         *
         * @throws IndexOutOfBoundsException if there are not enough bytes remaining
         *         to fill the requested Data Type, or index is negative.
         */
        virtual short getShortAt( int index ) const;

        /**
         * Writes the given byte into this buffer at the given index. The index is a
         * relative to the size of the type to be read, in other words when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& put( int index, unsigned char value );

        /**
         * Writes one byte containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in other words when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putChar( int index, char value );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in other words when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putDouble( int index, double value );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given byte index.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putDoubleAt( int index, double value );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in other words when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putFloat( int index, float value );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given byte index.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putFloatAt( int index, float value );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in other words when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putLong( int index, long long value );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given byte index.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putLongAt( int index, long long value );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in other words when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putInt( int index, int value );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given byte index.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putIntAt( int index, int value );

        /**
         * Writes two bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in other words when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write to the array.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putShort( int index, short value );

        /**
         * Writes two bytes containing the given value, into this buffer at the
         * given byte index.
         *
         * @param index
         *      The position in the Buffer to write the data.
         * @param value
         *      The value to write.
         *
         * @returns a reference to this buffer.
         *
         * @throw IndexOutOfBoundsException if index greater than the buffer's limit
         *        minus the size of the type being written, or index is negative.
         */
        virtual ByteArrayAdapter& putShortAt( int index, short value );

    private:

        void initialize( unsigned char* buffer, int size, bool own );

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_BYTEARRAYADAPTER_H_*/
