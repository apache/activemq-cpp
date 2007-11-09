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

import org.codehaus.jam.JAnnotation;
import org.codehaus.jam.JAnnotationValue;
import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
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

        if( className.equals( "BaseCommand") ) {
            return "transport::Command";
        }

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

        List properties = getProperties();
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
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

        List properties = getProperties();
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();

            if( !type.equals("boolean") ) {

                return true;
            }

        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // This section is for the tight wire format encoding generator
    //////////////////////////////////////////////////////////////////////////////////////

    protected void generateTightUnmarshalBodyForProperty(PrintWriter out, JProperty property, JAnnotationValue size) {

        String setter = property.getSetter().getSimpleName();
        String type = property.getType().getSimpleName();
        String nativeType = toCppType(property.getType());
        String propertyName = property.getType().getSimpleName();

        if( type.equals("boolean") ) {
            out.println("        info->" + setter + "( bs->readBoolean() );");
        }
        else if( type.equals("byte") ) {
            out.println("        info->" + setter + "( dataIn->readByte() );");
        }
        else if( type.equals("char") ) {
            out.println("        info->" + setter + "( dataIn->readChar() );");
        }
        else if( type.equals("short") ) {
            out.println("        info->" + setter + "( dataIn->readShort() );");
        }
        else if( type.equals("int") ) {
            out.println("        info->" + setter + "( dataIn->readInt() );");
        }
        else if( type.equals("long") ) {
            out.println("        info->" + setter + "( tightUnmarshalLong( wireFormat, dataIn, bs ) );");
        }
        else if( type.equals("String") ) {
            out.println("        info->" + setter + "( tightUnmarshalString( dataIn, bs ) );");
        }
        else if( type.equals("byte[]") || type.equals("ByteSequence") ) {
            if( size != null ) {
                out.println("        info->" + setter + "( tightUnmarshalConstByteArray( dataIn, bs, "+ size.asInt() +" ) );");
            }
            else {
                out.println("        info->" + setter + "( tightUnmarshalByteArray( dataIn, bs ) );");
            }
        }
        else if( isThrowable( property.getType() ) ) {
            out.println("        info->" + setter + "( dynamic_cast< " + nativeType + "* >(");
            out.println("            tightUnmarshalBrokerError( wireFormat, dataIn, bs ) ) );");
        }
        else if( isCachedProperty(property) ) {
            out.println("        info->" + setter + "( dynamic_cast< " + nativeType + "* >(");
            out.println("            tightUnmarshalCachedObject( wireFormat, dataIn, bs ) ) );");
        }
        else {
            out.println("        info->" + setter + "( dynamic_cast< " + nativeType + "* >(");
            out.println("            tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) );");
        }
    }

    protected void generateTightUnmarshalBodyForArrayProperty(PrintWriter out, JProperty property, JAnnotationValue size) {
        JClass propertyType = property.getType();
        String arrayType = propertyType.getArrayComponentType().getSimpleName();
        String setter = property.getSetter().getSimpleName();
        String getter = property.getGetter().getSimpleName();
        out.println();
        if (size != null) {
            out.println("        {");
            out.println("            " + arrayType + " value[] = new " + arrayType + "[" + size.asInt() + "];");
            out.println("            " + "for( int i = 0; i < " + size.asInt() + "; i++ ) {");
            out.println("                value[i] = (" + arrayType + ") tightUnmarshalNestedObject( wireFormat, dataIn, bs );");
            out.println("            }");
            out.println("            info->" + setter + "( value );");
            out.println("        }");
        }
        else {
            out.println("        if( bs->readBoolean() ) {");
            out.println("            short size = dataIn->readShort();");
            out.println("            info->" + getter + "().reserve( size );");
            out.println("            for( int i = 0; i < size; i++ ) {");
            out.println("                info->" + getter + "().push_back( dynamic_cast< " + arrayType + "* >(");
            out.println("                    tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) );");
            out.println("            }");
            out.println("        }");
            out.println("        else {");
            out.println("            info->" + getter + "().clear();");
            out.println("        }");
        }
    }

    protected int generateTightMarshal1Body(PrintWriter out) {
        List properties = getProperties();
        int baseSize = 0;
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();
            String getter = "info->" + property.getGetter().getSimpleName() + "()";

            if (type.equals("boolean")) {
                out.println("        bs->writeBoolean( " + getter + " );");
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
                out.println("        rc += tightMarshalLong1( wireFormat, " + getter + ", bs );");
            }
            else if (type.equals("String")) {
                out.print("");
                out.println("        rc += tightMarshalString1( " + getter + ", bs );" );
            }
            else if (type.equals("byte[]") || type.equals("ByteSequence")) {
                if (size == null) {
                    out.println("        bs->writeBoolean( " + getter + ".size() != 0 );" );
                    out.println("        rc += " + getter + ".size() == 0 ? 0 : (int)" + getter + ".size() + 4;");
                }
                else {
                    baseSize += size.asInt();
                }
            }
            else if (propertyType.isArrayType()) {
                if (size != null) {
                    out.println("        rc += tightMarshalObjectArrayConstSize1( wireFormat, " + getter + ", bs, " + size.asInt() + " );");
                }
                else {
                    out.println("        rc += tightMarshalObjectArray1( wireFormat, " + getter + ", bs );");
                }
            }
            else if (isThrowable(propertyType)) {
                out.println("        rc += tightMarshalBrokerError1( wireFormat, " + getter + ", bs );");
            }
            else {
                if (isCachedProperty(property)) {
                    out.println("        rc += tightMarshalCachedObject1( wireFormat, " + getter + ", bs );");
                }
                else {
                    out.println("        rc += tightMarshalNestedObject1( wireFormat, " + getter + ", bs );");
                }
            }
        }
        return baseSize;
    }

    protected void generateTightMarshal2Body(PrintWriter out) {
        List properties = getProperties();
        int count = 0;
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();
            String getter = "info->" + property.getGetter().getSimpleName() + "()";
            count++;

            if (type.equals("boolean")) {
                out.println("        bs->readBoolean();");
            }
            else if (type.equals("byte")) {
                out.println("        dataOut->write( " + getter + " );");
            }
            else if (type.equals("char")) {
                out.println("        dataOut->write( " + getter + " );");
            }
            else if (type.equals("short")) {
                out.println("        dataOut->writeShort( " + getter + " );");
            }
            else if (type.equals("int")) {
                out.println("        dataOut->writeInt( " + getter + " );");
            }
            else if (type.equals("long")) {
                out.println("        tightMarshalLong2( wireFormat, " + getter + ", dataOut, bs );");
            }
            else if (type.equals("String")) {
                out.println("        tightMarshalString2( " + getter + ", dataOut, bs );");
            }
            else if (type.equals("byte[]") || type.equals("ByteSequence")) {
                if (size != null) {
                    out.println("        dataOut->write( (const unsigned char*)(&" + getter + "[0]), 0, " + size.asInt() + " );");
                }
                else {
                    out.println("        if( bs->readBoolean() ) {");
                    out.println("            dataOut->writeInt( (int)" + getter + ".size() );");
                    out.println("            dataOut->write( (const unsigned char*)(&" + getter + "[0]), 0, (int)" + getter + ".size() );");
                    out.println("        }");
                }
            }
            else if (propertyType.isArrayType()) {
                if (size != null) {
                    out.println("        tightMarshalObjectArrayConstSize2( wireFormat, " + getter + ", dataOut, bs, " + size.asInt() + " );");
                }
                else {
                    out.println("        tightMarshalObjectArray2( wireFormat, " + getter + ", dataOut, bs );");
                }
            }
            else if( isThrowable(propertyType) ) {
                out.println("        tightMarshalBrokerError2( wireFormat, " + getter + ", dataOut, bs );");
            }
            else {
                if( isCachedProperty(property) ) {
                    out.println("        tightMarshalCachedObject2( wireFormat, "+getter+", dataOut, bs );");
                }
                else {
                    out.println("        tightMarshalNestedObject2( wireFormat, "+getter+", dataOut, bs );");
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // This section is for the loose wire format encoding generator
    //////////////////////////////////////////////////////////////////////////////////////

    protected void generateLooseUnmarshalBodyForProperty(PrintWriter out, JProperty property, JAnnotationValue size) {

        String propertyName = property.getSimpleName();
        String type = property.getType().getSimpleName();
        String nativeType = toCppType(property.getType());
        String setter = property.getSetter().getSimpleName();

        if (type.equals("boolean")) {
            out.println("        info->" + setter + "( dataIn->readBoolean() );");
        }
        else if (type.equals("byte")) {
            out.println("        info->" + setter + "( dataIn->readByte() );");
        }
        else if (type.equals("char")) {
            out.println("        info->" + setter + "( dataIn->readChar() );");
        }
        else if (type.equals("short")) {
            out.println("        info->" + setter + "( dataIn->readShort() );");
        }
        else if (type.equals("int")) {
            out.println("        info->" + setter + "( dataIn->readInt() );");
        }
        else if (type.equals("long")) {
            out.println("        info->" + setter + "( looseUnmarshalLong( wireFormat, dataIn ) );");
        }
        else if (type.equals("String")) {
            out.println("        info->" + setter + "( looseUnmarshalString( dataIn ) );");
        }
        else if (type.equals("byte[]") || type.equals("ByteSequence")) {
            if (size != null) {
                out.println("        info->" + setter + "( looseUnmarshalConstByteArray( dataIn, " + size.asInt() + " ) );");
            }
            else {
                out.println("        info->" + setter + "( looseUnmarshalByteArray( dataIn ) );");
            }
        }
        else if (isThrowable(property.getType())) {
            out.println("        info->" + setter + "( dynamic_cast< " + nativeType + "* >(");
            out.println("            looseUnmarshalBrokerError( wireFormat, dataIn ) ) );");
        }
        else if (isCachedProperty(property)) {
            out.println("        info->" + setter + "( dynamic_cast< " + nativeType + "* >( ");
            out.println("            looseUnmarshalCachedObject( wireFormat, dataIn ) ) );");
        }
        else {
            out.println("        info->" + setter + "( dynamic_cast< " + nativeType + "* >( ");
            out.println("            looseUnmarshalNestedObject( wireFormat, dataIn ) ) );");
        }
    }

    protected void generateLooseUnmarshalBodyForArrayProperty(PrintWriter out, JProperty property, JAnnotationValue size) {
        JClass propertyType = property.getType();
        String arrayType = propertyType.getArrayComponentType().getSimpleName();
        String propertyName = property.getSimpleName();
        String setter = property.getSetter().getSimpleName();
        String getter = property.getGetter().getSimpleName();

        out.println();
        if (size != null) {
            out.println("        {");
            out.println("            " + arrayType + "[] value = new " + arrayType + "[" + size.asInt() + "];");
            out.println("            " + "for( int i=0; i < " + size.asInt() + "; i++ ) {");
            out.println("                value[i] = (" + arrayType + ") looseUnmarshalNestedObject( wireFormat, dataIn );");
            out.println("            }");
            out.println("            info->" + setter + "( value );");
            out.println("        }");
        }
        else {
            out.println("        if( dataIn->readBoolean() ) {");
            out.println("            short size = dataIn->readShort();");
            out.println("            info->" + getter + "().reserve( size );");
            out.println("            for( int i = 0; i < size; i++ ) {");
            out.println("                info->" + getter + "().push_back( dynamic_cast<" + arrayType + "* >(");
            out.println("                    looseUnmarshalNestedObject( wireFormat, dataIn ) ) );");
            out.println("            }");
            out.println("        }");
            out.println("        else {");
            out.println("            info->" + getter + "().clear();");
            out.println("        }");
        }
    }


    protected void generateLooseMarshalBody(PrintWriter out) {
        List properties = getProperties();
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            JAnnotation annotation = property.getAnnotation("openwire:property");
            JAnnotationValue size = annotation.getValue("size");
            JClass propertyType = property.getType();
            String type = propertyType.getSimpleName();
            String getter = "info->" + property.getGetter().getSimpleName() + "()";

            if( type.equals( "boolean" ) ) {
                out.println("        dataOut->writeBoolean( " + getter + " );");
            }
            else if( type.equals("byte") ) {
                out.println("        dataOut->write( " + getter + " );");
            }
            else if( type.equals("char") ) {
                out.println("        dataOut->write( " + getter + " );");
            }
            else if( type.equals("short") ) {
                out.println("        dataOut->writeShort( " + getter + " );");
            }
            else if( type.equals("int")) {
                out.println("        dataOut->writeInt( " + getter + " );");
            }
            else if( type.equals("long") ) {
                out.println("        looseMarshalLong( wireFormat, " + getter + ", dataOut );");
            }
            else if( type.equals("String") ) {
                out.println("        looseMarshalString( " + getter + ", dataOut );");
            }
            else if( type.equals("byte[]") || type.equals("ByteSequence") ) {
                if(size != null) {
                    out.println("        dataOut->write( (const unsigned char*)(&" + getter + "[0]), 0, (int)" + size.asInt() + " );");
                }
                else {
                    out.println("        dataOut->write( " + getter + ".size() != 0 );");
                    out.println("        if( " + getter + ".size() != 0 ) {");
                    out.println("            dataOut->writeInt( (int)" + getter + ".size() );");
                    out.println("            dataOut->write( (const unsigned char*)(&" + getter + "[0]), 0, (int)" + getter + ".size() );");
                    out.println("        }");
                }
            }
            else if( propertyType.isArrayType() ) {
                if (size != null) {
                    out.println("        looseMarshalObjectArrayConstSize( wireFormat, " + getter + ", dataOut, " + size.asInt() + " );");
                }
                else {
                    out.println("        looseMarshalObjectArray( wireFormat, " + getter + ", dataOut );");
                }
            }
            else if( isThrowable( propertyType ) ) {
                out.println("        looseMarshalBrokerError( wireFormat, " + getter + ", dataOut );");
            }
            else {
                if( isCachedProperty( property ) ) {
                    out.println("        looseMarshalCachedObject( wireFormat, "+getter+", dataOut );");
                }
                else {
                    out.println("        looseMarshalNestedObject( wireFormat, "+getter+", dataOut );");
                }
            }
        }
    }


    protected void generateFile(PrintWriter out) throws Exception {
        generateLicence(out);

out.println("");
out.println("#include <activemq/connector/openwire/marshal/v"+getOpenwireVersion()+"/"+className+".h>");
out.println("");
out.println("#include <activemq/connector/openwire/commands/"+jclass.getSimpleName()+".h>");
out.println("#include <activemq/exceptions/ActiveMQException.h>");
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
out.println("using namespace activemq::connector;");
out.println("using namespace activemq::connector::openwire;");
out.println("using namespace activemq::connector::openwire::commands;");
out.println("using namespace activemq::connector::openwire::marshal;");
out.println("using namespace activemq::connector::openwire::utils;");
out.println("using namespace activemq::connector::openwire::marshal::v"+getOpenwireVersion()+";");
out.println("using namespace decaf;");
out.println("using namespace decaf::io;");
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
out.println("void "+className+"::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) throw( decaf::io::IOException ) {");
out.println("");
out.println("    try {");
out.println("");
out.println("        "+baseClass+"::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );");
out.println("");

    List properties = getProperties();
    boolean marshallerAware = isMarshallerAware();
    if( !properties.isEmpty() || marshallerAware ) {

        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>( dataStructure );");
    }

    if( marshallerAware ) {
out.println("        info->beforeUnmarshal( wireFormat );");
out.println("");
    }

    generateTightUnmarshalBody(out);

    if( marshallerAware ) {
out.println("");
out.println("        info->afterUnmarshal( wireFormat );");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW( decaf::io::IOException )" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )" );
out.println("    AMQ_CATCHALL_THROW( decaf::io::IOException )" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("int "+className+"::tightMarshal1( OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs ) throw( decaf::io::IOException ) {");
out.println("");
out.println("    try {");
out.println("");

    if( checkNeedsInfoPointerTM1() ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>( dataStructure );");
out.println("");
    }

    if( marshallerAware ) {
out.println("        info->beforeMarshal( wireFormat );");
    }

out.println("        int rc = "+baseClass+"::tightMarshal1( wireFormat, dataStructure, bs );");

    int baseSize = generateTightMarshal1Body(out);

out.println("");
out.println("        return rc + "+baseSize+";");
out.println("    }");
out.println("    AMQ_CATCH_RETHROW( decaf::io::IOException )" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )" );
out.println("    AMQ_CATCHALL_THROW( decaf::io::IOException )" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::tightMarshal2( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs ) throw( decaf::io::IOException ) {");
out.println("");
out.println("    try {");
out.println("");
out.println("        "+baseClass+"::tightMarshal2( wireFormat, dataStructure, dataOut, bs );");
out.println("");

    if( checkNeedsInfoPointerTM2() ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>( dataStructure );");
    }

    generateTightMarshal2Body(out);

    if( marshallerAware ) {
out.println("        info->afterMarshal( wireFormat );");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW( decaf::io::IOException )" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )" );
out.println("    AMQ_CATCHALL_THROW( decaf::io::IOException )" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::looseUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn ) throw( decaf::io::IOException ) {");
out.println("");
out.println("    try {");
out.println("");
out.println("        "+baseClass+"::looseUnmarshal( wireFormat, dataStructure, dataIn );");

    if( !properties.isEmpty() || marshallerAware ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>( dataStructure );");
    }

    if( marshallerAware ) {
out.println("        info->beforeUnmarshal( wireFormat );");
    }

    generateLooseUnmarshalBody(out);

    if( marshallerAware ) {
out.println("        info->afterUnmarshal( wireFormat );");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW( decaf::io::IOException )" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )" );
out.println("    AMQ_CATCHALL_THROW( decaf::io::IOException )" );
out.println("}");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void "+className+"::looseMarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut ) throw( decaf::io::IOException ) {");
out.println("");
out.println("    try {");
out.println("");

    if( !properties.isEmpty() || marshallerAware ) {
        String properClassName = getProperClassName( jclass.getSimpleName() );
out.println("        "+properClassName+"* info =");
out.println("            dynamic_cast<"+properClassName+"*>( dataStructure );");
    }

    if( marshallerAware ) {
out.println("        info->beforeMarshal( wireFormat );");
    }

out.println("        "+baseClass+"::looseMarshal( wireFormat, dataStructure, dataOut );");
out.println("");

    generateLooseMarshalBody(out);

    if( marshallerAware ) {
out.println("        info->afterMarshal( wireFormat );");
    }

out.println("    }");
out.println("    AMQ_CATCH_RETHROW( decaf::io::IOException )" );
out.println("    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )" );
out.println("    AMQ_CATCHALL_THROW( decaf::io::IOException )" );
out.println("}");
out.println("");
}

    public void generateFactory(PrintWriter out) {
        generateLicence(out);

out.println("#include <activemq/connector/openwire/marshal/v"+getOpenwireVersion()+"/MarshallerFactory.h>");

    List list = new ArrayList(getConcreteClasses());
    Collections.sort(list, new Comparator(){
        public int compare(Object o1, Object o2) {
            JClass c1 = (JClass) o1;
            JClass c2 = (JClass) o2;
            return c1.getSimpleName().compareTo(c2.getSimpleName());
    }});

    for (Iterator iter = list.iterator(); iter.hasNext();) {
        JClass jclass = (JClass) iter.next();
out.println("#include <activemq/connector/openwire/marshal/v"+getOpenwireVersion()+"/"+jclass.getSimpleName()+"Marshaller.h>");
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
out.println("using namespace activemq::connector;");
out.println("using namespace activemq::connector::openwire;");
out.println("using namespace activemq::connector::openwire::marshal;");
out.println("using namespace activemq::connector::openwire::marshal::v"+getOpenwireVersion()+";");
out.println("");
out.println("///////////////////////////////////////////////////////////////////////////////");
out.println("void MarshallerFactory::configure( OpenWireFormat* format ) {");
out.println("");

    for (Iterator iter = list.iterator(); iter.hasNext();) {
        JClass jclass = (JClass) iter.next();
out.println("    format->addMarshaller( new "+jclass.getSimpleName()+"Marshaller() );");
}

out.println("}");
out.println("");
    }
}
