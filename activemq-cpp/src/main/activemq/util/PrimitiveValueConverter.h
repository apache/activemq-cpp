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

#ifndef _ACTIVEMQ_UTIL_PRIMITIVEVALUECONVERTER_H_
#define _ACTIVEMQ_UTIL_PRIMITIVEVALUECONVERTER_H_

#include <activemq/util/Config.h>

#include <activemq/util/PrimitiveValueNode.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <string>

namespace activemq {
namespace util {

    /**
     * Class controls the conversion of data contained in a PrimitiveValueNode
     * from one type to another.  If the conversion is supported then calling
     * the convert method will throw an UnsupportedOperationException to indicate
     * that its not possible to perform the conversion.
     *
     * This class is used to implement the rules of conversion on CMS Message
     * properties, the following conversion table must be implemented.  A value
     * written as the row type can be read in the column type.
     *
     *    |        | boolean byte short int long float double String
     *    |----------------------------------------------------------
     *    |boolean |    X                                       X
     *    |byte    |          X     X    X   X                  X
     *    |short   |                X    X   X                  X
     *    |int     |                     X   X                  X
     *    |long    |                         X                  X
     *    |float   |                               X     X      X
     *    |double  |                                     X      X
     *    |String  |    X     X     X    X   X     X     X      X
     *    |----------------------------------------------------------
     *
     * @since 3.0
     */
    class AMQCPP_API PrimitiveValueConverter {
    public:

        PrimitiveValueConverter() {}
        virtual ~PrimitiveValueConverter() {}

        template< typename TO>
        TO convert( const PrimitiveValueNode& value ) const {

            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Invalid Conversion" );
        }

    };

    template<>
    bool PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    unsigned char PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    char PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    short PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    int PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    long long PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    float PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    double PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    std::string PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;
    template<>
    std::vector<unsigned char> PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const;

}}

#endif /* _ACTIVEMQ_UTIL_PRIMITIVEVALUECONVERTER_H_ */
