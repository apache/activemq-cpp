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

import java.io.PrintWriter;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.HashSet;

import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;


/**
 *
 * @version $Revision: 379734 $
 */
public class AmqCppHeadersGenerator extends AmqCppClassesGenerator {

    private Set<String> commandsWithShortcuts;

    /*
     * Here we store all Commands that need to have a isXXX method generated
     * such as isMessage.  We then check in the <code>checkNeedsShortcut</code>
     * method and if the Command being generated is in this list we create a
     * method call to override the virtual method in the base Command interface.
     */
    {
        commandsWithShortcuts = new HashSet<String>();
        commandsWithShortcuts.add( "Response" );
        commandsWithShortcuts.add( "MessageDispatch" );
        commandsWithShortcuts.add( "BrokerInfo" );
        commandsWithShortcuts.add( "WireFormatInfo" );
        commandsWithShortcuts.add( "Message" );
        commandsWithShortcuts.add( "MessageAck" );
        commandsWithShortcuts.add( "ProducerAck" );
        commandsWithShortcuts.add( "MessageDispatchNotification" );
        commandsWithShortcuts.add( "ShutdownInfo" );
    }

    protected void checkNeedsShortcut( String className, PrintWriter out ) {

        if( this.commandsWithShortcuts.contains( className ) ) {
            out.println("        /**");
            out.println("         * @return an answer of true to the is"+className+"() query.");
            out.println("         */");
            out.println("        virtual bool is"+className+"() const {");
            out.println("            return true;");
            out.println("        }");
            out.println("");
        }
    }

    protected String getFilePostFix() {
        return ".h";
    }

    protected void generateFile(PrintWriter out) {
        generateLicence(out);

        boolean comparable = className.endsWith( "Id" );
        boolean assignable = className.endsWith( "Id" );

out.println("");
out.println("#ifndef _ACTIVEMQ_COMMANDS_"+className.toUpperCase()+"_H_");
out.println("#define _ACTIVEMQ_COMMANDS_"+className.toUpperCase()+"_H_");
out.println("");
out.println("// Turn off warning message for ignored exception specification");
out.println("#ifdef _MSC_VER");
out.println("#pragma warning( disable : 4290 )");
out.println("#endif");
out.println("");
out.println("#include <activemq/util/Config.h>");
out.println("#include <activemq/commands/"+baseClass+".h>");
        if( className.equals( "Message" ) ) {
out.println("#include <activemq/util/PrimitiveMap.h>");
out.println("#include <decaf/util/Date.h>");
        }
out.println("#include <decaf/lang/Pointer.h>");

        if( comparable ) {
out.println("#include <decaf/lang/Comparable.h>");
        }

        List properties = getProperties();
        Set<String> includeNames = new HashSet<String>();
        for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("String") &&
                !property.getType().getSimpleName().equals("ByteSequence") )
            {
                includeNames.add( toHeaderFileName( property.getType() ) );
            }
        }

        for( String includeName : includeNames ) {
            if( includeName != null ) {
                out.println("#include <activemq/commands/"+includeName+".h>");
            }
        }

out.println("#include <vector>");
out.println("#include <string>");
out.println("");
out.println("namespace activemq{");
        if( className.equals( "Message" ) ) {
out.println("namespace core{");
out.println("    class ActiveMQAckHandler;");
out.println("}");
        }
out.println("namespace commands{");
out.println("");
out.println("    /*");
out.println("     *");
out.println("     *  Command and marshaling code for OpenWire format for "+className );
out.println("     *");
out.println("     *");
out.println("     *  NOTE!: This file is auto generated - do not modify!");
out.println("     *         if you need to make a change, please see the Java Classes");
out.println("     *         in the activemq-openwire-generator module");
out.println("     *");
out.println("     */");
out.print("    class AMQCPP_API "+className+" : " );

        if( comparable ) {
out.println("public "+ baseClass +", public decaf::lang::Comparable<"+className+"> {" );
        } else {
out.print("public "+ baseClass +" {" );
out.println("");
        }

        if( className.equals( "Message" ) ) {
out.println("    private:");
out.println("");
out.println("        // Used to allow a client to call Message::acknowledge when in the Client");
out.println("        // Ack mode.");
out.println("        core::ActiveMQAckHandler* ackHandler;");
out.println("");
out.println("        // Message properties, these are Marshaled and Unmarshaled from the Message");
out.println("        // Command's marshaledProperties vector.");
out.println("        activemq::util::PrimitiveMap properties;");
out.println("");
        }

out.println("    protected:");
out.println("");

            if( className.equals( "Message" ) ) {

out.println("        static const unsigned int DEFAULT_MESSAGE_SIZE = 1024;");
out.println("");

            }

       for (Iterator iter = properties.iterator(); iter.hasNext();) {
            JProperty property = (JProperty) iter.next();
            String type = toCppType(property.getType());
            String name = decapitalize(property.getSimpleName());

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

                type = "decaf::lang::Pointer<" + type + ">";
            }

            out.println("        "+type+" "+name+";");

       }

       String typeName = className.toUpperCase();

       if( !assignable ) {
out.println("");
out.println("    protected:");
out.println("");
out.println("        "+className+"( const "+className+"& other );");
out.println("        "+className+"& operator= ( const "+className+"& other );");
       }
out.println("");
out.println("    public:");
out.println("");
out.println("        const static unsigned char ID_"+typeName+" = "+getOpenWireOpCode(jclass)+";");
out.println("");
        if( comparable ) {
out.println("        typedef decaf::lang::PointerComparator<"+className+"> COMPARATOR;");
out.println("");
        }
out.println("    public:");
out.println("");
out.println("        "+className+"();");
            if( assignable ) {
out.println("        "+className+"( const "+className+"& other );");
            }
out.println("        virtual ~"+className+"();");
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
out.println("        virtual "+className+"* cloneDataStructure() const;");
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

        // Check if a isXXX object shorcut should be generated and add it if so.
        checkNeedsShortcut( className, out );

        if( className.equals( "Message" ) ) {

out.println("        /**");
out.println("         * Handles the marshaling of the objects properties into the");
out.println("         * internal byte array before the object is marshaled to the");
out.println("         * wire");
out.println("         * @param wireFormat - the wireformat controller");
out.println("         */");
out.println("        virtual void beforeMarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )");
out.println("            throw ( decaf::io::IOException );");
out.println("");
out.println("        /**");
out.println("         * Called after unmarshaling is started to cleanup the object being");
out.println("         * unmarshaled.");
out.println("         * @param wireFormat - the wireformat object to control unmarshaling");
out.println("         */");
out.println("        virtual void afterUnmarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )");
out.println("            throw ( decaf::io::IOException );");
out.println("");
out.println("        /**");
out.println("         * Indicates that this command is aware of Marshaling, and needs");
out.println("         * to have its Marshaling methods invoked.");
out.println("         * @returns boolean indicating desire to be in marshaling stages");
out.println("         */");
out.println("        virtual bool isMarshalAware() const {");
out.println("            return true;");
out.println("        }");
out.println("");
out.println("        /**");
out.println("         * Sets the Acknowledgment Handler that this Message will use");
out.println("         * when the Acknowledge method is called.");
out.println("         * @param handler ActiveMQAckHandler to call");
out.println("         */");
out.println("        virtual void setAckHandler( core::ActiveMQAckHandler* handler ) {");
out.println("            this->ackHandler = handler;");
out.println("        }");
out.println("");
out.println("        /**");
out.println("         * Gets the Acknowledgment Handler that this Message will use");
out.println("         * when the Acknowledge method is called.");
out.println("         * @returns handler ActiveMQAckHandler to call or NULL if not set");
out.println("         */");
out.println("        virtual core::ActiveMQAckHandler* getAckHandler() const {");
out.println("            return this->ackHandler;");
out.println("        }");
out.println("");
out.println("        /**");
out.println("         * Returns the Size of this message in Bytes.");
out.println("         * @returns number of bytes this message equates to.");
out.println("         */");
out.println("        virtual unsigned int getSize() const;");
out.println("");
out.println("        /**");
out.println("         * Returns if this message has expired, meaning that its");
out.println("         * Expiration time has elapsed.");
out.println("         * @returns true if message is expired.");
out.println("         */");
out.println("        virtual bool isExpired() const {");
out.println("            long long expireTime = this->getExpiration();");
out.println("            long long currentTime = decaf::util::Date::getCurrentTimeMilliseconds();");
out.println("            if( expireTime > 0 && currentTime > expireTime ) {");
out.println("                return true;");
out.println("            }");
out.println("            return false;");
out.println("        }");
out.println("");
out.println("        /**");
out.println("         * Gets a reference to the Message's Properties object, allows the derived");
out.println("         * classes to get and set their own specific properties.");
out.println("         *");
out.println("         * @return a reference to the Primitive Map that holds message properties.");
out.println("         */");
out.println("        util::PrimitiveMap& getMessageProperties() {");
out.println("            return this->properties;");
out.println("        }");
out.println("        const util::PrimitiveMap& getMessageProperties() const {");
out.println("            return this->properties;");
out.println("        }");
out.println("");

        }

        if( baseClass.equals( "BaseCommand" ) ) {

out.println("        /**" );
out.println("         * Allows a Visitor to visit this command and return a response to the" );
out.println("         * command based on the command type being visited.  The command will call" );
out.println("         * the proper processXXX method in the visitor." );
out.println("         * " );
out.println("         * @return a Response to the visitor being called or NULL if no response." );
out.println("         */" );
out.println("        virtual decaf::lang::Pointer<commands::Command> visit( activemq::state::CommandVisitor* visitor )" );
out.println("            throw( exceptions::ActiveMQException );" );
out.println("");

        }

        for( Iterator iter = properties.iterator(); iter.hasNext(); ) {
            JProperty property = (JProperty) iter.next();
            String type = toCppType(property.getType());
            String propertyName = property.getSimpleName();
            String parameterName = decapitalize(propertyName);
            String constness = "";

            if( !property.getType().isPrimitiveType() &&
                !property.getType().getSimpleName().equals("ByteSequence") &&
                !property.getType().getSimpleName().equals("String") &&
                !type.startsWith("std::vector") ) {

                    type = "decaf::lang::Pointer<" + type + ">&";
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

        if( comparable ) {
out.println("        virtual int compareTo( const "+className+"& value ) const;");
out.println("");
out.println("        virtual bool equals( const "+className+"& value ) const;");
out.println("");
out.println("        virtual bool operator==( const "+className+"& value ) const;");
out.println("");
out.println("        virtual bool operator<( const "+className+"& value ) const;");
out.println("");
        }

        if( assignable ) {
out.println("        "+className+"& operator= ( const "+className+"& other );");
        }

out.println("    };");
out.println("");
out.println("}}");
out.println("");
out.println("#endif /*_ACTIVEMQ_COMMANDS_"+className.toUpperCase()+"_H_*/");

    }

}
