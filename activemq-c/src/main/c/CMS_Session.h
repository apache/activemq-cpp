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

#ifndef _CMS_SESSION_WRAPPER_H_
#define _CMS_SESSION_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Given a Connection instance, create a new Session with the default Session
 * ack mode of Auto-Acknowledge.
 *
 * @param connection
 *      The Connection that is to be used to create the new Session.
 * @param session
 *      The memory location where the newly allocated Session instance is to be stored.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createDefaultSession(CMS_Connection* connection, CMS_Session** session);

/**
 * Given a Connection instance, create a new Session with the specified Session
 * ack mode.
 *
 * @param connection
 *      The Connection that is to be used to create the new Session.
 * @param session
 *      The memory location where the newly allocated Session instance is to be stored.
 * @param ackMode
 *      The Acknowledgment Mode that is assigned the newly created Session.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createSession(CMS_Connection* connection, CMS_Session** session, ACKNOWLEDGMENT_MODE ackMode);

/**
 * Destroys the given Connection instance, all Connection Resources should have been
 * deallocated at this point.
 *
 * @param session
 *      The Session that is to be destroyed.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status destroySession(CMS_Session* session);

/**
 * Closes the Session instance, all Session resources are also closed when the parent
 * Session is closed.  Client's must still deallocate the Session resource that they
 * created from this Session.
 *
 * @param session
 *      The Session that is to be closed.
 */
cms_status closeSession(CMS_Session* session);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_SESSION_WRAPPER_H_ */
