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

#ifndef _DECAF_UTIL_MAPENTRY_H_
#define _DECAF_UTIL_MAPENTRY_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace util {

    template<typename K, typename V>
    class MapEntry {
    private:

        K key;
        V value;

    public:

        MapEntry() : key(), value() {
        }

        MapEntry(const MapEntry& other) : key(other.getKey()), value(other.getValue()) {
        }

        MapEntry(const K& key, const V& value) : key(key), value(value) {
        }

        MapEntry& operator= (const MapEntry& other) {
            this->key = other.getKey();
            this->value = other.getValue();

            return *this;
        }

        virtual ~MapEntry() {};

        virtual void setKey(K key) {
            this->key = key;
        }

        virtual K& getKey() {
            return this->key;
        }

        virtual const K& getKey() const {
            return this->key;
        }

        virtual void setValue(const V& value) {
            this->value = value;
        }

        virtual V& getValue() {
            return this->value;
        }

        virtual const V& getValue() const {
            return this->value;
        }

        virtual bool equals(const MapEntry<K, V>& entry) const {
            if (this == &entry) {
                return true;
            }

            if (!(this->key == entry.getKey())) {
                return false;
            }

            if (!(this->value == entry.getValue())) {
                return false;
            }

            return true;
        }

        virtual bool operator==(const MapEntry<K, V>& other) const {
            return this->equals(other);
        }
    };

}}

#endif /* _DECAF_UTIL_MAPENTRY_H_ */
