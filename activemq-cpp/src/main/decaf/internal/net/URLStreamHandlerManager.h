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

#ifndef _DECAF_INTERNAL_NET_URLSTREAMHANDLERMANAGER_H_
#define _DECAF_INTERNAL_NET_URLSTREAMHANDLERMANAGER_H_

#include <decaf/util/Config.h>

#include <decaf/lang/String.h>

namespace decaf {
namespace net {
    class URLStreamHandler;
    class URLStreamHandlerFactory;
}
namespace internal {
namespace net {

    class Network;
    class URLStreamHandlerManagerImpl;

    class DECAF_API URLStreamHandlerManager {
    private:

        URLStreamHandlerManagerImpl* impl;

    private:

        static URLStreamHandlerManager* instance;

    private:

        URLStreamHandlerManager();

    public:

        virtual ~URLStreamHandlerManager();

    public:

        /**
         * Returns the one and only URLStreamHandlerManager instance for this
         * application.  The returned pointer should never be deleted by the
         * application.
         *
         * @return a URLStreamHandlerManager instance.
         */
        static URLStreamHandlerManager* getInstance();

    public:

        /**
         * Gets a URLStreamHandler for the specified protocol if one is available.
         *
         * @param protocol
         *      The protocol to return a URL Stream Handler instance for.
         *
         * @return a URLStreamHandler instance for the given protocol.
         */
        decaf::net::URLStreamHandler* getURLStreamHandler(const decaf::lang::String& protocol);

        /**
         * Sets an application's URLStreamHandlerFactory. This method can be called at most once.
         *
         * The URLStreamHandlerFactory instance is used to construct a stream protocol handler
         * from a protocol name.  The provided factory becomes the property of this runtime and
         * will be deleted at shutdown.
         *
         * @param factory
         *      the desired factory.
         *
         * @throws Exception if there is already a set factory.
         */
        void setURLStreamHandlerFactory(decaf::net::URLStreamHandlerFactory* factory);

    };

}}}

#endif /* _DECAF_INTERNAL_NET_URLSTREAMHANDLERMANAGER_H_ */
