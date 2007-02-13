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

#ifndef ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_
#define ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_
 
#include <activemq/io/FilterOutputStream.h>
#include <activemq/logger/LoggerDefines.h>

namespace activemq{
namespace io{
      
    /**
     * OutputStream filter that just logs the data being
     * written.
     */
    class LoggingOutputStream : public FilterOutputStream
    {     
    private:
    
        LOGCMS_DECLARE(logger)         
        
    public:
   
        /**
         * Constructor.
         * @param outputStream the OutputStream to wrap
         * @param own If true, this object will control the lifetime of the
         * output stream that it encapsulates.
         */
        LoggingOutputStream( OutputStream* next, bool own = false );
      
        virtual ~LoggingOutputStream();
       
        /**
         * Writes a single byte to the output stream.
         * @param c the byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( unsigned char c ) throw ( IOException );
      
        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The array of bytes to write.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const unsigned char* buffer, 
                            std::size_t len ) throw ( IOException );
                            
    private:
    
        /**
         * Logs the data in the buffer.
         */
        void log( const unsigned char* buffer, std::size_t len );
        
    };
   
}}

#endif /*ACTIVEMQ_IO_LOGGINGOUTPUTSTREAM_H_*/
