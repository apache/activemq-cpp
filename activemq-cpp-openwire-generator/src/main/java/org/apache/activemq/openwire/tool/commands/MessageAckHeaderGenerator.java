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

public class MessageAckHeaderGenerator extends CommandHeaderGenerator {

    protected void populateIncludeFilesSet() {
        Set<String> includes = getIncludeFiles();
        includes.add("<activemq/commands/Message.h>");
        includes.add("<activemq/commands/MessageDispatch.h>");

        super.populateIncludeFilesSet();
    }

    protected void generateAdditionalConstructors(PrintWriter out) {

        out.println("        "+getClassName()+"(const Pointer<Message>& message, int ackType, int messageCount);");
        out.println("");
        out.println("        "+getClassName()+"(const Pointer<MessageDispatch>& dispatch, int ackType, int messageCount);");
        out.println("");

        super.generateAdditionalConstructors(out);
    }

    protected void generateAdditonalMembers(PrintWriter out) {
        out.println("        bool isPoisonAck();");
        out.println("");
        out.println("        bool isStandardAck();");
        out.println("");
        out.println("        bool isDeliveredAck();");
        out.println("");
        out.println("        bool isRedeliveredAck();");
        out.println("");
        out.println("        bool isIndividualAck();");
        out.println("");
        out.println("        bool isUnmatchedAck();");
        out.println("");
        out.println("        bool isExpiredAck();");
        out.println("");

        super.generateAdditonalMembers( out );
    }
}
