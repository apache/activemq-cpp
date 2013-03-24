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

import java.util.HashSet;
import java.util.Set;

/**
 * Provides methods to get a Source file and Header file Code generator given a Class
 * name.
 *
 * @since 2.0
 */
public class CommandCodeGeneratorsFactory {

    private Set<String> commandsWithShortcuts;

    /*
     * Here we store all Commands that need to have a isXXX method generated
     * such as isMessage.  We then check in the <code>checkNeedsShortcut</code>
     * method and if the Command being generated is in this list we create a
     * method call to override the virtual method in the base Command interface.
     */
    {
        commandsWithShortcuts = new HashSet<String>();
        commandsWithShortcuts.add("BrokerInfo");
        commandsWithShortcuts.add("ControlCommand");
        commandsWithShortcuts.add("ConnectionControl");
        commandsWithShortcuts.add("ConnectionError");
        commandsWithShortcuts.add("ConnectionInfo");
        commandsWithShortcuts.add("ConsumerControl");
        commandsWithShortcuts.add("ConsumerInfo");
        commandsWithShortcuts.add("FlushCommand");
        commandsWithShortcuts.add("KeepAliveInfo");
        commandsWithShortcuts.add("Message");
        commandsWithShortcuts.add("MessageAck");
        commandsWithShortcuts.add("MessageDispatch");
        commandsWithShortcuts.add("MessageDispatchNotification");
        commandsWithShortcuts.add("MessagePull");
        commandsWithShortcuts.add("RemoveInfo");
        commandsWithShortcuts.add("RemoveSubscriptionInfo");
        commandsWithShortcuts.add("ReplayCommand");
        commandsWithShortcuts.add("Response");
        commandsWithShortcuts.add("ProducerAck");
        commandsWithShortcuts.add("ProducerInfo");
        commandsWithShortcuts.add("ShutdownInfo");
        commandsWithShortcuts.add("TransactionInfo");
        commandsWithShortcuts.add("WireFormatInfo");
    }

    /**
     * Given a class name return an instance of a Header File Generator
     * that can generate the header file for the Class.
     *
     * @param className - name of the class to find the generator for
     *
     * @return a new Header File code generator.
     */
    public CommandCodeGenerator getHeaderGenerator( String className ) {

        CommandCodeGenerator generator = null;

        generator = this.getCodeGenerator( className, "Header" );
        if (className.endsWith("Id")) {
            generator.setComparable(true);
            generator.setAssignable(true);
            generator.setHashable(true);
        }

        if (this.commandsWithShortcuts.contains(className)) {
            generator.setGenIsClass(true);
        }

        return generator;
    }

    /**
     * Given a class name return an instance of a Source File Generator
     * that can generate the source file for the Class.
     *
     * @param className - name of the class to find the generator for
     *
     * @return a new Source File code generator.
     */
    public CommandCodeGenerator getSourceGenerator( String className ) {

        CommandCodeGenerator generator = null;

        generator = this.getCodeGenerator(className, "Source");
        if (className.endsWith("Id")) {
            generator.setComparable(true);
            generator.setAssignable(true);
            generator.setHashable(true);
        }

        return generator;
    }

    /**
     * Given a class name return an instance of a CSharp Class File Generator
     * that can generate the file for the Class.
     *
     * @param className - name of the class to find the generator for
     * @param fileType - type identifier for the generator, either "Source" or "Header"
     *
     * @return a new Header File code generator.
     */
    public CommandCodeGenerator getCodeGenerator(String className, String fileType) {

        Class<?> generatorClass = null;

        try {
            generatorClass = Class.forName("org.apache.activemq.openwire.tool.commands." +
                                           className + fileType + "Generator");
        } catch (ClassNotFoundException e) {
            try {
                generatorClass = Class
                        .forName("org.apache.activemq.openwire.tool.commands.Command"
                                + fileType + "Generator");
            } catch (ClassNotFoundException e1) {
                e1.printStackTrace();
                return null;
            }
        }

        CommandCodeGenerator generator;
        try {
            generator = (CommandCodeGenerator) generatorClass.newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
            return null;
        } catch (IllegalAccessException e) {
            e.printStackTrace();
            return null;
        }

        return generator;
    }
}
