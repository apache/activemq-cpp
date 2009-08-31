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

#ifndef _DECAF_UTIL_CONCURRENT_SYNCHRONOUSQUEUE_H_
#define _DECAF_UTIL_CONCURRENT_SYNCHRONOUSQUEUE_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/BlockingQueue.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * A {@linkplain BlockingQueue blocking queue} in which each insert operation
     * must wait for a corresponding remove operation by another thread, and
     * vice versa.  A synchronous queue does not have any internal capacity,
     * not even a capacity of one.  You cannot <tt>peek</tt> at a synchronous
     * queue because an element is only present when you try to remove it; you
     * cannot insert an element (using any method) unless another thread is trying
     * to remove it; you cannot iterate as there is nothing to iterate.  The
     * <em>head</em> of the queue is the element that the first queued inserting
     * thread is trying to add to the queue; if there is no such queued thread
     * then no element is available for removal and <tt>poll()</tt> will return
     * <tt>null</tt>.  For purposes of other <tt>Collection</tt> methods (for
     * example <tt>contains</tt>), a <tt>SynchronousQueue</tt> acts as an
     * empty collection.  This queue does not permit <tt>null</tt> elements.
     *
     * <p>Synchronous queues are similar to rendezvous channels used in
     * CSP and Ada. They are well suited for handoff designs, in which an
     * object running in one thread must sync up with an object running
     * in another thread in order to hand it some information, event, or
     * task.
     *
     * <p> This class supports an optional fairness policy for ordering
     * waiting producer and consumer threads.  By default, this ordering
     * is not guaranteed. However, a queue constructed with fairness set
     * to <tt>true</tt> grants threads access in FIFO order.
     *
     * <p>This class and its iterator implement all of the
     * <em>optional</em> methods of the {@link Collection} and {@link
     * Iterator} interfaces.
     *
     * @since 1.0
     */
    template< typename E >
    class SynchronousQueue : public BlockingQueue<E> {
    private:

        SynchronousQueue( const SynchronousQueue& );
        SynchronousQueue& operator= ( const SynchronousQueue& );

    public:

        SynchronousQueue() {}

        virtual ~SynchronousQueue() {}

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_SYNCHRONOUSQUEUE_H_ */
