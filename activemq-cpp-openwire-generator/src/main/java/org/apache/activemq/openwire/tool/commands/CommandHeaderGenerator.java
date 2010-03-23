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

import org.codehaus.jam.JProperty;

public class CommandHeaderGenerator extends CommandCodeGenerator {

    public void generate( PrintWriter out ) {

        // Start with the license.
        generateLicence(out);

        out.println("#ifndef _ACTIVEMQ_COMMANDS_"+getClassName().toUpperCase()+"_H_");
        out.println("#define _ACTIVEMQ_COMMANDS_"+getClassName().toUpperCase()+"_H_");
        out.println("");
        out.println("// Turn off warning message for ignored exception specification");
        out.println("#ifdef _MSC_VER");
        out.println("#pragma warning( disable : 4290 )");
        out.println("#endif");
        out.println("");

        populateIncludeFilesSet();
        for( String include : getIncludeFiles() ) {
            if( include != null ) {
                out.println("#include "+include);
            }
        }

        out.println("");
        generateNamespaceWrapper( out );
        generateForwardDeclarations( out );

        out.println("    using decaf::lang::Pointer;");
        out.println("");
        out.println("    /*");
        out.println("     *");
        out.println("     *  Command code for OpenWire format for "+getClassName() );
        out.println("     *");
        out.println("     *  NOTE!: This file is auto generated - do not modify!");
        out.println("     *         if you need to make a change, please see the Java Classes");
        out.println("     *         in the activemq-cpp-openwire-generator module");
        out.println("     *");
        out.println("     */");

        generateClassDefinition( out );
        generateProperties( out );

        if( !isAssignable() ) {
            out.println("    protected:");
            out.println("");
            out.println("        "+getClassName()+"( const "+getClassName()+"& );");
            out.println("        "+getClassName()+"& operator= ( const "+getClassName()+"& ) { return *this; };");
            out.println("");
        }

        out.println("    public:");
        out.println("");
        out.println("        "+getClassName()+"();");
        out.println("");

        if( isAssignable() ) {
            out.println("        "+getClassName()+"( const "+getClassName()+"& other );");
            out.println("");
        }

        generateAdditionalConstructors( out );

        out.println("        virtual ~"+getClassName()+"();");
        out.println("");
        out.println("        /**");
        out.println("         * Get the unique identifier that this object and its own");
        out.println("         * Marshaler share.");
        out.println("         * @returns new DataStructure type copy.");
        out.println("         */");
        out.println("        virtual unsigned char getDataStructureType() const;");
        out.println("");
        out.println("        /**");
        out.println("         * Clone this object and return a new instance that the");
        out.println("         * caller now owns, this will be an exact copy of this one");
        out.println("         * @returns new copy of this object.");
        out.println("         */");
        out.println("        virtual "+getClassName()+"* cloneDataStructure() const;");
        out.println("");
        out.println("        /**");
        out.println("         * Copy the contents of the passed object into this object's");
        out.println("         * members, overwriting any existing data.");
        out.println("         * @param src - Source Object");
        out.println("         */");
        out.println("        virtual void copyDataStructure( const DataStructure* src );");
        out.println("");
        out.println("        /**");
        out.println("         * Returns a string containing the information for this DataStructure");
        out.println("         * such as its type and value of its elements.");
        out.println("         * @return formatted string useful for debugging.");
        out.println("         */");
        out.println("        virtual std::string toString() const;");
        out.println("");
        out.println("        /**" );
        out.println("         * Compares the DataStructure passed in to this one, and returns if" );
        out.println("         * they are equivalent.  Equivalent here means that they are of the" );
        out.println("         * same type, and that each element of the objects are the same." );
        out.println("         * @returns true if DataStructure's are Equal." );
        out.println("         */" );
        out.println("        virtual bool equals( const DataStructure* value ) const;" );
        out.println("");

        generateAdditonalMembers( out );
        generatePropertyAccessors( out );

        if( isComparable() ) {
            out.println("        virtual int compareTo( const "+getClassName()+"& value ) const;");
            out.println("");
            out.println("        virtual bool equals( const "+getClassName()+"& value ) const;");
            out.println("");
            out.println("        virtual bool operator==( const "+getClassName()+"& value ) const;");
            out.println("");
            out.println("        virtual bool operator<( const "+getClassName()+"& value ) const;");
            out.println("");
        }

        if( isAssignable() ) {
            out.println("        "+getClassName()+"& operator= ( const "+getClassName()+"& other );");
            out.println("");
        }

        if( isGenIsClass() ) {
            out.println("        /**");
            out.println("         * @return an answer of true to the is"+getClassName()+"() query.");
            out.println("         */");
            out.println("        virtual bool is"+getClassName()+"() const {");
            out.println("            return true;");
            out.println("        }");
            out.println("");
        }

        if( getBaseClassName().equals( "BaseCommand" ) ) {
            out.println("        /**" );
            out.println("         * Allows a Visitor to visit this command and return a response to the" );
            out.println("         * command based on the command type being visited.  The command will call" );
            out.println("         * the proper processXXX method in the visitor." );
            out.println("         * " );
            out.println("         * @return a Response to the visitor being called or NULL if no response." );
            out.println("         */" );
            out.println("        virtual Pointer<Command> visit( activemq::state::CommandVisitor* visitor )" );
            out.println("            throw( exceptions::ActiveMQException );" );
            out.println("");
        }

        out.println("    };");
        out.println("");
        out.println("}}");
        out.println("");
        out.println("#endif /*_ACTIVEMQ_COMMANDS_"+getClassName().toUpperCase()+"_H_*/");
    }

    protected void populateIncludeFilesSet() {

        Set<String> includes = getIncludeFiles();

        includes.add("<vector>");
        includes.add("<string>");
        includes.add("<activemq/util/Config.h>");
        includes.add("<activemq/commands/"+getBaseClassName()+".h>");
        includes.add("<decaf/lang/Pointer.h>");

        if( isComparable() ) {
            includes.add("<decaf/lang/Comparable.h>");
        }

        for( JProperty property : getProperties() ) {
            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("String") &&
                !property.getType().getSimpleName().equals("ByteSequence") )
            {

                if( toHeaderFileName( property.getType() ) != null ) {
                    includes.add(
                        "<activemq/commands/"+toHeaderFileName( property.getType() )+".h>");
                }
            }
        }
    }

    protected void generateNamespaceWrapper( PrintWriter out ) {
        out.println("namespace activemq{");
        out.println("namespace commands{");
        out.println("");
    }

    protected void generateForwardDeclarations( PrintWriter out ) {
    }

    protected void generateClassDefinition( PrintWriter out ) {
        out.print("    class AMQCPP_API "+getClassName()+" : " );

        if( isComparable() ) {
            out.println("public "+ getBaseClassName() +", public decaf::lang::Comparable<"+getClassName()+"> {" );
        } else {
            out.print("public "+ getBaseClassName() +" {" );
            out.println("");
        }
    }

    protected void generateProperties( PrintWriter out ) {
        out.println("    protected:");
        out.println("");

        for( JProperty property : getProperties() ) {
            String type = toCppType(property.getType());
            String name = decapitalize(property.getSimpleName());

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

                type = "Pointer<" + type + ">";
            }

            out.println("        "+type+" "+name+";");
        }

        out.println("");
        out.println("    public:");
        out.println("");
        out.println("        const static unsigned char ID_"+
                    getClassName().toUpperCase()+" = "+getOpenWireOpCode()+";");
        out.println("");
        if( isComparable() ) {
            out.println("        typedef decaf::lang::PointerComparator<"+getClassName()+"> COMPARATOR;");
            out.println("");
        }
    }

    protected void generateAdditionalConstructors( PrintWriter out ) {
    }

    protected void generateAdditonalMembers( PrintWriter out ) {
    }

    protected void generatePropertyAccessors( PrintWriter out ) {

        for( JProperty property : getProperties() ) {
            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);
            String constness = "";

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

                    type = "Pointer<" + type + ">&";
                    constness = "const ";
            } else if( property.getType().getSimpleName().equals("String") ||
                       type.startsWith("std::vector") ) {

                type = type + "&";
                constness = "const ";
            }

            if( property.getType().isPrimitiveType() ) {
                out.println("        virtual "+type+" "+property.getGetter().getSimpleName()+"() const;");
            } else {
                out.println("        virtual const "+type+" "+property.getGetter().getSimpleName()+"() const;");
                out.println("        virtual "+type+" "+property.getGetter().getSimpleName()+"();");
            }

            out.println("        virtual void "+property.getSetter().getSimpleName()+"( "+constness+type+" "+parameterName+" );");
            out.println("");
        }
    }

}
