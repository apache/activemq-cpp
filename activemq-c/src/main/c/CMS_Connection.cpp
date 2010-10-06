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

#include <CMS_Connection.h>

#include <Config.h>
#include <types/CMS_Types.h>

#include <activemq/core/ActiveMQConnection.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createDefaultConnection(CMS_ConnectionFactory* factory, CMS_Connection** connection) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Connection> wrapper( new CMS_Connection );

    try{

        if (factory == NULL) {
            result = CMS_ERROR;
        } else {
            wrapper->connection = factory->factory->createConnection();
        }

        *connection = wrapper.release();
    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createConnection(CMS_ConnectionFactory* factory,
                            CMS_Connection** connection,
                            const char* username,
                            const char* password,
                            const char* clientId) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Connection> wrapper( new CMS_Connection );

    try{

        if (factory == NULL) {
            result = CMS_ERROR;
        } else {
            wrapper->connection = factory->factory->createConnection(username, password, clientId);
        }

        *connection = wrapper.release();
    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroyConnection(CMS_Connection* connection) {

    cms_status result = CMS_SUCCESS;

    if(connection != NULL) {

        try{
            delete connection->connection;
            delete connection;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status startConnection(CMS_Connection* connection) {

    cms_status result = CMS_SUCCESS;

    if(connection != NULL) {

        try{
            connection->connection->start();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
