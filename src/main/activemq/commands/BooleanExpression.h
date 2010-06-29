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

#ifndef _ACTIVEMQ_COMMANDS_BOOLEANEXPRESSION_H_
#define _ACTIVEMQ_COMMANDS_BOOLEANEXPRESSION_H_

#include <activemq/commands/BaseDataStructure.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace commands{

    class AMQCPP_API BooleanExpression : public BaseDataStructure {
    public:

        BooleanExpression() : BaseDataStructure() {}

        virtual ~BooleanExpression() throw() {}

        virtual DataStructure* cloneDataStructure() const { return NULL; }

        virtual void copyDataStructure( const DataStructure* src AMQCPP_UNUSED) {}

        virtual std::string toString() const {
            std::ostringstream stream;

            stream << "Begin Class = BooleanExpression" << std::endl;
            stream << BaseDataStructure::toString();
            stream << "Begin Class = BooleanExpression" << std::endl;

            return stream.str();
        }

        virtual bool equals( const DataStructure* value ) const {
            return BaseDataStructure::equals( value );
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_BOOLEANEXPRESSION_H_*/
