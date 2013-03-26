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

#include "ActiveMQMessageAudit.h"

#include <activemq/util/IdGenerator.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/ProducerId.h>

#include <decaf/util/LRUCache.h>
#include <decaf/util/BitSet.h>
#include <decaf/util/concurrent/Mutex.h>

#include <string>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int ActiveMQMessageAudit::DEFAULT_WINDOW_SIZE = 2048;
const int ActiveMQMessageAudit::MAXIMUM_PRODUCER_COUNT = 64;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {

    class MessageAuditImpl {
    private:

        MessageAuditImpl(const MessageAuditImpl&);
        MessageAuditImpl& operator= (const MessageAuditImpl&);

    public:

        int auditDepth;
        int maximumNumberOfProducersToTrack;
        Mutex mutex;

        LRUCache<std::string, Pointer<BitSet> > map;

        MessageAuditImpl() : auditDepth(2048),
                             maximumNumberOfProducersToTrack(64),
                             mutex(),
                             map() {
        }

        MessageAuditImpl(int auditDepth, int maximumNumberOfProducersToTrack) :
            auditDepth(auditDepth),
            maximumNumberOfProducersToTrack(maximumNumberOfProducersToTrack),
            mutex(),
            map() {
        }

        void adjustMaxProducersToTrack(int value) {
            // When value is smaller than current we need to move the entries
            // to a new cache with that setting so that old ones are pruned
            // since putAll will access the entries in the right order,
            // this shouldn't result in wrong cache entries being removed
            if (value < maximumNumberOfProducersToTrack) {
                LRUCache<std::string, Pointer<BitSet> > newMap(0, value, 0.75f, true);
                newMap.putAll(this->map);
                this->map.clear();
                this->map.putAll(newMap);
            }
            this->map.setMaxCacheSize(value);
            this->maximumNumberOfProducersToTrack = value;
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageAudit::ActiveMQMessageAudit() : impl(new MessageAuditImpl) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageAudit::ActiveMQMessageAudit(int auditDepth, int maximumNumberOfProducersToTrack) :
    impl(new MessageAuditImpl(auditDepth, maximumNumberOfProducersToTrack)) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessageAudit::~ActiveMQMessageAudit() {
    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQMessageAudit::getAuditDepth() const {
    return this->impl->auditDepth;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAudit::setAuditDepth(int value) {
    this->impl->auditDepth = value;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQMessageAudit::getMaximumNumberOfProducersToTrack() const {
    return this->impl->maximumNumberOfProducersToTrack;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAudit::getMaximumNumberOfProducersToTrack(int value) {
    this->impl->adjustMaxProducersToTrack(value);
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageAudit::isDuplicate(const std::string& id) const {
    bool answer = false;
    std::string seed = IdGenerator::getSeedFromId(id);
    if (!seed.empty()) {

        synchronized(&this->impl->mutex) {

            Pointer<BitSet> bits;
            try {
                bits = this->impl->map.get(seed);
            } catch (NoSuchElementException& ex) {
                bits.reset(new BitSet(this->impl->auditDepth));
                this->impl->map.put(seed, bits);
            }

            long long index = IdGenerator::getSequenceFromId(id);
            if (index >= 0) {
                int scaledIndex = (int) index;
                if (index > Integer::MAX_VALUE) {
                    scaledIndex = (int)(index - Integer::MAX_VALUE);
                }

                answer = bits->get(scaledIndex);
                if (!answer) {
                    bits->set(scaledIndex, true);
                }
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageAudit::isDuplicate(decaf::lang::Pointer<MessageId> msgId) const {
    bool answer = false;

    if (msgId != NULL) {
        Pointer<ProducerId> pid = msgId->getProducerId();
        if (pid != NULL) {
            std::string seed = pid->toString();
            if (!seed.empty()) {

                synchronized(&this->impl->mutex) {

                    Pointer<BitSet> bits;
                    try {
                        bits = this->impl->map.get(seed);
                    } catch (NoSuchElementException& ex) {
                        bits.reset(new BitSet(this->impl->auditDepth));
                        this->impl->map.put(seed, bits);
                    }

                    long long index = msgId->getProducerSequenceId();
                    if (index >= 0) {
                        int scaledIndex = (int) index;
                        if (index > Integer::MAX_VALUE) {
                            scaledIndex = (int)(index - Integer::MAX_VALUE);
                        }

                        answer = bits->get(scaledIndex);
                        if (!answer) {
                            bits->set(scaledIndex, true);
                        }
                    }
                }
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAudit::rollback(const std::string& msgId) {
    std::string seed = IdGenerator::getSeedFromId(msgId);
    if (!seed.empty()) {

        synchronized(&this->impl->mutex) {

            Pointer<BitSet> bits;
            try {
                bits = this->impl->map.get(seed);
            } catch (NoSuchElementException& ex) {
            }

            if (bits != NULL) {
                long long index = IdGenerator::getSequenceFromId(msgId);
                if (index >= 0) {
                    int scaledIndex = (int) index;
                    if (index > Integer::MAX_VALUE) {
                        scaledIndex = (int)(index - Integer::MAX_VALUE);
                    }

                    bits->set(scaledIndex, false);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAudit::rollback(decaf::lang::Pointer<commands::MessageId> msgId) {
    if (msgId != NULL) {
        Pointer<ProducerId> pid = msgId->getProducerId();
        if (pid != NULL) {
            std::string seed = pid->toString();
            if (!seed.empty()) {

                synchronized(&this->impl->mutex) {

                    Pointer<BitSet> bits;
                    try {
                        bits = this->impl->map.get(seed);
                    } catch (NoSuchElementException& ex) {
                    }

                    if (bits != NULL) {
                        long long index = msgId->getProducerSequenceId();
                        if (index >= 0) {
                            int scaledIndex = (int) index;
                            if (index > Integer::MAX_VALUE) {
                                scaledIndex = (int)(index - Integer::MAX_VALUE);
                            }

                            bits->set(scaledIndex, false);
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageAudit::isInOrder(const std::string& msgId) const {
    bool answer = true;

    if (!msgId.empty()) {
        std::string seed = IdGenerator::getSeedFromId(msgId);
        if (!seed.empty()) {

            synchronized(&this->impl->mutex) {

                Pointer<BitSet> bits;
                try {
                    bits = this->impl->map.get(seed);
                } catch (NoSuchElementException& ex) {
                    bits.reset(new BitSet(this->impl->auditDepth));
                    this->impl->map.put(seed, bits);
                }

                long long index = IdGenerator::getSequenceFromId(msgId);
                if (index >= 0) {

                    int scaledIndex = (int) index;
                    if (index > Integer::MAX_VALUE) {
                        scaledIndex = (int)(index - Integer::MAX_VALUE);
                    }

                    answer = ((bits->length() - 1) == scaledIndex);
                }
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMessageAudit::isInOrder(decaf::lang::Pointer<commands::MessageId> msgId) const {
    bool answer = false;

    if (msgId != NULL) {
        Pointer<ProducerId> pid = msgId->getProducerId();
        if (pid != NULL) {
            std::string seed = pid->toString();
            if (!seed.empty()) {

                synchronized(&this->impl->mutex) {

                    Pointer<BitSet> bits;
                    try {
                        bits = this->impl->map.get(seed);
                    } catch (NoSuchElementException& ex) {
                        bits.reset(new BitSet(this->impl->auditDepth));
                        this->impl->map.put(seed, bits);
                    }

                    long long index = msgId->getProducerSequenceId();
                    if (index >= 0) {
                        int scaledIndex = (int) index;
                        if (index > Integer::MAX_VALUE) {
                            scaledIndex = (int)(index - Integer::MAX_VALUE);
                        }
                        answer = ((bits->length() - 1) == scaledIndex);
                    }
                }
            }
        }
    }
    return answer;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQMessageAudit::getLastSeqId(decaf::lang::Pointer<commands::ProducerId> id) const {
    long result = -1;
    if (id != NULL) {
        std::string seed = id->toString();
        if (!seed.empty()) {

            synchronized(&this->impl->mutex) {

                Pointer<BitSet> bits;
                try {
                    bits = this->impl->map.get(seed);
                } catch (NoSuchElementException& ex) {
                }

                if (bits != NULL) {
                    result = bits->length() - 1;
                }
            }
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMessageAudit::clear() {
    this->impl->map.clear();
}
