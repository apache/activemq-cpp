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

import org.codehaus.jam.JAnnotation;
import org.codehaus.jam.JAnnotationValue;
import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 *
 * @version $Revision: 381410 $
 */
public class AmqCppMarshallingClassesGenerator extends AmqCppMarshallingHeadersGenerator {

    protected String getFilePostFix() {
        return ".cpp";
    }

    protected String getProperClassName( String className ) {

        return className;
    }

    /**
     * Checks if the tightMarshal1 method needs an casted version of its
     * dataStructure argument and then returns true or false to indicate this
     * to the caller.
     * @returns true if the tightMarshal1 method needs an info pointer.
     */
    protected boolean checkNeedsInfoPointerTM1() {

        if( isMarshallerAware() ){
            return true;
        }

        for ( JProperty property : getProperties() ) {
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();

            if( !( type.equals("byte") ) &&
                !( type.equals("char") ) &&
                !( type.equals("short") ) &&
                !( type.equals("int") ) ) {

                return true;
            }

        }

        return false;
    }

    /**
     * Checks if the tightMarshal2 method needs an casted version of its
     * dataStructure argument and then returns true or false to indicate this
     * to the caller.
     * @returns true if the tightMarshal2 method needs an info pointer.
     */
    protected boolean checkNeedsInfoPointerTM2() {

        if( isMarshallerAware() ){
            return true;
        }

        for ( JProperty property : getProperties() ) {
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();

            if( !type.equals("boolean") ) {

                return true;
            }

        }

        return false;
    }

    protected boolean checkNeedsWireFormatVersion() {
        if( !getProperties().isEmpty() ) {

            for ( JProperty property : getProperties() ) {

                JAnnotation annotation = property.getAnnotation("openwire:property");
                JAnnotationValue version = annotation.getValue("version");

                if( version.asInt() > 1 ) {
                    return true;
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // This section is for the tight wire format encoding generator
    //////////////////////////////////////////////////////////////////////////////////////

    protected void generateTightUnmarshalBody(PrintWriter out) {

        for ( JProperty property : getProperties() ) {
            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue size = annotation.getValue("size");
            JAnnotationValue version = annotation.getValue("version");
            JClass propertyType = property.getType();
            String propertyTypeName = propertyType.getSimpleName();
            String indent = "        ";

            if( version.asInt() > 1 ) {
                indent = indent + "    ";
                out.println("        if (wireVersion >= " + version.asInt() + ") {");
            }

            if (propertyType.isArrayType() && !propertyTypeName.equals("byte[]")) {
                generateTightUnmarshalBodyForArrayProperty(out, property, size, indent);
            } else {
                generateTightUnmarshalBodyForProperty(out, property, size, indent);
            }

            if( version.asInt() > 1 ) {
                out.println("        }");
            }
        }
    }

    protected void generateTightUnmarshalBodyForProperty(PrintWriter out, JProperty property, JAnnotationValue size, String indent) {

        String setter = property.getSetter().getSimpleName();
        String type = property.getType().getSimpleName();
        String nativeType = toCppType(property.getType());

        if( type.equals("boolean") ) {
            out.println(indent + "info->" + setter + "(bs->readBoolean());");
        }
        else if( type.equals("byte") ) {
            out.println(indent + "info->" + setter + "(dataIn->readByte());");
        }
        else if( type.equals("char") ) {
            out.println(indent + "info->" + setter + "(dataIn->readChar());");
        }
        else if( type.equals("short") ) {
            out.println(indent + "info->" + setter + "(dataIn->readShort());");
        }
        else if( type.equals("int") ) {
            out.println(indent + "info->" + setter + "(dataIn->readInt());");
        }
        else if( type.equals("long") ) {
            out.println(indent + "info->" + setter + "(tightUnmarshalLong(wireFormat, dataIn, bs));");
        }
        else if( type.equals("String") ) {
            out.println(indent + "info->" + setter + "(tightUnmarshalString(dataIn, bs));");
        }
        else if( type.equals("byte[]") || type.equals("ByteSequence") ) {
            if( size != null ) {
                out.println(indent + "info->" + setter + "(tightUnmarshalConstByteArray(dataIn, bs, "+ size.asInt() +"));");
            }
            else {
                out.println(indent + "info->" + setter + "(tightUnmarshalByteArray(dataIn, bs));");
            }
        }
        else if( isThrowable( property.getType() ) ) {
            out.println(indent + "info->" + setter + "(Pointer<"+nativeType+">(dynamic_cast<" + nativeType + "* >(");
            out.println(indent + "    tightUnmarshalBrokerError(wireFormat, dataIn, bs))));");
        }
        else if( isCachedProperty(property) ) {
            out.println(indent + "info->" + setter + "(Pointer<"+nativeType+">(dynamic_cast<" + nativeType + "* >(");
            out.println(indent + "    tightUnmarshalCachedObject(wireFormat, dataIn, bs))));");
        }
        else {
            out.println(indent + "info->" + setter + "(Pointer<"+nativeType+">(dynamic_cast<" + nativeType + "* >(");
            out.println(indent + "    tightUnmarshalNestedObject(wireFormat, dataIn, bs))));");
        }
    }

    protected void generateTightUnmarshalBodyForArrayProperty(PrintWriter out, JProperty property, JAnnotationValue size, String indent) {
        JClass propertyType = property.getType();
        String arrayType = propertyType.getArrayComponentType().getSimpleName();
        String setter = property.getSetter().getSimpleName();
        String getter = property.getGetter().getSimpleName();
        out.println();
        if (size != null) {
            out.println(indent + "{");
            out.println(indent + "    " + arrayType + " value[] = new " + arrayType + "[" + size.asInt() + "];");
            out.println(indent + "    for (int i = 0; i < " + size.asInt() + "; i++) {");
            out.println(indent + "        value[i] = (" + arrayType + ") tightUnmarshalNestedObject(wireFormat, dataIn, bs);");
            out.println(indent + "    }");
            out.println(indent + "    info->" + setter + "(value);");
            out.println(indent + "}");
        }
        else {
            out.println(indent + "if (bs->readBoolean()) {");
            out.println(indent + "    short size = dataIn->readShort();");
            out.println(indent + "    info->" + getter + "().reserve(size);");
            out.println(indent + "    for (int i = 0; i < size; i++) {");
            out.println(indent + "        info->" + getter + "().push_back(Pointer<"+arrayType+">(dynamic_cast<" + arrayType + "*>(");
            out.println(indent + "            tightUnmarshalNestedObject(wireFormat, dataIn, bs))));");
            out.println(indent + "    }");
            out.println(indent + "} else {");
            out.println(indent + "    info->" + getter + "().clear();");
            out.println(indent + "}");
        }
    }

    protected int generateTightMarshal1Body(PrintWriter out) {
        int baseSize = 0;

        for ( JProperty property : getProperties() ) {
            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue version = annotation.getValue("version");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();
            String getter = "info->" + property.getGetter().getSimpleName() + "()";
            String indent = "        ";

            if( version.asInt() > 1 ) {
                indent = indent + "    ";
                out.println("        if (wireVersion >= " + version.asInt() + ") {");
            }

            if (type.equals("boolean")) {
                out.println(indent + "bs->writeBoolean(" + getter + ");");
            }
            else if (type.equals("byte")) {
                baseSize += 1;
            }
            else if (type.equals("char")) {
                baseSize += 2;
            }
            else if (type.equals("short")) {
                baseSize += 2;
            }
            else if (type.equals("int")) {
                baseSize += 4;
            }
            else if (type.equals("long")) {
                out.println(indent + "rc += tightMarshalLong1(wireFormat, " + getter + ", bs);");
            }
            else if (type.equals("String")) {
                out.print("");
                out.println(indent + "rc += tightMarshalString1(" + getter + ", bs);" );
            }
            else if (type.equals("byte[]") || type.equals("ByteSequence")) {
                if (size == null) {
                    out.println(indent + "bs->writeBoolean(" + getter + ".size() != 0);" );
                    out.println(indent + "rc += " + getter + ".size() == 0 ? 0 : (int)" + getter + ".size() + 4;");
                }
                else {
                    baseSize += size.asInt();
                }
            }
            else if (propertyType.isArrayType()) {
                if (size != null) {
                    out.println(indent + "rc += tightMarshalObjectArrayConstSize1(wireFormat, " + getter + ", bs, " + size.asInt() + ");");
                }
                else {
                    out.println(indent + "rc += tightMarshalObjectArray1(wireFormat, " + getter + ", bs);");
                }
            }
            else if (isThrowable(propertyType)) {
                out.println(indent + "rc += tightMarshalBrokerError1(wireFormat, " + getter + ".get(), bs);");
            }
            else {
                if (isCachedProperty(property)) {
                    out.println(indent + "rc += tightMarshalCachedObject1(wireFormat, " + getter + ".get(), bs);");
                }
                else {
                    out.println(indent + "rc += tightMarshalNestedObject1(wireFormat, " + getter + ".get(), bs);");
                }
            }

            if( version.asInt() > 1 ) {
                out.println("        }");
            }
        }
        return baseSize;
    }

    protected void generateTightMarshal2Body(PrintWriter out) {

        int count = 0;

        for ( JProperty property : getProperties() ) {

            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue version = annotation.getValue("version");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();
            String getter = "info->" + property.getGetter().getSimpleName() + "()";
            String indent = "        ";
            count++;

            if( version.asInt() > 1 ) {
                indent = indent + "    ";
                out.println("        if (wireVersion >= " + version.asInt() + ") {");
            }

            if (type.equals("boolean")) {
                out.println(indent + "bs->readBoolean();");
            }
            else if (type.equals("byte")) {
                out.println(indent + "dataOut->write(" + getter + ");");
            }
            else if (type.equals("char")) {
                out.println(indent + "dataOut->write(" + getter + ");");
            }
            else if (type.equals("short")) {
                out.println(indent + "dataOut->writeShort(" + getter + ");");
            }
            else if (type.equals("int")) {
                out.println(indent + "dataOut->writeInt(" + getter + ");");
            }
            else if (type.equals("long")) {
                out.println(indent + "tightMarshalLong2(wireFormat, " + getter + ", dataOut, bs);");
            }
            else if (type.equals("String")) {
                out.println(indent + "tightMarshalString2(" + getter + ", dataOut, bs);");
            }
            else if (type.equals("byte[]") || type.equals("ByteSequence")) {
                if (size != null) {
                    out.println(indent + "dataOut->write((const unsigned char*)(&" + getter + "[0]), " + size.asInt() + ", 0, " + size.asInt() + ");");
                }
                else {
                    out.println(indent + "if (bs->readBoolean()) {");
                    out.println(indent + "    dataOut->writeInt((int)" + getter + ".size() );");
                    out.println(indent + "    dataOut->write((const unsigned char*)(&" + getter + "[0]), (int)" + getter + ".size(), 0, (int)" + getter + ".size());");
                    out.println(indent + "}");
                }
            }
            else if (propertyType.isArrayType()) {
                if (size != null) {
                    out.println(indent + "tightMarshalObjectArrayConstSize2(wireFormat, " + getter + ", dataOut, bs, " + size.asInt() + ");");
                }
                else {
                    out.println(indent + "tightMarshalObjectArray2(wireFormat, " + getter + ", dataOut, bs);");
                }
            } else if( isThrowable(propertyType) ) {
                out.println(indent + "tightMarshalBrokerError2(wireFormat, " + getter + ".get(), dataOut, bs);");
            } else {
                if( isCachedProperty(property) ) {
                    out.println(indent + "tightMarshalCachedObject2(wireFormat, "+getter+".get(), dataOut, bs);");
                }
                else {
                    out.println(indent + "tightMarshalNestedObject2(wireFormat, "+getter+".get(), dataOut, bs);");
                }
            }

            if( version.asInt() > 1 ) {
                out.println("        }");
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // This section is for the loose wire format encoding generator
    //////////////////////////////////////////////////////////////////////////////////////

    protected void generateLooseUnmarshalBody(PrintWriter out) {

        for ( JProperty property : getProperties() ) {

            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue version = annotation.getValue("version");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String propertyTypeName = propertyType.getSimpleName();
            String indent = "        ";

            if( version.asInt() > 1 ) {
                indent = indent + "    ";
                out.println("        if (wireVersion >= " + version.asInt() + ") {");
            }

            if (propertyType.isArrayType() && !propertyTypeName.equals("byte[]")) {
                generateLooseUnmarshalBodyForArrayProperty(out, property, size, indent);
            } else {
                generateLooseUnmarshalBodyForProperty(out, property, size, indent);
            }

            if( version.asInt() > 1 ) {
                out.println("        }");
            }
        }
    }

    protected void generateLooseUnmarshalBodyForProperty(PrintWriter out, JProperty property, JAnnotationValue size, String indent) {

        String type = property.getType().getSimpleName();
        String nativeType = toCppType(property.getType());
        String setter = property.getSetter().getSimpleName();

        if (type.equals("boolean")) {
            out.println(indent + "info->" + setter + "(dataIn->readBoolean());");
        }
        else if (type.equals("byte")) {
            out.println(indent + "info->" + setter + "(dataIn->readByte());");
        }
        else if (type.equals("char")) {
            out.println(indent + "info->" + setter + "(dataIn->readChar());");
        }
        else if (type.equals("short")) {
            out.println(indent + "info->" + setter + "(dataIn->readShort());");
        }
        else if (type.equals("int")) {
            out.println(indent + "info->" + setter + "(dataIn->readInt());");
        }
        else if (type.equals("long")) {
            out.println(indent + "info->" + setter + "(looseUnmarshalLong(wireFormat, dataIn));");
        }
        else if (type.equals("String")) {
            out.println(indent + "info->" + setter + "(looseUnmarshalString(dataIn));");
        }
        else if (type.equals("byte[]") || type.equals("ByteSequence")) {
            if (size != null) {
                out.println(indent + "info->" + setter + "(looseUnmarshalConstByteArray(dataIn, " + size.asInt() + "));");
            }
            else {
                out.println(indent + "info->" + setter + "(looseUnmarshalByteArray(dataIn));");
            }
        }
        else if (isThrowable(property.getType())) {
            out.println(indent + "info->" + setter + "(Pointer<"+nativeType+">(dynamic_cast< " + nativeType + "*>(");
            out.println(indent + "    looseUnmarshalBrokerError(wireFormat, dataIn))));");
        }
        else if (isCachedProperty(property)) {
            out.println(indent + "info->" + setter + "(Pointer<"+nativeType+">(dynamic_cast<" + nativeType + "*>(");
            out.println(indent + "    looseUnmarshalCachedObject(wireFormat, dataIn))));");
        }
        else {
            out.println(indent + "info->" + setter + "(Pointer<"+nativeType+">(dynamic_cast<" + nativeType + "*>(");
            out.println(indent + "    looseUnmarshalNestedObject(wireFormat, dataIn))));");
        }
    }

    protected void generateLooseUnmarshalBodyForArrayProperty(PrintWriter out, JProperty property, JAnnotationValue size, String indent) {
        JClass propertyType = property.getType();
        String arrayType = propertyType.getArrayComponentType().getSimpleName();
        String setter = property.getSetter().getSimpleName();
        String getter = property.getGetter().getSimpleName();

        out.println();
        if (size != null) {
            out.println(indent + "{");
            out.println(indent + "    " + arrayType + "[] value = new " + arrayType + "[" + size.asInt() + "];");
            out.println(indent + "    for (int i=0; i < " + size.asInt() + "; i++) {");
            out.println(indent + "        value[i] = (" + arrayType + ") looseUnmarshalNestedObject(wireFormat, dataIn);");
            out.println(indent + "    }");
            out.println(indent + "    info->" + setter + "( value );");
            out.println(indent + "}");
        }
        else {
            out.println(indent + "if (dataIn->readBoolean()) {");
            out.println(indent + "    short size = dataIn->readShort();");
            out.println(indent + "    info->" + getter + "().reserve(size);");
            out.println(indent + "    for (int i = 0; i < size; i++) {");
            out.println(indent + "        info->" + getter + "().push_back( Pointer<"+arrayType+">(dynamic_cast<" + arrayType + "*>(");
            out.println(indent + "            looseUnmarshalNestedObject(wireFormat, dataIn))));");
            out.println(indent + "    }");
            out.println(indent + "} else {");
            out.println(indent + "    info->" + getter + "().clear();");
            out.println(indent + "}");
        }
    }


    protected void generateLooseMarshalBody(PrintWriter out) {

        for ( JProperty property : getProperties() ) {

            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue version = annotation.getValue("version");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();
            String getter = "info->" + property.getGetter().getSimpleName() + "()";
            String indent = "        ";

            if( version.asInt() > 1 ) {
                indent = indent + "    ";
                out.println("        if (wireVersion >= " + version.asInt() + ") {");
            }

            if( type.equals( "boolean" ) ) {
                out.println(indent + "dataOut->writeBoolean(" + getter + ");");
            }
            else if( type.equals("byte") ) {
                out.println(indent + "dataOut->write(" + getter + ");");
            }
            else if( type.equals("char") ) {
                out.println(indent + "dataOut->write(" + getter + ");");
            }
            else if( type.equals("short") ) {
                out.println(indent + "dataOut->writeShort(" + getter + ");");
            }
            else if( type.equals("int")) {
                out.println(indent + "dataOut->writeInt(" + getter + ");");
            }
            else if( type.equals("long") ) {
                out.println(indent + "looseMarshalLong(wireFormat, " + getter + ", dataOut);");
            }
            else if( type.equals("String") ) {
                out.println(indent + "looseMarshalString(" + getter + ", dataOut);");
            }
            else if( type.equals("byte[]") || type.equals("ByteSequence") ) {
                if(size != null) {
                    out.println(indent + "dataOut->write((const unsigned char*)(&" + getter + "[0]), " + size.asInt() + ", 0, " + size.asInt() + ");");
                }
                else {
                    out.println(indent + "dataOut->write( " + getter + ".size() != 0 );");
                    out.println(indent + "if( " + getter + ".size() != 0 ) {");
                    out.println(indent + "    dataOut->writeInt( (int)" + getter + ".size() );");
                    out.println(indent + "    dataOut->write((const unsigned char*)(&" + getter + "[0]), (int)" + getter + ".size(), 0, (int)" + getter + ".size());");
                    out.println(indent + "}");
                }
            }
            else if( propertyType.isArrayType() ) {
                if (size != null) {
                    out.println(indent + "looseMarshalObjectArrayConstSize(wireFormat, " + getter + ", dataOut, " + size.asInt() + ");");
                }
                else {
                    out.println(indent + "looseMarshalObjectArray(wireFormat, " + getter + ", dataOut);");
                }
            }
            else if( isThrowable( propertyType ) ) {
                out.println(indent + "looseMarshalBrokerError(wireFormat, " + getter + ".get(), dataOut);");
            }
            else {
                if( isCachedProperty( property ) ) {
                    out.println(indent + "looseMarshalCachedObject(wireFormat, "+getter+".get(), dataOut);");
                }
                else {
                    out.println(indent + "looseMarshalNestedObject(wireFormat, "+getter+".get(), dataOut);");
                }
            }

            if( version.asInt() > 1 ) {
                out.println("        }");
            }
        }
    }


    protected void generateFile(PrintWriter out) throws Exception {
        generateLicence(out);

out.println("");
out.println("#include <activemq/wireformat/openwire/marshal/generated/"+className+".h>");
out.println("");
out.println("#include <activemq/commands/"+jclass.getSimpleName()+".h>");
out.println("#include <activemq/exceptions/ActiveMQException.h>");
out.println("#include <decaf/lang/Pointer.h>");
out.println("");
out.println("//");
out.println("//     NOTE!: This file is autogenerated - do not modify!");
out.println("//            if you need to make a change, please see the Java Classes in the");
out.println("//            activemq-core module");
out.println("//");
out.println("");
out.println("using namespace std;");
out.println("using namespace activemq;");
out.println("using namespace activemq::exceptions;");
out.println("using namespace activemq::commands;");
out.println("using namespace activemq::wireformat;");
out.println("using namespace activemq::wireformat::openwire;");
out.println("using namespace activemq::wireformat::openwire::marshal;");
out.println("using namespace activemq::wireformat::openwire::utils;");
out.println("using namespace activemq::wireformat::openwire::marshal::generated;");
out.println("using namespace decaf;");
out.println("using namespace decaf::io;");
out.println("using namespace decaf::lang;");
out.println("");

    String typeName = jclass.getSimpleName().toUpperCase();

    if( !isAbstractClass() ) {
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("DataStructure* "+className+"::createObject() const {");
out.println("    return new "+jclass.getSimpleName()+"();");
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("unsigned char "+className+"::getDataStructureType() const {");
out.println("    return "+jclass.getSimpleName()+"::ID_"+typeName+";");
out.println("}");
out.println("");
    }

out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::tightUnmarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs) {");
out.println("");
out.println("    try {");
out.println("");
out.println("        "+baseClass+"::tightUnmarshal(wireFormat, dataStructure, dataIn, bs);");
out.println("");

    List<JProperty> properties = getProperties();
    boolean marshallerAware = isMarshallerAware();
    if( !properties.isEmpty() || marshallerAware ) {

        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>(dataStructure);");
    }

    if( marshallerAware ) {
out.println("        info->beforeUnmarshal(wireFormat);");
out.println("");
    }

    if( checkNeedsWireFormatVersion() ) {
        out.println("");
        out.println("        int wireVersion = wireFormat->getVersion();");
        out.println("");
    }

    generateTightUnmarshalBody(out);

    if( marshallerAware ) {
out.println("");
out.println("        info->afterUnmarshal( wireFormat );");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)" );
out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("int "+className+"::tightMarshal1(OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs) {");
out.println("");
out.println("    try {");
out.println("");

    if( checkNeedsInfoPointerTM1() ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>(dataStructure);");
out.println("");
    }

    if( marshallerAware ) {
out.println("        info->beforeMarshal(wireFormat);");
    }

out.println("        int rc = "+baseClass+"::tightMarshal1(wireFormat, dataStructure, bs);");

    if( checkNeedsWireFormatVersion() ) {
        out.println("");
        out.println("        int wireVersion = wireFormat->getVersion();");
        out.println("");
    }

    int baseSize = generateTightMarshal1Body(out);

out.println("");
out.println("        return rc + "+baseSize+";");
out.println("    }");
out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)" );
out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::tightMarshal2(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs) {");
out.println("");
out.println("    try {");
out.println("");
out.println("        "+baseClass+"::tightMarshal2(wireFormat, dataStructure, dataOut, bs );");
out.println("");

    if( checkNeedsInfoPointerTM2() ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>(dataStructure);");
    }

    if( checkNeedsWireFormatVersion() ) {
        out.println("");
        out.println("        int wireVersion = wireFormat->getVersion();");
        out.println("");
    }

    generateTightMarshal2Body(out);

    if( marshallerAware ) {
out.println("        info->afterMarshal(wireFormat);");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException)" );
out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::looseUnmarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn) {");
out.println("");
out.println("    try {");
out.println("");
out.println("        "+baseClass+"::looseUnmarshal(wireFormat, dataStructure, dataIn);");

    if( !properties.isEmpty() || marshallerAware ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>(dataStructure);");
    }

    if( marshallerAware ) {
out.println("        info->beforeUnmarshal(wireFormat);");
    }

    if( checkNeedsWireFormatVersion() ) {
        out.println("");
        out.println("        int wireVersion = wireFormat->getVersion();");
        out.println("");
    }

    generateLooseUnmarshalBody(out);

    if( marshallerAware ) {
out.println("        info->afterUnmarshal(wireFormat);");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)" );
out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::looseMarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut) {");
out.println("");
out.println("    try {");
out.println("");

    if( !properties.isEmpty() || marshallerAware ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>(dataStructure);");
    }

    if( marshallerAware ) {
out.println("        info->beforeMarshal(wireFormat);");
    }

out.println("        "+baseClass+"::looseMarshal(wireFormat, dataStructure, dataOut);");

    if( checkNeedsWireFormatVersion() ) {
        out.println("");
        out.println("        int wireVersion = wireFormat->getVersion();");
        out.println("");
    }

    generateLooseMarshalBody(out);

    if( marshallerAware ) {
out.println("        info->afterMarshal(wireFormat);");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW(decaf::io::IOException)" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)" );
out.println("    AMQ_CATCHALL_THROW(decaf::io::IOException)" );
out.println("}");
out.println("");
}

    public void generateFactory(PrintWriter out) {
        generateLicence(out);

out.println("#include <activemq/wireformat/openwire/marshal/generated/MarshallerFactory.h>");

        List<JClass> list = new ArrayList<JClass>(getConcreteClasses());
        Collections.sort(list, new Comparator<JClass>(){
            public int compare(JClass c1, JClass c2) {
                return c1.getSimpleName().compareTo(c2.getSimpleName());
        }});

        for ( JClass jclass : list ) {
out.println("#include <activemq/wireformat/openwire/marshal/generated/"+jclass.getSimpleName()+"Marshaller.h>");
        }

out.println("");
out.println("/*");
out.println(" *");
out.println(" *  Command and marshaling code for OpenWire format for MarshallerFactory");
out.println(" *");
out.println(" *");
out.println(" *  NOTE!: This file is autogenerated - do not modify!");
out.println(" *         if you need to make a change, please see the Java Classes");
out.println(" *         in the activemq-openwire-generator module");
out.println(" *");
out.println(" */");
out.println("");
out.println("using namespace activemq;");
out.println("using namespace activemq::wireformat;");
out.println("using namespace activemq::wireformat::openwire;");
out.println("using namespace activemq::wireformat::openwire::marshal;");
out.println("using namespace activemq::wireformat::openwire::marshal::generated;");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void MarshallerFactory::configure(OpenWireFormat* format) {");
out.println("");

        for ( JClass jclass : list ) {
out.println("    format->addMarshaller(new "+jclass.getSimpleName()+"Marshaller());");
        }

out.println("}");
out.println("");
    }
}
