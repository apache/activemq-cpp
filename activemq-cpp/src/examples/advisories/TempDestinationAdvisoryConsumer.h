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

#ifndef _ACTIVEMQCPP_EXAMPLES_ADVISORIES_TEMPDESTINATIONADVISORYCONSUMER_H_
#define _ACTIVEMQCPP_EXAMPLES_ADVISORIES_TEMPDESTINATIONADVISORYCONSUMER_H_

#include <string>
#include <memory>

#include <cms/Session.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageListener.h>

#include <decaf/lang/Runnable.h>

namespace activemqcpp {
namespace examples {
namespace advisories {

    /**
     * Monitors a Broker for Temporary Topic creation and destruction.
     *
     * @since 3.0
     */
    class TempDestinationAdvisoryConsumer : public cms::MessageListener {
    private:

        cms::Session* session;
        std::auto_ptr<cms::MessageConsumer> consumer;

    private:

        TempDestinationAdvisoryConsumer(const TempDestinationAdvisoryConsumer&);
        TempDestinationAdvisoryConsumer& operator=(const TempDestinationAdvisoryConsumer&);

    public:

        TempDestinationAdvisoryConsumer(cms::Session* session);
        virtual ~TempDestinationAdvisoryConsumer();

        /**
         * Async Message callback.
         */
        virtual void onMessage(const cms::Message* message);

    };

}}}

#endif /* _ACTIVEMQCPP_EXAMPLES_ADVISORIES_TEMPDESTINATIONADVISORYCONSUMER_H_ */
