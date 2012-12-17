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

#include "ByteArrayAdapter.h"

#include <string.h>
#include <stdio.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(int size) :
        array(), size(size), own(true) {

    try {

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "Array size given must be greater than zero.");
        }

        this->array.bytes = new unsigned char[size];
        memset(this->array.bytes, 0, size);
        this->initialize(array.bytes, size, true);
    }
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(unsigned char* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(array, size, own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(char* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(reinterpret_cast<unsigned char*>(array), size, own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(double* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(reinterpret_cast<unsigned char*>(array), size * (int) sizeof(double), own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(float* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(reinterpret_cast<unsigned char*>(array), size * (int) sizeof(float), own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(long long* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(reinterpret_cast<unsigned char*>(array), size * (int) sizeof(long long), own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(int* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(reinterpret_cast<unsigned char*>(array), size * (int) sizeof(int), own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::ByteArrayAdapter(short* array, int size, bool own) :
        array(), size(size), own(own) {

    try {
        this->initialize(reinterpret_cast<unsigned char*>(array), size * (int) sizeof(short), own);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(NullPointerException)
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::initialize(unsigned char* array, int size, bool own) {

    try {

        if (array == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "ByteArrayAdapter::initialize - Passed Buffer is null");
        }

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "Array size given must be greater than zero.");
        }

        this->array.bytes = array;
        this->size = size;
        this->own = own;
    }
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter::~ByteArrayAdapter() {

    try {
        if (own) {
            delete[] array.bytes;
        }
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::read(unsigned char* buffer, int size, int offset, int length) const {

    try {

        if (length == 0) {
            return;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "ByteArrayAdapter::write - Passed buffer is null");
        }

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
        }

        if (offset < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
        }

        if (length < 0 || length > size) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
        }

        if ((offset + length) > this->size) {
            throw BufferUnderflowException(__FILE__, __LINE__, "Not enough data to fill request.");
        }

        // Read, starting at offset, length number of bytes to Buffer
        memcpy(buffer, this->array.bytes + offset, length);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(BufferUnderflowException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, BufferUnderflowException)
    DECAF_CATCHALL_THROW(BufferUnderflowException)
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::write(unsigned char* buffer, int size, int offset, int length) {

    try {

        if (length == 0) {
            return;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "ByteArrayAdapter::write - Passed buffer is null");
        }

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
        }

        if (offset < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
        }

        if (length < 0 || length > size) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
        }

        if ((offset + length) > this->size) {
            throw BufferOverflowException(__FILE__, __LINE__, "ByteArrayAdapter::read - Not room in array to hold request.");
        }

        // Write, starting at offset, length number of bytes from buffer.
        memcpy(this->array.bytes + offset, buffer, length);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(BufferOverflowException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, BufferOverflowException)
    DECAF_CATCHALL_THROW(BufferOverflowException)
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::resize(int size) {

    try {

        if (!own) {
            throw InvalidStateException(__FILE__, __LINE__, "ByteArrayAdapter::resize - Not the array owner, can't resize");
        }

        if (size < 0) {
            throw IllegalArgumentException(__FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
        }

        // Save old state
        int oldCapacity = this->size;
        unsigned char* oldArray = this->array.bytes;

        // Resize and copy as much of the old as we can back and delete old array
        this->array.bytes = new unsigned char[size];
        this->size = size;
        memcpy(this->array.bytes, oldArray, Math::min((int) oldCapacity, (int) size));
        delete[] oldArray;
    }
    DECAF_CATCH_RETHROW(InvalidStateException)
    DECAF_CATCHALL_THROW(InvalidStateException)
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayAdapter::clear() {
    memset(this->array.bytes, 0, this->size);
}

////////////////////////////////////////////////////////////////////////////////
unsigned char& ByteArrayAdapter::operator[](int index) {

    try {

        if (index < 0 || index > this->size) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index %d is out of bounds", index);
        }

        return this->array.bytes[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char& ByteArrayAdapter::operator[](int index) const {

    try {

        if (index < 0 || index > this->size) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index %d is out of bounds", index);
        }

        return this->array.bytes[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ByteArrayAdapter::get(int index) const {

    try {

        if (index < 0 || index >= this->getCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::get - Not enough data to fill request.");
        }

        return (*this)[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
char ByteArrayAdapter::getChar(int index) const {

    try {
        return (char) this->get(index);
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
double ByteArrayAdapter::getDouble(int index) const {

    try {

        if (index < 0 || index >= this->getDoubleCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getDouble(i) - Not enough data to fill a long long.");
        }

        return this->array.doubles[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
double ByteArrayAdapter::getDoubleAt(int index) const {

    try {

        unsigned long long lvalue = this->getLongAt(index);
        return Double::longBitsToDouble(lvalue);
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
float ByteArrayAdapter::getFloat(int index) const {

    try {

        if (index < 0 || index >= this->getFloatCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getFloat(i) - Not enough data to fill a long long.");
        }

        return this->array.floats[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
float ByteArrayAdapter::getFloatAt(int index) const {

    try {

        unsigned int ivalue = this->getIntAt(index);
        return Float::intBitsToFloat(ivalue);
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayAdapter::getLong(int index) const {

    try {

        if (index < 0 || index >= this->getLongCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getLong(i) - Not enough data to fill a long long.");
        }

        return this->array.longs[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayAdapter::getLongAt(int index) const {

    try {

        if (index < 0 || (index + (int) sizeof(long long)) > this->getCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getLong(i) - Not enough data to fill a long long.");
        }

        unsigned long long value = 0;
        unsigned char buffer[sizeof(value)] = { 0 };
        this->read(buffer, sizeof(value), index, sizeof(value));

        // Have to do it this way because on Solaris and Cygwin we get all
        // kinds of warnings when shifting a byte up into a long long.
        unsigned long long byte1 = buffer[0] & 0x00000000000000FFULL;
        unsigned long long byte2 = buffer[1] & 0x00000000000000FFULL;
        unsigned long long byte3 = buffer[2] & 0x00000000000000FFULL;
        unsigned long long byte4 = buffer[3] & 0x00000000000000FFULL;
        unsigned long long byte5 = buffer[4] & 0x00000000000000FFULL;
        unsigned long long byte6 = buffer[5] & 0x00000000000000FFULL;
        unsigned long long byte7 = buffer[6] & 0x00000000000000FFULL;
        unsigned long long byte8 = buffer[7] & 0x00000000000000FFULL;

        value = (byte1 << 56 | byte2 << 48 | byte3 << 40 | byte4 << 32 | byte5 << 24 | byte6 << 16 | byte7 << 8 | byte8 << 0);

        return value;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayAdapter::getInt(int index) const {

    try {

        if (index < 0 || index >= this->getIntCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getInt(i) - Not enough data to fill an int.");
        }

        return this->array.ints[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayAdapter::getIntAt(int index) const {

    try {

        if (index < 0 || (index + (int) sizeof(int)) > this->getCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getInt(i) - Not enough data to fill an int.");
        }

        unsigned int value = 0;
        unsigned char buffer[sizeof(value)] = { 0 };
        this->read(buffer, sizeof(value), index, sizeof(value));
        value |= (buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3] << 0);

        return value;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
short ByteArrayAdapter::getShort(int index) const {

    try {

        if (index < 0 || index >= this->getShortCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getShort(i) - Not enough data to fill a short.");
        }

        return this->array.shorts[index];
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
short ByteArrayAdapter::getShortAt(int index) const {

    try {

        if (index < 0 || (index + (int) sizeof(short)) > this->getCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::getShort(i) - Not enough data to fill a short.");
        }

        short value = 0;
        unsigned char buffer[sizeof(value)] = { 0 };
        this->read(buffer, sizeof(value), index, sizeof(value));
        value |= (short) (buffer[0] << 8 | buffer[1] << 0);

        return value;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::put(int index, unsigned char value) {

    try {

        if (index < 0 || index >= this->getCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::put(i,i) - Not enough data to fill request.");
        }

        (*this)[index] = value;

        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putChar(int index, char value) {

    try {

        this->put(index, value);
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putDouble(int index, double value) {

    try {

        if (index < 0 || index >= this->getDoubleCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::putDouble(i,i) - Not enough data to fill request.");
        }

        this->array.doubles[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putDoubleAt(int index, double value) {

    try {

        this->putLongAt(index, Double::doubleToLongBits(value));
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putFloat(int index, float value) {

    try {

        if (index < 0 || index >= this->getFloatCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::putFloat(i,i) - Not enough data to fill request.");
        }

        this->array.floats[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putFloatAt(int index, float value) {

    try {

        this->putIntAt(index, Float::floatToIntBits(value));
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putLong(int index, long long value) {

    try {

        if (index < 0 || index >= this->getLongCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::putLong(i,i) - Not enough data to fill request.");
        }

        this->array.longs[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putLongAt(int index, long long value) {

    try {

        unsigned char buffer[sizeof(value)];

        buffer[0] = (unsigned char) ((value & 0xFF00000000000000ULL) >> 56);
        buffer[1] = (unsigned char) ((value & 0x00FF000000000000ULL) >> 48);
        buffer[2] = (unsigned char) ((value & 0x0000FF0000000000ULL) >> 40);
        buffer[3] = (unsigned char) ((value & 0x000000FF00000000ULL) >> 32);
        buffer[4] = (unsigned char) ((value & 0x00000000FF000000ULL) >> 24);
        buffer[5] = (unsigned char) ((value & 0x0000000000FF0000ULL) >> 16);
        buffer[6] = (unsigned char) ((value & 0x000000000000FF00ULL) >> 8);
        buffer[7] = (unsigned char) ((value & 0x00000000000000FFULL) >> 0);

        this->write(buffer, sizeof(value), index, sizeof(value));

        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putInt(int index, int value) {

    try {

        if (index < 0 || index >= this->getIntCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::putInt(i,i) - Not enough data to fill request.");
        }

        this->array.ints[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putIntAt(int index, int value) {

    try {

        unsigned char buffer[sizeof(value)];

        buffer[0] = (unsigned char) ((value & 0xFF000000) >> 24);
        buffer[1] = (unsigned char) ((value & 0x00FF0000) >> 16);
        buffer[2] = (unsigned char) ((value & 0x0000FF00) >> 8);
        buffer[3] = (unsigned char) ((value & 0x000000FF) >> 0);

        this->write(buffer, sizeof(value), index, sizeof(value));

        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putShort(int index, short value) {

    try {

        if (index < 0 || index >= this->getShortCapacity()) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "ByteArrayAdapter::putShort(i,i) - Not enough data to fill request.");
        }

        this->array.shorts[index] = value;
        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayAdapter& ByteArrayAdapter::putShortAt(int index, short value) {

    try {

        unsigned char buffer[sizeof(value)];

        buffer[0] = (unsigned char) ((value & 0xFF00) >> 8);
        buffer[1] = (unsigned char) ((value & 0x00FF) >> 0);

        this->write(buffer, sizeof(value), index, sizeof(value));

        return *this;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}
