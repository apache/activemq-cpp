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
dnl check_visibility.m4 - checks support for visibility family of CXX flags
dnl
dnl This macro determines whether or not the CXX flags -fvisibility and 
dnl -fvisibility-inlines-hidden are allowed with the compiler.  
dnl
dnl If so, the result is "yes" and HAVE_VISIBILITY_OPTIONS is defined.  Else, the
dnl result is "no". 
dnl

AC_DEFUN([CHECK_VISIBILITY], [
    AC_MSG_CHECKING([whether -fvisibility flags work])
           
    OLDCXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$CXXFLAGS -Werror -fvisibility=hidden -fvisibility-inlines-hidden"
    
    AC_TRY_COMPILE([],
      [
      ],
      [
        AC_MSG_RESULT(yes)
        AC_DEFINE([HAVE_VISIBILITY_OPTIONS],[], [define if you have -fvisibility options])
      ],
      [
        AC_MSG_RESULT(no)
      ])
      
    CXXFLAGS="$OLDCXXFLAGS"
])
