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
#include <activemq/commands/ActiveMQObjectMessage.h>

#include <activemq/util/CMSExceptionSupport.h>

#include <decaf/io/FilterOutputStream.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/EOFException.h>
#include <decaf/io/IOException.h>

#include <decaf/util/zip/DeflaterOutputStream.h>
#include <decaf/util/zip/InflaterInputStream.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
ActiveMQObjectMessage::ActiveMQObjectMessage() :
    ActiveMQMessageTemplate<cms::ObjectMessage> () {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQObjectMessage::getDataStructureType() const {
    return ActiveMQObjectMessage::ID_ACTIVEMQOBJECTMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQObjectMessage* ActiveMQObjectMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQObjectMessage> message(new ActiveMQObjectMessage());
    message->copyDataStructure(this);
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQObjectMessage::copyDataStructure(const DataStructure* src) {
    ActiveMQMessageTemplate<cms::ObjectMessage>::copyDataStructure(src);
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQObjectMessage::toString() const {
    return ActiveMQMessageTemplate<cms::ObjectMessage>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQObjectMessage::equals(const DataStructure* value) const {
    return ActiveMQMessageTemplate<cms::ObjectMessage>::equals(value);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQObjectMessage::setObjectBytes(const std::vector<unsigned char>& bytes) {

    this->failIfReadOnlyBody();
    try {

        if (bytes.size() == 0) {
            return;
        }

        if (this->connection != NULL && this->connection->isUseCompression()) {
            this->compressed = true;

            ByteArrayOutputStream bytesOut;
            Deflater* deflator = new Deflater(this->connection->getCompressionLevel());
            DeflaterOutputStream out(&bytesOut, deflator, false, true);
            out.write(&bytes[0], (int)bytes.size());

            std::pair<unsigned char*, int> array = bytesOut.toByteArray();
            this->setContent(std::vector<unsigned char>(array.first, array.first + array.second));
            delete[] array.first;
        } else {
            this->setContent(bytes);
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> ActiveMQObjectMessage::getObjectBytes() const {

    this->failIfWriteOnlyBody();
    try {

        if (this->isCompressed()) {

            int length = 0;
            ByteArrayInputStream is(this->getContent());
            std::vector<unsigned char> uncompressed;

            try {

                DataInputStream dis(&is);
                length = dis.readInt();

                if (length == 0) {
                    return std::vector<unsigned char>();
                }

                uncompressed.resize(length);
            } catch (IOException& ex) {
                throw CMSExceptionSupport::create(ex);
            }

            InflaterInputStream inflater(&is, false);
            inflater.read(&uncompressed[0], length);
            inflater.close();

            return uncompressed;

        } else {
            return this->getContent();
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
