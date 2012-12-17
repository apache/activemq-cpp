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

#include "TimerTaskHeap.h"

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
TimerTaskHeap::TimerTaskHeap() : heap() {
}

////////////////////////////////////////////////////////////////////////////////
TimerTaskHeap::~TimerTaskHeap() {
}

////////////////////////////////////////////////////////////////////////////////
Pointer<TimerTask> TimerTaskHeap::peek() {

    if (heap.empty()) {
        return Pointer<TimerTask>();
    }

    return heap[0];
}

////////////////////////////////////////////////////////////////////////////////
bool TimerTaskHeap::isEmpty() const {
    return heap.empty();
}

////////////////////////////////////////////////////////////////////////////////
std::size_t TimerTaskHeap::size() const {
    return this->heap.size();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTaskHeap::insert(const Pointer<TimerTask>& task) {

    heap.push_back(task);
    upHeap();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTaskHeap::remove(std::size_t pos) {

    // possible to delete any position of the heap
    if (pos < heap.size()) {
        heap[pos] = heap.back();
        heap.pop_back();
        downHeap(pos);
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimerTaskHeap::upHeap() {

    std::size_t current = heap.size() - 1;
    std::size_t parent = (current - 1) / 2;

    while (current != 0 && heap[current]->when < heap[parent]->when) {

        // swap the two
        Pointer<TimerTask> tmp = heap[current];
        heap[current] = heap[parent];
        heap[parent] = tmp;

        // update parent and current positions.
        current = parent;
        parent = (current - 1) / 2;
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimerTaskHeap::downHeap(std::size_t pos) {

    std::size_t current = pos;
    std::size_t child = 2 * current + 1;

    while (child < heap.size() && !heap.empty()) {

        // compare the children if they exist
        if (child + 1 < heap.size() && heap[child + 1]->when < heap[child]->when) {
            child++;
        }

        // compare selected child with parent
        if (heap[current]->when < heap[child]->when) {
            break;
        }

        // swap the two
        Pointer<TimerTask> tmp = heap[current];
        heap[current] = heap[child];
        heap[child] = tmp;

        // update child and current positions
        current = child;
        child = 2 * current + 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
void TimerTaskHeap::reset() {
    heap.clear();
}

////////////////////////////////////////////////////////////////////////////////
void TimerTaskHeap::adjustMinimum() {
    downHeap(0);
}

////////////////////////////////////////////////////////////////////////////////
std::size_t TimerTaskHeap::deleteIfCancelled() {

    std::size_t result = 0;

    for (std::size_t i = 0; i < heap.size(); ++i) {
        if (heap[i]->cancelled) {
            result++;
            remove(i);
            // re-try this point
            i--;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t TimerTaskHeap::find(const Pointer<TimerTask>& task) const {

    for (std::size_t i = 0; i < heap.size(); ++i) {
        if (heap[i] == task) {
            return i;
        }
    }

    return -1;
}
