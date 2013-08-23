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

public class XATransactionIdSourceGenerator extends CommandSourceGenerator {

    protected void generateAdditionalConstructors( PrintWriter out ) {

        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("XATransactionId::XATransactionId(const cms::Xid* xid) :");
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        out.println("    if (xid == NULL) {");
        out.println("        return;");
        out.println("    }");
        out.println("");
        out.println("    this->formatId = xid->getFormatId();");
        out.println("    this->branchQualifier.resize(cms::Xid::MAXBQUALSIZE);");
        out.println("    this->globalTransactionId.resize(cms::Xid::MAXGTRIDSIZE);");
        out.println("");
        out.println("    int result = xid->getBranchQualifier(&this->branchQualifier[0], cms::Xid::MAXBQUALSIZE);");
        out.println("");
        out.println("    if (result == -1) {");
        out.println("        throw cms::XAException(\"Invalid Xid Branch Qualifier is larger than MAXBQUALSIZE\");");
        out.println("    } else {");
        out.println("        this->branchQualifier.resize( result );");
        out.println("    }");
        out.println("");
        out.println("    result = xid->getGlobalTransactionId(&this->globalTransactionId[0], cms::Xid::MAXGTRIDSIZE);");
        out.println("");
        out.println("    if (result == -1) {");
        out.println("        throw cms::XAException(\"Invalid Xid Global Transaction Id is larger than MAXGTRIDSIZE\");");
        out.println("    } else {");
        out.println("        this->globalTransactionId.resize(result);");
        out.println("    }");
        out.println("}");
        out.println("");

        super.generateAdditionalConstructors(out);
    }

    protected void populateIncludeFilesSet() {

        super.populateIncludeFilesSet();

        Set<String> includes = getIncludeFiles();
        includes.add("<cms/XAException.h>");
    }

    protected void populateBaseClassesSet() {
        super.populateBaseClassesSet();
        Set<String> classes = getBaseClasses();
        classes.add("cms::Xid");
    }

    protected void generateAdditionalMethods( PrintWriter out ) {
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("cms::Xid* XATransactionId::clone() const {");
        out.println("");
        out.println("    XATransactionId* theClone = new XATransactionId();");
        out.println("");
        out.println("    theClone->formatId = this->formatId;");
        out.println("    theClone->globalTransactionId = this->globalTransactionId;");
        out.println("    theClone->branchQualifier = this->branchQualifier;");
        out.println("");
        out.println("    return theClone;");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("bool XATransactionId::equals(const cms::Xid* other) const {");
        out.println("");
        out.println("    if ((void*)this == other) {");
        out.println("        return true;");
        out.println("    }");
        out.println("");
        out.println("    if (other == NULL) {");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    if (this->formatId != other->getFormatId()) {");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    std::vector<unsigned char> otherBQual(Xid::MAXBQUALSIZE);");
        out.println("");
        out.println("    other->getBranchQualifier(&otherBQual[0], Xid::MAXBQUALSIZE);");
        out.println("");
        out.println("    if (this->branchQualifier != otherBQual) {");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    std::vector<unsigned char> otherGTXID( Xid::MAXBQUALSIZE);");
        out.println("");
        out.println("    other->getGlobalTransactionId( &otherGTXID[0], Xid::MAXGTRIDSIZE);");
        out.println("");
        out.println("    if (this->globalTransactionId != otherGTXID) {");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        out.println("    return true;");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("int XATransactionId::getBranchQualifier( unsigned char* buffer, int size ) const {");
        out.println("");
        out.println("    if( size < 0 ) {");
        out.println("        throw cms::XAException(\"Error: Negative size value passed to getBranchQualifier()\");");
        out.println("    }");
        out.println("");
        out.println("    if( buffer == NULL ) {");
        out.println("        throw cms::XAException(\"Error: NULL buffer pointer passed to getBranchQualifier()\");");
        out.println("    }");
        out.println("");
        out.println("    if( size < (int)this->branchQualifier.size() ) {");
        out.println("        return -1;");
        out.println("    }");
        out.println("");
        out.println("    if( this->branchQualifier.size() == 0 ) {");
        out.println("        return 0;");
        out.println("    }");
        out.println("");
        out.println("    for (int i = 0; i < (int)this->branchQualifier.size(); ++i) {");
        out.println("        buffer[i] = this->branchQualifier.at(i);");
        out.println("    }");
        out.println("");
        out.println("    return (int)this->branchQualifier.size();");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("int XATransactionId::getGlobalTransactionId( unsigned char* buffer, int size ) const {");
        out.println("");
        out.println("    if( size < 0 ) {");
        out.println("        throw cms::XAException(\"Error: Negative size value passed to getGlobalTransactionId()\");");
        out.println("    }");
        out.println("");
        out.println("    if( buffer == NULL ) {");
        out.println("        throw cms::XAException(\"Error: NULL buffer pointer passed to getGlobalTransactionId()\");");
        out.println("    }");
        out.println("");
        out.println("    if( size < (int)this->globalTransactionId.size() ) {");
        out.println("        return -1;");
        out.println("    }");
        out.println("");
        out.println("    if( this->globalTransactionId.size() == 0 ) {");
        out.println("        return 0;");
        out.println("    }");
        out.println("");
        out.println("    for (int i = 0; i < (int)this->globalTransactionId.size(); ++i) {");
        out.println("        buffer[i] = this->globalTransactionId.at(i);");
        out.println("    }");
        out.println("");
        out.println("    return (int)this->globalTransactionId.size();");
        out.println("}");
        out.println("");

        super.generateAdditionalMethods(out);
    }

}
