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

#ifndef _ATIVEMQ_UTIL_IDGENERATOR_H_
#define _ATIVEMQ_UTIL_IDGENERATOR_H_

#include <activemq/util/Config.h>

#include <string>

namespace activemq {
namespace library {
    class ActiveMQCPP;
}
namespace util {

    class IdGeneratorKernel;

    class AMQCPP_API IdGenerator {
    private:

        std::string prefix;
        mutable std::string seed;
        mutable long long sequence;

        static IdGeneratorKernel* kernel;

    private:

        IdGenerator( const IdGenerator& );
        IdGenerator& operator= ( const IdGenerator& );

    public:

        IdGenerator();

        IdGenerator( const std::string& prefix );

        virtual ~IdGenerator();

    public:

        /**
         * @returns a newly generated unique id.
         */
        std::string generateId() const;

    public:

        /**
         * Since the initialization of this object results in the retrieval of the
         * machine's host name we can quickly return it here.
         *
         * @return the previously retrieved host name.
         */
        static std::string getHostname();

        /**
         * Gets the seed value from a Generated Id, the count portion is removed.
         *
         * @returns the seed portion of the Id, minus the count value.
         */
        static std::string getSeedFromId( const std::string& id );

        /**
         * Gets the count value from a Generated Id, the seed portion is removed.
         *
         * @returns the sequence count portion of the id, minus the seed value.
         */
        static long long getSequenceFromId( const std::string& id );

        /**
         * Compares two generated id values.
         *
         * @param id1
         *      The first id to compare, or left hand side.
         * @param id2
         *      The second id to compare, or right hand side.
         *
         * @returns zero if ids are equal or positove if id1 > id2...
         */
        static int compare( const std::string& id1, const std::string& id2 );

    private:

        static void initialize();
        static void shutdown();

        friend class activemq::library::ActiveMQCPP;
    };

}}

#endif /* _ATIVEMQ_UTIL_IDGENERATOR_H_ */
