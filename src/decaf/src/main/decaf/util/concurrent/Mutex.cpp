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

#include <decaf/util/concurrent/Mutex.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex()
{
#ifdef HAVE_PTHREAD_H
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
#else
    InitializeCriticalSection(&mutex);
#endif

    lock_owner = 0;
    lock_count = 0;
}

////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex()
{
    // Unlock the mutex.
    unlock();

#ifdef HAVE_PTHREAD_H
    pthread_mutex_destroy(&mutex);
#else
    DeleteCriticalSection(&mutex);
#endif
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::lock() throw( lang::Exception )
{
    if(isLockOwner())
    {
        lock_count++;
    }
    else
    {
#ifdef HAVE_PTHREAD_H
        pthread_mutex_lock(&mutex);
#else
        EnterCriticalSection(&mutex);
#endif

        lock_count = 1;
        lock_owner = lang::Thread::getId();
    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::unlock() throw( lang::Exception )
{
    if(lock_owner == 0)
    {
        return;
    }

    if(!isLockOwner())
    {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::unlock - Failed, not Lock Owner!" );
    }

    lock_count--;

    if(lock_count == 0)
    {
        lock_owner = 0;

#ifdef HAVE_PTHREAD_H
        pthread_mutex_unlock(&mutex);
#else
        LeaveCriticalSection(&mutex);
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait() throw( lang::Exception )
{
    // Delegate to the timed version
    wait( WAIT_INFINITE );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( unsigned long millisecs )
    throw( lang::Exception )
{
    if(!isLockOwner())
    {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::wait - Failed, not Lock Owner!");
    }

    // Save the current owner and Lock count as we are going to
    // unlock and release for someone else to lock on potentially.
    // When we come back and re-lock we want to restore to the
    // state we were in before.
    unsigned long lock_owner = this->lock_owner;
    int           lock_count = this->lock_count;

    this->lock_count = 0;
    this->lock_owner = 0;

#ifdef HAVE_PTHREAD_H

    // Create this threads wait event
    pthread_cond_t waitEvent;
    pthread_cond_init(&waitEvent, NULL);

    // Store the event in the queue so that a notify can
    // call it and wake up the thread.
    eventQ.push_back(&waitEvent);

    int returnValue = 0;
    if(millisecs != WAIT_INFINITE)
    {
        timeval now = {0,0};
        gettimeofday(&now, NULL);

        timespec wait = {0,0};
        wait.tv_sec = now.tv_sec + (millisecs / 1000);
        wait.tv_nsec = (now.tv_usec * 1000) + ((millisecs % 1000) * 1000000);

        if(wait.tv_nsec > 1000000000)
        {
            wait.tv_sec++;
            wait.tv_nsec -= 1000000000;
        }

        returnValue =  pthread_cond_timedwait(&waitEvent, &mutex, &wait);
    }
    else
    {
        returnValue = pthread_cond_wait(&waitEvent, &mutex);
    }

    // If the wait did not succeed for any reason, remove it
    // from the queue.
    if( returnValue != 0 ){
        std::list<pthread_cond_t*>::iterator iter = eventQ.begin();
        for( ; iter != eventQ.end(); ++iter ){
            if( *iter == &waitEvent ){
                eventQ.erase(iter);
                break;
            }
        }
    }

    // Destroy our wait event now, the notify method will have removed it
    // from the event queue.
    pthread_cond_destroy(&waitEvent);

#else // !defined(HAVE_PTHREAD_H)

    // Create the event to wait on
    HANDLE waitEvent = CreateEvent( NULL, false, false, NULL );

    if(waitEvent == NULL)
    {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::Mutex - Failed Creating Event." );
    }

    eventQ.push_back( waitEvent );

    // Release the Lock
    LeaveCriticalSection( &mutex );

    // Wait for a signal
    WaitForSingleObject( waitEvent, millisecs );

    // Reaquire the Lock
    EnterCriticalSection( &mutex );

    // Clean up the event, the notif methods will have
    // already poped it from the queue.
    CloseHandle( waitEvent );

#endif // !defined(HAVE_PTHREAD_H)

    // restore the owner
    this->lock_owner = lock_owner;
    this->lock_count = lock_count;
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notify() throw( lang::Exception )
{
    if( !isLockOwner() )
    {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::Notify - Failed, not Lock Owner!" );
    }

    if( !eventQ.empty() )
    {
#ifdef HAVE_PTHREAD_H
        pthread_cond_signal( eventQ.front() );
        eventQ.pop_front();
#else
        SetEvent( eventQ.front() );
        eventQ.pop_front();
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notifyAll() throw( lang::Exception )
{
    if(!isLockOwner())
    {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::NotifyAll - Failed, not Lock Owner!" );
    }

#ifdef HAVE_PTHREAD_H

    while(!eventQ.empty())
    {
         pthread_cond_signal( eventQ.front() );
         eventQ.pop_front();
    }

#else

    while(!eventQ.empty())
    {
         SetEvent( eventQ.front() );
         eventQ.pop_front();
    }

#endif
}

