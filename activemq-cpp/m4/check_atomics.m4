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
dnl check_atomics.m4 - checks support for atomics and determines what the
dnl                    features of the installed library are on this platform.
dnl
dnl This macro checks for the presence of the atomics operations.  If found then
dnl the library is tested to determine what features it supports, or lacks and
dnl configuration options are set to indicate this information.
dnl

dnl
dnl DECAF_CHECK_FOR_ATOMIC_BUILTINS in GCC
dnl
AC_DEFUN([DECAF_CHECK_FOR_ATOMIC_BUILTINS], [

    AC_CACHE_CHECK([whether the compiler provides atomic builtins], [ap_cv_atomic_builtins],
    [AC_TRY_RUN([
    int main()
    {
        unsigned long val = 1010, tmp, *mem = &val;

        if (__sync_fetch_and_add(&val, 1010) != 1010 || val != 2020)
            return 1;

        tmp = val;

        if (__sync_fetch_and_sub(mem, 1010) != tmp || val != 1010)
            return 1;

        if (__sync_sub_and_fetch(&val, 1010) != 0 || val != 0)
            return 1;

        tmp = 3030;

        if (__sync_val_compare_and_swap(mem, 0, tmp) != 0 || val != tmp)
            return 1;

        if (__sync_lock_test_and_set(&val, 4040) != 3030)
            return 1;

        mem = &tmp;

        if (__sync_val_compare_and_swap(&mem, &tmp, &val) != &tmp)
            return 1;

        __sync_synchronize();

        if (mem != &val)
            return 1;

        return 0;
    }], [ap_cv_atomic_builtins=yes], [ap_cv_atomic_builtins=no], [ap_cv_atomic_builtins=no])])

    if test "$ap_cv_atomic_builtins" = "yes"; then
        AC_DEFINE(HAVE_ATOMIC_BUILTINS, 1, [Define if compiler provides atomic builtins])
    fi

])dnl

dnl ---------------------------------------------------------------------------
dnl Checks for atomic operations support and the various features that are
dnl needed in order to build the DECAF Code that uses atomics provided by
dnl the compiler or OS rather than using Mutex based atomic operations.
dnl ---------------------------------------------------------------------------

AC_DEFUN([DECAF_CHECK_ATOMICS], [

    dnl Attempts to enable atomic builtins compilation on this platform.
    DECAF_CHECK_FOR_ATOMIC_BUILTINS

])
