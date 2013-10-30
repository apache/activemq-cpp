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

#ifndef _DECAF_NET_URLCONNECTION_H_
#define _DECAF_NET_URLCONNECTION_H_

#include <decaf/util/Config.h>

#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/net/URL.h>
#include <decaf/net/UnknownServiceException.h>

namespace decaf {
namespace net {

    class URLConnectionImpl;

    /**
     * Concrete implementations of the abstract {@code URLConnection} class provide
     * a communication link to a URL for exchanging data with a specific protocol
     * type. A {@code URLConnection} can only be set up after the instantiation but
     * before connecting to the remote resource.
     *
     * @since 1.0
     */
    class DECAF_API URLConnection {
    private:

        URLConnectionImpl* impl;

        static bool defaultAllowUserInteraction;

        static bool defaultUseCaches;

    protected:

        URL url;

        /**
         * The data must be modified more recently than this time in milliseconds
         * since January 1, 1970, GMT to be transmitted.
         */
        long long ifModifiedSince;

        /**
         * Specifies whether the using of caches is enabled or the data has to be
         * recent for every request.
         */
        bool useCaches;

        /**
         * Specifies whether this {@code URLConnection} is already connected to the
         * remote resource. If this field is set to {@code true} the flags for
         * setting up the connection are not changeable anymore.
         */
        bool connected;

        /**
         * Specifies whether this {@code URLConnection} allows sending data.
         */
        bool doOutput;

        /**
         * Specifies whether this {@code URLConnection} allows receiving data.
         */
        bool doInput;

        /**
         * Specifies whether this {@code URLConnection} allows user interaction as
         * it is needed for authentication purposes.
         */
        bool allowUserInteraction;

    private:

        URLConnection(const URLConnection&);
        URLConnection& operator= (const URLConnection&);

    protected:

        URLConnection(const URL& url);

    public:

        virtual ~URLConnection();

        /**
         * Establishes the connection to the earlier configured resource. The
         * connection can only be set up before this method has been called.
         *
         * @throws IOException if an error occurs while connecting to the resource.
         */
        virtual void connect() = 0;

    public:

        /**
         * Gets the option value which indicates whether user interaction is allowed
         * on this URLConnection.
         *
         * @return the value of the option allowUserInteraction.
         */
        bool getAllowUserInteraction() const {
            return allowUserInteraction;
        }

        /**
         * Sets the flag indicating whether this connection allows user interaction
         * or not. This method can only be called prior to the connection
         * establishment.
         *
         * @param newValue
         *      the value of the flag to be set.
         * @throws IllegalStateException
         *      if this method attempts to change the flag after the connection has been established.
         */
        void setAllowUserInteraction(bool newValue);

        /**
         * Gets the content encoding type specified by the response header field
         * content-encoding or empty string if this field is not set.
         *
         * @return the value of the response header field 'content-encoding'.
         */
        std::string getContentEncoding() const {
            return getHeaderField("Content-Encoding");
        }

        /**
         * Gets the content length in bytes specified by the response header field
         * 'content-length' or '-1' if this field is not set.
         *
         * @return the value of the response header field 'content-length'.
         */
        int getContentLength() const {
            return getHeaderFieldInt("Content-Length", -1);
        }

        /**
         * Gets the MIME-type of the content specified by the response header field
         * 'content-type' or empty string if type is unknown.
         *
         * @return the value of the response header field 'content-type'.
         */
        std::string getContentType() const {
            return getHeaderField("Content-Type");
        }

        /**
         * Gets the timestamp when this response has been sent as a date in
         * milliseconds since January 1, 1970 GMT or '0' if this timestamp is
         * unknown.
         *
         * @return the sending timestamp of the current response.
         */
        long long getDate() const {
            return getHeaderFieldDate("Date", 0);
        }

        /**
         * Gets the default setting whether this connection allows using caches.
         *
         * @return the value of the default setting defaultUseCaches.
         */
        bool getDefaultUseCaches() const {
            return defaultUseCaches;
        }

        /**
         * Sets the default value for the flag indicating whether this connection
         * allows to use caches. Existing URLConnections are unaffected.
         *
         * @param newValue
         *      the default value of the flag to be used for new connections.
         */
        void setDefaultUseCaches(bool newValue);

        /**
         * Gets the value of the option {@code doInput} which specifies whether this
         * connection allows to receive data.
         *
         * @return true if this connection allows input, false otherwise.
         */
        bool getDoInput() const {
            return doInput;
        }

        /**
         * Sets the flag indicating whether this URLConnection allows input.
         * It cannot be set after the connection is established.
         *
         * @param newValue
         *      the new value for the flag to be set.
         * @throws IllegalAccessError
         *      if this method attempts to change the value after the
         *      connection has been already established.
         */
        void setDoInput(bool newValue);

        /**
         * Gets the value of the option doOutput which specifies whether
         * this connection allows to send data.
         *
         * @return true if this connection allows output, false otherwise.
         */
        bool getDoOutput() const {
            return doOutput;
        }

        /**
         * Sets the flag indicating whether this URLConnection allows
         * output. It cannot be set after the connection is established.
         *
         * @param newValue
         *      the new value for the flag to be set.
         * @throws IllegalAccessError
         *      if this method attempts to change the value after the
         *      connection has been already established.
         */
        void setDoOutput(bool newValue);

        /**
         * Gets the timestamp when this response will be expired in milliseconds
         * since January 1, 1970 GMT or 0 if this timestamp is unknown.
         *
         * @return the value of the response header field expires.
         */
        long getExpiration() const {
            return getHeaderFieldDate("Expires", 0);
        }

        /**
         * Gets the URL represented by this URLConnection.
         *
         * @return the URL of this connection.
         */
        URL getURL() const {
            return url;
        }

        /**
         * Gets the value of the flag which specifies whether this URLConnection
         * allows to use caches.
         *
         * @return true if using caches is allowed, false otherwise.
         */
        bool getUseCaches() const {
            return useCaches;
        }

        /**
         * Sets the flag indicating whether this connection allows to use caches or
         * not. This method can only be called prior to the connection
         * establishment.
         *
         * @param newValue
         *      the value of the flag to be set.
         * @throws IllegalStateException
         *      if this method attempts to change the flag after the connection has been established.
         */
        void setUseCaches(bool newValue);

        /**
         * Gets the configured connecting timeout.
         *
         * @return the connecting timeout value in milliseconds.
         */
        int getConnectTimeout() const;

        /**
         * Sets the timeout value in milliseconds for establishing the connection to
         * the resource pointed by this URLConnection instance. A  SocketTimeoutException
         * is thrown if the connection could not be established in this time. Default is
         * 0 which stands for an infinite timeout.
         *
         * @param timeout
         *      the connecting timeout in milliseconds.
         * @throws IllegalArgumentException
         *      if the parameter timeout is less than zero.
         */
        void setConnectTimeout(int timeout);

        /**
         * Gets the configured timeout for reading from the input stream of an
         * established connection to the resource.
         *
         * @return the reading timeout value in milliseconds.
         */
        int getReadTimeout() const;

        /**
         * Sets the timeout value in milliseconds for reading from the input stream
         * of an established connection to the resource. A SocketTimeoutException
         * is thrown if the connection could not be established in this time. Default
         * is code 0 which stands for an infinite timeout.
         *
         * @param timeout
         *      the reading timeout in milliseconds.
         * @throws IllegalArgumentException
         *      if the parameter timeout is less than zero.
         */
        void setReadTimeout(int timeout);

        /**
         * Returns the string representation containing the name of this class and
         * the URL.
         *
         * @return the string representation of this URLConnection instance.
         */
        virtual std::string toString() const {
            return std::string("URLConnection:") + url.toString();
        }

        /**
         * Gets the header value at the field position pos or empty string
         * if the header has fewer than pos fields. The current implementation
         * of this method returns always empty string.
         *
         * @param pos
         *      the field position of the response header.
         *
         * @return the value of the field at position pos.
         */
        virtual std::string getHeaderField(int pos DECAF_UNUSED) const {
            return "";
        }

        /**
         * Gets the value of the header field specified by key or empty string
         * if there is no field with this name. The current implementation of
         * this method returns always empty string.
         *
         * @param key
         *      the name of the header field.
         *
         * @return the value of the header field.
         */
        virtual std::string getHeaderField(const std::string& key DECAF_UNUSED) const {
            return "";
        }

        /**
         * Gets the specified header value as a date in milliseconds since January
         * 1, 1970 GMT. Returns the defaultValue if no such header field could be found.
         *
         * @param field
         *      the header field name whose value is needed.
         * @param defaultValue
         *      the default value if no field has been found.
         *
         * @return the value of the specified header field as a date in milliseconds.
         */
        long long getHeaderFieldDate(const std::string& field, long long defaultValue) const;

        /**
         * Gets the specified header value as a number. Returns the defaultValue} if no
         * such header field could be found or the value could not be parsed as an Integer.
         *
         * @param field
         *      the header field name whose value is needed.
         * @param defaultValue
         *      the default value if no field has been found.
         *
         * @return the value of the specified header field as a number.
         */
        int getHeaderFieldInt(const std::string& field, int defaultValue) const;

        /**
         * Gets the value of the response header field 'last-modified' or zero if
         * this value is not set.
         *
         * @return the value of the 'last-modified' header field.
         */
        long long getLastModified() const;

        /**
         * Gets an InputStream for reading data from the resource pointed by this
         * URLConnection.  It throws an UnknownServiceException by default. This method
         * must be overridden by its subclasses.
         *
         * @return the InputStream to read data from.
         *
         * @throws IOException if no InputStream could be created.
         */
        virtual decaf::io::InputStream* getInputStream() {
            throw UnknownServiceException(
                __FILE__, __LINE__, "Does not support writing to the input stream");
        }

        /**
         * Gets an OutputStream for writing data to this URLConnection. It throws an
         * UnknownServiceException by default. This method must be overridden by its
         * subclasses.
         *
         * @return the OutputStream to write data.
         *
         * @throws IOException if no OutputStream could be created.
         */
        virtual decaf::io::OutputStream* getOutputStream() {
            throw UnknownServiceException(
                __FILE__, __LINE__, "Does not support writing to the output stream");
        }

        /**
         * Gets the point of time since when the data must be modified to be
         * transmitted. Some protocols transmit data only if it has been modified
         * more recently than a particular time.
         *
         * @return the time in milliseconds since January 1, 1970 GMT.
         */
        long long getIfModifiedSince() const {
            return ifModifiedSince;
        }

        /**
         * Sets the point of time since when the data must be modified to be
         * transmitted. Some protocols transmit data only if it has been modified
         * more recently than a particular time. The data will be transmitted
         * regardless of its timestamp if this option is set to 0.
         *
         * @param newValue
         *      the time in milliseconds since January 1, 1970 GMT.
         * @throws IllegalStateException
         *      if this URLConnection has already been connected.
         */
        void setIfModifiedSince(long long newValue);

    public:

        /**
         * Gets the default setting whether this connection allows user interaction.
         *
         * @return the value of the default setting defaultAllowUserInteraction.
         */
        static bool getDefaultAllowUserInteraction() {
            return defaultAllowUserInteraction;
        }

        /**
         * Sets the default value for the flag indicating whether this connection
         * allows user interaction or not. Existing URLConnections are unaffected.
         *
         * @param allows
         *      the default value of the flag to be used for new connections.
         */
        static void setDefaultAllowUserInteraction(bool allows) {
            defaultAllowUserInteraction = allows;
        }

    };

}}

#endif /* _DECAF_NET_URLCONNECTION_H_ */
