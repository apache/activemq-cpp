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

#include <CMS_ConnectionFactory.h>

#include <Config.h>
#include <types/CMS_Types.h>

#include <activemq/core/ActiveMQConnectionFactory.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createDefaultConnectionFactory(CMS_ConnectionFactory** factory) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_ConnectionFactory> wrapper( new CMS_ConnectionFactory );

    try{
        wrapper->factory = new activemq::core::ActiveMQConnectionFactory();
        *factory = wrapper.release();
    } catch(cms::CMSException& ex) {
        ex.printStackTrace();
        result = CMS_ERROR;
    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status createConnectionFactory(CMS_ConnectionFactory** factory,
                                   const char* brokerUri,
                                   const char* username,
                                   const char* password) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_ConnectionFactory> wrapper( new CMS_ConnectionFactory );

    try{

        if (brokerUri == NULL) {
            wrapper->factory = new activemq::core::ActiveMQConnectionFactory();
        } else {

            std::string user = username == NULL ? "" : std::string(username);
            std::string pass = password == NULL ? "" : std::string(password);

            wrapper->factory = new activemq::core::ActiveMQConnectionFactory( brokerUri, user, pass );
        }

        *factory = wrapper.release();
    } catch(cms::CMSException& ex) {
        ex.printStackTrace();
        result = CMS_ERROR;
    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    } catch(...) {
        std::cout << "Caught an unknown exception." << std::endl;
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroyConnectionFactory(CMS_ConnectionFactory* factory) {

    cms_status result = CMS_SUCCESS;

    if(factory != NULL) {

        try{
            delete factory->factory;
            delete factory;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
