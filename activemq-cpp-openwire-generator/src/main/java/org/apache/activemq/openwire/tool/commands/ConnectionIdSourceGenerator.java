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

public class ConnectionIdSourceGenerator extends CommandSourceGenerator {

    protected void populateIncludeFilesSet() {
        Set<String> includes = getIncludeFiles();
        includes.add("<activemq/commands/SessionId.h>");
        includes.add("<activemq/commands/ProducerId.h>");
        includes.add("<activemq/commands/ConsumerId.h>");

        super.populateIncludeFilesSet();
    }

    protected void generateAdditionalConstructors( PrintWriter out ) {
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("ConnectionId::ConnectionId(const SessionId* sessionId)");
        out.println("    : " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->value = sessionId->getConnectionId();");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("ConnectionId::ConnectionId(const ProducerId* producerId)");
        out.println("    : " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->value = producerId->getConnectionId();");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("ConnectionId::ConnectionId(const ConsumerId* consumerId)");
        out.println("    : " + generateInitializerList() + " {");
        out.println("");
        out.println("    this->value = consumerId->getConnectionId();");
        out.println("}");
        out.println("");

        super.generateAdditionalConstructors(out);
    }

    protected void generateToStringBody( PrintWriter out ) {
        out.println("    return this->value;");
    }
}
