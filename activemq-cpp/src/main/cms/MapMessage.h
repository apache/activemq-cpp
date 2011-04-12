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

#ifndef _CMS_MAPMESSAGE_H_
#define _CMS_MAPMESSAGE_H_

#include <cms/Config.h>
#include <cms/Message.h>

#include <cms/CMSException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageNotWriteableException.h>

namespace cms{

    /**
     * A MapMessage object is used to send a set of name-value pairs. The
     * names are String objects, and the values are primitive data types in
     * the Java programming language. The names must have a value that is
     * not null, and not an empty string. The entries can be accessed
     * sequentially or randomly by name. The order of the entries is
     * undefined. MapMessage inherits from the Message interface and adds
     * a message body that contains a Map.
     *
     * When a client receives a MapMessage, it is in read-only mode. If a
     * client attempts to write to the message at this point, a
     * MessageNotWriteableException is thrown.  To place the MapMessage back
     * into a state where it can be read from and written to, call the
     * <code>clearBody</code> method.
     *
     * MapMessage objects support the following conversion table. The marked
     * cases must be supported. The unmarked cases must throw a CMSException.
     * The String-to-primitive conversions may throw a MessageFormatException if
     * the primitive's valueOf() method does not accept it as a valid String
     * representation of the primitive.
     *
     * A value written as the row type can be read as the column type.
     *
     * <PRE>
     *  |        | boolean byte short char int long float double String byte[]
     *  |----------------------------------------------------------------------
     *  |boolean |    X                                            X
     *  |byte    |          X     X         X   X                  X
     *  |short   |                X         X   X                  X
     *  |char    |                     X                           X
     *  |int     |                          X   X                  X
     *  |long    |                              X                  X
     *  |float   |                                    X     X      X
     *  |double  |                                          X      X
     *  |String  |    X     X     X         X   X     X     X      X
     *  |byte[]  |                                                        X
     *  |----------------------------------------------------------------------
     * </PRE>
     *
     * @since 1.0
     */
    class CMS_API MapMessage : public Message {
    public:

        virtual ~MapMessage() throw();

        /**
         * Returns true if there are no values stored in the MapMessage body.
         *
         * @returns true if the body of the MapMessage contains no elements.
         *
         * @throws CMSException if the operation fails due to an internal error.
         */
        virtual bool isEmpty() const = 0;

        /**
         * Returns an Enumeration of all the names in the MapMessage
         * object.
         *
         * @return STL Vector of String values, each of which is the
         *         name of an item in the MapMessage
         *
         * @throws CMSException - if the operation fails due to an internal error.
         */
        virtual std::vector< std::string > getMapNames() const = 0;

        /**
         * Indicates whether an item exists in this MapMessage object.
         *
         * @param name
         *      String name of the Object in question
         * @return boolean value indicating if the name is in the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         */
        virtual bool itemExists( const std::string& name ) const = 0;

        /**
         * Returns the Boolean value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual bool getBoolean( const std::string& name ) const = 0;

        /**
         * Sets a boolean value with the specified name into the Map.
         *
         * @param name
         *      the name of the boolean
         * @param value
         *      the boolean value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWritableException - if the Message is in Read-only Mode.
         */
        virtual void setBoolean( const std::string& name, bool value ) = 0;

        /**
         * Returns the Byte value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual unsigned char getByte( const std::string& name ) const = 0;

        /**
         * Sets a Byte value with the specified name into the Map.
         *
         * @param name
         *      the name of the Byte
         * @param value
         *      the Byte value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setByte( const std::string& name, unsigned char value ) = 0;

        /**
         * Returns the Bytes value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual std::vector<unsigned char> getBytes( const std::string& name ) const = 0;

        /**
         * Sets a Bytes value with the specified name into the Map.
         *
         * @param name
         *      The name of the Bytes
         * @param value
         *      The Bytes value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setBytes( const std::string& name,
                               const std::vector<unsigned char>& value ) = 0;

        /**
         * Returns the Char value of the Specified name
         *
         * @param name
         *      name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual char getChar( const std::string& name ) const = 0;

        /**
         * Sets a Char value with the specified name into the Map.
         *
         * @param name
         *      the name of the Char
         * @param value
         *      the Char value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setChar( const std::string& name, char value ) = 0;

        /**
         * Returns the Double value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual double getDouble( const std::string& name ) const = 0;

        /**
         * Sets a Double value with the specified name into the Map.
         *
         * @param name
         *      The name of the Double
         * @param value
         *      The Double value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setDouble( const std::string& name, double value ) = 0;

        /**
         * Returns the Float value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual float getFloat( const std::string& name ) const = 0;

        /**
         * Sets a Float value with the specified name into the Map.
         *
         * @param name
         *      The name of the Float
         * @param value
         *      The Float value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setFloat( const std::string& name, float value ) = 0;

        /**
         * Returns the Int value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual int getInt( const std::string& name ) const = 0;

        /**
         * Sets a Int value with the specified name into the Map.
         *
         * @param name
         *      The name of the Int
         * @param value
         *      The Int value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setInt( const std::string& name, int value ) = 0;

        /**
         * Returns the Long value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual long long getLong( const std::string& name ) const = 0;

        /**
         * Sets a Long value with the specified name into the Map.
         *
         * @param name
         *      The name of the Long
         * @param value
         *      The Long value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setLong( const std::string& name, long long value ) = 0;

        /**
         * Returns the Short value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual short getShort( const std::string& name ) const = 0;

        /**
         * Sets a Short value with the specified name into the Map.
         *
         * @param name
         *      The name of the Short
         * @param value
         *      The Short value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setShort( const std::string& name, short value ) = 0;

        /**
         * Returns the String value of the Specified name
         *
         * @param name
         *      Name of the value to fetch from the map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual std::string getString( const std::string& name ) const = 0;

        /**
         * Sets a String value with the specified name into the Map.
         *
         * @param name
         *      The name of the String
         * @param value
         *      The String value to set in the Map
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setString( const std::string& name, const std::string& value ) = 0;

    };

}

#endif /*_CMS_MAPMESSAGE_H_*/
