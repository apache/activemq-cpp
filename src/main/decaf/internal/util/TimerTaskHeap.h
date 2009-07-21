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

#ifndef _DECAF_INTERNAL_UTIL_TIMERTASKHEAP_H_
#define _DECAF_INTERNAL_UTIL_TIMERTASKHEAP_H_

#include <decaf/util/Config.h>

#include <decaf/util/TimerTask.h>
#include <decaf/lang/Pointer.h>

namespace decaf {
namespace internal {
namespace util {

    using decaf::lang::Pointer;
    using decaf::util::TimerTask;

    /**
     * A Binary Heap implemented specifically for the Timer class in Decaf Util.
     *
     * @since 1.0
     */
    class DECAF_API TimerTaskHeap {
    private:

        // The default size of the Heap at creation.
        static const int DEFAULT_HEAP_SIZE = 256;

        // Dynamic Array of TimerTasks used to represent the heap
        std::vector< Pointer<TimerTask> > heap;

    public:

        TimerTaskHeap();
        virtual ~TimerTaskHeap();

        /**
         * Peaks at the Head of the Heap, returns the task with the nearest scheduled run time.
         *
         * @return The TimerTask that is scheduled to be executed next if the Heap is empty
         *         a Null Pointer value is returned.
         */
        Pointer<TimerTask> peek();

        /**
         * @returns true if the heap is empty.
         */
        bool isEmpty() const;

        /**
         * @returns the size of the heap.
         */
        std::size_t size() const;

        /**
         * Inserts the specified Task into the heap, heap is reordered to reflect the addition
         * of a new element.
         *
         * @param task
         *      The TimerTask to insert into the heap.
         */
        void insert( const Pointer<TimerTask>& task );

        /**
         * Removes the Task at the specified position from the heap, resorts the heap from the
         * position down to the bottom.
         *
         * @param pos
         *      The position at which to remove the TimerTask and begin a resort of the heap.
         */
        void remove( std::size_t pos );

        /**
         * Clear all contents from the heap.
         */
        void reset();

        /**
         * Resorts the heap starting at the top.
         */
        void adjustMinimum();

        /**
         * Runs through the heap removing all cancelled Tasks from it, this is not normally
         * used but in case a a cancellation of a large number of tasks the user can perform
         * this purge.
         *
         * @return the number of task that were removed from the heap becuase they were cancelled.
         */
        std::size_t deleteIfCancelled();

        /**
         * Searches the heap for the specified TimerTask element and returns its position in the
         * heap.  Returns the unsigned equivalent of -1 if the element is not found.
         *
         * @returns the position in the Heap where the Task is stored, or npos.
         */
        std::size_t find( const Pointer<TimerTask>& task ) const;

    private:

        void upHeap();
        void downHeap( std::size_t pos );

    };

}}}

#endif /* _DECAF_INTERNAL_UTIL_TIMERTASKHEAP_H_ */
