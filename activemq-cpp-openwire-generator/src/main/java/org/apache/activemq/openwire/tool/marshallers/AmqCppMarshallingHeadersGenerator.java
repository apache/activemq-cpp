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
package org.apache.activemq.openwire.tool.marshallers;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import org.apache.activemq.openwire.tool.MultiSourceGenerator;
import org.codehaus.jam.JAnnotation;
import org.codehaus.jam.JAnnotationValue;
import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;

/**
 *
 * @version $Revision: 381410 $
 */
public class AmqCppMarshallingHeadersGenerator extends MultiSourceGenerator {

    protected String targetDir="./src/main";
    protected List<JClass> concreteClasses = new ArrayList<JClass>();
    protected File factoryFile;
    protected String factoryFileName = "MarshallerFactory";
    protected String indent = "    ";

    /**
     * Overrides the base class init since we don't mark any marshaller classes as
     * being manually maintained.
     */
    protected void initialiseManuallyMaintainedClasses() {
    }

    /**
     * Returns all the valid properties available on the current class. Overrides the
     * method in {@link MultiSourceGenerator} to add filtering on the Openwire Version
     * number so that we can rerun this tool for older versions and produce an exact
     * match to what was previously generated.
     *
     * @return List of Properties valid for the current {@link JClass} and Openwire version.
     */
    public List<JProperty> getProperties() {
        List<JProperty> answer = new ArrayList<JProperty>();
        JProperty[] properties = jclass.getDeclaredProperties();
        for (int i = 0; i < properties.length; i++) {
            JProperty property = properties[i];
            if (isValidProperty(property)) {

                JAnnotation annotation = property.getAnnotation("openwire:property");
                JAnnotationValue version = annotation.getValue("version");

                if( version.asInt() <= this.getOpenwireVersion() ) {
                    answer.add(property);
                }
            }
        }
        return answer;
    }

    public Object run() {
        filePostFix = getFilePostFix();
        if (destDir == null) {
            destDir = new File(targetDir+"/activemq/wireformat/openwire/marshal/generated");
        }
        Object answer = super.run();
        processFactory();
        return answer;
    }

    protected void processClass(JClass jclass) {
        super.processClass(jclass);

        if (!jclass.isAbstract()) {
            concreteClasses.add(jclass);
        }
    }

    protected String getClassName(JClass jclass) {
        return super.getClassName(jclass) + "Marshaller";
    }

    protected String getBaseClassName(JClass jclass) {
        String answer = jclass.getSimpleName();

        if( answer.equals("ActiveMQTextMessage") ) {
            answer = "MessageMarshaller";
        } else if( answer.equals("ActiveMQBytesMessage") ) {
            answer = "MessageMarshaller";
        } else if( answer.equals("ActiveMQMapMessage") ) {
            answer = "MessageMarshaller";
        } else if( answer.equals("ActiveMQObjectMessage") ) {
            answer = "MessageMarshaller";
        } else if( answer.equals("ActiveMQStreamMessage") ) {
            answer = "MessageMarshaller";
        } else if( answer.equals("ActiveMQBlobMessage") ) {
            answer = "MessageMarshaller";
        }

        // We didn't map it directly so we turn it into something generic.
        if( answer.equals( jclass.getSimpleName() ) ) {
            answer = "BaseDataStreamMarshaller";
            JClass superclass = jclass.getSuperclass();
            if (superclass != null) {
                String superName = superclass.getSimpleName();
                if (!superName.equals("Object") && !superName.equals("JNDIBaseStorable") && !superName.equals("DataStructureSupport")) {
                    answer = superName + "Marshaller";
                }
            }
            return answer;
        }

        return answer;
    }

    public boolean isMarshallAware(JClass j) {

        String answer = jclass.getSimpleName();

        if( answer.equals("ActiveMQTextMessage") ) {
            return true;
        } else if( answer.equals("ActiveMQBytesMessage") ) {
            return true;
        } else if( answer.equals("ActiveMQMapMessage") ) {
            return true;
        } else if( answer.equals("ActiveMQObjectMessage") ) {
            return true;
        } else if( answer.equals("ActiveMQStreamMessage") ) {
            return true;
        } else if( answer.equals("ActiveMBlobMessage") ) {
            return true;
        } else {
            return super.isMarshallAware(jclass);
        }
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
out.println("#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_"+className.toUpperCase()+"_H_");
out.println("#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_"+className.toUpperCase()+"_H_");
out.println("");
out.println("// Turn off warning message for ignored exception specification");
out.println("#ifdef _MSC_VER");
out.println("#pragma warning( disable : 4290 )");
out.println("#endif");
out.println("");

    if( baseClass.equals("BaseDataStreamMarshaller") ) {
        out.println("#include <activemq/wireformat/openwire/marshal/"+baseClass+".h>");
    } else {
        out.println("#include <activemq/wireformat/openwire/marshal/generated/"+baseClass+".h>");
    }

out.println("");
out.println("#include <decaf/io/DataInputStream.h>");
out.println("#include <decaf/io/DataOutputStream.h>");
out.println("#include <decaf/io/IOException.h>");
out.println("#include <activemq/util/Config.h>");
out.println("#include <activemq/commands/DataStructure.h>");
out.println("#include <activemq/wireformat/openwire/OpenWireFormat.h>");
out.println("#include <activemq/wireformat/openwire/utils/BooleanStream.h>");
out.println("");
out.println("namespace activemq {");
out.println("namespace wireformat {");
out.println("namespace openwire {");
out.println("namespace marshal {");
out.println("namespace generated {");
out.println("");
out.println("    /**");
out.println("     * Marshaling code for Open Wire Format for "+className);
out.println("     *");
out.println("     *  NOTE!: This file is auto generated - do not modify!");
out.println("     *         if you need to make a change, please see the Java Classes");
out.println("     *         in the activemq-openwire-generator module");
out.println("     */");
out.println("    class AMQCPP_API "+className+" : public "+baseClass+" {");
out.println("    public:");
out.println("");
out.println("        "+className+"() {}");
out.println("        virtual ~"+className+"() {}");
out.println("");

    if( !isAbstractClass() ) {

out.println("        virtual commands::DataStructure* createObject() const;");
out.println("");
out.println("        virtual unsigned char getDataStructureType() const;");
out.println("");
    }
out.println("        virtual void tightUnmarshal(OpenWireFormat* wireFormat,");
out.println("                                    commands::DataStructure* dataStructure,");
out.println("                                    decaf::io::DataInputStream* dataIn,");
out.println("                                    utils::BooleanStream* bs);");
out.println("");
out.println("        virtual int tightMarshal1(OpenWireFormat* wireFormat,");
out.println("                                  commands::DataStructure* dataStructure,");
out.println("                                  utils::BooleanStream* bs);");
out.println("");
out.println("        virtual void tightMarshal2(OpenWireFormat* wireFormat,");
out.println("                                   commands::DataStructure* dataStructure,");
out.println("                                   decaf::io::DataOutputStream* dataOut,");
out.println("                                   utils::BooleanStream* bs);");
out.println("");
out.println("        virtual void looseUnmarshal(OpenWireFormat* wireFormat,");
out.println("                                    commands::DataStructure* dataStructure,");
out.println("                                    decaf::io::DataInputStream* dataIn);");
out.println("");
out.println("        virtual void looseMarshal(OpenWireFormat* wireFormat,");
out.println("                                  commands::DataStructure* dataStructure,");
out.println("                                  decaf::io::DataOutputStream* dataOut);");
out.println("");
out.println("    };");
out.println("");
out.println("}}}}}");
out.println("");
out.println("#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_"+className.toUpperCase()+"_H_*/");
out.println("");
        }

    protected void processFactory() {
        if (factoryFile == null) {
            factoryFile = new File(destDir, factoryFileName + filePostFix);
        }
        PrintWriter out = null;
        try {
            out = new PrintWriter(new FileWriter(factoryFile));
            generateFactory(out);
        } catch (Exception e) {
            throw new RuntimeException(e);
        } finally {
            if (out != null) {
                out.close();
            }
        }
    }

    public void generateFactory(PrintWriter out) {
        generateLicence(out);
out.println("#ifndef _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_MARSHALERFACTORY_H_");
out.println("#define _ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSAHAL_GENERATED_MARSHALERFACTORY_H_");
out.println("");
out.println("//       Turn off warning message for ignored exception specification");
out.println("#ifdef _MSC_VER");
out.println("#pragma warning( disable : 4290 )");
out.println("#endif");
out.println("");
out.println("#include <activemq/wireformat/openwire/OpenWireFormat.h>");
out.println("");
out.println("namespace activemq {");
out.println("namespace wireformat {");
out.println("namespace openwire {");
out.println("namespace marshal {");
out.println("namespace generated {");
out.println("");
out.println("    /**");
out.println("     * Used to create marshallers for a specific version of the wire");
out.println("     * protocol.");
out.println("     *");
out.println("     *  NOTE!: This file is auto generated - do not modify!");
out.println("     *         if you need to make a change, please see the Groovy scripts");
out.println("     *         in the activemq-openwire-generator module");
out.println("     */");
out.println("    class MarshallerFactory {");
out.println("    public:");
out.println("");
out.println("        virtual ~MarshallerFactory() {};");
out.println("");
out.println("        virtual void configure(OpenWireFormat* format);");
out.println("");
out.println("    };");
out.println("");
out.println("}}}}}");
out.println("");
out.println("#endif /*_ACTIVEMQ_WIREFORMAT_OPENWIRE_MARSHAL_GENERATED_MARSHALLERFACTORY_H_*/");
    }

    public List<JClass> getConcreteClasses() {
        return concreteClasses;
    }

    public void setConcreteClasses(List<JClass> concreteClasses) {
        this.concreteClasses = concreteClasses;
    }

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir(String targetDir) {
        this.targetDir = targetDir;
    }
}
