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

public class MessageIdSourceGenerator extends CommandSourceGenerator {

    protected void generateAdditionalConstructors( PrintWriter out ) {

        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("MessageId::MessageId(const std::string& messageKey) :");
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->setValue(messageKey);");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("MessageId::MessageId(const Pointer<ProducerInfo>& producerInfo, long long producerSequenceId) :");
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->producerId = producerInfo->getProducerId();");
        out.println("    this->producerSequenceId = producerSequenceId;");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("MessageId::MessageId(const Pointer<ProducerId>& producerId, long long producerSequenceId) :");
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->producerId = producerId;");
        out.println("    this->producerSequenceId = producerSequenceId;");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("MessageId::MessageId(const std::string& producerId, long long producerSequenceId) :");
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->producerId.reset(new ProducerId(producerId));");
        out.println("    this->producerSequenceId = producerSequenceId;");
        out.println("}");
        out.println("");

        super.generateAdditionalConstructors(out);
    }

    protected String generateInitializerList() {
        return super.generateInitializerList() + ", key(\"\")";
    }

    protected void generateAdditionalMethods( PrintWriter out ) {
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("void MessageId::setValue(const std::string& key) {");
        out.println("");
        out.println("    std::string messageKey = key;");
        out.println("");
        out.println("    // Parse off the sequenceId");
        out.println("    std::size_t p = messageKey.rfind( ':' );");
        out.println("");
        out.println("    if (p != std::string::npos) {");
        out.println("        producerSequenceId = Long::parseLong(messageKey.substr(p + 1, std::string::npos));");
        out.println("        messageKey = messageKey.substr(0, p);");
        out.println("    }");
        out.println("");
        out.println("    this->producerId.reset(new ProducerId(messageKey));");
        out.println("    this->key = messageKey;");
        out.println("}");
        out.println("");

        super.generateAdditionalMethods(out);
    }

    protected void populateIncludeFilesSet() {

        super.populateIncludeFilesSet();

        Set<String> includes = getIncludeFiles();
        includes.add("<decaf/lang/Long.h>");
        includes.add("<sstream>");
    }

    protected void generateToStringBody( PrintWriter out ) {
        out.println("    if (key.empty()) {");
        out.println("        if (!textView.empty()) {");
        out.println("            if (textView.find_first_of(\"ID:\") == 0) {");
        out.println("                key = textView;");
        out.println("            } else {");
        out.println("                key = \"ID:\" + textView;");
        out.println("            }");
        out.println("        } else {");
        out.println("            this->key = this->producerId->toString() + \":\" + ");
        out.println("                        Long::toString(this->producerSequenceId);");
        out.println("        }");
        out.println("    }");
        out.println("");
        out.println("    return this->key;");
    }

}
