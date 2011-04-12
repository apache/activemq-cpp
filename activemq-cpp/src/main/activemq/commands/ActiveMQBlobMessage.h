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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQBLOBMESSAGE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQBLOBMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQMessageTemplate.h>
#include <cms/Message.h>
#include <string>
#include <memory>

namespace activemq {
namespace commands {

    class AMQCPP_API ActiveMQBlobMessage :
        public ActiveMQMessageTemplate< cms::Message > {

    private:

        std::string remoteBlobUrl;
        std::string mimeType;
        std::string name;
        bool deletedByBroker;

    public:

        static const unsigned char ID_ACTIVEMQBLOBMESSAGE = 29;
        static const std::string BINARY_MIME_TYPE;

    private:

        ActiveMQBlobMessage( const ActiveMQBlobMessage& );
        ActiveMQBlobMessage& operator= ( const ActiveMQBlobMessage& );

    public:

        ActiveMQBlobMessage();
        virtual ~ActiveMQBlobMessage() throw() {}

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQBlobMessage* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

    public:  // cms::Message

        virtual cms::Message* clone() const {
            return dynamic_cast<cms::Message*>( this->cloneDataStructure() );
        }

    public: // BlobMessage

        /**
         * Get the Remote URL of the Blob.
         * @returns string from of the Remote Blob URL.
         */
        std::string getRemoteBlobUrl() const {
            return this->remoteBlobUrl;
        }

        /**
         * Set the Remote URL of the Blob
         * @param remoteURL - String form of the Remote URL.
         */
        void setRemoteBlobUrl( const std::string& remoteURL ) {
            this->remoteBlobUrl = remoteURL;
        }

        /**
         * Get the Mime Type of the Blob.
         * @returns string holding the MIME Type.
         */
        std::string getMimeType() const {
            return this->mimeType;
        }

        /**
         * Set the Mime Type of the Blob
         * @param mimeType - String holding the MIME Type.
         */
        void setMimeType( const std::string& mimeType ) {
            this->mimeType = mimeType;
        }

        /**
         * Gets the Name of the Blob.
         * @returns string name of the Blob.
         */
        std::string getName() const {
            return this->name;
        }

        /**
         * Sets the Name of the Blob.
         * @param name - Name of the Blob.
         */
        void setName( const std::string& name ) {
            this->name = name;
        }

        /**
         * Gets if this Blob is deleted by the Broker.
         * @return true if the Blob is deleted by the Broker.
         */
        bool isDeletedByBroker() const {
            return this->deletedByBroker;
        }

        /**
         * Sets the Deleted By Broker flag.
         * @param value - set the Delete by broker flag to value.
         */
        void setDeletedByBroker( bool value ) {
            this->deletedByBroker = value;
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQBLOBMESSAGE_H_*/
