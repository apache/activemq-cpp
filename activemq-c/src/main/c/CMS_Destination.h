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

#include <cms.h>

#ifndef _CMS_DESTINATION_WRAPPER_H_
#define _CMS_DESTINATION_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a Destination from the Given Session instance.  The type of Destination is
 * given by the DESTINATION_TYPE parameter.
 *
 * @param session
 *      The Session to use to create the new Destination.
 * @param type
 *      The Type of Destination that is to be created.
 * @param name
 *      The name to assign the Destination, in the case of Temporary Destinations
 *      this parameter is ignored.
 * @param destination
 *      The address of the location to store the new Destination instance.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createDestination(CMS_Session* session, DESTINATION_TYPE type, const char* name, CMS_Destination** destination);

/**
 * Destroy the given Destination instance.
 *
 * @param destination
 *      The Destination to destroy.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status destroyDestination(CMS_Destination* destination);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_DESTINATION_WRAPPER_H_ */
