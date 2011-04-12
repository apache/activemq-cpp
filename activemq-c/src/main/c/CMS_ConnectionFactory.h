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

#ifndef _CMS_CONNECTIONFACTORY_WRAPPER_H_
#define _CMS_CONNECTIONFACTORY_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a Connection Factory using the Default URI and Connection options.
 *
 * @param factory
 *      The memory address where the newly created ConnectionFactory will be stored.
 *
 * @returns result code indicating if the operation was successful.
 */
cms_status createDefaultConnectionFactory(CMS_ConnectionFactory** factory);

/**
 * Creates a Connection Factory using the options provided.  The URI option if null will
 * use the default URI from the ConnectionFactory implementation and cause any username or
 * password values to be ignored, the username and password can be NULL if none is needed
 * to connect to the Broker.
 *
 * @param factory
 *      The memory address where the newly created ConnectionFactory will be stored.
 * @param brokerUri
 *      The URI string describing the location and options used to connect to the broker.
 * @param username
 *      The user name that will be passed to the Broker for Authentication.
 * @param password
 *      The password that will be passed to the Broker for Authentication.
 *
 * @returns result code indicating if the operation was successful.
 */
cms_status createConnectionFactory(CMS_ConnectionFactory** factory,
                                   const char* brokerUri,
                                   const char* username,
                                   const char* password);

/**
 * Any ConnectionFactory created from the C Wrapper library must be destroyed using this
 * destroy method to ensure proper cleanup of allocated resources.
 *
 * @param factory
 *      The ConnectionFactory instance to destroy.
 *
 * @returns result code indicating if the operation was successful.
 */
cms_status destroyConnectionFactory(CMS_ConnectionFactory* factory);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_CONNECTIONFACTORY_WRAPPER_H_ */
