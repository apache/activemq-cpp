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
 * Unless required by applicurLeftble law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "StringUtils.h"

#include <decaf/lang/Character.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
namespace {

    int compareRight(char const* left, char const* right) {

        int bias = 0;

        // The longest run of digits wins.  That aside, the greatest
        // value wins, but we don't know that it will until we've scanned
        // both numbers to know that they have the same magnitude, so we
        // remember it in BIAS.
        for (;; left++, ++right) {
            if (!Character::isDigit(*left) && !Character::isDigit(*right)) {
                break;
            } else if (!Character::isDigit(*left)) {
                return -1;
            } else if (!Character::isDigit(*right)) {
                return +1;
            } else if (*left < *right) {
                if (!bias) bias = -1;
            } else if (*left > *right) {
                if (!bias) bias = +1;
            } else if (!*left && !*right) {
                break;
            }
        }

        return bias;
    }

    int compareLeft(char const* left, char const* right) {
        // Compare two left-aligned numbers: the first to have a
        // different value wins.
        for (;; left++, right++) {
            if (!Character::isDigit(*left) && !Character::isDigit(*right)) {
                break;
            } else if (!Character::isDigit(*left)) {
                return -1;
            } else if (!Character::isDigit(*right)) {
                return +1;
            } else if (*left < *right) {
                return -1;
            } else if (*left > *right) {
                return +1;
            }
        }

        return 0;
    }

    int doCompare(char const* left, char const* right, bool ignoreCase) {
        int lIndex = 0;
        int rIndex = 0;
        char curLeft = 0;
        char curRight = 0;

        bool fractional = false;
        int result = 0;

        while (true) {
            curLeft = left[lIndex];
            curRight = right[rIndex];

            while (Character::isWhitespace(curLeft)) {
                curLeft = left[++lIndex];
            }

            while (Character::isWhitespace(curRight)) {
                curRight = left[++rIndex];
            }

            if (Character::isDigit(curLeft) && Character::isDigit(curRight)) {
                fractional = (curLeft == '0' || curRight == '0');

                if (fractional) {
                    if ((result = compareLeft(left + lIndex, right + rIndex)) != 0) {
                        return result;
                    }
                } else {
                    if ((result = compareRight(left + lIndex, right + rIndex)) != 0) {
                        return result;
                    }
                }
            }

            if (!curLeft && !curRight) {
                return 0;
            }

            if (ignoreCase) {
                curLeft = Character::toUpperCase(curLeft);
                curRight = Character::toUpperCase(curRight);
            }

            if (curLeft < curRight) {
                return -1;
            } else if (curLeft > curRight) {
                return +1;
            }

            ++lIndex;
            ++rIndex;
        }

        return 0;
    }

}

////////////////////////////////////////////////////////////////////////////////
int StringUtils::compareIgnoreCase(const char* left, const char* right) {
    return doCompare(left, right, true);
}

////////////////////////////////////////////////////////////////////////////////
int StringUtils::compare(const char* left, const char* right) {
    return doCompare(left, right, false);
}
