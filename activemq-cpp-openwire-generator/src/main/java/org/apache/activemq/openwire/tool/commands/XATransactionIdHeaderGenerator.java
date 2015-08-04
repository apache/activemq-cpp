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

public class XATransactionIdHeaderGenerator extends CommandHeaderGenerator {

    protected void populateIncludeFilesSet() {
        Set<String> includes = getIncludeFiles();
        includes.add("<cms/Xid.h>");

        super.populateIncludeFilesSet();
    }

    protected void generateAdditionalConstructors(PrintWriter out) {
        out.println("        " + getClassName() + "( const cms::Xid* xid );");
        out.println("");

        super.generateAdditionalConstructors(out);
    }

    protected void populateBaseClassesSet() {
        super.populateBaseClassesSet();
        Set<String> classes = getBaseClasses();
        classes.add("cms::Xid");
    }

    protected void generateProperties(PrintWriter out) {
        out.println("    protected:");
        out.println("");

        out.println("        using TransactionId::compareTo;");
        out.println("        using TransactionId::equals;");
        out.println("        using TransactionId::operator <;");
        out.println("        using TransactionId::operator==;");

        out.println("");

        super.generateProperties(out);
    }

    protected void generateAdditonalMembers(PrintWriter out) {
        out.println("        virtual bool isXATransactionId() const {");
        out.println("            return true;");
        out.println("        }");
        out.println("");

        out.println("    public:  // Xid interface implementation.");
        out.println("");
        out.println("        virtual Xid* clone() const;");
        out.println("");
        out.println("        virtual bool equals(const Xid* other) const;");
        out.println("");
        out.println("        virtual int getBranchQualifier(unsigned char* buffer, int size) const;");
        out.println("");
        out.println("        virtual int getGlobalTransactionId(unsigned char* buffer, int size) const;");
        out.println("");

        super.generateAdditonalMembers(out);
    }
}
