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

#ifndef _ACTIVEMQC_H_
#define _ACTIVEMQC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Types used throughout the C Wrapper lib are declared here, the actual definition
 * is done elsewhere.
 */

/** The Opaque Connection Factory Structure */
typedef struct CMS_ConnectionFactory CMS_ConnectionFactory;

/** The Opaque Connection Structure */
typedef struct CMS_Connection CMS_Connection;

/** The Opaque Session Structure */
typedef struct CMS_Session CMS_Session;

/** The Opaque Message Producer Structure */
typedef struct CMS_MessageProducer CMS_MessageProducer;

/** The Opaque Message Consumer Structure */
typedef struct CMS_MessageConsumer CMS_MessageConsumer;

/** The Opaque Message Structure */
typedef struct CMS_Message CMS_Message;

/** The Opaque Destination Structure */
typedef struct CMS_Destination CMS_Destination;

/**
 * This section defines types used by the C client code to interact with the
 * C++ library via the Wrapper functions.
 */

/** Enum that defines the various message types supported by CMS. */
typedef enum {
    CMS_MESSAGE,
    CMS_BYTES_MESSAGE,
    CMS_MAP_MESSAGE,
    CMS_STREAM_MESSAAGE,
    CMS_TEXT_MESSAGE
};

/** Result code returned from wrapper functions to indicate success or failure. */
typedef int cms_status;

/**
 * C Functions used to initialize and shutdown the ActiveMQ-C library.
 */

/**
 * Initializes the ActiveMQ-C library and any underlying resources or attached
 * libraries.  This method must be called once before any other library function
 * in order to allocate the resources needed to use the ActiveMQ-CPP library.
 */
void cms_initialize();

/**
 * Shutdown the ActiveMQ-C library and destroy any resources that are held by this
 * library.  This method must be called once all the CMS resources that have been
 * allocated are destroyed.  Calling this method before destroying all CMS resources
 * can result in a segfault condition.
 */
void cms_terminate();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ACTIVEMQC_H_ */
