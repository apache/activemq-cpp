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
#ifndef _DECAF_UTIL_LOGGING_PROPERTIESCHANGELISTENER_H_
#define _DECAF_UTIL_LOGGING_PROPERTIESCHANGELISTENER_H_

#include <decaf/util/Config.h>

namespace decaf{
namespace util{
namespace logging{

    /**
     * Defines the interface that classes can use to listen for change
     * events on Properties.
     *
     * @since 1.0
     */
    class DECAF_API PropertiesChangeListener {
    public:

        virtual ~PropertiesChangeListener() {}

        /**
         * Indicates that the Properties have all been reset and should be
         * considered to be back to their default values.
         */
        virtual void onPropertiesReset() = 0;

        /**
         * Change Event, called when a property is changed, includes the
         * name of the property that was changed along with it old and
         * new values.
         *
         * @param name
         *      The name of the Property that changed.
         * @param oldValue
         *      The old Value of the Property.
         * @param newValue
         *      The new Value of the Property.
         */
        virtual void onPropertyChanged( const std::string& name,
                                        const std::string& oldValue,
                                        const std::string& newValue ) = 0;

   };

}}}

#endif /*_DECAF_UTIL_LOGGING_PROPERTIESCHANGELISTENER_H_*/
