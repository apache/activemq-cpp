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

public class ConsumerIdSourceGenerator extends CommandSourceGenerator {

    protected void generateAdditionalConstructors( PrintWriter out ) {

        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("ConsumerId::ConsumerId(const SessionId& sessionId, long long consumerId) :");
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->connectionId = sessionId.getConnectionId();");
        out.println("    this->sessionId = sessionId.getValue();");
        out.println("    this->value = consumerId;");
        out.println("}");
        out.println("");

        super.generateAdditionalConstructors(out);
    }

    protected String generateInitializerList() {
        return super.generateInitializerList() + ", parentId()";
    }

    protected void generateAdditionalMethods( PrintWriter out ) {
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("const Pointer<SessionId>& ConsumerId::getParentId() const {");
        out.println("    if (this->parentId == NULL) {");
        out.println("        this->parentId.reset(new SessionId(this));");
        out.println("    }");
        out.println("    return this->parentId;");
        out.println("}");
        out.println("");

        super.generateAdditionalMethods(out);
    }

    protected void populateIncludeFilesSet() {

        super.populateIncludeFilesSet();

        Set<String> includes = getIncludeFiles();
        includes.add("<sstream>");
    }

    protected void generateToStringBody( PrintWriter out ) {
        out.println("    ostringstream stream;" );
        out.println("");
        out.println("    stream << this->connectionId << \":\"");
        out.println("           << this->sessionId << \":\"");
        out.println("           << this->value;");
        out.println("");
        out.println("    return stream.str();");
    }

}
