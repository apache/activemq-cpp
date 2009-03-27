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
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/nio/BufferUnderflowException.h>
#include <decaf/nio/BufferOverflowException.h>

namespace decaf{
namespace internal{
namespace util{

    /**
     * This class adapts primitve type arrays to a base byte array so that the
     * classes can interoperate on the same base byte array without copying data.
     * All the array types are mapped down to a byte array and methods are
     * supplied for accesing the data in any of the primitve type forms.
     * <p>
     * Methods in this class that do not return a specifc value return a
     * reference to this object so that calls can be chained.
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
        std::size_t capacity;

        // Wether this object owns the buffer
        bool own;

    public:

        /**
         * Creates a byte array object that is allocated internally and is then owned
         * and deleted when this object is deleted.  The array is initially created
         * with all elements initialized to zero.
         * @param capacity - size of the array, this is the limit we read and write to.
         */
        ByteArrayAdapter( std::size_t capacity );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( unsigned char* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( char* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( double* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( float* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( long long* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( int* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        /**
         * Creates a byte array object that wraps the given array.  If the own flag
         * is set then it will delete this array when this object is deleted.
         * @param array - array to wrap
         * @param capacity - size of the array, this is the limit we read and write to.
         * @param own - is this class now the owner of the pointer.
         * @throws NullPointerException if buffer is NULL
         */
        ByteArrayAdapter( short* array, std::size_t capacity, bool own = false )
            throw( lang::exceptions::NullPointerException );

        virtual ~ByteArrayAdapter();

        /**
         * Gets the capacity of the underlying array.
         * @return the size the array.
         */
        virtual std::size_t getCapacity() const {
            return this->capacity;
        }

        /**
         * Gets the capacity of the underlying array as if it contains chars
         * @return the size the array.
         */
        virtual std::size_t getCharCapacity() const {
            return this->capacity;
        }

        /**
         * Gets the capacity of the underlying array as if it contains doubles
         * @return the size the array.
         */
        virtual std::size_t getDoubleCapacity() const {
            return this->capacity / sizeof( double );
        }

        /**
         * Gets the capacity of the underlying array as if it contains doubles
         * @return the size the array.
         */
        virtual std::size_t getFloatCapacity() const {
            return this->capacity / sizeof( float );
        }

        /**
         * Gets the capacity of the underlying array as if it contains doubles
         * @return the size the array.
         */
        virtual std::size_t getLongCapacity() const {
            return this->capacity / sizeof( long long );
        }

        /**
         * Gets the capacity of the underlying array as if it contains ints
         * @return the size the array.
         */
        virtual std::size_t getIntCapacity() const {
            return this->capacity / sizeof( int );
        }

        /**
         * Gets the capacity of the underlying array as if it contains shorts
         * @return the size the array.
         */
        virtual std::size_t getShortCapacity() const {
            return this->capacity / sizeof( short );
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
         * the specified length.  If the length is greater than the capacity of this
         * underlying byte array then an BufferUnderflowException is thrown.
         * @param buffer - the buffer to read data from this array into.
         * @param offset - postition in this array to start reading from.
         * @param length - the amount of data to read from this array.
         * @throws NullPointerException if buffer is null
         * @throws BufferUnderflowException if there is not enought data to read
         * because the offset or the length is greater than the size of this array.
         */
        virtual void read( unsigned char* buffer, std::size_t offset, std::size_t length ) const
            throw( decaf::lang::exceptions::NullPointerException,
                   decaf::nio::BufferUnderflowException );

        /**
         * Writes from the Byte array given, starting at the specified offset and writing
         * the specified amoutn of data into this objects internal array..  If the length
         * is greater than the capacity of this underlying byte array then an
         * BufferOverflowException is thrown.
         * @param buffer - the buffer to write get data written into this array.
         * @param offset - postition in this array to start writing from.
         * @param length - the amount of data to write to this array.
         * @throws NullPointerException if buffer is null
         * @throws BufferOverflowException if the amount of data to be written to this
         * array or the offset given are larger than this array's capacity.
         */
        virtual void write( unsigned char* buffer, std::size_t offset, std::size_t length )
            throw( decaf::lang::exceptions::NullPointerException,
                   decaf::nio::BufferOverflowException );

        /**
         * Resizes the underlying array to the new given capacity, preserving all the
         * Data that was previouly in the array, unless the resize is smaller than the
         * current size in which case only the data that will fit into the new array is
         * preserved.
         * <p>
         * A ByteArrayAdapter can only be resized when it owns the underlying array, if it does
         * not then it will throw an IllegalStateException.
         * @param capacity - the new capacity of the array.
         * @throws InvalidStateException if this object does not own the buffer.
         */
        virtual void resize( std::size_t capacity )
            throw ( lang::exceptions::InvalidStateException );

        /**
         * Clear all data from that Array, setting the underlying bytes to zero.
         */
        virtual void clear();

        /**
         * Allows the ByteArrayAdapter to be indexed as a standard array.  calling the
         * non const version allows the user to change the value at index
         * @param index - the position in the array to access
         * @throws IndexOutOfBoundsException
         */
        unsigned char& operator[]( std::size_t index )
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException );
        const unsigned char& operator[]( std::size_t index ) const
            throw ( decaf::lang::exceptions::IndexOutOfBoundsException );

        /**
         * Absolute get method. Reads the byte at the given index.
         * @param index - the index in the Buffer where the byte is to be read
         * @returns the byte that is located at the given index
         * @throws IndexOutOfBoundsException - If index is not smaller than the
         * buffer's limit
         */
        virtual unsigned char get( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads one byte at the given index and returns it
         * @param index - the index in the Buffer where the byte is to be read
         * @returns the char at the given index in the buffer
         * @throws IndexOutOfBoundsException - If index is not smaller than the
         * buffer's limit
         */
        virtual char getChar( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException ) {

            return (char)this->get( index );
        }

        /**
         * Reads eight bytes at the given index and returns it.  The index is a
         * relative to the size of the type to be read, in otherwords when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the double at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual double getDouble( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads eight bytes at the given byte index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the double at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual double getDoubleAt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads four bytes at the given index and returns it. The index is a
         * relative to the size of the type to be read, in otherwords when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the float at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual float getFloat( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads four bytes at the given byte index and returns it
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the float at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual float getFloatAt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads eight bytes at the given index and returns it.  The index is a
         * relative to the size of the type to be read, in otherwords when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the long long at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual long long getLong( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads eight bytes at the given byte index and returns it.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the long long at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual long long getLongAt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads four bytes at the given index and returns it.  The index is a
         * relative to the size of the type to be read, in otherwords when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the int at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual int getInt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads four bytes at the given byte index and returns it.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the int at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual int getIntAt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads two bytes at the given index and returns it. The index is a
         * relative to the size of the type to be read, in otherwords when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the short at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual short getShort( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Reads two bytes at the given byte index and returns it.
         * @param index - the index in the Buffer where the bytes are to be read
         * @returns the short at the given index in the buffer
         * @throws IndexOutOfBoundsException - If there are not enough bytes
         * remaining to fill the requested Data Type
         */
        virtual short getShortAt( std::size_t index ) const
            throw ( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes the given byte into this buffer at the given index. The index is a
         * relative to the size of the type to be read, in otherwords when accessing
         * the element in the array index * sizeof( type ) if the actual start index
         * of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the byte to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& put( std::size_t index, unsigned char value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes one byte containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in otherwords when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putChar( std::size_t index, char value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in otherwords when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putDouble( std::size_t index, double value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given byte index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putDoubleAt( std::size_t index, double value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in otherwords when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putFloat( std::size_t index, float value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given byte index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putFloatAt( std::size_t index, float value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in otherwords when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putLong( std::size_t index, long long value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes eight bytes containing the given value, into this buffer at the
         * given byte index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putLongAt( std::size_t index, long long value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in otherwords when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putInt( std::size_t index, int value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes four bytes containing the given value, into this buffer at the
         * given byte index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putIntAt( std::size_t index, int value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes two bytes containing the given value, into this buffer at the
         * given index. The index is a relative to the size of the type to be read,
         * in otherwords when accessing the element in the array index * sizeof( type )
         * if the actual start index of the type to be read.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putShort( std::size_t index, short value )
            throw( lang::exceptions::IndexOutOfBoundsException );

        /**
         * Writes two bytes containing the given value, into this buffer at the
         * given byte index.
         * @param index - position in the Buffer to write the data
         * @param value - the value to write.
         * @returns a reference to this buffer
         * @throw IndexOutOfBoundsException - If index greater than the buffer's limit
         * minus the size of the type being written.
         */
        virtual ByteArrayAdapter& putShortAt( std::size_t index, short value )
            throw( lang::exceptions::IndexOutOfBoundsException );

    private:

        void initialize( unsigned char* buffer, std::size_t capacity, bool own );

    };

}}}

#endif /*_DECAF_INTERNAL_UTIL_BYTEARRAYADAPTER_H_*/
