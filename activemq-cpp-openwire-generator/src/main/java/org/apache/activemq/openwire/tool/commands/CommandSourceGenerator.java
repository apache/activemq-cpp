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
import java.util.Iterator;
import java.util.Set;

import org.codehaus.jam.JProperty;

public class CommandSourceGenerator extends CommandCodeGenerator {

    public void generate( PrintWriter out ) {

        // Start with the license.
        generateLicence(out);

        populateBaseClassesSet();
        populateIncludeFilesSet();
        for( String include : getIncludeFiles() ) {
            if( include != null ) {
                out.println("#include "+include);
            }
        }
        out.println("");
        out.println("using namespace std;");
        out.println("using namespace activemq;");
        out.println("using namespace activemq::exceptions;");
        out.println("using namespace activemq::commands;");
        out.println("using namespace decaf::lang;");
        out.println("using namespace decaf::lang::exceptions;");
        if( isComparable() ) {
            out.println("using namespace decaf::internal::util;");
        }
        out.println("");
        out.println("/*");
        out.println(" *");
        out.println(" *  Command code for OpenWire format for "+getClassName() );
        out.println(" *");
        out.println(" *  NOTE!: This file is auto generated - do not modify!");
        out.println(" *         if you need to make a change, please see the Java Classes in the");
        out.println(" *         activemq-cpp-openwire-generator module");
        out.println(" *");
        out.println(" */");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println(""+getClassName()+"::"+getClassName()+"() :" );
        out.println("    " + generateInitializerList() + " {");
        out.println("");
        generateDefaultConstructorBody(out);
        out.println("}");
        out.println("");
        if( isAssignable() ) {
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println(""+getClassName()+"::"+getClassName()+"(const "+getClassName()+"& other) :");
            out.println("    " + generateInitializerList() + " {");
            out.println("");
            out.println("    this->copyDataStructure(&other);");
            out.println("}");
            out.println("");
        }
        generateAdditionalConstructors(out);
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println(""+getClassName()+"::~"+getClassName()+"() {");
        generateDestructorBody(out);
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println(getClassName()+"* "+getClassName()+"::cloneDataStructure() const {");
        generateCloneDataStructureBody(out);
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("void "+getClassName()+"::copyDataStructure(const DataStructure* src) {");
        out.println("");
        out.println("    // Protect against invalid self assignment.");
        out.println("    if (this == src) {");
        out.println("        return;");
        out.println("    }");
        out.println("");
        out.println("    const "+getClassName()+"* srcPtr = dynamic_cast<const "+getClassName()+"*>(src);");
        out.println("");
        out.println("    if (srcPtr == NULL || src == NULL) {");
        out.println("        throw decaf::lang::exceptions::NullPointerException(");
        out.println("            __FILE__, __LINE__,");
        out.println("            \""+getClassName()+"::copyDataStructure - src is NULL or invalid\");");
        out.println("    }");
        out.println("");
        if( getBaseClassName() != null ) {
            out.println("    // Copy the data of the base class or classes");
            out.println("    "+ getBaseClassName() +"::copyDataStructure(src);");
            out.println("");
        }
        generateCopyDataStructureBody(out);
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("unsigned char "+getClassName()+"::getDataStructureType() const {");
        out.println("    return "+getClassName()+"::ID_" + getClassName().toUpperCase() + ";");
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("std::string "+getClassName()+"::toString() const {");
        out.println("");
        generateToStringBody(out);
        out.println("}");
        out.println("");
        out.println("////////////////////////////////////////////////////////////////////////////////");
        out.println("bool "+getClassName()+"::equals(const DataStructure* value) const {");
        out.println("");
        out.println("    if (this == value) {");
        out.println("        return true;");
        out.println("    }");
        out.println("");
        out.println("    const "+getClassName()+"* valuePtr = dynamic_cast<const "+getClassName()+"*>(value);");
        out.println("");
        out.println("    if (valuePtr == NULL || value == NULL) {");
        out.println("        return false;");
        out.println("    }");
        out.println("");
        generateEqualsBody(out);
        if( getBaseClassName() != null ) {
            out.println("    if (!"+ getBaseClassName() +"::equals(value)) {");
            out.println("        return false;");
            out.println("    }");
        }
        out.println("    return true;" );
        out.println("}");
        out.println("");
        generatePropertyAccessors(out);
        if( isComparable() ) {
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("int " + getClassName() + "::compareTo(const "+getClassName()+"& value) const {");
            out.println("");
            out.println("    if (this == &value) {");
            out.println("        return 0;");
            out.println("    }");
            out.println("");
            generateCompareToBody(out);
            out.println("    return 0;");
            out.println("}");
            out.println("");
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("bool " + getClassName() + "::equals(const "+getClassName()+"& value) const {");
            out.println("    return this->equals((const DataStructure*)&value);");
            out.println("}");
            out.println("");
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("bool " + getClassName() + "::operator==(const "+getClassName()+"& value) const {");
            out.println("    return this->compareTo(value) == 0;");
            out.println("}");
            out.println("");
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("bool " + getClassName() + "::operator<(const "+getClassName()+"& value) const {");
            out.println("    return this->compareTo(value) < 0;");
            out.println("}");
            out.println("");
        }

        if( isAssignable() ) {
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println(""+getClassName()+"& "+getClassName()+"::operator= (const "+getClassName()+"& other) {");
            out.println("    this->copyDataStructure(&other);");
            out.println("    return *this;");
            out.println("}");
            out.println("");
        }

        if (isHashable()) {
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("int " + getClassName() + "::getHashCode() const {");
            out.println("    return decaf::util::HashCode<std::string>()(this->toString());");
            out.println("}");
            out.println("");
        }

        if( getBaseClassName().equals( "BaseCommand" ) ) {
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("decaf::lang::Pointer<commands::Command> "+getClassName()+"::visit(activemq::state::CommandVisitor* visitor) {");
            out.println("    return visitor->process"+getClassName()+"(this);");
            out.println("}");
        }

        generateAdditionalMethods(out);
    }

    protected void populateIncludeFilesSet() {
        Set<String> includes = getIncludeFiles();
        includes.add("<activemq/commands/"+getClassName()+".h>");
        includes.add("<activemq/state/CommandVisitor.h>");
        includes.add("<activemq/exceptions/ActiveMQException.h>");
        includes.add("<decaf/lang/exceptions/NullPointerException.h>");
        if( isHashable() ) {
            includes.add("<decaf/util/HashCode.h>");
        }
        if( isComparable() ) {
            includes.add("<decaf/internal/util/StringUtils.h>");
        }
    }

    protected void populateBaseClassesSet() {
        Set<String> classes = getBaseClasses();
        classes.add(getBaseClassName());
    }

    protected void generateDefaultConstructorBody( PrintWriter out ) {
    }

    protected String generateInitializerList() {

        StringBuilder result = new StringBuilder();

        Iterator<String> iter = getBaseClasses().iterator();
        while(iter.hasNext()) {
            result.append(iter.next());
            result.append("()");

            if(iter.hasNext()) {
                result.append(", ");
            }
        }

        int lastLineEnds = 0;
        for( JProperty property : getProperties() ) {
            String value = toCppDefaultValue(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);

            result.append(", ");

            int currentLength = result.toString().length();
            if( ( currentLength - lastLineEnds ) >= 120 ) {
                lastLineEnds = currentLength;
                result.append("\n");
                result.append("      ");
            }
            result.append(parameterName + "(" + value + ")" );
        }

        return result.toString();
    }

    protected void generateDestructorBody( PrintWriter out ) {
    }

    protected void generateAdditionalConstructors( PrintWriter out ) {
    }

    protected void generateCloneDataStructureBody( PrintWriter out ) {
        String newInstance = decapitalize( getClassName() );

        out.println("    std::auto_ptr<"+getClassName()+"> "+newInstance+"(new "+getClassName()+"());");
        out.println("");
        out.println("    // Copy the data from the base class or classes");
        out.println("    "+newInstance+"->copyDataStructure(this);");
        out.println("");
        out.println("    return "+newInstance+".release();");
    }

    protected void generateCopyDataStructureBody( PrintWriter out ) {
        for( JProperty property : getProperties() ) {
            String getter = property.getGetter().getSimpleName();
            String setter = property.getSetter().getSimpleName();
            out.println("    this->"+setter+"(srcPtr->"+getter+"());");
        }
    }

    protected void generateToStringBody( PrintWriter out ) {

        out.println("    ostringstream stream;" );
        out.println("");

        if( getBaseClassName().equals( "BaseCommand" ) ) {
            out.println("    stream << \""+getClassName()+" { \"" );
            out.println("           << \"commandId = \" << this->getCommandId() << \", \"");
            out.println("           << \"responseRequired = \" << boolalpha << this->isResponseRequired();");

            if( getProperties().size() > 0 ) {
                out.println("    stream << \", \";");
            }
        } else {
            out.println("    stream << \""+getClassName()+" { \";" );
        }

        int length = getProperties().size();
        int count = 0;

        for( JProperty property : getProperties() ) {

            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);
            String getter = property.getGetter().getSimpleName();

            if( property.getType().getSimpleName().equals("ByteSequence") ) {

                out.println("    stream << \""+propertyName+" = \";");
                out.println("    if (this->"+getter+"().size() > 0) {");
                out.println("        stream << \"[size=\" << this->" + getter + "().size() << \"]\";");
                out.println("    } else {");
                out.println("        stream << \"NULL\";");
                out.println("    }");

            } else if( type.equals("unsigned char") ){
                out.println("    stream << \""+propertyName+" = \" << (int)this->"+getter+"();");

            } else if( property.getType().isPrimitiveType() || type.equals("std::string") ){

                out.println("    stream << \""+propertyName+" = \" << this->"+getter+"();");

            } else if( property.getType().isArrayType() &&
                       !property.getType().getArrayComponentType().isPrimitiveType() ) {

                out.println("    stream << \""+propertyName+" = \";");
                out.println("    if (this->"+getter+"().size() > 0) {");
                out.println("        stream << \"[\";");
                out.println("        for (size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + ") {");
                out.println("            if (this->"+getter+"()[i"+parameterName+"] != NULL) {");
                out.println("                stream << this->"+getter+"()[i"+parameterName+"]->toString() << \", \";");
                out.println("            } else {");
                out.println("                stream << \"NULL\" << \", \";");
                out.println("            }");
                out.println("        }");
                out.println("        stream << \"]\";");
                out.println("    } else {");
                out.println("        stream << \"NULL\";");
                out.println("    }");

            } else if( property.getType().isArrayType() &&
                       property.getType().getArrayComponentType().isPrimitiveType() ) {

                out.println("    stream << \""+propertyName+" = \";");
                out.println("    if (this->"+getter+"().size() > 0) {");
                out.println("        stream << \"[\";");
                out.println("        for (size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + ") {");
                out.println("            stream << this->"+getter+"()[i"+parameterName+"] << \",\";");
                out.println("        }");
                out.println("        stream << \"]\";");
                out.println("    } else {");
                out.println("        stream << \"NULL\";");
                out.println("    }");

            } else {

                out.println("    stream << \""+propertyName+" = \";" );
                out.println("    if (this->"+getter+"() != NULL) {");
                out.println("        stream << this->"+getter+"()->toString();");
                out.println("    } else {");
                out.println("        stream << \"NULL\";");
                out.println("    }");
            }

            if( ++count < length ) {
                out.println("    stream << \", \";");
            }
        }

        out.println("    stream << \" }\";" );
        out.println("");
        out.println("    return stream.str();");
    }

    protected void generateEqualsBody( PrintWriter out ) {
        for( JProperty property : getProperties() ) {
            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);
            String getter = property.getGetter().getSimpleName();

            if( property.getType().getSimpleName().equals("ByteSequence") ) {
                out.println("    for (size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + ") {");
                out.println("        if (this->"+getter+"()[i" + parameterName+"] != valuePtr->"+getter+"()[i"+parameterName+"]) {" );
                out.println("            return false;" );
                out.println("        }" );
                out.println("    }" );
            } else if( property.getType().isPrimitiveType() || type.equals("std::string") ){
                out.println("    if (this->"+getter+"() != valuePtr->"+getter+"()) {");
                out.println("        return false;" );
                out.println("    }" );
            } else if( property.getType().isArrayType() &&
                       !property.getType().getArrayComponentType().isPrimitiveType() ) {

                out.println("    for (size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + ") {");
                out.println("        if (this->"+getter+"()[i"+parameterName+"] != NULL ) {" );
                out.println("            if (!this->"+getter+"()[i"+parameterName+"]->equals(valuePtr->"+getter+"()[i"+parameterName+"].get())) {" );
                out.println("                return false;");
                out.println("            }");
                out.println("        } else if (valuePtr->"+getter+"()[i"+parameterName+"] != NULL) {");
                out.println("            return false;");
                out.println("        }");
                out.println("    }");
            } else if( property.getType().isArrayType() &&
                       property.getType().getArrayComponentType().isPrimitiveType() ) {

                out.println("    for (size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + ") {");
                out.println("        if (this->"+getter+"()[i"+parameterName+"] != valuePtr->"+getter+"()[i"+parameterName+"]) {");
                out.println("            return false;");
                out.println("        }");
                out.println("    }");
            } else {
                out.println("    if (this->"+getter+"() != NULL) {");
                out.println("        if (!this->"+getter+"()->equals(valuePtr->"+getter+"().get())) {" );
                out.println("            return false;");
                out.println("        }");
                out.println("    } else if (valuePtr->"+getter+"() != NULL) {");
                out.println("        return false;");
                out.println("    }");
            }
        }
    }

    protected void generatePropertyAccessors( PrintWriter out ) {
        for( JProperty property : getProperties() ) {

            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);
            String getter = property.getGetter().getSimpleName();
            String setter = property.getSetter().getSimpleName();
            String constNess = "";

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

                type = "decaf::lang::Pointer<" + type + ">&";
                constNess = "const ";
            } else if( property.getType().getSimpleName().equals("String") ||
                       type.startsWith( "std::vector") ) {
                type = type + "&";
                constNess = "const ";
            }

            if( property.getType().isPrimitiveType() ) {
                out.println("////////////////////////////////////////////////////////////////////////////////");
                out.println(type+" "+getClassName()+"::"+getter+"() const {");
                out.println("    return "+parameterName+";");
                out.println("}");
                out.println("");
            } else {
                out.println("////////////////////////////////////////////////////////////////////////////////");
                out.println("const "+type+" "+getClassName()+"::"+getter+"() const {");
                out.println("    return "+parameterName+";");
                out.println("}");
                out.println("");
                out.println("////////////////////////////////////////////////////////////////////////////////");
                out.println(""+type+" "+getClassName()+"::"+getter+"() {");
                out.println("    return "+parameterName+";");
                out.println("}");
                out.println("");
            }
            out.println("////////////////////////////////////////////////////////////////////////////////");
            out.println("void " + getClassName() + "::" + setter+"(" + constNess + type+ " " + parameterName +") {");
            out.println("    this->"+parameterName+" = "+parameterName+";");
            out.println("}");
            out.println("");
        }
    }

    protected void generateCompareToBody( PrintWriter out ) {
        for( JProperty property : getProperties() ) {

            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

                out.println("    int "+parameterName+"Comp = this->"+parameterName+"->compareTo(*(value."+parameterName+"));");
                out.println("    if ("+parameterName+"Comp != 0) {");
                out.println("        return "+parameterName+"Comp;");
                out.println("    }");
                out.println("");
            } else if( property.getType().getSimpleName().equals("String") ) {
                out.println("    int "+parameterName+"Comp = StringUtils::compareIgnoreCase(this->"+parameterName+".c_str(), value."+parameterName+".c_str());");
                out.println("    if ("+parameterName+"Comp != 0) {");
                out.println("        return "+parameterName+"Comp;");
                out.println("    }");
                out.println("");
            } else {
                out.println("    if (this->"+parameterName+" > value."+parameterName+") {");
                out.println("        return 1;");
                out.println("    } else if(this->"+parameterName+" < value."+parameterName+") {");
                out.println("        return -1;");
                out.println("    }");
                out.println("");
            }
        }
    }

    protected void generateAdditionalMethods( PrintWriter out ) {}

}
