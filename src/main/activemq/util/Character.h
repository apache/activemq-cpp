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

#ifndef _ACTIVEMQ_UTIL_CHARACTER_H_
#define _ACTIVEMQ_UTIL_CHARACTER_H_

namespace activemq{
namespace util{
  
    class Character{
    
    public:
    
        /**
         * Indicates whether or not the given character is considered
         * whitespace.
         */
        static bool isWhitespace( char c ){
            switch ( c ) 
            {
                case '\n':
                case '\t':
                case '\r':
                case '\f':
                case ' ':                
                    return true;
            } 
           
            return false;
        }
        
        /**
         * Indicates whether or not the given character is
         * a digit.
         */
        static bool isDigit( char c ){
            return c >= '0' && c <= '9';
        }
        
        /**
         * Indicates whether or not the given character is
         * a lower case character.
         */
        static bool isLowerCase( char c ){
            return c >= 'a' && c <= 'z';
        }
        
        /**
         * Indicates whether or not the given character is
         * a upper case character.
         */
        static bool isUpperCase( char c ){
            return c >= 'A' && c <= 'Z';
        }
        
        /**
         * Indicates whether or not the given character is
         * a letter.
         */  
        static bool isLetter( char c ){
            return isUpperCase(c) || isLowerCase(c);
        }
        
        /**
         * Indicates whether or not the given character is
         * either a letter or a digit.
         */
        static bool isLetterOrDigit( char c ){
            return isLetter(c) || isDigit(c);
        }
    };
    
}}

#endif /*_ACTIVEMQ_UTIL_CHARACTER_H_*/
