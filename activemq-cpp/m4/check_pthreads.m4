dnl -------------------------------------------------------- -*- autoconf -*-
dnl Licensed to the Apache Software Foundation (ASF) under one or more
dnl contributor license agreements.  See the NOTICE file distributed with
dnl this work for additional information regarding copyright ownership.
dnl The ASF licenses this file to You under the Apache License, Version 2.0
dnl (the "License"); you may not use this file except in compliance with
dnl the License.  You may obtain a copy of the License at
dnl
dnl     http://www.apache.org/licenses/LICENSE-2.0
dnl
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.

dnl
dnl check_pthreads.m4 - checks support for pthreads and determines what the
dnl                     features of the installed library are on this platform.
dnl
dnl This macro checks for the presence of the pthreads library.  If found then
dnl the library is tested to determine what features it supports, or lacks and
dnl configuration options are set to indicate this information.
dnl

dnl
dnl AMQCPP_CHECK_PTHREADS_H([ ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl gcc issues warnings when parsing AIX 4.3.3's pthread.h which causes
dnl autoconf to incorrectly conclude that pthreads is not available.
dnl Turn off warnings if we're using gcc.
dnl
AC_DEFUN([DECAF_CHECK_PTHREADS_H], [
    if test "$GCC" = "yes"; then
        SAVE_FL="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS -w"
        AC_CHECK_HEADERS(pthread.h, [ $1 ] , [ $2 ] )
        CPPFLAGS="$SAVE_FL"
    else
        AC_CHECK_HEADERS(pthread.h, [ $1 ] , [ $2 ] )
    fi
])dnl

dnl
dnl DECAF_CHECK_PTHREAD_GETSPECIFIC_TWO_ARGS
dnl
AC_DEFUN([DECAF_CHECK_PTHREAD_GETSPECIFIC_TWO_ARGS], [

    AC_CACHE_CHECK(whether pthread_getspecific takes two arguments, ac_cv_pthread_getspecific_two_args,[
    AC_TRY_COMPILE([
            #include <pthread.h>
        ],[
            pthread_key_t key;
            void *tmp;
            pthread_getspecific(key,&tmp);
        ],[
            ac_cv_pthread_getspecific_two_args=yes
        ],[
            ac_cv_pthread_getspecific_two_args=no
        ])
    ])

    if test "$ac_cv_pthread_getspecific_two_args" = "yes"; then
      AC_DEFINE(PTHREAD_GETSPECIFIC_TAKES_TWO_ARGS, 1, [Define if pthread_getspecific() has two args])
    fi
])dnl


dnl
dnl DECAF_CHECK_PTHREAD_ATTR_GETDETACHSTATE_ONE_ARG
dnl
AC_DEFUN([DECAF_CHECK_PTHREAD_ATTR_GETDETACHSTATE_ONE_ARG], [
    AC_CACHE_CHECK([whether pthread_attr_getdetachstate takes one argument],
                   [ac_cv_pthread_attr_getdetachstate_one_arg], [
        AC_TRY_COMPILE([
            #include <pthread.h>
        ],[
            pthread_attr_t *attr;
            pthread_attr_getdetachstate(attr);
        ],[
            ac_cv_pthread_attr_getdetachstate_one_arg=yes
        ],[
            ac_cv_pthread_attr_getdetachstate_one_arg=no
        ])
    ])

    if test "$ac_cv_pthread_attr_getdetachstate_one_arg" = "yes"; then
          AC_DEFINE(PTHREAD_ATTR_GETDETACHSTATE_TAKES_ONE_ARG, 1, [Define if pthread_attr_getdetachstate() has one arg])
    fi
])dnl

dnl
dnl DECAF_CHECK_SIGWAIT_ONE_ARG
dnl
AC_DEFUN([DECAF_CHECK_SIGWAIT_ONE_ARG], [
    AC_CACHE_CHECK([whether sigwait takes one argument],
                   [ac_cv_sigwait_one_arg],[
        AC_TRY_COMPILE(
            [
                #if defined(__NETBSD__) || defined(DARWIN)
                    /* When using the unproven-pthreads package, we need to pull in this
                     * header to get a prototype for sigwait().  Else things will fail later
                     * on.  XXX Should probably be fixed in the unproven-pthreads package.
                     * Darwin is declaring sigwait() in the wrong place as well.
                     */
                    #include <pthread.h>
                #endif
                #include <signal.h>
            ],
            [
                sigset_t set;
                sigwait(&set);
            ],
            [ac_cv_sigwait_one_arg=yes],
            [ac_cv_sigwait_one_arg=no]
        )
    ])

    if test "$ac_cv_sigwait_one_arg" = "yes"; then
        AC_DEFINE([SIGWAIT_TAKES_ONE_ARG], 1 , [Define if sigwait takes one arg])
    fi
])

dnl Check for recursive mutex support (per SUSv3).
AC_DEFUN([DECAF_CHECK_PTHREAD_RECURSIVE_MUTEX], [
    AC_CACHE_CHECK([for recursive mutex support],
                   [ac_cv_mutex_recursive], [
        AC_TRY_RUN([
            #include <sys/types.h>
            #include <pthread.h>
            #include <stdlib.h>

            int main() {
                pthread_mutexattr_t attr;
                pthread_mutex_t m;

                exit ( pthread_mutexattr_init(&attr) ||
                       pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) ||
                       pthread_mutex_init(&m, &attr) );
            }
            ],
            [ac_cv_mutex_recursive=yes],
            [ac_cv_mutex_recursive=no],
            [ac_cv_mutex_recursive=no]
        )
    ])

    if test "$ac_cv_mutex_recursive" = "yes"; then
       AC_DEFINE([HAVE_PTHREAD_MUTEX_RECURSIVE], 1,
                 [Define if recursive pthread mutexes are available])
    fi
])

dnl Check for Read / Write locks
AC_DEFUN([DECAF_CHECK_PTHREAD_EXTRAS], [

    AC_CHECK_FUNCS([pthread_key_delete pthread_rwlock_init pthread_attr_setguardsize pthread_yield])

    if test "$ac_cv_func_pthread_rwlock_init" = "yes"; then
        dnl ----------------------------- Checking for pthread_rwlock_t
        AC_CACHE_CHECK([for pthread_rwlock_t],
                       [ac_cv_type_rwlock_t],
            AC_TRY_COMPILE(
                [
                    #include <sys/types.h>
                    #include <pthread.h>
                ],
                [
                    pthread_rwlock_t *rwlock;
                ],
                [ac_cv_type_rwlock_t=yes],
                [ac_cv_type_rwlock_t=no],
                [ac_cv_type_rwlock_t=no]
            )
        )
        if test "$ac_cv_type_rwlock_t" = "yes"; then
           AC_DEFINE(HAVE_PTHREAD_RWLOCKS, 1, [Define if pthread rwlocks are available])
        fi
    fi

    if test "$ac_cv_func_pthread_yield" = "no"; then
        dnl ----------------------------- Checking for sched_yield
        AC_CHECK_HEADERS([sched.h])
        AC_CHECK_FUNCS([sched_yield])
    fi

    dnl ----------------------------- Check for non-posix pthreads methods
    AC_CHECK_FUNCS([pthread_tryjoin_np pthread_timedjoin_np])

])

dnl
dnl DECAF_PTHREADS_TRY_RUN(actions-if-success)
dnl
dnl Try running a program which uses pthreads, executing the
dnl actions-if-success commands on success.
dnl
AC_DEFUN([DECAF_PTHREADS_TRY_RUN], [
    AC_TRY_RUN([
        #include <pthread.h>
        #include <stddef.h>

        void *thread_routine(void *data) {
            return data;
        }

        int main() {
            pthread_t thd;
            pthread_mutexattr_t mattr;
            pthread_once_t once_init = PTHREAD_ONCE_INIT;
            int data = 1;
            pthread_mutexattr_init(&mattr);
            return pthread_create(&thd, NULL, thread_routine, &data);
        }
    ],
    [ac_pthread_run=yes],
    [ac_pthread_run=no],
    [ac_pthread_run=no])

    if test $ac_pthread_run = yes; then
        $1
    fi

])dnl


dnl
dnl DECAF_PTHREADS_CONFIGURE()
dnl
dnl Try to find a way to enable POSIX threads.  Sets the
dnl pthreads_working variable to "yes" on success.
dnl
AC_DEFUN([DECAF_PTHREADS_CONFIGURE], [
    AC_CACHE_CHECK( [for CFXXLAGS needed for pthreads],
                    [ac_cv_pthreads_cxxflags],
    [
        var_ptc_cxxflags=$CXXFLAGS
        for flag in none -kthread -pthread -pthreads -mt -mthreads -Kthread -threads; do
            CXXFLAGS=$var_ptc_cxxflags
            test "x$flag" != "xnone" && CXXFLAGS="$CXXFLAGS $flag"
            DECAF_PTHREADS_TRY_RUN([
                ac_cv_pthreads_cxxflags="$flag"
                break
            ])
        done
        CXXFLAGS=$var_ptc_cxxflags
    ])

    if test -n "$ac_cv_pthreads_cxxflags"; then
        pthreads_working=yes
        if test "x$ac_cv_pthreads_cxxflags" != "xnone"; then
            AC_ADDTO_VARIABLE( [CXXFLAGS], [$ac_cv_pthreads_cxxflags])
        fi
    fi

    # The CFLAGS may or may not be sufficient to ensure that our lib
    # depends on the pthreads library: some versions of libtool
    # drop -pthread when passed on the link line; some versions of
    # gcc ignore -pthread when linking a shared object.  So always
    # try and add the relevant library to LIBS too.

    AC_CACHE_CHECK( [for LIBS needed for pthreads],
                    [ac_cv_pthreads_libs],
    [
        var_ptc_libs=$LIBS
        for lib in -lpthread -lpthreads -lc_r; do
            LIBS="$var_ptc_libs $lib"
            DECAF_PTHREADS_TRY_RUN([
                ac_cv_pthreads_libs=$lib
                break
            ])
        done
        LIBS=$var_ptc_libs
    ])

    if test -n "$ac_cv_pthreads_libs"; then
        pthreads_working=yes
        AC_ADDTO_VARIABLE( LIBS, [$ac_cv_pthreads_libs] )
    fi

    if test "$pthreads_working" = "yes"; then
        threads_result="POSIX Threads found"
    else
        threads_result="POSIX Threads not found"
    fi

])dnl


dnl ---------------------------------------------------------------------------
dnl Checks for pthreads support and the various features that are needed in
dnl order to build the DECAF Code that uses pthreads.  Also checks for the
dnl presence of some optional functions that we can work around if not
dnl present.
dnl ---------------------------------------------------------------------------

AC_DEFUN([DECAF_CHECK_PTHREADS], [

    AC_CHECK_LIB( pthread, pthread_create, [have_pthread="yes"],
                  AC_MSG_ERROR( [libpthread not found!] ) )

    DECAF_CHECK_PTHREADS_H( [have_pthread="yes"], AC_MSG_ERROR( [pthreads.h not found!] ) )

    dnl Attempts to enable pthreads compilation on this platform.
    DECAF_PTHREADS_CONFIGURE

    DECAF_CHECK_PTHREAD_GETSPECIFIC_TWO_ARGS
    DECAF_CHECK_PTHREAD_ATTR_GETDETACHSTATE_ONE_ARG
    DECAF_CHECK_SIGWAIT_ONE_ARG
    DECAF_CHECK_PTHREAD_RECURSIVE_MUTEX
    DECAF_CHECK_PTHREAD_EXTRAS

])
