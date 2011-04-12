/*
* Licensed to the Apache Software Foundation (ASF) under one or more
* contributor license agreements.  See the NOTICE file distributed with
* this work for additional information regarding copyright ownership.
* The ASF licenses this file to You under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef _DECAF_UTIL_LOGGING_LOGMANAGER_H_
#define _DECAF_UTIL_LOGGING_LOGMANAGER_H_

#include <map>
#include <list>
#include <string>
#include <vector>

#include <decaf/lang/Pointer.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

#include <decaf/io/IOException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace decaf{
namespace lang{
    class Runtime;
}
namespace io{
    class InputStream;
}
namespace util{
namespace logging{

    class Logger;
    class LogManagerInternals;
    class PropertyChangeListener;

    using decaf::lang::Pointer;

    /**
     * There is a single global LogManager object that is used to maintain
     * a set of shared state about Loggers and log services.
     *
     * This LogManager object:
     *
     *   * Manages a hierarchical namespace of Logger objects. All named
     *     Loggers are stored in this namespace.
     *   * Manages a set of logging control properties. These are simple
     *     key-value pairs that can be used by Handlers and other logging
     *     objects to configure themselves.
     *
     * The global LogManager object can be retrieved using
     * LogManager::getLogManager(). The LogManager object is created during
     * class initialization and cannot subsequently be changed.
     *
     * ***TODO****
     * By default, the LogManager reads its initial configuration from a
     * properties file "lib/logging.properties" in the JRE directory. If
     * you edit that property file you can change the default logging
     * configuration for all uses of that JRE.
     *
     * In addition, the LogManager uses two optional system properties that
     * allow more control over reading the initial configuration:
     *
     *    * "decaf.logger.config.class"
     *    * "decaf.logger.config.file"
     *
     * These two properties may be set via the Preferences API, or as
     * command line property definitions to the "java" command, or as
     * system property definitions passed to JNI_CreateJavaVM.
     *
     * If the "java.util.logging.config.class" property is set, then the
     * property value is treated as a class name. The given class will be
     * loaded, an object will be instantiated, and that object's constructor
     * is responsible for reading in the initial configuration. (That object
     * may use other system properties to control its configuration.) The
     * alternate configuration class can use readConfiguration(InputStream)
     * to define properties in the LogManager.
     *
     * If "decaf.util.logging.config.class" property is not set, then the
     * "decaf.util.logging.config.file" system property can be used to specify
     * a properties file (in decaf.util.Properties format). The initial
     * logging configuration will be read from this file.
     *
     * If neither of these properties is defined then, as described above,
     * the LogManager will read its initial configuration from a properties
     * file "lib/logging.properties" in the working directory.
     *
     * The properties for loggers and Handlers will have names starting with
     * the dot-separated name for the handler or logger.
     * ***TODO****
     *
     * The global logging properties may include:
     *
     *    * A property "handlers". This defines a whitespace separated
     *      list of class names for handler classes to load and register as
     *      handlers on the root Logger (the Logger named ""). Each class
     *      name must be for a Handler class which has a default constructor.
     *      Note that these Handlers may be created lazily, when they are
     *      first used.
     *    * A property "<logger>.handlers". This defines a whitespace or
     *      comma separated list of class names for handlers classes to load
     *      and register as handlers to the specified logger. Each class name
     *      must be for a Handler class which has a default constructor. Note
     *      that these Handlers may be created lazily, when they are first
     *      used.
     *    * A property "<logger>.useParentHandlers". This defines a boolean
     *      value. By default every logger calls its parent in addition to
     *      handling the logging message itself, this often result in
     *      messages being handled by the root logger as well. When setting
     *      this property to false a Handler needs to be configured for this
     *      logger otherwise no logging messages are delivered.
     *    * A property "config". This property is intended to allow arbitrary
     *      configuration code to be run. The property defines a whitespace
     *      separated list of class names. A new instance will be created for
     *      each named class. The default constructor of each class may
     *      execute arbitrary code to update the logging configuration, such
     *      as setting logger levels, adding handlers, adding filters, etc.
     *
     * Loggers are organized into a naming hierarchy based on their dot
     * separated names. Thus "a.b.c" is a child of "a.b", but "a.b1" and
     * a.b2" are peers.
     *
     * All properties whose names end with ".level" are assumed to define
     * log levels for Loggers. Thus "foo.level" defines a log level for
     * the logger called "foo" and (recursively) for any of its children
     * in the naming hierarchy. Log Levels are applied in the order they
     * are defined in the properties file. Thus level settings for child
     * nodes in the tree should come after settings for their parents. The
     * property name ".level" can be used to set the level for the root of
     * the tree.
     *
     * All methods on the LogManager object are multi-thread safe.
     *
     * @since 1.0
     */
    class DECAF_API LogManager {
    private:

        // Change listener on this class's Properties
        std::list<PropertyChangeListener*> listeners;

        // Properties of the Log Manager
        util::Properties properties;

        // Data structure for LogManager Internal data.
        Pointer<LogManagerInternals> internal;

    public:

        virtual ~LogManager();

        /**
         * Add a named logger. This does nothing and returns false if a logger with
         * the same name is already registered.
         *
         * The Logger factory methods call this method to register each newly created Logger.
         *
         * @param logger
         *      The new Logger instance to add to this LogManager.
         *
         * @throws NullPointerException if logger is NULL.
         * @throws IllegalArgumentException if the logger has no name.
         */
        bool addLogger( Logger* logger );

        /**
         * Retrieves or creates a new Logger using the name specified
         * a new logger inherits the configuration of the logger's
         * parent if there is no configuration data for the logger.
         * @param name The name of the Logger.
         */
        Logger* getLogger( const std::string& name );

        /**
         * Gets a list of known Logger Names from this Manager, new loggers added
         * while this method is in progress are not garunteed to be in the list.
         *
         * @param names
         *      STL Vector to hold string logger names.
         *
         * @return names count of how many loggers were inserted.
         */
        int getLoggerNames( const std::vector<std::string>& names );

        /**
         * Sets the Properties this LogManager should use to configure
         * its loggers.  Once set a properties change event is fired.
         * @param properties Pointer to read the configuration from
         */
        void setProperties( const util::Properties& properties );

        /**
         * Gets a reference to the Logging Properties used by this
         * logger.
         * @returns The Logger Properties Pointer
         */
        const util::Properties& getProperties() const {
            return properties;
        }

        /**
         * Gets the value of a named property of this LogManager.
         *
         * @param name
         *      The name of the Property to retrieve.
         *
         * @return the value of the property
         */
        std::string getProperty( const std::string& name );

        /**
         * Adds a change listener for LogManager Properties, adding the same
         * instance of a change event listener does nothing.
         *
         * @param listener
         *      The PropertyChangeListener to add (can be NULL).
         */
        void addPropertyChangeListener( PropertyChangeListener* listener );

        /**
         * Removes a properties change listener from the LogManager, if the
         * listener is not found of the param is NULL this method returns
         * silently.
         *
         * @param listener
         *      The PropertyChangeListener to remove from the listeners set.
         */
        void removePropertyChangeListener( PropertyChangeListener* listener );

        /**
         * Reinitialize the logging properties and reread the logging configuration.
         *
         * The same rules are used for locating the configuration properties as are used at
         * startup. So normally the logging properties will be re-read from the same file
         * that was used at startup.
         *
         * Any log level definitions in the new configuration file will be applied using
         * Logger.setLevel(), if the target Logger exists.
         *
         * A PropertyChangeEvent will be fired after the properties are read.
         *
         * @throws IOException if an I/O error occurs.
         */
        void readConfiguration();

        /**
         * Reinitialize the logging properties and reread the logging configuration from the
         * given stream, which should be in decaf.util.Properties format. A PropertyChangeEvent
         * will be fired after the properties are read.
         *
         * Any log level definitions in the new configuration file will be applied using
         * Logger.setLevel(), if the target Logger exists.
         *
         * @param stream
         *      The InputStream to read the Properties from.
         *
         * @throws NullPointerException if stream is NULL.
         * @throws IOException if an I/O error occurs.
         */
        void readConfiguration( decaf::io::InputStream* stream );

        /**
         * Reset the logging configuration.
         *
         * For all named loggers, the reset operation removes and closes all Handlers and
         * (except for the root logger) sets the level to INHERIT. The root logger's level
         * is set to Level::INFO.
         */
        void reset();

    public:     // Static Singleton Methods.

        /**
         * Get the global LogManager instance.
         * @return A reference to the global LogManager instance.
         */
        static LogManager& getLogManager();

    private:

        /**
         * Initialize the Logging subsystem.
         */
        static void initialize();

        /**
         * Shutdown the Logging subsystem.
         */
        static void shutdown();

    protected:

        /**
         * Constructor, hidden to protect against direct instantiation
         */
        LogManager();

        /**
         * Copy Constructor
         * @param manager the Manager to copy
         */
        LogManager( const LogManager& manager );

        /**
         * Assignment operator
         * @param manager the manager to assign from
         */
        void operator=( const LogManager& manager );

    private:

        friend class decaf::lang::Runtime;

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_LOGMANAGER_H_*/
