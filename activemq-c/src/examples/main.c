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
#include <CMS_ConnectionFactory.h>
#include <CMS_Connection.h>
#include <CMS_Session.h>
#include <CMS_Message.h>
#include <CMS_MessageConsumer.h>
#include <CMS_MessageProducer.h>
#include <CMS_Destination.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    cms_initialize();

    printf("=====================================================\n");
    printf("Starting the example:\n");
    printf("-----------------------------------------------------\n");

    const char* brokerUri = "failover:(tcp://127.0.0.1:61616)";
    const char* queueName = "cms.test.c.client.queue";

    CMS_ConnectionFactory* factory = NULL;
    CMS_Connection* connection = NULL;
    CMS_Session* session = NULL;
    CMS_Destination* destination = NULL;
    CMS_MessageProducer* producer = NULL;
    CMS_MessageConsumer* consumer = NULL;
    CMS_Message* txtMessage = NULL;

    if (createConnectionFactory(&factory, brokerUri, NULL, NULL) != CMS_SUCCESS) {
        printf("Failed to create a Connection Factory\n");
        exit(1);
    }

    if (createDefaultConnection(factory, &connection) != CMS_SUCCESS) {
        printf("Failed to create a Connection\n");
        exit(1);
    }

    destroyConnectionFactory(factory);

    if (createDefaultSession(connection, &session) != CMS_SUCCESS) {
        printf("Failed to create a Session\n");
        exit(1);
    }

    if (createDestination(session, CMS_QUEUE, queueName, &destination) != CMS_SUCCESS) {
        printf("Failed to create a Destination\n");
        exit(1);
    }

    if (createDefaultConsumer(session, destination, &consumer) != CMS_SUCCESS) {
        printf("Failed to create a MessageConsumer\n");
        exit(1);
    }
    if (createProducer(session, destination, &producer) != CMS_SUCCESS) {
        printf("Failed to create a MessageProducer\n");
        exit(1);
    }

    int i = 0;
    for(; i < 10; ++i) {
        CMS_Message* message = NULL;
        createTextMessage(session, &message, "Test Message");

        if (producerSendWithDefaults(producer, message) != CMS_SUCCESS) {
            printf("Failed to send the Message\n");
            destroyMessage(message);
            exit(1);
        }

        destroyMessage(message);
    }

    if (startConnection(connection) != CMS_SUCCESS) {
        printf("Failed to start the Connection\n");
        exit(1);
    }

    for(i = 0; i < 10; ++i) {
        CMS_Message* message = NULL;
        if (consumerReceiveWithTimeout(consumer, &message, 5000) != CMS_SUCCESS) {
            printf("Timed Receive call terminated abnormally\n");
            exit(1);
        }

        printf("Received Message #%d\n", i);
        destroyMessage(message);
    }

    destroyProducer(producer);
    destroyConsumer(consumer);
    destroyDestination(destination);
    destroySession(session);
    destroyConnection(connection);

    printf("-----------------------------------------------------\n");
    printf("Finished with the example.\n");
    printf("=====================================================\n");

    cms_terminate();
}
