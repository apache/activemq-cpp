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

public class MessageSourceGenerator extends CommandSourceGenerator {

    protected void populateIncludeFilesSet() {
        super.populateIncludeFilesSet();
        Set<String> includes = getIncludeFiles();
        includes.add("<activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.h>");
        includes.add("<activemq/wireformat/openwire/marshal/PrimitiveTypesMarshaller.h>");
        includes.add("<activemq/core/ActiveMQAckHandler.h>");
        includes.add("<activemq/core/ActiveMQConnection.h>");
        includes.add("<decaf/lang/System.h>");
    }

    protected String generateInitializerList() {
        StringBuilder result = new StringBuilder();

        result.append(super.generateInitializerList());
        result.append(", ackHandler(NULL)");
        result.append(", properties()");
        result.append(", readOnlyProperties(false)");
        result.append(", readOnlyBody(false)");
        result.append(", connection(NULL)");

        return result.toString();
    }

    protected void generateCopyDataStructureBody( PrintWriter out ) {
        super.generateCopyDataStructureBody(out);

        out.println("    this->properties.copy(srcPtr->properties);");
        out.println("    this->setAckHandler(srcPtr->getAckHandler());");
        out.println("    this->setReadOnlyBody(srcPtr->isReadOnlyBody());");
        out.println("    this->setReadOnlyProperties(srcPtr->isReadOnlyProperties());");
        out.println("    this->setConnection(srcPtr->getConnection());");
    }

    protected void generateToStringBody( PrintWriter out ) {
        super.generateToStringBody(out);
    }

    protected void generateEqualsBody( PrintWriter out ) {
        super.generateEqualsBody(out);

        out.println("    if (ackHandler != valuePtr->getAckHandler()){");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    if (!properties.equals(valuePtr->properties)) {");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    if (readOnlyBody != valuePtr->isReadOnlyBody()){");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    if (readOnlyProperties != valuePtr->isReadOnlyProperties()){");
        out.println("        return false;");
        out.println("    }");
        out.println("");
    }

    protected void generateCompareToBody( PrintWriter out ) {
        super.generateCompareToBody(out);
    }

    protected void generateAdditionalMethods( PrintWriter out ) {
        super.generateAdditionalMethods(out);

        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("bool Message::isExpired() const {");
        out.println("    long long expireTime = this->getExpiration();");
        out.println("    long long currentTime = decaf::lang::System::currentTimeMillis();");
        out.println("    if (expireTime > 0 && currentTime > expireTime) {");
        out.println("        return true;");
        out.println("    }");
        out.println("    return false;");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("unsigned int Message::getSize() const {");
        out.println("");
        out.println("    unsigned int size = DEFAULT_MESSAGE_SIZE;");
        out.println("");
        out.println("    size += (unsigned int)this->getContent().size();");
        out.println("    size += (unsigned int)this->getMarshalledProperties().size();");
        out.println("");
        out.println("    return size;");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("void Message::beforeMarshal(wireformat::WireFormat* wireFormat AMQCPP_UNUSED) {");
        out.println("");
        out.println("    try {");
        out.println("        marshalledProperties.clear();");
        out.println("        if (!properties.isEmpty()) {");
        out.println("            wireformat::openwire::marshal::PrimitiveTypesMarshaller::marshal(");
        out.println("                &properties, marshalledProperties );");
        out.println("        }");
        out.println("    }");
        out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)");
        out.println("    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)");
        out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("void Message::afterUnmarshal(wireformat::WireFormat* wireFormat AMQCPP_UNUSED) {");
        out.println("");
        out.println("    try {");
        out.println("        wireformat::openwire::marshal::PrimitiveTypesMarshaller::unmarshal(");
        out.println("            &properties, marshalledProperties);");
        out.println("    }");
        out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)");
        out.println("    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::Exception, decaf::io::IOException)");
        out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)");
        out.println("}");
        out.println("");
    }

}
