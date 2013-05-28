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
#include "ActiveMQConnectionFactory.h"

#include <cms/MessageTransformer.h>
#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQMessageAudit.h>
#include <activemq/core/policies/DefaultPrefetchPolicy.h>
#include <activemq/core/policies/DefaultRedeliveryPolicy.h>
#include <activemq/util/URISupport.h>
#include <activemq/util/CompositeData.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::policies;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const std::string ActiveMQConnectionFactory::DEFAULT_URI = "failover:(tcp://localhost:61616)";

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class FactorySettings {
    private:

        FactorySettings(const FactorySettings&);
        FactorySettings& operator=(const FactorySettings&);

    public:

        Mutex configLock;

        Pointer<Properties> properties;

        std::string username;
        std::string password;
        std::string clientId;

        URI brokerURI;

        bool dispatchAsync;
        bool alwaysSyncSend;
        bool useAsyncSend;
        bool sendAcksAsync;
        bool messagePrioritySupported;
        bool useCompression;
        bool useRetroactiveConsumer;
        bool watchTopicAdvisories;
        bool checkForDuplicates;
        bool optimizeAcknowledge;
        bool exclusiveConsumer;
        bool transactedIndividualAck;
        bool nonBlockingRedelivery;
        int compressionLevel;
        unsigned int sendTimeout;
        unsigned int closeTimeout;
        unsigned int producerWindowSize;
        int auditDepth;
        int auditMaximumProducerNumber;
        long long optimizeAcknowledgeTimeOut;
        long long optimizedAckScheduledAckInterval;
        long long consumerFailoverRedeliveryWaitPeriod;

        cms::ExceptionListener* defaultListener;
        cms::MessageTransformer* defaultTransformer;
        std::auto_ptr<PrefetchPolicy> defaultPrefetchPolicy;
        std::auto_ptr<RedeliveryPolicy> defaultRedeliveryPolicy;

        FactorySettings() : configLock(),
                            properties(new Properties()),
                            username(),
                            password(),
                            clientId(),
                            brokerURI(ActiveMQConnectionFactory::DEFAULT_URI),
                            dispatchAsync(true),
                            alwaysSyncSend(false),
                            useAsyncSend(false),
                            sendAcksAsync(true),
                            messagePrioritySupported(true),
                            useCompression(false),
                            useRetroactiveConsumer(false),
                            watchTopicAdvisories(true),
                            checkForDuplicates(true),
                            optimizeAcknowledge(false),
                            exclusiveConsumer(false),
                            transactedIndividualAck(false),
                            nonBlockingRedelivery(false),
                            compressionLevel(-1),
                            sendTimeout(0),
                            closeTimeout(15000),
                            producerWindowSize(0),
                            auditDepth(ActiveMQMessageAudit::DEFAULT_WINDOW_SIZE),
                            auditMaximumProducerNumber(ActiveMQMessageAudit::MAXIMUM_PRODUCER_COUNT),
                            optimizeAcknowledgeTimeOut(300),
                            optimizedAckScheduledAckInterval(0),
                            consumerFailoverRedeliveryWaitPeriod(0),
                            defaultListener(NULL),
                            defaultTransformer(NULL),
                            defaultPrefetchPolicy(new DefaultPrefetchPolicy()),
                            defaultRedeliveryPolicy(new DefaultRedeliveryPolicy()) {
        }

        void updateConfiguration(const URI& uri) {

            this->brokerURI = uri;
            this->properties->clear();

            if (uri.getQuery() != "") {
                // Not a composite URI so this works fine.
                try {
                    URISupport::parseQuery(uri.getQuery(), properties.get());
                } catch (URISyntaxException& ex) {
                }
            } else {
                // Composite URI won't indicate it has a query even if it does.
                try {
                    CompositeData composite = URISupport::parseComposite(uri);
                    *this->properties = composite.getParameters();
                } catch (URISyntaxException& ex) {
                }
            }

            // Check the connection options
            this->alwaysSyncSend = Boolean::parseBoolean(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_ALWAYSSYNCSEND), Boolean::toString(alwaysSyncSend)));
            this->useAsyncSend = Boolean::parseBoolean(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_USEASYNCSEND), Boolean::toString(useAsyncSend)));
            this->useCompression = Boolean::parseBoolean(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_USECOMPRESSION), Boolean::toString(useCompression)));
            this->compressionLevel = Integer::parseInt(
                properties->getProperty("connection.compressionLevel", Integer::toString(compressionLevel)));
            this->messagePrioritySupported = Boolean::parseBoolean(
                properties->getProperty("connection.messagePrioritySupported", Boolean::toString(messagePrioritySupported)));
            this->checkForDuplicates = Boolean::parseBoolean(
                properties->getProperty("connection.checkForDuplicates", Boolean::toString(checkForDuplicates)));
            this->auditDepth = Integer::parseInt(
                properties->getProperty("connection.auditDepth", Integer::toString(auditDepth)));
            this->auditMaximumProducerNumber = Integer::parseInt(
                properties->getProperty("connection.auditMaximumProducerNumber", Integer::toString(auditMaximumProducerNumber)));
            this->dispatchAsync = Boolean::parseBoolean(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_DISPATCHASYNC), Boolean::toString(dispatchAsync)));
            this->producerWindowSize = Integer::parseInt(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_PRODUCERWINDOWSIZE), Integer::toString(producerWindowSize)));
            this->sendTimeout = decaf::lang::Integer::parseInt(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_SENDTIMEOUT), Integer::toString(sendTimeout)));
            this->closeTimeout = decaf::lang::Integer::parseInt(
                properties->getProperty(core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::CONNECTION_CLOSETIMEOUT), Integer::toString(closeTimeout)));
            this->clientId = properties->getProperty(
                core::ActiveMQConstants::toString(core::ActiveMQConstants::PARAM_CLIENTID), clientId);
            this->username = properties->getProperty(
                core::ActiveMQConstants::toString(core::ActiveMQConstants::PARAM_USERNAME), username);
            this->password = properties->getProperty(
                core::ActiveMQConstants::toString(core::ActiveMQConstants::PARAM_PASSWORD), password);
            this->optimizeAcknowledge = Boolean::parseBoolean(
                properties->getProperty("connection.optimizeAcknowledge", Boolean::toString(optimizeAcknowledge)));
            this->exclusiveConsumer = Boolean::parseBoolean(
                properties->getProperty("connection.exclusiveConsumer", Boolean::toString(exclusiveConsumer)));
            this->transactedIndividualAck = Boolean::parseBoolean(
                properties->getProperty("connection.transactedIndividualAck", Boolean::toString(transactedIndividualAck)));
            this->useRetroactiveConsumer = Boolean::parseBoolean(
                properties->getProperty("connection.useRetroactiveConsumer", Boolean::toString(useRetroactiveConsumer)));
            this->sendAcksAsync = Boolean::parseBoolean(
                properties->getProperty("connection.sendAcksAsync", Boolean::toString(sendAcksAsync)));
            this->optimizeAcknowledgeTimeOut = Long::parseLong(
                properties->getProperty("connection.optimizeAcknowledgeTimeOut", Long::toString(optimizeAcknowledgeTimeOut)));
            this->optimizedAckScheduledAckInterval = Long::parseLong(
                properties->getProperty("connection.optimizedAckScheduledAckInterval", Long::toString(optimizedAckScheduledAckInterval)));
            this->consumerFailoverRedeliveryWaitPeriod = Long::parseLong(
                properties->getProperty("connection.consumerFailoverRedeliveryWaitPeriod", Long::toString(consumerFailoverRedeliveryWaitPeriod)));
            this->nonBlockingRedelivery = Boolean::parseBoolean(
                properties->getProperty("connection.nonBlockingRedelivery", Boolean::toString(nonBlockingRedelivery)));
            this->watchTopicAdvisories = Boolean::parseBoolean(
                properties->getProperty("connection.watchTopicAdvisories", Boolean::toString(watchTopicAdvisories)));

            this->defaultPrefetchPolicy->configure(*properties);
            this->defaultRedeliveryPolicy->configure(*properties);
        }

        static URI createURI(const std::string& uriString) {
            try {
                return URI(uriString);
            } catch (URISyntaxException& ex) {
                throw cms::CMSException("Invalid Connection Uri detected.");
            }
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
cms::ConnectionFactory* cms::ConnectionFactory::createCMSConnectionFactory(const std::string& brokerURI) {
    return new ActiveMQConnectionFactory( brokerURI );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory() : settings(new FactorySettings()) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory(const std::string& uri,
                                                     const std::string& username,
                                                     const std::string& password) : settings( new FactorySettings() ) {

    this->setBrokerURI(FactorySettings::createURI(uri));

    // Store login data in the properties
    if (!username.empty()) {
        this->settings->username = username;
    }
    if (!password.empty()) {
        this->settings->password = password;
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory(const decaf::net::URI& uri,
                                                     const std::string& username,
                                                     const std::string& password) : settings( new FactorySettings() ) {

    this->setBrokerURI(uri);

    // Store login data in the properties
    if (!username.empty()) {
        this->settings->username = username;
    }
    if (!password.empty()) {
        this->settings->password = password;
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::~ActiveMQConnectionFactory() {
    try {
        delete this->settings;
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection() {
    return doCreateConnection(settings->brokerURI, settings->username, settings->password, settings->clientId);
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(const std::string& username, const std::string& password) {
    return doCreateConnection(settings->brokerURI, username, password, settings->clientId);
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(const std::string& username, const std::string& password, const std::string& clientId) {
    return doCreateConnection(settings->brokerURI, username, password, clientId);
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::doCreateConnection(const decaf::net::URI& uri,
                                                               const std::string& username,
                                                               const std::string& password,
                                                               const std::string& clientId) {

    Pointer<Transport> transport;
    auto_ptr<ActiveMQConnection> connection;

    try {

        synchronized(&this->settings->configLock) {

            this->setBrokerURI(uri);

            // Store login data in the properties
            if (!username.empty()) {
                this->settings->username = username;
            }
            if (!password.empty()) {
                this->settings->password = password;
            }
            if (!clientId.empty()) {
                this->settings->clientId = clientId;
            }

            // Use the TransportBuilder to get our Transport
            transport = TransportRegistry::getInstance().findFactory(uri.getScheme())->create(uri);

            if (transport == NULL) {
                throw ActiveMQException(__FILE__, __LINE__, "ActiveMQConnectionFactory::createConnection - "
                        "failed creating new Transport");
            }

            Pointer<Properties> properties(this->settings->properties->clone());

            // Create and Return the new connection object.
            connection.reset(createActiveMQConnection(transport, properties));

            // Set all options parsed from the URI.
            configureConnection(connection.get());

            // Now start the connection since all other configuration is done.
            transport->start();

            if (!this->settings->clientId.empty()) {
                connection->setDefaultClientId(this->settings->clientId);
            }
        }

        return connection.release();
    } catch (cms::CMSException& ex) {
        ex.setMark(__FILE__, __LINE__);
        throw ex;
    } catch (activemq::exceptions::ActiveMQException& ex) {
        ex.setMark(__FILE__, __LINE__);
        throw ex.convertToCMSException();
    } catch (decaf::lang::Exception& ex) {
        ex.setMark(__FILE__, __LINE__);
        activemq::exceptions::ActiveMQException amqEx(ex);
        throw amqEx.convertToCMSException();
    } catch (std::exception& ex) {
        throw cms::CMSException(ex.what(), NULL);
    } catch (...) {
        throw cms::CMSException("Caught Unknown Exception", NULL);
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection* ActiveMQConnectionFactory::createActiveMQConnection(
    const Pointer<transport::Transport>& transport,
    const Pointer<decaf::util::Properties>& properties) {

    return new ActiveMQConnection(transport, properties);
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(const std::string& uri,
                                                             const std::string& username,
                                                             const std::string& password,
                                                             const std::string& clientId) {
    ActiveMQConnectionFactory factory;
    return factory.doCreateConnection(URI(uri), username, password, clientId);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::configureConnection(ActiveMQConnection* connection) {

    connection->setUsername(this->settings->username);
    connection->setPassword(this->settings->password);
    connection->setDispatchAsync(this->settings->dispatchAsync);
    connection->setAlwaysSyncSend(this->settings->alwaysSyncSend);
    connection->setUseAsyncSend(this->settings->useAsyncSend);
    connection->setUseCompression(this->settings->useCompression);
    connection->setCompressionLevel(this->settings->compressionLevel);
    connection->setSendTimeout(this->settings->sendTimeout);
    connection->setCloseTimeout(this->settings->closeTimeout);
    connection->setProducerWindowSize(this->settings->producerWindowSize);
    connection->setPrefetchPolicy(this->settings->defaultPrefetchPolicy->clone());
    connection->setRedeliveryPolicy(this->settings->defaultRedeliveryPolicy->clone());
    connection->setMessagePrioritySupported(this->settings->messagePrioritySupported);
    connection->setWatchTopicAdvisories(this->settings->watchTopicAdvisories);
    connection->setCheckForDuplicates(this->settings->checkForDuplicates);
    connection->setAuditDepth(this->settings->auditDepth);
    connection->setAuditMaximumProducerNumber(this->settings->auditMaximumProducerNumber);
    connection->setOptimizeAcknowledge(this->settings->optimizeAcknowledge);
    connection->setOptimizeAcknowledgeTimeOut(this->settings->optimizeAcknowledgeTimeOut);
    connection->setOptimizedAckScheduledAckInterval(this->settings->optimizedAckScheduledAckInterval);
    connection->setSendAcksAsync(this->settings->sendAcksAsync);
    connection->setExclusiveConsumer(this->settings->exclusiveConsumer);
    connection->setTransactedIndividualAck(this->settings->transactedIndividualAck);
    connection->setUseRetroactiveConsumer(this->settings->useRetroactiveConsumer);
    connection->setNonBlockingRedelivery(this->settings->nonBlockingRedelivery);
    connection->setConsumerFailoverRedeliveryWaitPeriod(this->settings->consumerFailoverRedeliveryWaitPeriod);

    if (this->settings->defaultListener) {
        connection->setExceptionListener(this->settings->defaultListener);
    }

    if (this->settings->defaultTransformer) {
        connection->setMessageTransformer(this->settings->defaultTransformer);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUsername(const std::string& username) {
    settings->username = username;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnectionFactory::getUsername() const {
    return settings->username;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setPassword(const std::string& password) {
    settings->password = password;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnectionFactory::getPassword() const {
    return settings->password;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnectionFactory::getClientId() const {
    return this->settings->clientId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setClientId(const std::string& clientId) {
    this->settings->clientId = clientId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setBrokerURI(const std::string& uri) {
    this->setBrokerURI(FactorySettings::createURI(uri));
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setBrokerURI(const decaf::net::URI& uri) {
    synchronized(&this->settings->configLock) {
        this->settings->updateConfiguration(uri);
    }
}

////////////////////////////////////////////////////////////////////////////////
const decaf::net::URI& ActiveMQConnectionFactory::getBrokerURI() const {
    return settings->brokerURI;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setExceptionListener(cms::ExceptionListener* listener) {
    this->settings->defaultListener = listener;
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQConnectionFactory::getExceptionListener() const {
    return this->settings->defaultListener;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setMessageTransformer(cms::MessageTransformer* transformer) {
    this->settings->defaultTransformer = transformer;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageTransformer* ActiveMQConnectionFactory::getMessageTransformer() const {
    return this->settings->defaultTransformer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setPrefetchPolicy(PrefetchPolicy* policy) {
    this->settings->defaultPrefetchPolicy.reset(policy);
}

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy* ActiveMQConnectionFactory::getPrefetchPolicy() const {
    return this->settings->defaultPrefetchPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setRedeliveryPolicy(RedeliveryPolicy* policy) {
    this->settings->defaultRedeliveryPolicy.reset(policy);
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConnectionFactory::getRedeliveryPolicy() const {
    return this->settings->defaultRedeliveryPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isDispatchAsync() const {
    return this->settings->dispatchAsync;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setDispatchAsync(bool value) {
    this->settings->dispatchAsync = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isAlwaysSyncSend() const {
    return this->settings->alwaysSyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setAlwaysSyncSend(bool value) {
    this->settings->alwaysSyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isUseAsyncSend() const {
    return this->settings->useAsyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUseAsyncSend(bool value) {
    this->settings->useAsyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isSendAcksAsync() const {
    return this->settings->sendAcksAsync;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setSendAcksAsync(bool sendAcksAsync) {
    this->settings->sendAcksAsync = sendAcksAsync;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isUseCompression() const {
    return this->settings->useCompression;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUseCompression(bool value) {
    this->settings->useCompression = value;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionFactory::getCompressionLevel() const {
    return this->settings->compressionLevel;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setCompressionLevel(int value) {

    if (value < 0) {
        this->settings->compressionLevel = -1;
    }

    this->settings->compressionLevel = Math::min(value, 9);
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnectionFactory::getSendTimeout() const {
    return this->settings->sendTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setSendTimeout(unsigned int timeout) {
    this->settings->sendTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnectionFactory::getCloseTimeout() const {
    return this->settings->closeTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setCloseTimeout(unsigned int timeout) {
    this->settings->closeTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnectionFactory::getProducerWindowSize() const {
    return this->settings->producerWindowSize;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setProducerWindowSize(unsigned int windowSize) {
    this->settings->producerWindowSize = windowSize;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isMessagePrioritySupported() const {
    return this->settings->messagePrioritySupported;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setMessagePrioritySupported(bool value) {
    this->settings->messagePrioritySupported = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isWatchTopicAdvisories() const {
    return this->settings->watchTopicAdvisories;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setWatchTopicAdvisories(bool value) {
    this->settings->watchTopicAdvisories = value;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionFactory::getAuditDepth() const {
    return this->settings->auditDepth;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setAuditDepth(int auditDepth) {
    this->settings->auditDepth = auditDepth;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnectionFactory::getAuditMaximumProducerNumber() const {
    return this->settings->auditMaximumProducerNumber;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setAuditMaximumProducerNumber(int auditMaximumProducerNumber) {
    this->settings->auditMaximumProducerNumber = auditMaximumProducerNumber;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isCheckForDuplicates() const {
    return this->settings->checkForDuplicates;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setCheckForDuplicates(bool checkForDuplicates) {
    this->settings->checkForDuplicates = checkForDuplicates;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isTransactedIndividualAck() const {
    return this->settings->transactedIndividualAck;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setTransactedIndividualAck(bool transactedIndividualAck) {
    this->settings->transactedIndividualAck = transactedIndividualAck;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isNonBlockingRedelivery() const {
    return this->settings->nonBlockingRedelivery;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setNonBlockingRedelivery(bool nonBlockingRedelivery) {
    this->settings->nonBlockingRedelivery = nonBlockingRedelivery;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isOptimizeAcknowledge() const {
    return this->settings->optimizeAcknowledge;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setOptimizeAcknowledge(bool optimizeAcknowledge) {
    this->settings->optimizeAcknowledge = optimizeAcknowledge;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnectionFactory::getOptimizeAcknowledgeTimeOut() const {
    return this->settings->optimizeAcknowledgeTimeOut;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setOptimizeAcknowledgeTimeOut(long long optimizeAcknowledgeTimeOut) {
    this->settings->optimizeAcknowledgeTimeOut = optimizeAcknowledgeTimeOut;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnectionFactory::getOptimizedAckScheduledAckInterval() const {
    return this->settings->optimizedAckScheduledAckInterval;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setOptimizedAckScheduledAckInterval(long long optimizedAckScheduledAckInterval) {
    this->settings->optimizedAckScheduledAckInterval = optimizedAckScheduledAckInterval;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnectionFactory::getConsumerFailoverRedeliveryWaitPeriod() const {
    return this->settings->consumerFailoverRedeliveryWaitPeriod;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setConsumerFailoverRedeliveryWaitPeriod(long long value) {
    this->settings->consumerFailoverRedeliveryWaitPeriod = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isUseRetroactiveConsumer() const {
    return this->settings->useRetroactiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUseRetroactiveConsumer(bool useRetroactiveConsumer) {
    this->settings->useRetroactiveConsumer = useRetroactiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isExclusiveConsumer() const {
    return this->settings->exclusiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setExclusiveConsumer(bool exclusiveConsumer) {
    this->settings->exclusiveConsumer = exclusiveConsumer;
}
