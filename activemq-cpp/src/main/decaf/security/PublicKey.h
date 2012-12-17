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

#ifndef _DECAF_SECURITY_PUBLICKEY_H_
#define _DECAF_SECURITY_PUBLICKEY_H_

#include <decaf/security/Key.h>
#include <decaf/util/Config.h>

namespace decaf {
namespace security {

    /**
     * A public key. This interface contains no methods or constants. It merely
     * serves to group (and provide type safety for) all public key interfaces.
     */
    class DECAF_API PublicKey : public Key {

    public:

        virtual ~PublicKey() {}
    };

}}

#endif /*_DECAF_SECURITY_PUBLICKEY_H_*/
