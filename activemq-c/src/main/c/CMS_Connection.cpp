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

#include <cms/IllegalStateException.h>
#include <cms/InvalidClientIdException.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

using namespace cms;

////////////////////////////////////////////////////////////////////////////////
cms_status createDefaultConnection(CMS_ConnectionFactory* factory, CMS_Connection** connection) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Connection> wrapper( new CMS_Connection );

    try{

        if (factory == NULL) {
            result = CMS_ERROR;
        } else {
            wrapper->connection = factory->factory->createConnection();
            *connection = wrapper.release();
        }

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

            std::string user = username == NULL ? "" : std::string(username);
            std::string pass = password == NULL ? "" : std::string(password);
            std::string id = clientId == NULL ? "" : std::string(clientId);

            wrapper->connection = factory->factory->createConnection(user, pass, id);
            *connection = wrapper.release();
        }

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

////////////////////////////////////////////////////////////////////////////////
cms_status stopConnection(CMS_Connection* connection) {

    cms_status result = CMS_SUCCESS;

    if(connection != NULL) {

        try{
            connection->connection->stop();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status closeConnection(CMS_Connection* connection) {

    cms_status result = CMS_SUCCESS;

    if(connection != NULL) {

        try{
            connection->connection->close();
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status setConnectionClientId(CMS_Connection* connection, const char* clientId) {

    cms_status result = CMS_SUCCESS;

    if(connection != NULL && clientId != NULL) {

        try{
            connection->connection->setClientID(clientId);
        } catch(IllegalStateException& ex) {
            result = CMS_ILLEGAL_STATE;
        } catch(InvalidClientIdException& ex) {
            result = CMS_INVALID_CLIENTID;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status getConnectionClientId(CMS_Connection* connection, char* clientId, int size) {

    cms_status result = CMS_SUCCESS;

    if(connection != NULL && clientId != NULL && size > 0) {

        try{
            std::string theClientId = connection->connection->getClientID();

            if(theClientId.size() < size) {

                for(int i = 0; i < theClientId.size(); ++i) {
                    clientId[i] = theClientId.at(i);
                }

                clientId[theClientId.size()] = '\0';

            } else {
                result = CMS_ERROR;
            }

        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
