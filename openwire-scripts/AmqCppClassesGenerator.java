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

import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;

/**
 * 
 * @version $Revision: 409828 $
 */
public class AmqCppClassesGenerator extends MultiSourceGenerator {

	protected String targetDir="./src/main";

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

    public String toCppType(JClass type) {
        String name = type.getSimpleName();
        if (name.equals("String")) {
            return "std::string";
        }
        else if( type.isArrayType() ) {
            if( name.equals( "byte[]" ) )
                name = "char[]";
            
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
            return "std::vector<char>";
        }
        else if( name.equals("boolean") ) {
            return "bool";
        }
        else if( name.equals("long") ) {
            return "long long";
        }
        else if( name.equals("byte") ) {
            return "char";
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
        } else if ( name.equals("String") ) {
            return "\"\"";
        } else if (!type.isPrimitiveType()) {
            return "NULL";
        } else {
            return "0";
        }
    }

    /**
     * Converts the Java type to the name of the C++ marshal method to be used
     */
    public String toMarshalMethodName(JClass type) {
        String name = type.getSimpleName();
        if (name.equals("String")) {
            return "marshalString";
        } else if (type.isArrayType()) {
            if (type.getArrayComponentType().isPrimitiveType()
                    && name.equals("byte[]"))
                return "marshalByteArray";
            else
                return "marshalObjectArray";
        } else if (name.equals("ByteSequence")) {
            return "marshalByteArray";
        } else if (name.equals("short")) {
            return "marshalShort";
        } else if (name.equals("int")) {
            return "marshalInt";
        } else if (name.equals("long")) {
            return "marshalLong";
        } else if (name.equals("byte")) {
            return "marshalByte";
        } else if (name.equals("double")) {
            return "marshalDouble";
        } else if (name.equals("float")) {
            return "marshalFloat";
        } else if (name.equals("boolean")) {
            return "marshalBoolean";
        } else if (!type.isPrimitiveType()) {
            return "marshalObject";
        } else {
            return name;
        }
    }

    /**
     * Converts the Java type to the name of the C++ unmarshal method to be used
     */
    public String toUnmarshalMethodName(JClass type) {
        String name = type.getSimpleName();
        if (name.equals("String")) {
            return "unmarshalString";
        } else if (type.isArrayType()) {
            if (type.getArrayComponentType().isPrimitiveType()
                    && name.equals("byte[]"))
                return "unmarshalByteArray";
            else
                return "unmarshalObjectArray";
        } else if (name.equals("ByteSequence")) {
            return "unmarshalByteArray";
        } else if (name.equals("short")) {
            return "unmarshalShort";
        } else if (name.equals("int")) {
            return "unmarshalInt";
        } else if (name.equals("long")) {
            return "unmarshalLong";
        } else if (name.equals("byte")) {
            return "unmarshalByte";
        } else if (name.equals("double")) {
            return "unmarshalDouble";
        } else if (name.equals("float")) {
            return "unmarshalFloat";
        } else if (name.equals("boolean")) {
            return "unmarshalBoolean";
        } else if (!type.isPrimitiveType()) {
            return "unmarshalObject";
        } else {
            return name;
        }
    }

    /**
     * Converts the Java type to a C++ pointer cast
     */
    public String toUnmarshalCast(JClass type) {
        String name = toCppType(type);

        if (name.startsWith("p<"))
            return "p_cast<" + name.substring(2);
        else if (name.startsWith("array<")
                && (type.isArrayType() && !type.getArrayComponentType()
                        .isPrimitiveType())
                && !type.getSimpleName().equals("ByteSequence"))
            return "array_cast<" + name.substring(6);
        else
            return "";
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
out.println("#include <activemq/exceptions/NullPointerException.h>");
out.println("");
out.println("using namespace std;");
out.println("using namespace activemq;");
out.println("using namespace activemq::exceptions;");
out.println("using namespace activemq::connector;");
out.println("using namespace activemq::connector::openwire;");
out.println("using namespace activemq::connector::openwire::commands;");
out.println("");
out.println("/*");
out.println(" *");
out.println(" *  Command and marshalling code for OpenWire format for "+className+"");
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
out.println("DataStructure* "+className+"::cloneDataStructure() const {");

    String newInstance = decapitalize( className );

out.println("    "+className+"* "+newInstance+" = new "+className+"();");
out.println("");
out.println("    // Copy the data from the base class or classes");
out.println("    "+newInstance+"->copyDataStructure( this );");
out.println("");
out.println("    return "+newInstance+";");
out.println("}");

out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::copyDataStructure( const DataStructure* src ) {");
out.println("");

        if( baseClass != null ) {
out.println("    // Copy the data of the base class or classes");
out.println("    "+baseClass+"::copyDataStructure( src );");
out.println("");
        }
        
out.println("    const "+className+"* srcPtr = dynamic_cast<const "+className+"*>( src );");
out.println("");
out.println("    if( srcPtr == NULL || src == NULL ) {");
out.println("    ");
out.println("        throw exceptions::NullPointerException(");
out.println("            __FILE__, __LINE__,");
out.println("            \""+className+"::copyDataStructure - src is NULL or invalid\" );");
out.println("    }");

    for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
        JProperty property = (JProperty) iter.next();
        String type = toCppType(property.getType());
        String propertyName = property.getSimpleName();
        String parameterName = decapitalize(propertyName);
        String constNess = "";
    
        if( property.getType().isPrimitiveType() ||
            type.equals("std::string") ||
            property.getType().getSimpleName().equals("ByteSequence") ){
    out.println("    this->set"+propertyName+"( srcPtr->get"+propertyName+"() );");
        } else if( property.getType().isArrayType() &&
                   !property.getType().getArrayComponentType().isPrimitiveType() ) {
    out.println("    for( size_t i" + parameterName + " = 0; i" + parameterName + " < srcPtr->get"+propertyName+"().size(); ++i" + parameterName + " ) {");
    out.println("        this->get"+propertyName+"().push_back( ");
    out.println("            srcPtr->get"+propertyName+"()[i"+parameterName+"]->cloneDataStructure() );");            
    out.println("    }");
        } else if( property.getType().isArrayType() &&
                   property.getType().getArrayComponentType().isPrimitiveType() ) {
    out.println("    this->set"+propertyName+"( srcPtr->get"+propertyName+"() );");
        } else {
    out.println("    this->set"+propertyName+"( ");
    out.println("        dynamic_cast<"+type+"*>( ");
    out.println("            srcPtr->get"+propertyName+"()->cloneDataStructure() ) );");
        }
    }

out.println("}");


out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("unsigned char "+className+"::getDataStructureType() const {");
out.println("    return "+className+"::ID_" + className.toUpperCase() + "; ");
out.println("}");

       for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
			JProperty property = (JProperty) iter.next();
	        String type = toCppType(property.getType());
	        String propertyName = property.getSimpleName();
	        String parameterName = decapitalize(propertyName);
            String constNess = "";

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") && 
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {
                   
                type = type + "*";
            } else if( property.getType().getSimpleName().equals("String") ) {
                type = type + "&";
                constNess = "const ";
            }

           
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("const "+type+" "+className+"::get"+propertyName+"() const {");
out.println("    return "+parameterName+";");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println(""+type+" "+className+"::get"+propertyName+"() {");
out.println("    return "+parameterName+";");
out.println("}");
out.println("");
out.println("////////////////////////////////////////////////////////////////////////////////");
out.println("void " + className + "::set" + propertyName+"(" + constNess + type+ " " + parameterName +" ) {");
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
