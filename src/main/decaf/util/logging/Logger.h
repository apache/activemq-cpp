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
#ifndef _DECAF_UTIL_LOGGING_LOGGER_H_
#define _DECAF_UTIL_LOGGING_LOGGER_H_

#include <decaf/util/logging/LoggerCommon.h>
#include <decaf/util/logging/LogRecord.h>
#include <decaf/util/logging/LogManager.h>
#include <decaf/util/logging/Handler.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <list>
#include <string>
#include <stdarg.h>

namespace decaf{
namespace util{
namespace logging{

    class Filter;

    /**
     * A Logger object is used to log messages for a specific system or application component.
     * Loggers are normally named, using a hierarchical dot-separated namespace. Logger names
     * can be arbitrary strings, but they should normally be based on the namespace or class
     * name of the logged component, such as decaf.net or org.apache.decaf. In addition it is
     * possible to create "anonymous" Loggers that are not stored in the Logger namespace.
     *
     * Logger objects may be obtained by calls on one of the getLogger factory methods. These
     * will either create a new Logger or return a suitable existing Logger.
     *
     * Logging messages will be forwarded to registered Handler objects, which can forward
     * the messages to a variety of destinations, including consoles, files, OS logs, etc.
     *
     * Each Logger keeps track of a "parent" Logger, which is its nearest existing ancestor
     * in the Logger namespace.
     *
     * Each Logger has a "Level" associated with it. This reflects a minimum Level that this
     * logger cares about. If a Logger's level is set to Level::INHERIT, then its effective level
     * is inherited from its parent, which may in turn obtain it recursively from its parent,
     * and so on up the tree.
     *
     * The log level can be configured based on the properties from the logging configuration
     * file, as described in the description of the LogManager class. However it may also be
     * dynamically changed by calls on the Logger.setLevel method. If a logger's level is changed
     * the change may also affect child loggers, since any child logger that has 'inherit' as its
     * level will inherit its effective level from its parent.
     *
     * On each logging call the Logger initially performs a cheap check of the request level
     * (e.g. SEVERE or FINE) against the effective log level of the logger. If the request level
     * is lower than the log level, the logging call returns immediately.
     *
     * After passing this initial (cheap) test, the Logger will allocate a LogRecord to describe
     * the logging message. It will then call a Filter (if present) to do a more detailed check
     * on whether the record should be published. If that passes it will then publish the LogRecord
     * to its output Handlers. By default, loggers also publish to their parent's Handlers,
     * recursively up the tree.
     *
     * Formatting is the responsibility of the output Handler, which will typically call a Formatter.
     *
     * Note that formatting need not occur synchronously. It may be delayed until a LogRecord is
     * actually written to an external sink.
     *
     * All methods on Logger are thread safe.
     *
     * @since 1.0
     */
    class DECAF_API Logger {
    private:

        // The name of this Logger
        std::string name;

        // The Parent of this Logger
        Logger* parent;

        // list of Handlers owned by this logger
        std::list<Handler*> handlers;

        // Filter used by this Logger
        Filter* filter;

        // The Log Level of this Logger
        Level level;

        // Using Parent Handlers?
        bool useParentHandlers;

    private:

        Logger( const Logger& );
        Logger& operator= ( const Logger& );

    protected:

        /**
         * Creates a new instance of the Logger with the given name.
         *
         * The logger will be initially configured with a null Level
         * and with useParentHandlers true.
         *
         * @param name
         *      A name for the logger. This should be a dot-separated name and
         *      should normally be based on the package name or class name of the
         *      subsystem, such as decaf.net or org.apache.decaf.  It may be empty
         *      for anonymous Loggers.
         */
        Logger( const std::string& name );

    public:

        virtual ~Logger();

        /**
         * Gets the name of this Logger
         * @return logger name
         */
        const std::string& getName() const {
            return name;
        }

        /**
         * Gets the parent of this Logger which will be the nearest existing Logger in this
         * Loggers namespace.
         *
         * If this is the Root Logger than this method returns NULL.
         *
         * @return Pointer to this Loggers nearest parent Logger.
         */
        Logger* getParent() const {
            return this->parent;
        }

        /**
         * Set the parent for this Logger. This method is used by the LogManager to update
         * a Logger when the namespace changes.
         *
         * It should not be called from application code.
         */
        void setParent( Logger* parent ) {
            this->parent = parent;
        }

        /**
         * Add a log Handler to receive logging messages.
         *
         * By default, Loggers also send their output to their parent logger.
         * Typically the root Logger is configured with a set of Handlers
         * that essentially act as default handlers for all loggers.
         *
         * The ownership of the given Handler is passed to the Logger and the
         * Handler will be deleted when this Logger is destroyed unless the caller
         * first calls removeHandler with the same pointer value as was originally
         * given.
         *
         * @param handler A Logging Handler
         *
         * @throws NullPointerException if the Handler given is NULL.
         */
        void addHandler( Handler* handler );

        /**
         * Removes the specified Handler from this logger, ownership of the
         * Handler pointer is returned to the caller.
         *
         * Returns silently if the given Handler is not found.
         *
         * @param handler The Handler to remove
         */
        void removeHandler( Handler* handler );

        /**
         * Gets a vector containing all the handlers that this class
         * has been assigned to use.
         * @returns a list of handlers that are used by this logger
         */
        const std::list<Handler*>& getHandlers() const;

        /**
         * Set a filter to control output on this Logger.
         *
         * After passing the initial "level" check, the Logger will call
         * this Filter to check if a log record should really be published.
         *
         * The caller releases ownership of this filter to this logger
         *
         * @param filter
         *      The Filter to use, (can be NULL).
         */
        void setFilter( Filter* filter );

        /**
         * Gets the Filter object that this class is using.
         * @return the Filter in use, (can be NULL).
         */
        const Filter* getFilter() const {
            return filter;
        }

        /**
         * Get the log Level that has been specified for this Logger. The
         * result may be the INHERIT level, which means that this logger's
         * effective level will be inherited from its parent.
         *
         * @return the level that is currently set
         */
        Level getLevel() const {
            return level;
        }

        /**
         * Set the log level specifying which message levels will be logged
         * by this logger. Message levels lower than this value will be
         * discarded. The level value Level::OFF can be used to turn off
         * logging.
         *
         * If the new level is the INHERIT Level, it means that this node
         * should inherit its level from its nearest ancestor with a
         * specific (non-INHERIT) level value.
         *
         * @param level
         *      The new Level value to use when logging.
         */
        void setLevel( const Level& level ) {
            this->level = level;
        }

        /**
         * Discover whether or not this logger is sending its output to
         * its parent logger.
         * @return true if using Parent Handlers
         */
        bool getUseParentHandlers() const {
            return useParentHandlers;
        }

        /**
         * Specify whether or not this logger should send its output to it's
         * parent Logger. This means that any LogRecords will also be
         * written to the parent's Handlers, and potentially to its parent,
         * recursively up the namespace.
         *
         * @param value
         *      True is output is to be written to the parent
         */
        void setUseParentHandlers( bool value ) {
            this->useParentHandlers = value;
        }

    public:   // Logging Methods.

        /**
         * Logs an Block Enter message
         *
         * This is a convenience method that is used to tag a block enter, a log record
         * with the given information is logged at the Level::FINER log level.
         *
         * @param blockName
         *      The source block name, (usually ClassName::MethodName, or MethodName).
         * @param file
         *      The source file name where this method was called from.
         * @param line
         *      The source line number where this method was called from.
         */
        virtual void entering( const std::string& blockName,
                               const std::string& file,
                               const int line );

        /**
         * Logs an Block Exit message
         *
         * This is a convenience method that is used to tag a block enter, a log record
         * with the given information is logged at the Level::FINER log level.
         *
         * @param blockName
         *      The source block name, (usually ClassName::MethodName, or MethodName).
         * @param file
         *      The source file name where this method was called from.
         * @param line
         *      The source line number where this method was called from.
         */
        virtual void exiting( const std::string& blockName,
                              const std::string& file,
                              const int line );

        /**
         * Log a SEVERE Level Log
         *
         * If the logger is currently enabled for the SEVERE message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void severe( const std::string& file,
                             const int line,
                             const std::string functionName,
                             const std::string& message );

        /**
         * Log a WARN Level Log
         *
         * If the logger is currently enabled for the WARN message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void warning( const std::string& file,
                              const int line,
                              const std::string functionName,
                              const std::string& message );

        /**
         * Log a INFO Level Log
         *
         * If the logger is currently enabled for the INFO message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void info( const std::string& file,
                           const int line,
                           const std::string functionName,
                           const std::string& message );

        /**
         * Log a DEBUG Level Log
         *
         * If the logger is currently enabled for the DEBUG message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void debug( const std::string& file,
                            const int line,
                            const std::string functionName,
                            const std::string& message );

        /**
         * Log a CONFIG Level Log
         *
         * If the logger is currently enabled for the CONFIG message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void config( const std::string& file,
                             const int line,
                             const std::string functionName,
                             const std::string& message );

        /**
         * Log a FINE Level Log
         *
         * If the logger is currently enabled for the FINE message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void fine( const std::string& file,
                           const int line,
                           const std::string functionName,
                           const std::string& message );

        /**
         * Log a FINER Level Log
         *
         * If the logger is currently enabled for the FINER message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void finer( const std::string& file,
                            const int line,
                            const std::string functionName,
                            const std::string& message );

        /**
         * Log a FINEST Level Log
         *
         * If the logger is currently enabled for the FINEST message level then the
         * given message is forwarded to all the registered output Handler objects.
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param message
         *      The message to log at this Level.
         */
        virtual void finest( const std::string& file,
                             const int line,
                             const std::string functionName,
                             const std::string& message );

        /**
         * Log throwing an exception.
         *
         * This is a convenience method to log that a method is terminating by throwing an
         * exception. The logging is done using the FINER level.
         *
         * If the logger is currently enabled for the given message level then the given
         * arguments are stored in a LogRecord which is forwarded to all registered output
         * handlers. The LogRecord's message is set to "THROW".
         *
         * @param file
         *      The file name where the log was generated.
         * @param line
         *      The line number where the log was generated.
         * @param functionName
         *      The name of the function that logged this.
         * @param thrown
         *      The Throwable that will be thrown, will be cloned.
         */
        virtual void throwing( const std::string& file,
                               const int line,
                               const std::string functionName,
                               const decaf::lang::Throwable& thrown );

        /**
         * Check if a message of the given level would actually be logged
         * by this logger. This check is based on the Loggers effective
         * level, which may be inherited from its parent.
         * @param level - a message logging level
         * @returns true if the given message level is currently being logged.
         */
        virtual bool isLoggable( const Level& level ) const;

        /**
         * Log a LogRecord.
         * All the other logging methods in this class call through this
         * method to actually perform any logging. Subclasses can override
         * this single method to capture all log activity.
         * @param record - the LogRecord to be published
         */
        virtual void log( LogRecord& record );

        /**
         * Log a message, with no arguments.
         * <p>
         * If the logger is currently enabled for the given message level
         * then the given message is forwarded to all the registered output
         * Handler objects
         * @param level the Level to log at
         * @param message the message to log
         */
        virtual void log( const Level& level, const std::string& message );

        /**
         * Log a message, with the list of params that is formatted into
         * the message string.
         * <p>
         * If the logger is currently enabled for the given message level
         * then the given message is forwarded to all the registered output
         * Handler objects
         * @param level the Level to log at
         * @param file the message to log
         * @param line the line in the file
         * @param ... variable length argument to format the message string.
         */
        virtual void log( const Level& levels,
                          const std::string& file,
                          const int line,
                          const std::string& message, ... );

        /**
         * Log a message, with associated Throwable information.
         * If the logger is currently enabled for the given message level
         * then the given arguments are stored in a LogRecord which is
         * forwarded to all registered output handlers.
         * Note that the thrown argument is stored in the LogRecord thrown
         * property, rather than the LogRecord parameters property. Thus is
         * it processed specially by output Formatters and is not treated
         * as a formatting parameter to the LogRecord message property.
         * @param level the Level to log at.
         * @param file File that the message was logged in.
         * @param line the line number where the message was logged at.
         * @param message the message to log.
         * @param ex the Exception to log
         */
        virtual void log( const Level& level,
                          const std::string& file,
                          const int line,
                          const std::string& message,
                          lang::Exception& ex );

    public:

        /**
         * Creates an anonymous logger
         * <p>
         * The newly created Logger is not registered in the LogManager
         * namespace. There will be no access checks on updates to the
         * logger.
         * Even although the new logger is anonymous, it is configured to
         * have the root logger ("") as its parent. This means that by
         * default it inherits its effective level and handlers from the
         * root logger.
         * <p>
         * The caller is responsible for destroying the returned logger.
         * @return Newly created anonymous logger
         */
        static Logger* getAnonymousLogger();

        /**
         * Find or create a logger for a named subsystem. If a logger has
         * already been created with the given name it is returned.
         * Otherwise a new logger is created.
         * <p>
         * If a new logger is created its log level will be configured based
         * on the LogManager and it will configured to also send logging
         * output to its parent loggers Handlers. It will be registered in
         * the LogManager global namespace.
         * @param name - A name for the logger. This should be a
         * dot-separated name and should normally be based on the package
         * name or class name of the subsystem, such as cms or
         * activemq.core.ActiveMQConnection
         * @return a suitable logger.
         */
        static Logger* getLogger( const std::string& name );

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_LOGGER_H_*/
