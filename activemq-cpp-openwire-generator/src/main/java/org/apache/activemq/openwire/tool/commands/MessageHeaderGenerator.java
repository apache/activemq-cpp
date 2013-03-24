/**
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
package org.apache.activemq.openwire.tool.commands;

import java.io.PrintWriter;
import java.util.Set;

public class MessageHeaderGenerator extends CommandHeaderGenerator {

    protected void populateIncludeFilesSet() {

        super.populateIncludeFilesSet();

        Set<String> includes = getIncludeFiles();
        includes.add("<activemq/util/PrimitiveMap.h>");
        includes.add("<activemq/core/ActiveMQAckHandler.h>");
    }

    protected void generateNamespaceWrapper( PrintWriter out ) {
        out.println("namespace activemq{");
        out.println("namespace core{");
        out.println("    class ActiveMQAckHandler;");
        out.println("    class ActiveMQConnection;");
        out.println("}");
        out.println("namespace commands{");
        out.println("");
    }

    protected void generateProperties( PrintWriter out ) {

        super.generateProperties( out );

        out.println("    private:");
        out.println("");
        out.println("        // Used to allow a client to call Message::acknowledge when in the Client");
        out.println("        // Ack mode.");
        out.println("        Pointer<core::ActiveMQAckHandler> ackHandler;");
        out.println("");
        out.println("        // Message properties, these are Marshaled and Unmarshaled from the Message");
        out.println("        // Command's marshaledProperties vector.");
        out.println("        activemq::util::PrimitiveMap properties;");
        out.println("");
        out.println("        // Indicates if the Message Properties are Read Only");
        out.println("        bool readOnlyProperties;");
        out.println("");
        out.println("        // Indicates if the Message Body are Read Only");
        out.println("        bool readOnlyBody;");
        out.println("");
        out.println("    protected:");
        out.println("");
        out.println("        core::ActiveMQConnection* connection;");
        out.println("");
        out.println("        static const unsigned int DEFAULT_MESSAGE_SIZE = 1024;");
        out.println("");
    }

    protected void generateAdditonalMembers( PrintWriter out ) {

        super.generateAdditonalMembers(out);


        out.println("        /**");
        out.println("         * Create a Pointer based copy of this message.  Useful for chaining a clone");
        out.println("         * operation with other operation such as casting to a cms Message type.");
        out.println("         *");
        out.println("         *   Pointer<cms::Message> cmsMsg = message->copy().dynamic_cast<cms::Message>();");
        out.println("         *");
        out.println("         * @returns a Pointer<Message> which is a duplicate of this object.");
        out.println("         */");
        out.println("        Pointer<Message> copy() const {");
        out.println("            return Pointer<Message>(this->cloneDataStructure());");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Handles the marshaling of the objects properties into the");
        out.println("         * internal byte array before the object is marshaled to the");
        out.println("         * wire");
        out.println("         * @param wireFormat - the wireformat controller");
        out.println("         */");
        out.println("        virtual void beforeMarshal(wireformat::WireFormat* wireFormat AMQCPP_UNUSED);");
        out.println("");
        out.println("        /**");
        out.println("         * Called after unmarshaling is started to cleanup the object being");
        out.println("         * unmarshaled.");
        out.println("         * @param wireFormat - the wireformat object to control unmarshaling");
        out.println("         */");
        out.println("        virtual void afterUnmarshal(wireformat::WireFormat* wireFormat AMQCPP_UNUSED);");
        out.println("");
        out.println("        /**");
        out.println("         * Indicates that this command is aware of Marshaling, and needs");
        out.println("         * to have its Marshaling methods invoked.");
        out.println("         * @returns boolean indicating desire to be in marshaling stages");
        out.println("         */");
        out.println("        virtual bool isMarshalAware() const {");
        out.println("            return true;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Sets the Acknowledgment Handler that this Message will use");
        out.println("         * when the Acknowledge method is called.");
        out.println("         * @param handler ActiveMQAckHandler to call");
        out.println("         */");
        out.println("        virtual void setAckHandler(const Pointer<core::ActiveMQAckHandler>& handler) {");
        out.println("            this->ackHandler = handler;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Gets the Acknowledgment Handler that this Message will use");
        out.println("         * when the Acknowledge method is called.");
        out.println("         * @returns handler ActiveMQAckHandler to call or NULL if not set");
        out.println("         */");
        out.println("        virtual Pointer<core::ActiveMQAckHandler> getAckHandler() const {");
        out.println("            return this->ackHandler;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Sets the ActiveMQConnection instance that this Command was created from");
        out.println("         * when the session create methods are called to create a Message..");
        out.println("         * @param handler ActiveMQConnection parent for this message");
        out.println("         */");
        out.println("        void setConnection(core::ActiveMQConnection* connection) {");
        out.println("            this->connection = connection;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Gets the ActiveMQConnection instance that this Command was created from");
        out.println("         * when the session create methods are called to create a Message..");
        out.println("         * @returns the ActiveMQConnection parent for this Message or NULL if not set.");
        out.println("         */");
        out.println("        core::ActiveMQConnection* getConnection() const {");
        out.println("            return this->connection;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Returns the Size of this message in Bytes.");
        out.println("         * @returns number of bytes this message equates to.");
        out.println("         */");
        out.println("        virtual unsigned int getSize() const;");
        out.println("");
        out.println("        /**");
        out.println("         * Returns if this message has expired, meaning that its");
        out.println("         * Expiration time has elapsed.");
        out.println("         * @returns true if message is expired.");
        out.println("         */");
        out.println("        virtual bool isExpired() const;");
        out.println("");
        out.println("        /**");
        out.println("         * Allows derived Message classes to perform tasks before a message is sent.");
        out.println("         */");
        out.println("        virtual void onSend() {}");
        out.println("");
        out.println("        /**");
        out.println("         * Gets a reference to the Message's Properties object, allows the derived");
        out.println("         * classes to get and set their own specific properties.");
        out.println("         *");
        out.println("         * @return a reference to the Primitive Map that holds message properties.");
        out.println("         */");
        out.println("        util::PrimitiveMap& getMessageProperties() {");
        out.println("            return this->properties;");
        out.println("        }");
        out.println("        const util::PrimitiveMap& getMessageProperties() const {");
        out.println("            return this->properties;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Returns if the Message Properties Are Read Only");
        out.println("         * @return true if Message Properties are Read Only.");
        out.println("         */");
        out.println("        bool isReadOnlyProperties() const {");
        out.println("            return this->readOnlyProperties;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Set the Read Only State of the Message Properties.");
        out.println("         * @param value - true if Properties should be read only.");
        out.println("         */");
        out.println("        void setReadOnlyProperties(bool value) {");
        out.println("            this->readOnlyProperties = value;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Returns if the Message Body is Read Only");
        out.println("         * @return true if Message Content is Read Only.");
        out.println("         */");
        out.println("        bool isReadOnlyBody() const {");
        out.println("            return this->readOnlyBody;");
        out.println("        }");
        out.println("");
        out.println("        /**");
        out.println("         * Set the Read Only State of the Message Content.");
        out.println("         * @param value - true if Content should be read only.");
        out.println("         */");
        out.println("        void setReadOnlyBody(bool value) {");
        out.println("            this->readOnlyBody = value;");
        out.println("        }");
        out.println("");
    }

}
