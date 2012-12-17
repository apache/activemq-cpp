/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/commands/ActiveMQStreamMessage.h>
#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/MarshallingSupport.h>

#include <cms/MessageEOFException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>

#include <algorithm>

#include <decaf/lang/Math.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Byte.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/util/zip/DeflaterOutputStream.h>
#include <decaf/util/zip/InflaterInputStream.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace commands {

    class ActiveMQStreamMessageImpl {
    private:

        ActiveMQStreamMessageImpl(const ActiveMQStreamMessageImpl&);
        ActiveMQStreamMessageImpl& operator= (const ActiveMQStreamMessageImpl&);

    public:

        ActiveMQStreamMessageImpl() : bytesOut(NULL), remainingBytes(-1) {}
        ~ActiveMQStreamMessageImpl() {}

    public:

        // Holds the contents of the message once written.
        decaf::io::ByteArrayOutputStream* bytesOut;

        // When reading an array of bytes this value indicates how many bytes
        // are left unread since the last readBytes call.
        mutable int remainingBytes;

    };
}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::ActiveMQStreamMessage() :
    ActiveMQMessageTemplate<cms::StreamMessage>(), impl(new ActiveMQStreamMessageImpl()), dataIn(), dataOut() {

    this->clearBody();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::~ActiveMQStreamMessage() throw () {
    try {
        this->reset();
        delete impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQStreamMessage::getDataStructureType() const {
    return ActiveMQStreamMessage::ID_ACTIVEMQSTREAMMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage* ActiveMQStreamMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQStreamMessage> message(new ActiveMQStreamMessage());
    message->copyDataStructure(this);
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const ActiveMQStreamMessage* srcPtr = dynamic_cast<const ActiveMQStreamMessage*> (src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(__FILE__, __LINE__, "ActiveMQStreamMessage::copyDataStructure - src is NULL or invalid");
    }

    ActiveMQStreamMessage* nonConstSrc = const_cast<ActiveMQStreamMessage*> (srcPtr);
    nonConstSrc->storeContent();

    ActiveMQMessageTemplate<cms::StreamMessage>::copyDataStructure(src);
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQStreamMessage::toString() const {
    return ActiveMQMessageTemplate<cms::StreamMessage>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQStreamMessage::equals(const DataStructure* value) const {
    return ActiveMQMessageTemplate<cms::StreamMessage>::equals(value);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::clearBody() {

    // Invoke base class's version.
    ActiveMQMessageTemplate<cms::StreamMessage>::clearBody();

    this->dataIn.reset(NULL);
    this->dataOut.reset(NULL);
    this->impl->bytesOut = NULL;
    this->impl->remainingBytes = -1;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::onSend() {

    this->storeContent();
    ActiveMQMessageTemplate<cms::StreamMessage>::onSend();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::reset() {

    try {
        storeContent();
        this->impl->bytesOut = NULL;
        this->dataIn.reset(NULL);
        this->dataOut.reset(NULL);
        this->impl->remainingBytes = -1;
        this->setReadOnlyBody(true);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQStreamMessage::readBoolean() const {

    try {

        initializeReading();

        this->dataIn->mark(10);

        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("Reached the end of the Stream", NULL);
        }
        if (type == PrimitiveValueNode::BOOLEAN_TYPE) {
            return this->dataIn->readBoolean();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Boolean::valueOf(this->dataIn->readUTF()).booleanValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to boolean.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException("not a boolean type", NULL);
        }

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBoolean(bool value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::BOOLEAN_TYPE);
        this->dataOut->writeBoolean(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQStreamMessage::readByte() const {

    initializeReading();
    try {

        this->dataIn->mark(10);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::BYTE_TYPE) {
            return this->dataIn->readByte();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Byte::valueOf(this->dataIn->readUTF()).byteValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to byte.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a byte type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& e) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::createMessageFormatException(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeByte(unsigned char value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::BYTE_TYPE);
        this->dataOut->writeByte(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQStreamMessage::readBytes(std::vector<unsigned char>& value) const {

    if (value.size() == 0) {
        return 0;
    }

    return this->readBytes(&value[0], (int) value.size());
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes(const std::vector<unsigned char>& value) {

    initializeWriting();
    try {

        int size = (int) value.size();
        this->dataOut->write(PrimitiveValueNode::BYTE_ARRAY_TYPE);
        this->dataOut->writeInt((int) size);
        this->dataOut->write(&value[0], size, 0, size);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQStreamMessage::readBytes(unsigned char* buffer, int length) const {

    initializeReading();
    try {

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Passed buffer was NULL");
        }

        if (this->impl->remainingBytes == -1) {

            this->dataIn->mark((int) length + 1);
            int type = this->dataIn->read();

            if (type == -1) {
                throw MessageEOFException("reached end of data", NULL);
            }

            if (type != PrimitiveValueNode::BYTE_ARRAY_TYPE) {
                throw MessageFormatException("Not a byte array", NULL);
            }

            this->impl->remainingBytes = this->dataIn->readInt();

        } else if (this->impl->remainingBytes == 0) {
            this->impl->remainingBytes = -1;
            return -1;
        }

        if (length <= this->impl->remainingBytes) {
            // small buffer
            this->impl->remainingBytes -= (int) length;
            this->dataIn->readFully(buffer, length);
            return length;
        } else {
            // big buffer
            int rc = this->dataIn->read(buffer, length, 0, this->impl->remainingBytes);
            this->impl->remainingBytes = 0;
            return rc;
        }

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes(const unsigned char* value, int offset, int length) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::BYTE_ARRAY_TYPE);
        this->dataOut->writeInt((int) length);
        this->dataOut->write(value, length, offset, length);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQStreamMessage::readChar() const {

    initializeReading();
    try {

        this->dataIn->mark(17);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::CHAR_TYPE) {
            return this->dataIn->readChar();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to char.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a char type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& ioe) {
            throw CMSExceptionSupport::create(ioe);
        }

        throw CMSExceptionSupport::create(ex);
        ;

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeChar(char value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::CHAR_TYPE);
        this->dataOut->writeChar(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQStreamMessage::readFloat() const {

    initializeReading();
    try {

        this->dataIn->mark(33);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::FLOAT_TYPE) {
            return this->dataIn->readFloat();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Float::valueOf(this->dataIn->readUTF()).floatValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to float.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a float type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& ioe) {
            throw CMSExceptionSupport::create(ioe);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeFloat(float value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::FLOAT_TYPE);
        this->dataOut->writeFloat(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQStreamMessage::readDouble() const {

    initializeReading();
    try {

        this->dataIn->mark(33);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::DOUBLE_TYPE) {
            return this->dataIn->readDouble();
        }
        if (type == PrimitiveValueNode::FLOAT_TYPE) {
            return this->dataIn->readFloat();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Double::valueOf(this->dataIn->readUTF()).doubleValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to double.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a double type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& ioe) {
            throw CMSExceptionSupport::create(ioe);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeDouble(double value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::DOUBLE_TYPE);
        this->dataOut->writeDouble(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQStreamMessage::readShort() const {

    initializeReading();
    try {

        this->dataIn->mark(17);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::SHORT_TYPE) {
            return this->dataIn->readShort();
        }
        if (type == PrimitiveValueNode::BYTE_TYPE) {
            return this->dataIn->readByte();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Short::valueOf(this->dataIn->readUTF()).shortValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to short.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a short type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& e) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeShort(short value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::SHORT_TYPE);
        this->dataOut->writeShort(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQStreamMessage::readUnsignedShort() const {

    initializeReading();
    try {

        this->dataIn->mark(17);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::SHORT_TYPE) {
            return this->dataIn->readUnsignedShort();
        }
        if (type == PrimitiveValueNode::BYTE_TYPE) {
            return this->dataIn->readByte();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Short::valueOf(this->dataIn->readUTF()).shortValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to short.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a short type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& e) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeUnsignedShort(unsigned short value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::SHORT_TYPE);
        this->dataOut->writeUnsignedShort(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQStreamMessage::readInt() const {

    initializeReading();
    try {

        this->dataIn->mark(33);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::INTEGER_TYPE) {
            return this->dataIn->readInt();
        }
        if (type == PrimitiveValueNode::SHORT_TYPE) {
            return this->dataIn->readShort();
        }
        if (type == PrimitiveValueNode::BYTE_TYPE) {
            return this->dataIn->readByte();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Integer::valueOf(this->dataIn->readUTF()).intValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to int.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a int type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& e) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeInt(int value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::INTEGER_TYPE);
        this->dataOut->writeInt(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQStreamMessage::readLong() const {

    initializeReading();
    try {

        this->dataIn->mark(65);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::LONG_TYPE) {
            return this->dataIn->readLong();
        }
        if (type == PrimitiveValueNode::INTEGER_TYPE) {
            return this->dataIn->readInt();
        }
        if (type == PrimitiveValueNode::SHORT_TYPE) {
            return this->dataIn->readShort();
        }
        if (type == PrimitiveValueNode::BYTE_TYPE) {
            return this->dataIn->readByte();
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return Long::valueOf(this->dataIn->readUTF()).longValue();
        }

        if (type == PrimitiveValueNode::NULL_TYPE) {
            this->dataIn->reset();
            throw NullPointerException(__FILE__, __LINE__, "Cannot convert NULL value to long.");
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a long type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& e) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeLong(long long value) {

    initializeWriting();
    try {
        this->dataOut->write(PrimitiveValueNode::LONG_TYPE);
        this->dataOut->writeLong(value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQStreamMessage::readString() const {

    initializeReading();
    try {

        this->dataIn->mark(65);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }
        if (type == PrimitiveValueNode::NULL_TYPE) {
            return "";
        }
        if (type == PrimitiveValueNode::BIG_STRING_TYPE) {
            return MarshallingSupport::readString32(*this->dataIn);
        }
        if (type == PrimitiveValueNode::STRING_TYPE) {
            return MarshallingSupport::readString16(*this->dataIn);
        }
        if (type == PrimitiveValueNode::LONG_TYPE) {
            return Long(this->dataIn->readLong()).toString();
        }
        if (type == PrimitiveValueNode::INTEGER_TYPE) {
            return Integer(this->dataIn->readInt()).toString();
        }
        if (type == PrimitiveValueNode::SHORT_TYPE) {
            return Short(this->dataIn->readShort()).toString();
        }
        if (type == PrimitiveValueNode::BYTE_TYPE) {
            return Byte(this->dataIn->readByte()).toString();
        }
        if (type == PrimitiveValueNode::FLOAT_TYPE) {
            return Float(this->dataIn->readFloat()).toString();
        }
        if (type == PrimitiveValueNode::DOUBLE_TYPE) {
            return Double(this->dataIn->readDouble()).toString();
        }
        if (type == PrimitiveValueNode::BOOLEAN_TYPE) {
            return (this->dataIn->readBoolean() ? Boolean::_TRUE : Boolean::_FALSE).toString();
        }

        if (type == PrimitiveValueNode::CHAR_TYPE) {
            return Character(this->dataIn->readChar()).toString();
        } else {
            this->dataIn->reset();
            throw MessageFormatException(" not a String type", NULL);
        }

    } catch (NumberFormatException& ex) {

        try {
            this->dataIn->reset();
        } catch (IOException& ioe) {
            throw CMSExceptionSupport::create(ioe);
        }

        throw CMSExceptionSupport::create(ex);

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeString(const std::string& value) {

    initializeWriting();
    try {
        MarshallingSupport::writeString(*this->dataOut, value);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message::ValueType ActiveMQStreamMessage::getNextValueType() const {

    initializeReading();
    try {

        if (this->impl->remainingBytes != -1) {
            throw cms::IllegalStateException(
                "Cannot read the next type during an byte array read operation, complete the read first.");
        }

        this->dataIn->mark(10);
        int type = this->dataIn->read();

        if (type == -1) {
            throw MessageEOFException("reached end of data", NULL);
        }

        this->dataIn->reset();

        switch(type) {
            case util::PrimitiveValueNode::NULL_TYPE:
                return cms::Message::NULL_TYPE;
            case util::PrimitiveValueNode::BOOLEAN_TYPE:
                return cms::Message::BOOLEAN_TYPE;
            case util::PrimitiveValueNode::BYTE_TYPE:
                return cms::Message::BYTE_TYPE;
            case util::PrimitiveValueNode::BYTE_ARRAY_TYPE:
                return cms::Message::BYTE_ARRAY_TYPE;
            case util::PrimitiveValueNode::CHAR_TYPE:
                return cms::Message::CHAR_TYPE;
            case util::PrimitiveValueNode::SHORT_TYPE:
                return cms::Message::SHORT_TYPE;
            case util::PrimitiveValueNode::INTEGER_TYPE:
                return cms::Message::INTEGER_TYPE;
            case util::PrimitiveValueNode::LONG_TYPE:
                return cms::Message::LONG_TYPE;
            case util::PrimitiveValueNode::DOUBLE_TYPE:
                return cms::Message::DOUBLE_TYPE;
            case util::PrimitiveValueNode::FLOAT_TYPE:
                return cms::Message::FLOAT_TYPE;
            case util::PrimitiveValueNode::STRING_TYPE:
            case util::PrimitiveValueNode::BIG_STRING_TYPE:
                return cms::Message::STRING_TYPE;
            default:
                throw MessageFormatException("Unknown type found in stream", NULL);
        }

        return cms::Message::UNKNOWN_TYPE;

    } catch (EOFException& e) {
        throw CMSExceptionSupport::createMessageEOFException(e);
    } catch (IOException& e) {
        throw CMSExceptionSupport::createMessageFormatException(e);
    } catch (Exception& e) {
        throw CMSExceptionSupport::create(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::storeContent() {

    if (this->dataOut.get() != NULL) {

        this->dataOut->close();

        if (this->impl->bytesOut->size() > 0) {
            std::pair<unsigned char*, int> array = this->impl->bytesOut->toByteArray();
            this->setContent(std::vector<unsigned char>(array.first, array.first + array.second));
            delete[] array.first;
        }

        this->dataOut.reset(NULL);
        this->impl->bytesOut = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::initializeReading() const {

    this->failIfWriteOnlyBody();
    try {
        if (this->dataIn.get() == NULL) {
            InputStream* is = new ByteArrayInputStream(this->getContent());

            if (isCompressed()) {
                is = new InflaterInputStream(is, true);
                is = new BufferedInputStream(is, true);
            }

            this->dataIn.reset(new DataInputStream(is, true));
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::initializeWriting() {

    this->failIfReadOnlyBody();
    try {
        if (this->dataOut.get() == NULL) {
            this->impl->bytesOut = new ByteArrayOutputStream();

            OutputStream* os = this->impl->bytesOut;

            if (this->connection != NULL && this->connection->isUseCompression()) {
                this->compressed = true;

                Deflater* deflator = new Deflater(this->connection->getCompressionLevel());

                os = new DeflaterOutputStream(os, deflator, true, true);
            }

            this->dataOut.reset(new DataOutputStream(os, true));
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
