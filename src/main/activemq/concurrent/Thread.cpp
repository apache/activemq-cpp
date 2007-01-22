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
 
#include "Thread.h"
#include <errno.h>

#ifdef HAVE_PTHREAD_H
    #include <errno.h> // EINTR
    extern int errno;
#else
    #include <process.h> // _endthreadex
#endif

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/exceptions/RuntimeException.h>

using namespace activemq;
using namespace activemq::concurrent;


////////////////////////////////////////////////////////////////////////////////
Thread::Thread()
{
    task = this;
    started = false;
    joined = false;
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task )
{
    this->task = task;
    started = false;
    joined = false;
}

////////////////////////////////////////////////////////////////////////////////
Thread::~Thread()
{
}

////////////////////////////////////////////////////////////////////////////////
void Thread::start() throw ( exceptions::ActiveMQException )
{
    if (this->started) {
        throw exceptions::ActiveMQException( __FILE__, __LINE__,
            "Thread already started");
    }
    
#ifdef HAVE_PTHREAD_H
    
    ::pthread_attr_init (&attributes);
    ::pthread_attr_setdetachstate (&attributes, PTHREAD_CREATE_JOINABLE);
    int err = ::pthread_create (
        &this->threadHandle,
        &attributes,
        runCallback,
        this);
    if (err != 0) {
        throw exceptions::ActiveMQException( __FILE__, __LINE__,
            "Coud not start thread");
    }
    
#else

    unsigned int threadId = 0;
    this->threadHandle = 
        (HANDLE)::_beginthreadex(NULL, 0, runCallback, this, 0, &threadId);
    if (this->threadHandle == NULL) {
        throw exceptions::ActiveMQException( __FILE__, __LINE__,
            "Coud not start thread");
    }
    
#endif

    // Mark the thread as started.
    started = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() throw( exceptions::ActiveMQException )
{
    if (!this->started) {
        throw exceptions::ActiveMQException( __FILE__, __LINE__,
            "Thread::join() called without having called Thread::start()");
    }
    if (!this->joined) {
        
#ifdef HAVE_PTHREAD_H
        ::pthread_join(this->threadHandle, NULL);
#else
        ::WaitForSingleObject (this->threadHandle, INFINITE);       
#endif

    }
    this->joined = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( int millisecs )
{
#ifdef HAVE_PTHREAD_H
    struct timespec rec, rem;
    rec.tv_sec = millisecs / 1000;
    rec.tv_nsec = (millisecs % 1000) * 1000000;
    while( nanosleep( &rec, &rem ) == -1 ){
        if( errno != EINTR ){
            break;
        }
    }
    
#else
    ::Sleep (millisecs);
#endif
}

////////////////////////////////////////////////////////////////////////////////
unsigned long Thread::getId(void)
{
   #ifdef HAVE_PTHREAD_H
      return (long)(pthread_self());
   #else
      return GetCurrentThreadId();
   #endif
}

////////////////////////////////////////////////////////////////////////////////
#ifdef HAVE_PTHREAD_H
    void*
#else
    unsigned int WINAPI
#endif
Thread::runCallback( void* param )
{
    // Get the instance.
    Thread* thread = (Thread*)param;
    
    // Invoke run on the task.
    try{
        thread->task->run();
    } catch( ... ){
        exceptions::RuntimeException ex(__FILE__, __LINE__, "unhandled exception bubbled up to Thread::run");
        ex.printStackTrace();
    }

#ifdef HAVE_PTHREAD_H
    ::pthread_attr_destroy( &thread->attributes );
    return NULL;
#else

    // Needed when using threads and CRT in Windows. Otherwise memleak can appear.
    ::_endthreadex(0); 
    
    // _endthreadex (unlike _endthread) does not automatically close the thread handle
    // so we need to do this manually.
    ::CloseHandle(thread->threadHandle);
    
    return 0;
#endif
}

