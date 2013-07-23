/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/util/URISupport.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/AdvisorySupport.h>

#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>

#include <decaf/util/StringTokenizer.h>
#include <decaf/util/StlSet.h>
#include <decaf/util/HashCode.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/internal/util/StringUtils.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace decaf::internal::util;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const std::string ActiveMQDestination::DEFAULT_ORDERED_TARGET = "coordinator";
const std::string ActiveMQDestination::TEMP_PREFIX = "{TD{";
const std::string ActiveMQDestination::TEMP_POSTFIX = "}TD}";
const std::string ActiveMQDestination::COMPOSITE_SEPARATOR = ",";
const std::string ActiveMQDestination::DestinationFilter::ANY_CHILD = ">";
const std::string ActiveMQDestination::DestinationFilter::ANY_DESCENDENT = "*";
const std::string ActiveMQDestination::QUEUE_QUALIFIED_PREFIX = "queue://";
const std::string ActiveMQDestination::TOPIC_QUALIFIED_PREFIX = "topic://";
const std::string ActiveMQDestination::TEMP_QUEUE_QUALIFED_PREFIX = "temp-queue://";
const std::string ActiveMQDestination::TEMP_TOPIC_QUALIFED_PREFIX = "temp-topic://";
const std::string ActiveMQDestination::TEMP_DESTINATION_NAME_PREFIX = "ID:";

////////////////////////////////////////////////////////////////////////////////
namespace {

    const std::string trim(const std::string& input, const std::string& ws = " \t")
    {
        const size_t beginStr = input.find_first_not_of(ws);
        if (beginStr == std::string::npos)
        {
            // no content
            return "";
        }

        const size_t endStr = input.find_last_not_of(ws);
        const size_t range = endStr - beginStr + 1;

        return input.substr(beginStr, range);
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination() :
    BaseDataStructure(), exclusive(false), ordered(false), advisory(false), compositeDestinations(),
    orderedTarget(DEFAULT_ORDERED_TARGET), physicalName(), options(), hashCode() {

}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::ActiveMQDestination(const std::string& physicalName) :
    BaseDataStructure(), exclusive(false), ordered(false), advisory(false), compositeDestinations(),
    orderedTarget(DEFAULT_ORDERED_TARGET), physicalName(), options(), hashCode() {

    this->setPhysicalName(physicalName);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestination::~ActiveMQDestination() throw() {
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestination::setPhysicalName(const std::string& physicalName) {

    this->physicalName = physicalName;

    size_t pos = physicalName.find_first_of('?');
    if (pos != std::string::npos) {
        std::string optstring = physicalName.substr(pos + 1);
        this->physicalName = physicalName.substr(0, pos);
        URISupport::parseQuery(optstring, &options.getProperties());
    }

    this->advisory = physicalName.find_first_of(AdvisorySupport::ADVISORY_TOPIC_PREFIX) == 0;
    this->compositeDestinations.clear();

    this->hashCode = HashCode<std::string>()(this->physicalName);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestination::copyDataStructure(const DataStructure* src) {

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure( src );

    const ActiveMQDestination* srcPtr = dynamic_cast<const ActiveMQDestination*> (src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__, "BrokerId::copyDataStructure - src is NULL or invalid");
    }

    this->setPhysicalName(srcPtr->getPhysicalName());
    this->setAdvisory(srcPtr->isAdvisory());
    this->setOrdered(srcPtr->isOrdered());
    this->setExclusive(srcPtr->isExclusive());
    this->setOrderedTarget(srcPtr->getOrderedTarget());
    this->options.copy(&srcPtr->getOptions());
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQDestination::toString() const {

    switch (this->getDestinationType()) {
        case cms::Destination::TOPIC:
            return std::string("topic://") + this->getPhysicalName();
        case cms::Destination::TEMPORARY_TOPIC:
            return std::string("temp-topic://") + this->getPhysicalName();
        case cms::Destination::TEMPORARY_QUEUE:
            return std::string("temp-queue://") + this->getPhysicalName();
        default:
            return std::string("queue://") + this->getPhysicalName();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestination::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const ActiveMQDestination* valuePtr = dynamic_cast<const ActiveMQDestination*> (value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    return this->getPhysicalName() == valuePtr->getPhysicalName();
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQDestination::getDataStructureType() const {
    return ActiveMQDestination::ID_ACTIVEMQDESTINATION;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQDestination::getClientId(const ActiveMQDestination* destination) {

    std::string answer = "";
    if (destination != NULL && destination->isTemporary()) {
        std::string name = destination->getPhysicalName();
        size_t start = name.find(TEMP_PREFIX);
        if (start != std::string::npos) {
            start += TEMP_PREFIX.length();
            size_t stop = name.rfind(TEMP_POSTFIX);
            if (stop > start && stop < name.length()) {
                answer = name.substr(start, stop - start);
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ActiveMQDestination> ActiveMQDestination::createDestination(int type, const std::string& name) {

    Pointer<ActiveMQDestination> result;

    if (name.find(QUEUE_QUALIFIED_PREFIX) == 0) {
        result.reset(new ActiveMQQueue(name.substr(QUEUE_QUALIFIED_PREFIX.length())));
        return result;
    } else if (name.find(TOPIC_QUALIFIED_PREFIX) == 0) {
        result.reset(new ActiveMQTopic(name.substr(TOPIC_QUALIFIED_PREFIX.length())));
        return result;
    } else if (name.find(TEMP_QUEUE_QUALIFED_PREFIX) == 0) {
        result.reset(new ActiveMQTempQueue(name.substr(TEMP_QUEUE_QUALIFED_PREFIX.length())));
        return result;
    } else if (name.find(TEMP_TOPIC_QUALIFED_PREFIX) == 0) {
        result.reset(new ActiveMQTempTopic(name.substr(TEMP_TOPIC_QUALIFED_PREFIX.length())));
        return result;
    }

    switch (type) {
        case cms::Destination::QUEUE:
            result.reset(new ActiveMQQueue(name));
            return result;
        case cms::Destination::TOPIC:
            result.reset(new ActiveMQTopic(name));
            return result;
        case cms::Destination::TEMPORARY_QUEUE:
            result.reset(new ActiveMQTempQueue(name));
            return result;
        case cms::Destination::TEMPORARY_TOPIC:
            result.reset(new ActiveMQTempTopic(name));
            return result;
        default:
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Invalid default destination type: %d", type);
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQDestination::getDestinationTypeAsString() const {
    switch (getDestinationType()) {
        case cms::Destination::QUEUE:
            return "Queue";
        case cms::Destination::TOPIC:
            return "Topic";
        case cms::Destination::TEMPORARY_QUEUE:
            return "TempQueue";
        case cms::Destination::TEMPORARY_TOPIC:
            return "TempTopic";
        default:
            throw new IllegalArgumentException(
                __FILE__, __LINE__, "Invalid destination type: " + getDestinationType());
    }
}

////////////////////////////////////////////////////////////////////////////////
decaf::util::ArrayList< Pointer<ActiveMQDestination> > ActiveMQDestination::getCompositeDestinations() const {

    if (!this->compositeDestinations.isEmpty()) {
        return this->compositeDestinations;
    }

    if (this->isComposite()) {

        StlSet<std::string> components;
        StringTokenizer iter(this->physicalName, ActiveMQDestination::COMPOSITE_SEPARATOR);
        while (iter.hasMoreTokens()) {
            std::string name = trim(iter.nextToken());
            if (name.length() == 0) {
                continue;
            }
            components.add(name);
        }

        Pointer< Iterator<std::string> > iterator(components.iterator());
        while (iterator->hasNext()) {
            compositeDestinations.add(createDestination(iterator->next()));
        }
    }

    return this->compositeDestinations;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQDestination::compareTo(const ActiveMQDestination& value) const {

    if (this == &value) {
        return 0;
    }

    int valueComp = StringUtils::compare(this->getPhysicalName().c_str(), value.getPhysicalName().c_str());
    if (valueComp != 0) {
        return valueComp;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestination::equals(const ActiveMQDestination& value) const {
    return this->getPhysicalName() == value.getPhysicalName();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestination::operator==(const ActiveMQDestination& value) const {
    return this->getPhysicalName() == value.getPhysicalName();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestination::operator<(const ActiveMQDestination& value) const {
    return this->compareTo(value) < 0;
}
