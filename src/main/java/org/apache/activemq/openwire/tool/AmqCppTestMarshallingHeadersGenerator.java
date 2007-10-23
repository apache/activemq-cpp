/**
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
import java.util.ArrayList;

import org.codehaus.jam.JClass;

/**
 *
 * @version $Revision: 381410 $
 */
public class AmqCppTestMarshallingHeadersGenerator extends JavaMarshallingGenerator {

    protected String targetDir="./src/main";

    protected void processClass(JClass jclass) {

        if( isAbstractClass() ) {
            return;
        }

        super.processClass( jclass );
    }

    protected void processFactory() {}

    public Object run() {

        filePostFix = getFilePostFix();
        if (destDir == null) {
            destDir = new File(targetDir+"/activemq/connector/openwire/marshal/v"+getOpenwireVersion());
        }
        return super.run();
    }

    protected String getClassName(JClass jclass) {
        return super.getClassName(jclass) + "Test";
    }

    protected String getTargetClassName(JClass jclass) {
        return super.getClassName(jclass);
    }

    protected String getFilePostFix() {
        return ".h";
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
            return "std::vector<char>";
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

out.println("");
out.println("#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_"+className.toUpperCase()+"_H_");
out.println("#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_"+className.toUpperCase()+"_H_");
out.println("");
out.println("// Turn off warning message for ignored exception specification");
out.println("#ifdef _MSC_VER");
out.println("#pragma warning( disable : 4290 )");
out.println("#endif");
out.println("");
out.println("#include <cppunit/TestFixture.h>");
out.println("#include <cppunit/extensions/HelperMacros.h>");
out.println("");
out.println("namespace activemq{");
out.println("namespace connector{");
out.println("namespace openwire{");
out.println("namespace marshal{");
out.println("namespace v"+getOpenwireVersion()+"{");
out.println("");
out.println("    /**");
out.println("     * Marshalling Test code for Open Wire Format for "+className);
out.println("     *");
out.println("     *  NOTE!: This file is autogenerated - do not modify!");
out.println("     *         if you need to make a change, please see the Java Classes");
out.println("     *         in the activemq-openwire-generator module");
out.println("     */");
out.println("    class "+className+" : public CppUnit::TestFixture {" );
out.println("");
out.println("        CPPUNIT_TEST_SUITE( "+className+" );");
out.println("        CPPUNIT_TEST( test );");
out.println("        CPPUNIT_TEST( testLooseMarshal );");
out.println("        CPPUNIT_TEST( testTightMarshal );");
out.println("        CPPUNIT_TEST_SUITE_END();");
out.println("");
out.println("    public:");
out.println("");
out.println("        "+className+"() {}");
out.println("        virtual ~"+className+"() {}");
out.println("");
out.println("        /**");
out.println("         * Test the marshaller and its marshalled type.");
out.println("         */");
out.println("        virtual void test();");
out.println("        virtual void testLooseMarshal();");
out.println("        virtual void testTightMarshal();");
out.println("");
out.println("    };");
out.println("");
out.println("}}}}}");
out.println("");
out.println("#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_"+className.toUpperCase()+"_H_*/");
out.println("");
        }

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir(String targetDir) {
        this.targetDir = targetDir;
    }
}
