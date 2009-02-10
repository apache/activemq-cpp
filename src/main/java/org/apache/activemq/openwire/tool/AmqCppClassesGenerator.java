/*
 *
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
package org.apache.activemq.openwire.tool;

import java.io.File;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;

import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;

/**
 *
 * @version $Revision: 409828 $
 */
public class AmqCppClassesGenerator extends MultiSourceGenerator {

    protected String targetDir="./src/main";

    protected void processClass(JClass jclass) {
        super.processClass( jclass );
    }

    public Object run() {
        filePostFix = getFilePostFix();
        if (destDir == null) {
            destDir = new File(
                targetDir+"/activemq/connector/openwire/commands");
        }

        return super.run();
    }

    protected String getFilePostFix() {
        return ".cpp";
    }

    protected String getProperBaseClassName( String className, String baseClass ) {

        if( baseClass == null || className == null ) {
            return null;
        }

        // The C++ BaseCommand class is a template, which requires either
        // transport::Command, or transport::Response.
        if( className.equals( "Response" ) ) {
            return "BaseCommand<transport::Response>";
        } else if( baseClass.equals( "BaseCommand" ) ) {
            return "BaseCommand<transport::Command>";
        }

        // No change.
        return baseClass;
    }

    public String toCppType(JClass type) {
        String name = type.getSimpleName();
        if (name.equals("String")) {
            return "std::string";
        }
        else if( type.isArrayType() ) {
            if( name.equals( "byte[]" ) )
                name = "unsigned char[]";

            JClass arrayClass = type.getArrayComponentType();

            if( arrayClass.isPrimitiveType() ) {
                return "std::vector<" + name.substring(0, name.length()-2) + ">";
            } else {
                return "std::vector<" + name.substring(0, name.length()-2) + "*>";
            }
        }
        else if( name.equals( "Throwable" ) || name.equals( "Exception" ) ) {
            return "BrokerError";
        }
        else if( name.equals("BaseDataStructure" ) ){
            return "DataStructure";
        }
        else if( name.equals("ByteSequence") ) {
            return "std::vector<unsigned char>";
        }
        else if( name.equals("boolean") ) {
            return "bool";
        }
        else if( name.equals("long") ) {
            return "long long";
        }
        else if( name.equals("byte") ) {
            return "unsigned char";
        }
        else if( !type.isPrimitiveType() ) {
            return name;
        }
        else {
            return name;
        }
    }

    /**
     * Converts the Java type to a C++ default value
     */
    public String toCppDefaultValue(JClass type) {
        String name = type.getSimpleName();

        if (name.equals("boolean")) {
            return "false";
        } else if( name.equals("String") ) {
            return "\"\"";
        } else if( !type.isPrimitiveType() ) {
            return "NULL";
        } else {
            return "0";
        }
    }

    protected void generateLicence(PrintWriter out) {
out.println("/*");
out.println(" * Licensed to the Apache Software Foundation (ASF) under one or more");
out.println(" * contributor license agreements.  See the NOTICE file distributed with");
out.println(" * this work for additional information regarding copyright ownership.");
out.println(" * The ASF licenses this file to You under the Apache License, Version 2.0");
out.println(" * (the \"License\"); you may not use this file except in compliance with");
out.println(" * the License.  You may obtain a copy of the License at");
out.println(" *");
out.println(" * http://www.apache.org/licenses/LICENSE-2.0");
out.println(" *");
out.println(" * Unless required by applicable law or agreed to in writing, software");
out.println(" * distributed under the License is distributed on an \"AS IS\" BASIS,");
out.println(" * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.");
out.println(" * See the License for the specific language governing permissions and");
out.println(" * limitations under the License.");
out.println(" */");
    }

    protected void generateFile(PrintWriter out) throws Exception {
        generateLicence(out);
out.println("#include <activemq/connector/openwire/commands/"+className+".h>");
out.println("#include <activemq/exceptions/ActiveMQException.h>");
out.println("#include <decaf/lang/exceptions/NullPointerException.h>");
out.println("#include <memory>");
out.println("");
out.println("using namespace std;");
out.println("using namespace activemq;");
out.println("using namespace activemq::exceptions;");
out.println("using namespace activemq::connector;");
out.println("using namespace activemq::connector::openwire;");
out.println("using namespace activemq::connector::openwire::commands;");
out.println("using namespace decaf::lang::exceptions;");
out.println("");
out.println("/*");
out.println(" *");
out.println(" *  Command and marshaling code for OpenWire format for "+className );
out.println(" *");
out.println(" *");
out.println(" *  NOTE!: This file is autogenerated - do not modify!");
out.println(" *         if you need to make a change, please see the Java Classes in the");
out.println(" *         activemq-core module");
out.println(" *");
out.println(" */");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+className+"::"+className+"()");
out.println("{");

        List properties = getProperties();
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            String type = toCppType(property.getType());
            String value = toCppDefaultValue(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);

            if( !type.startsWith("std::vector") ) {
out.println("    this->"+parameterName+" = "+value+";");
            }
        }
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+className+"::~"+className+"()");
out.println("{");

    for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
        JProperty property = (JProperty) iter.next();
        String type = toCppType(property.getType());
        String propertyName = property.getSimpleName();
        String parameterName = decapitalize(propertyName);

        if( property.getType().isPrimitiveType() ||
            property.getType().getSimpleName().equals("String") ) {
            continue;
        }

        if( !type.startsWith("std::vector" ) ) {
out.println("    delete this->" + parameterName + ";");
        } else if( type.contains( "*" ) ) {
out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < " + parameterName + ".size(); ++i" + parameterName + " ) {");
out.println("        delete " + parameterName + "[i" + parameterName + "];");
out.println("    }");
        }
    }
out.println("}");

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(className+"* "+className+"::cloneDataStructure() const {");

    String newInstance = decapitalize( className );

out.println("    std::auto_ptr<"+className+"> "+newInstance+"( new "+className+"() );");
out.println("");
out.println("    // Copy the data from the base class or classes");
out.println("    "+newInstance+"->copyDataStructure( this );");
out.println("");
out.println("    return "+newInstance+".release();");
out.println("}");

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::copyDataStructure( const DataStructure* src ) {");
out.println("");

        if( baseClass != null ) {
out.println("    // Copy the data of the base class or classes");
out.println("    "+getProperBaseClassName( className, baseClass )+"::copyDataStructure( src );");
out.println("");
        }

out.println("    const "+className+"* srcPtr = dynamic_cast<const "+className+"*>( src );");
out.println("");
out.println("    if( srcPtr == NULL || src == NULL ) {");
out.println("");
out.println("        throw decaf::lang::exceptions::NullPointerException(");
out.println("            __FILE__, __LINE__,");
out.println("            \""+className+"::copyDataStructure - src is NULL or invalid\" );");
out.println("    }");

    for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
        JProperty property = (JProperty) iter.next();
        String type = toCppType(property.getType());
        String propertyName = property.getSimpleName();
        String parameterName = decapitalize(propertyName);
        String constNess = "";
        String getter = property.getGetter().getSimpleName();
        String setter = property.getSetter().getSimpleName();

        if( property.getType().isPrimitiveType() ||
            type.equals("std::string") ||
            property.getType().getSimpleName().equals("ByteSequence") ){
    out.println("    this->"+setter+"( srcPtr->"+getter+"() );");
        } else if( property.getType().isArrayType() &&
                   !property.getType().getArrayComponentType().isPrimitiveType() ) {

            String arrayType = property.getType().getArrayComponentType().getSimpleName();

    out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < srcPtr->"+getter+"().size(); ++i" + parameterName + " ) {");
    out.println("        if( srcPtr->"+getter+"()[i"+parameterName+"] != NULL ) {");
    out.println("            this->"+getter+"().push_back(");
    out.println("                dynamic_cast<"+arrayType+"*>(");
    out.println("                    srcPtr->"+getter+"()[i"+parameterName+"]->cloneDataStructure() ) );");
    out.println("        } else {");
    out.println("            this->"+getter+"().push_back( NULL );");
    out.println("        }");
    out.println("    }");
        } else if( property.getType().isArrayType() &&
                   property.getType().getArrayComponentType().isPrimitiveType() ) {
    out.println("    this->"+setter+"( srcPtr->"+getter+"() );");
        } else {
    out.println("    if( srcPtr->"+getter+"() != NULL ) {");
    out.println("        this->"+setter+"(");
    out.println("            dynamic_cast<"+type+"*>(");
    out.println("                srcPtr->"+getter+"()->cloneDataStructure() ) );");
    out.println("    }");
        }
    }

out.println("}");


// getDataStructureType

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("unsigned char "+className+"::getDataStructureType() const {");
out.println("    return "+className+"::ID_" + className.toUpperCase() + ";");
out.println("}");

// toString

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("std::string "+className+"::toString() const {");
out.println("");

out.println("    ostringstream stream;" );
out.println("");
out.println("    stream << \"Begin Class = "+className+"\" << std::endl;" );
out.println("    stream << \" Value of "+className+"::ID_" + className.toUpperCase() + " = "+getOpenWireOpCode(jclass)+"\" << std::endl;");

for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
    JProperty property = (JProperty) iter.next();
    String type = toCppType(property.getType());
    String propertyName = property.getSimpleName();
    String parameterName = decapitalize(propertyName);
    String constNess = "";
    String getter = property.getGetter().getSimpleName();
    String setter = property.getSetter().getSimpleName();

    if( property.getType().getSimpleName().equals("ByteSequence") ) {

out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        stream << \" Value of "+propertyName+"[\" << i" + parameterName+" << \"] = \" << this->"+getter+"()[i"+parameterName+"] << std::endl;" );
out.println("    }" );

    } else if( type.equals("unsigned char") ){

out.println("    stream << \" Value of "+propertyName+" = \" << (int)this->"+getter+"() << std::endl;");

    } else if( property.getType().isPrimitiveType() ||
               type.equals("std::string") ){

out.println("    stream << \" Value of "+propertyName+" = \" << this->"+getter+"() << std::endl;");

    } else if( property.getType().isArrayType() &&
               !property.getType().getArrayComponentType().isPrimitiveType() ) {

        String arrayType = property.getType().getArrayComponentType().getSimpleName();

out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        stream << \" Value of "+propertyName+"[\" << i" + parameterName+" << \"] is Below:\" << std::endl;" );
out.println("        if( this->"+getter+"()[i"+parameterName+"] != NULL ) {");
out.println("            stream << this->"+getter+"()[i"+parameterName+"]->toString() << std::endl;");
out.println("        } else {");
out.println("            stream << \"   Object is NULL\" << std::endl;");
out.println("        }");
out.println("    }");
    } else if( property.getType().isArrayType() &&
               property.getType().getArrayComponentType().isPrimitiveType() ) {
out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        stream << \" Value of "+propertyName+"[\" << i"+parameterName+" << \"] = \" << this->"+getter+"()[i"+parameterName+"] << std::endl;");
out.println("    }");
    } else {
out.println("    stream << \" Value of "+propertyName+" is Below:\" << std::endl;" );
out.println("    if( this->"+getter+"() != NULL ) {");
out.println("        stream << this->"+getter+"()->toString() << std::endl;");
out.println("    } else {");
out.println("        stream << \"   Object is NULL\" << std::endl;");
out.println("    }");
    }
}

    if( baseClass != null ) {
out.println("    stream << "+getProperBaseClassName( className, baseClass )+"::toString();");
    }

out.println("    stream << \"End Class = "+className+"\" << std::endl;" );
out.println("");
out.println("    return stream.str();");
out.println("}");

// equals

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("bool "+className+"::equals( const DataStructure* value ) const {");

out.println("    const "+className+"* valuePtr = dynamic_cast<const "+className+"*>( value );");
out.println("");
out.println("    if( valuePtr == NULL || value == NULL ) {");
out.println("        return false;");
out.println("    }");

for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
    JProperty property = (JProperty) iter.next();
    String type = toCppType(property.getType());
    String propertyName = property.getSimpleName();
    String parameterName = decapitalize(propertyName);
    String constNess = "";
    String getter = property.getGetter().getSimpleName();
    String setter = property.getSetter().getSimpleName();

    if( property.getType().getSimpleName().equals("ByteSequence") ) {

out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        if( this->"+getter+"()[i" + parameterName+"] != valuePtr->"+getter+"()[i"+parameterName+"] ) {" );
out.println("            return false;" );
out.println("        }" );
out.println("    }" );

    } else if( property.getType().isPrimitiveType() ||
               type.equals("std::string") ){

out.println("    if( this->"+getter+"() != valuePtr->"+getter+"() ) {");
out.println("        return false;" );
out.println("    }" );

    } else if( property.getType().isArrayType() &&
               !property.getType().getArrayComponentType().isPrimitiveType() ) {

        String arrayType = property.getType().getArrayComponentType().getSimpleName();

out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        if( this->"+getter+"()[i"+parameterName+"] != NULL ) {" );
out.println("            if( !this->"+getter+"()[i"+parameterName+"]->equals( valuePtr->"+getter+"()[i"+parameterName+"] ) ) {" );
out.println("                return false;");
out.println("            }");
out.println("        } else if( valuePtr->"+getter+"()[i"+parameterName+"] != NULL ) {");
out.println("            return false;");
out.println("        }");
out.println("    }");
    } else if( property.getType().isArrayType() &&
               property.getType().getArrayComponentType().isPrimitiveType() ) {
out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        if( this->"+getter+"()[i"+parameterName+"] != valuePtr->"+getter+"()[i"+parameterName+"] ) {");
out.println("            return false;");
out.println("        }");
out.println("    }");
    } else {
out.println("    if( this->"+getter+"() != NULL ) {");
out.println("        if( !this->"+getter+"()->equals( valuePtr->"+getter+"() ) ) {" );
out.println("            return false;");
out.println("        }");
out.println("    } else if( valuePtr->"+getter+"() != NULL ) {");
out.println("        return false;");
out.println("    }");
    }
}

    if( baseClass != null ) {
out.println("    if( !"+getProperBaseClassName( className, baseClass )+"::equals( value ) ) {");
out.println("        return false;");
out.println("    }");
    }

out.println("    return true;" );
out.println("}");

   if( className.equals( "Message" ) ) {
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("unsigned int "+className+"::getSize() const {");
out.println("");
out.println("    unsigned int size = DEFAULT_MESSAGE_SIZE;");
out.println("");
out.println("    size += (unsigned int)this->getContent().size();");
out.println("    size += (unsigned int)this->getMarshalledProperties().size();");
out.println("");
out.println("    return size;");
out.println("}");
   }

       for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
            JProperty property = (JProperty) iter.next();
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

                type = type + "*";
            } else if( property.getType().getSimpleName().equals("String") ||
                       type.startsWith( "std::vector") ) {
                type = type + "&";
                constNess = "const ";
            }


out.println("");

    if( property.getType().isPrimitiveType() ) {

out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(type+" "+className+"::"+getter+"() const {");
out.println("    return "+parameterName+";");
out.println("}");
out.println("");

    } else {

out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("const "+type+" "+className+"::"+getter+"() const {");
out.println("    return "+parameterName+";");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+type+" "+className+"::"+getter+"() {");
out.println("    return "+parameterName+";");
out.println("}");
out.println("");

    }

out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("void " + className + "::" + setter+"(" + constNess + type+ " " + parameterName +" ) {");
out.println("    this->"+parameterName+" = "+parameterName+";");
out.println("}");
        }
out.println("");
    }

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir(String targetDir) {
        this.targetDir = targetDir;
    }

}
