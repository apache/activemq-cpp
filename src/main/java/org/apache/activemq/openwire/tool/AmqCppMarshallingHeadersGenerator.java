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
public class AmqCppMarshallingHeadersGenerator extends JavaMarshallingGenerator {

    protected String targetDir="./src/main";

    public Object run() {
        filePostFix = getFilePostFix();
        if (destDir == null) {
            destDir = new File(targetDir+"/activemq/connector/openwire/marshal/v"+getOpenwireVersion());
        }
        return super.run();
    }

    protected void processClass(JClass jclass) {
        super.processClass( jclass );
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
        }

        // We didn't map it, so let the base class handle it.
        if( answer.equals( jclass.getSimpleName() ) ) {
            answer = super.getBaseClassName(jclass);
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
out.println("#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_"+className.toUpperCase()+"_H_");
out.println("#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_"+className.toUpperCase()+"_H_");
out.println("");
out.println("// Turn off warning message for ignored exception specification");
out.println("#ifdef _MSC_VER");
out.println("#pragma warning( disable : 4290 )");
out.println("#endif");
out.println("");

    if( baseClass.equals("BaseDataStreamMarshaller") ) {
        out.println("#include <activemq/connector/openwire/marshal/"+baseClass+".h>");
    } else {
        out.println("#include <activemq/connector/openwire/marshal/v"+getOpenwireVersion()+"/"+baseClass+".h>");
    }

out.println("");
out.println("#include <decaf/io/DataInputStream.h>");
out.println("#include <decaf/io/DataOutputStream.h>");
out.println("#include <decaf/io/IOException.h>");
out.println("#include <activemq/util/Config.h>");
out.println("#include <activemq/connector/openwire/OpenWireFormat.h>");
out.println("#include <activemq/connector/openwire/commands/DataStructure.h>");
out.println("#include <activemq/connector/openwire/utils/BooleanStream.h>");
out.println("");
out.println("namespace activemq{");
out.println("namespace connector{");
out.println("namespace openwire{");
out.println("namespace marshal{");
out.println("namespace v"+getOpenwireVersion()+"{");
out.println("");
out.println("    /**");
out.println("     * Marshalling code for Open Wire Format for "+className);
out.println("     *");
out.println("     *  NOTE!: This file is autogenerated - do not modify!");
out.println("     *         if you need to make a change, please see the Java Classes");
out.println("     *         in the activemq-openwire-generator module");
out.println("     */");
out.println("    class AMQCPP_API "+className+" : public "+baseClass);
out.println("    {");
out.println("    public:");
out.println("");
out.println("        "+className+"() {}");
out.println("        virtual ~"+className+"() {}");
out.println("");

    if( !isAbstractClass() ) {

out.println("        /**");
out.println("         * Creates a new instance of this marshalable type.");
out.println("         * @return new DataStructure object pointer caller owns it.");
out.println("         */");
out.println("        virtual commands::DataStructure* createObject() const;");
out.println("");
out.println("        /**");
out.println("         * Get the Data Structure Type that identifies this Marshaller");
out.println("         * @return byte holding the data structure type value");
out.println("         */");
out.println("        virtual unsigned char getDataStructureType() const;");
out.println("");
    }
out.println("        /**");
out.println("         * Un-marshal an object instance from the data input stream");
out.println("         * @param wireFormat - describs the wire format of the broker");
out.println("         * @param o - Object to be un-marshaled");
out.println("         * @param dataIn - BinaryReader that provides that data");
out.println("         * @param bs - BooleanStream");
out.println("         */");
out.println("        virtual void tightUnmarshal( OpenWireFormat* wireFormat,");
out.println("                                     commands::DataStructure* dataStructure,");
out.println("                                     decaf::io::DataInputStream* dataIn,");
out.println("                                     utils::BooleanStream* bs ) throw( decaf::io::IOException );");
out.println("");
out.println("        /**");
out.println("         * Write the booleans that this object uses to a BooleanStream");
out.println("         * @param wireFormat - describis the wire format of the broker");
out.println("         * @param o - Object to be marshaled");
out.println("         * @param bs - BooleanStream");
out.println("         * @returns int");
out.println("         */");
out.println("        virtual int tightMarshal1( OpenWireFormat* wireFormat,");
out.println("                                   commands::DataStructure* dataStructure,");
out.println("                                   utils::BooleanStream* bs ) throw( decaf::io::IOException );");
out.println("");
out.println("        /**");
out.println("         * Write a object instance to data output stream");
out.println("         * @param wireFormat - describs the wire format of the broker");
out.println("         * @param o - Object to be marshaled");
out.println("         * @param dataOut - BinaryReader that provides that data sink");
out.println("         * @param bs - BooleanStream");
out.println("         */");
out.println("        virtual void tightMarshal2( OpenWireFormat* wireFormat,");
out.println("                                    commands::DataStructure* dataStructure,");
out.println("                                    decaf::io::DataOutputStream* dataOut,");
out.println("                                    utils::BooleanStream* bs ) throw( decaf::io::IOException );");
out.println("");
out.println("        /**");
out.println("         * Un-marshal an object instance from the data input stream");
out.println("         * @param wireFormat - describs the wire format of the broker");
out.println("         * @param o - Object to be marshaled");
out.println("         * @param dataIn - BinaryReader that provides that data source");
out.println("         */");
out.println("        virtual void looseUnmarshal( OpenWireFormat* wireFormat,");
out.println("                                     commands::DataStructure* dataStructure,");
out.println("                                     decaf::io::DataInputStream* dataIn ) throw( decaf::io::IOException );");
out.println("");
out.println("        /**");
out.println("         * Write a object instance to data output stream");
out.println("         * @param wireFormat - describs the wire format of the broker");
out.println("         * @param o - Object to be marshaled");
out.println("         * @param dataOut - BinaryWriter that provides that data sink");
out.println("         */");
out.println("        virtual void looseMarshal( OpenWireFormat* wireFormat,");
out.println("                                   commands::DataStructure* dataStructure,");
out.println("                                   decaf::io::DataOutputStream* dataOut ) throw( decaf::io::IOException );");
out.println("");
out.println("    };");
out.println("");
out.println("}}}}}");
out.println("");
out.println("#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_"+className.toUpperCase()+"_H_*/");
out.println("");
        }

    public void generateFactory(PrintWriter out) {
        generateLicence(out);
out.println("#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_MARSHALERFACTORY_H_");
out.println("#define _ACTIVEMQ_CONNECTOR_OPENWIRE_MARSAHAL_V"+getOpenwireVersion()+"_MARSHALERFACTORY_H_");
out.println("");
out.println("//       Turn off warning message for ignored exception specification");
out.println("#ifdef _MSC_VER");
out.println("#pragma warning( disable : 4290 )");
out.println("#endif");
out.println("");
out.println("#include <activemq/connector/openwire/OpenWireFormat.h>");
out.println("");
out.println("namespace activemq{");
out.println("namespace connector{");
out.println("namespace openwire{");
out.println("namespace marshal{");
out.println("namespace v"+getOpenwireVersion()+"{");
out.println("");
out.println("    /**");
out.println("     * Used to create marshallers for a specific version of the wire");
out.println("     * protocol.");
out.println("     *");
out.println("     *  NOTE!: This file is autogenerated - do not modify!");
out.println("     *         if you need to make a change, please see the Groovy scripts");
out.println("     *         in the activemq-openwire-generator module");
out.println("     */");
out.println("    class MarshallerFactory");
out.println("    {");
out.println("    public:");
out.println("");
out.println("        virtual ~MarshallerFactory() {};");
out.println("");
out.println("        virtual void configure( OpenWireFormat* format );");
out.println("");
out.println("    };");
out.println("");
out.println("}}}}}");
out.println("");
out.println("#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_MARSHAL_V"+getOpenwireVersion()+"_MARSHALLERFACTORY_H_*/");
    }

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir(String targetDir) {
        this.targetDir = targetDir;
    }
}
