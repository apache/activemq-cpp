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
                targetDir+"/activemq/commands");
        }

        return super.run();
    }

    protected String getFilePostFix() {
        return ".cpp";
    }

    public String toHeaderFileName( JClass type ) {
        String name = type.getSimpleName();

        if( name.equals( "String" ) ) {
            return null;
        } else if( type.isArrayType() ) {
            JClass arrayClass = type.getArrayComponentType();
            return toHeaderFileName( arrayClass );
        } else if( name.equals( "Throwable" ) || name.equals( "Exception" ) ) {
            return "BrokerError";
        } else if( name.equals("BaseDataStructure" ) ){
            return "DataStructure";
        } else if( name.equals("ByteSequence") ) {
            return "std::vector<unsigned char>";
        } else if( !type.isPrimitiveType() ) {
            return name;
        } else {
            return null;
        }
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
                return "std::vector<" + name.substring( 0, name.length()-2 ) + ">";
            } else {
                return "std::vector< decaf::lang::Pointer<" +
                       name.substring( 0, name.length()-2 ) + "> >";
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

        boolean comparable = className.endsWith("Id");
        boolean assignable = className.endsWith("Id");

out.println("#include <activemq/commands/"+className+".h>");
out.println("#include <activemq/state/CommandVisitor.h>");
out.println("#include <activemq/exceptions/ActiveMQException.h>");
out.println("#include <decaf/lang/exceptions/NullPointerException.h>");

        if( className.equals( "Message" ) ) {
out.println("#include <activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.h>");
out.println("#include <activemq/wireformat/openwire/marshal/PrimitiveMapMarshaller.h>");
        }

        if( comparable ) {
out.println("#include <apr_strings.h>");
        }

out.println("");
out.println("using namespace std;");
out.println("using namespace activemq;");
out.println("using namespace activemq::exceptions;");
out.println("using namespace activemq::commands;");
out.println("using namespace decaf::lang;");
out.println("using namespace decaf::lang::exceptions;");
out.println("");
out.println("/*");
out.println(" *");
out.println(" *  Command and marshaling code for OpenWire format for "+className );
out.println(" *");
out.println(" *");
out.println(" *  NOTE!: This file is auto generated - do not modify!");
out.println(" *         if you need to make a change, please see the Java Classes in the");
out.println(" *         activemq-core module");
out.println(" *");
out.println(" */");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+className+"::"+className+"() {");
out.println("");

        if( className.equals( "Message" ) ) {
out.println("    this->ackHandler = NULL;");
        }

        List properties = getProperties();
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            String type = toCppType(property.getType());
            String value = toCppDefaultValue(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);

            if( property.getType().isPrimitiveType() ||
                type.startsWith("std::string") ) {

out.println("    this->"+parameterName+" = "+value+";");
            }
        }
out.println("}");
out.println("");
    if( assignable ) {
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+className+"::"+className+"( const "+className+"& other ) {");
out.println("    this->copyDataStructure( &other );");
out.println("}");
out.println("");
    }
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+className+"::~"+className+"() {");
out.println("");
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
out.println("    // Protect against invalid self assignment.");
out.println("    if( this == src ) {");
out.println("        return;");
out.println("    }");
out.println("");
out.println("    const "+className+"* srcPtr = dynamic_cast<const "+className+"*>( src );");
out.println("");
out.println("    if( srcPtr == NULL || src == NULL ) {");
out.println("        throw decaf::lang::exceptions::NullPointerException(");
out.println("            __FILE__, __LINE__,");
out.println("            \""+className+"::copyDataStructure - src is NULL or invalid\" );");
out.println("    }");
out.println("");

            if( baseClass != null ) {
    out.println("    // Copy the data of the base class or classes");
    out.println("    "+ baseClass +"::copyDataStructure( src );");
    out.println("");
            }

        if( className.equals( "Message" ) ) {
out.println("    this->properties.copy( srcPtr->properties );");
out.println("    this->setAckHandler( srcPtr->getAckHandler() );");
        }

        for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
             JProperty property = (JProperty) iter.next();
             String getter = property.getGetter().getSimpleName();
             String setter = property.getSetter().getSimpleName();

             if( className.equals( "Message" ) &&
                 property.getType().getSimpleName().equals( "MessageId" ) ) {
out.println("    this->"+setter+"( Pointer<MessageId>( new MessageId( *( srcPtr->"+getter+"() ) ) ) );" );

             } else {
out.println("    this->"+setter+"( srcPtr->"+getter+"() );");
             }
    }

out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("unsigned char "+className+"::getDataStructureType() const {");
out.println("    return "+className+"::ID_" + className.toUpperCase() + ";");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("std::string "+className+"::toString() const {");
out.println("");

out.println("    ostringstream stream;" );
out.println("");
out.println("    stream << \"Begin Class = "+className+"\" << std::endl;" );
out.println("    stream << \" Value of "+className+"::ID_" + className.toUpperCase() + " = "+getOpenWireOpCode(jclass)+"\" << std::endl;");

        if( className.equals( "Message" ) ) {
out.println("    stream << \" Value of ackHandler = \" << ackHandler << std::endl;");
out.println("    stream << \" Value of properties = \" << this->properties.toString() << std::endl;");
        }

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
out.println("    stream << "+ baseClass +"::toString();");
    }

out.println("    stream << \"End Class = "+className+"\" << std::endl;" );
out.println("");
out.println("    return stream.str();");
out.println("}");

// equals

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("bool "+className+"::equals( const DataStructure* value ) const {");

out.println("");
out.println("    if( this == value ) {");
out.println("        return true;");
out.println("    }");
out.println("");
out.println("    const "+className+"* valuePtr = dynamic_cast<const "+className+"*>( value );");
out.println("");
out.println("    if( valuePtr == NULL || value == NULL ) {");
out.println("        return false;");
out.println("    }");
out.println("");

        if( className.equals( "Message" ) ) {
out.println("    if( ackHandler != valuePtr->getAckHandler() ){");
out.println("        return false;");
out.println("    }");
out.println("");
out.println("    if( !properties.equals( valuePtr->properties ) ) {");
out.println("        return false;");
out.println("    }");
out.println("");
        }

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

    } else if( property.getType().isPrimitiveType() || type.equals("std::string") ){

out.println("    if( this->"+getter+"() != valuePtr->"+getter+"() ) {");
out.println("        return false;" );
out.println("    }" );

    } else if( property.getType().isArrayType() &&
               !property.getType().getArrayComponentType().isPrimitiveType() ) {

        String arrayType = property.getType().getArrayComponentType().getSimpleName();

out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < this->"+getter+"().size(); ++i" + parameterName + " ) {");
out.println("        if( this->"+getter+"()[i"+parameterName+"] != NULL ) {" );
out.println("            if( !this->"+getter+"()[i"+parameterName+"]->equals( valuePtr->"+getter+"()[i"+parameterName+"].get() ) ) {" );
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
out.println("        if( !this->"+getter+"()->equals( valuePtr->"+getter+"().get() ) ) {" );
out.println("            return false;");
out.println("        }");
out.println("    } else if( valuePtr->"+getter+"() != NULL ) {");
out.println("        return false;");
out.println("    }");
    }
}

    if( baseClass != null ) {
out.println("    if( !"+ baseClass +"::equals( value ) ) {");
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

   if( baseClass.equals( "BaseCommand" ) ) {
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("decaf::lang::Pointer<commands::Command> "+className+"::visit( activemq::state::CommandVisitor* visitor ) ");
out.println("    throw( exceptions::ActiveMQException ) {");
out.println("");
out.println("    return visitor->process"+className+"( this );");
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

                type = "decaf::lang::Pointer<" + type + ">&";
                constNess = "const ";
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
out.println("void " + className + "::" + setter+"( " + constNess + type+ " " + parameterName +" ) {");
out.println("    this->"+parameterName+" = "+parameterName+";");
out.println("}");
        }
out.println("");

        if( comparable ) {
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("int " + className + "::compareTo( const "+className+"& value ) const {");
out.println("");
out.println("    if( this == &value ) {");
out.println("        return 0;");
out.println("    }");
out.println("");

        for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
            JProperty property = (JProperty) iter.next();
            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);
            String getter = property.getGetter().getSimpleName();
            String setter = property.getSetter().getSimpleName();

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

out.println("    int "+parameterName+"Comp = this->"+parameterName+"->compareTo( *( value."+parameterName+" ) );");
out.println("    if( "+parameterName+"Comp != 0 ) {");
out.println("        return "+parameterName+"Comp;");
out.println("    }");
out.println("");

            } else if( property.getType().getSimpleName().equals("String") ) {

out.println("    int "+parameterName+"Comp = apr_strnatcasecmp( this->"+parameterName+".c_str(), value."+parameterName+".c_str() );");
out.println("    if( "+parameterName+"Comp != 0 ) {");
out.println("        return "+parameterName+"Comp;");
out.println("    }");
out.println("");
            } else {

out.println("    if( this->"+parameterName+" > value."+parameterName+" ) {");
out.println("        return 1;");
out.println("    } else if( this->"+parameterName+" < value."+parameterName+" ) {");
out.println("        return -1;");
out.println("    }");
out.println("");

            }
        }

out.println("    return 0;");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("bool " + className + "::equals( const "+className+"& value ) const {");
out.println("    return this->equals( &value );");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("bool " + className + "::operator==( const "+className+"& value ) const {");
out.println("    return this->compareTo( value ) == 0;");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("bool " + className + "::operator<( const "+className+"& value ) const {");
out.println("    return this->compareTo( value ) < 0;");
out.println("}");
        }

        if( assignable ) {
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+className+"& "+className+"::operator= ( const "+className+"& other ) {");
out.println("    this->copyDataStructure( &other );");
out.println("}");
out.println("");
        }

        if( className.equals( "Message" ) ) {

out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("    void Message::beforeMarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )");
out.println("        throw ( decaf::io::IOException ) {");
out.println("");
out.println("        try{");
out.println("");
out.println("            marshalledProperties.clear();");
out.println("            if( !properties.isEmpty() )");
out.println("            {");
out.println("                wireformat::openwire::marshal::PrimitiveMapMarshaller::marshal(");
out.println("                    &properties, marshalledProperties );");
out.println("            }");
out.println("        }");
out.println("        AMQ_CATCH_RETHROW( decaf::io::IOException )");
out.println("        AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )");
out.println("        AMQ_CATCHALL_THROW( decaf::io::IOException )");
out.println("    }");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("    void Message::afterUnmarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )");
out.println("        throw ( decaf::io::IOException ) {");
out.println("");
out.println("        try{");
out.println("");
out.println("            wireformat::openwire::marshal::PrimitiveMapMarshaller::unmarshal(");
out.println("                &properties, marshalledProperties );");
out.println("        }");
out.println("        AMQ_CATCH_RETHROW( decaf::io::IOException )");
out.println("        AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )");
out.println("        AMQ_CATCHALL_THROW( decaf::io::IOException )");
out.println("    }");
out.println("");
        }
    }

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir(String targetDir) {
        this.targetDir = targetDir;
    }

}
