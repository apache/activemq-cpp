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

#include "StompFrame.h"

#include <string>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Integer.h>

#include <activemq/wireformat/stomp/StompCommandConstants.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StompFrame::StompFrame() :
        command(), properties(), body() {
}

////////////////////////////////////////////////////////////////////////////////
StompFrame::~StompFrame() {
}

////////////////////////////////////////////////////////////////////////////////
StompFrame* StompFrame::clone() const {
    StompFrame* frame = new StompFrame();
    frame->copy(this);
    return frame;
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::copy(const StompFrame* src) {

    this->setCommand(src->getCommand());
    this->properties = src->getProperties();
    this->body = src->getBody();
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::setBody(const unsigned char* bytes, std::size_t numBytes) {

    // Remove old data
    body.clear();
    body.reserve(numBytes);

    // Copy data to internal buffer.
    this->body.insert(this->body.begin(), bytes, bytes + numBytes);
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::toStream(decaf::io::DataOutputStream* stream) const {

    if (stream == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Stream Passed is Null");
    }

    // Write the command.
    const string& cmdString = this->getCommand();
    stream->write((unsigned char*) cmdString.c_str(), (int) cmdString.length(), 0, (int) cmdString.length());
    stream->write('\n');

    // Write all the headers.
    vector<pair<string, string> > headers = this->getProperties().toArray();
    for (std::size_t ix = 0; ix < headers.size(); ++ix) {
        string& name = headers[ix].first;
        string& value = headers[ix].second;

        stream->write((unsigned char*) name.c_str(), (int) name.length(), 0, (int) name.length());
        stream->write(':');
        stream->write((unsigned char*) value.c_str(), (int) value.length(), 0, (int) value.length());
        stream->write('\n');
    }

    // Finish the header section with a form feed.
    stream->write('\n');

    // Write the body.
    const std::vector<unsigned char>& body = this->getBody();
    if (body.size() > 0) {
        stream->write(&body[0], (int) body.size(), 0, (int) body.size());
    }

    if ((this->getBodyLength() == 0) || (this->getProperty(StompCommandConstants::HEADER_CONTENTLENGTH) != "")) {
        stream->write('\0');
    }

    stream->write('\n');

    // Flush the stream.
    stream->flush();
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::fromStream(decaf::io::DataInputStream* in) {

    if (in == NULL) {
        throw decaf::io::IOException(__FILE__, __LINE__, "DataInputStream passed is NULL");
    }

    try {

        // Read the command header.
        readCommandHeader(in);

        // Read the headers.
        readHeaders(in);

        // Read the body.
        readBody(in);
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::readCommandHeader(decaf::io::DataInputStream* in) {

    try {

        std::vector<unsigned char> buffer;

        while (true) {

            // The command header is formatted just like any other stomp header.
            readHeaderLine(buffer, in);

            // Ignore all white space before the command.
            long long offset = -1;
            for (size_t ix = 0; ix < buffer.size() - 1; ++ix) {

                // Find the first non whitespace character
                if (!Character::isWhitespace(buffer[ix])) {
                    offset = (long long) ix;
                    break;
                }
            }

            if (offset >= 0) {
                // Set the command in the frame - copy the memory.
                this->setCommand(reinterpret_cast<char*>(&buffer[(size_t) offset]));
                break;
            }
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::readHeaders(decaf::io::DataInputStream* in) {

    try {

        // Read the command;
        bool endOfHeaders = false;
        std::vector<unsigned char> buffer;

        while (!endOfHeaders) {

            // Read in the next header line.
            std::size_t numChars = readHeaderLine(buffer, in);

            if (numChars == 0) {

                // should never get here
                throw decaf::io::IOException(__FILE__, __LINE__, "StompWireFormat::readStompHeaders: no characters read");
            }

            // Check for an empty line to demark the end of the header section.
            // if its not the end then we have a header to process, so parse it.
            if (numChars == 1 && buffer[0] == '\0') {
                endOfHeaders = true;
            } else {

                // Search through this line to separate the key/value pair.
                for (size_t ix = 0; ix < buffer.size(); ++ix) {

                    // If found the key/value separator...
                    if (buffer[ix] == ':') {

                        // Null-terminate the key.
                        buffer[ix] = '\0';

                        const char* key = reinterpret_cast<char*>(&buffer[0]);
                        const char* value = reinterpret_cast<char*>(&buffer[ix + 1]);

                        // Assign the header key/value pair.
                        if (!this->getProperties().hasProperty(key)) {
                            this->getProperties().setProperty(key, value);
                        }

                        // Break out of the for loop.
                        break;
                    }
                }
            }
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::size_t StompFrame::readHeaderLine(std::vector<unsigned char>& buffer, decaf::io::DataInputStream* in) {

    try {

        // Clear any data from the buffer.
        buffer.clear();

        std::size_t count = 0;

        while (true) {

            // Read the next char from the stream.
            buffer.push_back(in->readByte());

            // Increment the position pointer.
            count++;

            // If we reached the line terminator, return the total number
            // of characters read.
            if (buffer[count - 1] == '\n') {
                // Overwrite the line feed with a null character.
                buffer[count - 1] = '\0';
                return count;
            }
        }

        // If we get here something bad must have happened.
        throw decaf::io::IOException(__FILE__, __LINE__, "StompWireFormat::readStompHeaderLine: "
                "Unrecoverable, error condition");
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
void StompFrame::readBody(decaf::io::DataInputStream* in) {

    try {

        // Clear any data from the body.
        this->body.clear();

        unsigned int content_length = 0;

        if (this->hasProperty(StompCommandConstants::HEADER_CONTENTLENGTH)) {
            string length = this->getProperty(StompCommandConstants::HEADER_CONTENTLENGTH);
            content_length = (unsigned int) Integer::parseInt(length);
        }

        if (content_length != 0) {
            // For this case its assumed that content length indicates how
            // much to read.  We reserve space in the buffer for it to
            // minimize the number of reallocs that might occur.  We are
            // assuming that content length doesn't count the trailing null
            // that indicates the end of frame.  The reserve won't do anything
            // if the buffer already has that much capacity.  The resize call
            // basically sets the end iterator to the correct location since
            // this is a char vector and we already reserve enough space.
            // Resize doesn't realloc the vector smaller if content_length
            // is less than capacity of the buffer, it just move the end
            // iterator.  Reserve adds the benefit that the mem is set to
            // zero.

            this->body.reserve((std::size_t) content_length);
            this->body.resize((std::size_t) content_length);

            // Read the Content Length now
            in->readFully(&body[0], body.size());

            // Content Length read, now pop the end terminator off (\0\n).
            if (in->readByte() != '\0') {
                throw decaf::io::IOException(__FILE__, __LINE__, "StompWireFormat::readStompBody: "
                        "Read Content Length, and no trailing null");
            }

        } else {

            // Content length was either zero, or not set, so we read until the
            // first null is encountered.
            while (true) {

                char byte = in->readByte();
                this->body.push_back(byte);

                if (byte != '\0') {
                    continue;
                }

                break; // Read null and newline we are done.
            }
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}
