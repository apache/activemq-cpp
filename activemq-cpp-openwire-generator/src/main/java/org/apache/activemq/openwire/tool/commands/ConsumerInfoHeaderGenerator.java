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

public class ConsumerInfoHeaderGenerator extends CommandHeaderGenerator {

    protected void populateIncludeFilesSet() {

        Set<String> includes = getIncludeFiles();
        includes.add("<activemq/commands/RemoveInfo.h>");

        super.populateIncludeFilesSet();
    }

    protected void generateProperties( PrintWriter out ) {

        super.generateProperties(out);

        out.println("    private:");
        out.println("");
        out.println("        // Prefetch size that is controlled by the Broker." );
        out.println("        int currentPrefetchSize;");
        out.println("");
    }

    protected void generateAdditonalMembers( PrintWriter out ) {
        out.println("        Pointer<RemoveInfo> createRemoveCommand() const;");
        out.println("");
        out.println("        int getCurrentPrefetchSize() const {");
        out.println("            return this->currentPrefetchSize;");
        out.println("        }");
        out.println("");
        out.println("        void setCurrentPrefetchSize(int currentPrefetchSize) {");
        out.println("            this->currentPrefetchSize = currentPrefetchSize;");
        out.println("        }");
        out.println("");

        super.generateAdditonalMembers( out );
    }
}
