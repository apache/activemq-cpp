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

#ifndef _ACTIVEMQCPP_EXAMPLES_ADVISORIES_ADVISORYPRODUCER_H_
#define _ACTIVEMQCPP_EXAMPLES_ADVISORIES_ADVISORYPRODUCER_H_

#include <string>
#include <memory>

#include <cms/Session.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageListener.h>

#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>

namespace activemqcpp {
namespace examples {
namespace advisories {

    /**
     * A sample Producer that will only send Messages on its Topic when it has
     * received an advisory indicating that there is an active MessageConsumer
     * on the Topic.  Once another message comes in indicating that there is no
     * longer a consumer then this Producer stops producing again.
     *
     * @since 3.0
     */
    class AdvisoryProducer : public decaf::lang::Runnable,
                             public cms::MessageListener {
    private:

        volatile bool consumerOnline;
        volatile bool shutdown;
        decaf::util::concurrent::CountDownLatch shutdownLatch;

        cms::Session* session;
        std::auto_ptr<cms::MessageConsumer> consumer;
        std::auto_ptr<cms::MessageProducer> producer;

    private:

        AdvisoryProducer( const AdvisoryProducer& );
        AdvisoryProducer& operator= ( const AdvisoryProducer& );

    public:

        AdvisoryProducer( cms::Session* session );
        virtual ~AdvisoryProducer();

        /**
         * Shut down the processing that occurs in the Run method.
         */
        void stop();

        /**
         * Run the producer code.
         */
        virtual void run();

        /**
         * Async Message callback.
         */
        virtual void onMessage( const cms::Message* message );

    };

}}}

#endif /* _ACTIVEMQCPP_EXAMPLES_ADVISORIES_ADVISORYPRODUCER_H_ */
