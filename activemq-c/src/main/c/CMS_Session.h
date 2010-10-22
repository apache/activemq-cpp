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
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status closeSession(CMS_Session* session);

/**
 * Commit the current transaction in progress for the Session.
 *
 * @param session
 *      The Session that will have its current transaction committed.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status commitSession(CMS_Session* session);

/**
 * Roll back the current transaction in progress for the Session.
 *
 * @param session
 *      The Session that will have its current transaction rolled back.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status rollbackSession(CMS_Session* session);

/**
 * Stop Message delivery in the given session and restart from the oldest unacknowledged
 * Message.
 *
 * @param session
 *      The Session that will be recovered.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status recoverSession(CMS_Session* session);

/**
 * Get the Acknowledge Mode that was used to create the given Session.
 *
 * @param session
 *      The session that is the target for this operation.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getSessionAcknowledgeMode(CMS_Session* session, int* mode);

/**
 * Query the given Session to determine if it is a Transacted Session, if the Session is
 * transacted then a value of 1 is written into the result address, otherwise zero is
 * written into the result location.
 *
 * @param session
 *      The session that is the target for this operation.
 * @param transacted
 *      The address where the boolean value is to be written.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status isSessionTransacted(CMS_Session* session, int* transacted);

/**
 *
 * @param session
 *    The session that is the target for this operation.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status unsubscribeSessionDurableConsumer(CMS_Session* session, const char* subscription);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_SESSION_WRAPPER_H_ */
