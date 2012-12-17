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
dnl check_addrinfo.m4 - checks support for addrinfo structure
dnl
dnl This macro determines if the platform supports the addrinfo structure.
dnl If this platform supports addrinfo, defines HAVE_STRUCT_ADDRINFO.
dnl

AC_DEFUN([CHECK_ADDRINFO], [
    AC_MSG_CHECKING([whether struct addrinfo is defined])
    AC_TRY_COMPILE(
      [ #include <stdio.h>
        #ifdef HAVE_UNISTD_H
        # include <unistd.h>
        #endif
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netdb.h>
      ],
      [
        do {
          struct addrinfo a;
          (void) a.ai_flags;
        } while(0)
      ],
      [
        AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_STRUCT_ADDRINFO,, [define if you have struct addrinfo])
      ],
      [
        AC_MSG_RESULT(no)
      ])
])
