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

#ifndef _CMS_ENHANCEDCONNECTION_H_
#define _CMS_ENHANCEDCONNECTION_H_

#include <cms/Config.h>
#include <cms/Connection.h>
#include <cms/DestinationSource.h>

namespace cms {

    /**
     * An enhanced CMS Connection instance that provides additional features above the
     * default required features of a CMS Connection instance.
     *
     * @since 3.2
     */
    class CMS_API EnhancedConnection : public virtual cms::Connection {
    public:

        virtual ~EnhancedConnection();

        /**
         * Returns the DestinationSource} object which can be used to listen to destinations
         * being created or destroyed or to enquire about the current destinations available
         * on the message Provider.
         *
         * @return a new instance of a DestinationSource that is owned by the caller.
         *
         * @throws CMSException if an error occurs while creating the destination source.
         */
        virtual cms::DestinationSource* getDestinationSource() = 0;

    };

}

#endif /* _CMS_ENHANCEDCONNECTION_H_ */
