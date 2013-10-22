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

#ifndef _DECAF_NET_CONTENTHANDLERFACTORY_H_
#define _DECAF_NET_CONTENTHANDLERFACTORY_H_

#include <decaf/util/Config.h>

#include <string>

namespace decaf {
namespace net {

    class ContentHandler;

    /**
     * Defines a factory interface for content handlers. An implementation of this interface
     * must map a MIME type into an instance of ContentHandler.
     *
     * @since 1.0
     */
    class DECAF_API ContentHandlerFactory {
    public:

        virtual ~ContentHandlerFactory();

        /**
         * Creates a content handler to handle contentType.
         *
         * @param contentType
         *      specifies the content type which is handled by the returned ContentHandler.
         *
         * @return content handler object for a specific content type.
         */
        virtual ContentHandler* createContentHandler(const std::string& contentType) = 0;

    };

}}

#endif /* _DECAF_NET_CONTENTHANDLERFACTORY_H_ */
