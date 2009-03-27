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

#ifndef _ACTIVEMQ_UTIL_CMSLISTENER_H_
#define _ACTIVEMQ_UTIL_CMSLISTENER_H_

#include <cms/Session.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>

#include <decaf/util/concurrent/Mutex.h>

namespace activemq {
namespace util {

    class CMSListener : public cms::MessageListener,
                        public cms::ExceptionListener {
    private:

        unsigned int numReceived;
        decaf::util::concurrent::Mutex mutex;
        cms::Session* session;

    public:

        CMSListener( cms::Session* session );
        virtual ~CMSListener();

        unsigned int getNumReceived() const {
            return this->numReceived;
        }

        virtual void reset();

        virtual void asyncWaitForMessages( unsigned int count );

        virtual void onException( const cms::CMSException& error );
        virtual void onMessage( const cms::Message* message );

    };

}}

#endif /* _ACTIVEMQ_UTIL_CMSLISTENER_H_ */
