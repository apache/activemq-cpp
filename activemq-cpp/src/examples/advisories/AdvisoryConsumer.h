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

#ifndef _ACTIVEMQCPP_EXAMPLES_ADVISORIES_ADVISORYCONSUMER_H_
#define _ACTIVEMQCPP_EXAMPLES_ADVISORIES_ADVISORYCONSUMER_H_

#include <string>
#include <memory>

#include <cms/Closeable.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageListener.h>

namespace activemqcpp {
namespace examples {
namespace advisories {

    /**
     * A simple Consumer that compliements the AdvisoryProducer example.  This
     * consumer listens on the Topic that the Producer is waiting to publish on
     * and will display the count of Producers that are active on the Topic
     * any time that it sees an advisory message indicating a consumer has
     * stopped or started.
     *
     * @since 3.0
     */
    class AdvisoryConsumer : public cms::Closeable,
                             public cms::MessageListener {
     private:

         cms::Session* session;
         std::auto_ptr<cms::MessageConsumer> consumer;
         std::auto_ptr<cms::MessageConsumer> advisoryConsumer;

     private:

         AdvisoryConsumer(const AdvisoryConsumer&);
        AdvisoryConsumer& operator=(const AdvisoryConsumer&);

    public:

        AdvisoryConsumer(cms::Session* session);
        virtual ~AdvisoryConsumer();

        /**
         * Close down Consumer resources.
         */
        virtual void close();

        /**
         * Async Message callback.
         */
        virtual void onMessage(const cms::Message* message);

    };

}}}

#endif /* _ACTIVEMQCPP_EXAMPLES_ADVISORIES_ADVISORYCONSUMER_H_ */
