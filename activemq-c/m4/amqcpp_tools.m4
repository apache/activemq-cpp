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
dnl -------------------------------------------------------- -*- autoconf -*-
dnl
dnl  This Macro File defines functions that help to manage the libraries
dnl  dependance on the Apache Portable Runtime.
dnl
dnl  DECAF_CONFIGURE_APR()
dnl
dnl  Check configure options and assign variables related to
dnl  the Apache Portable Runtime (APR) library.
dnl

AC_DEFUN([AMQ_CONFIGURE_ACTIVEMQCPP],
[
  AC_MSG_NOTICE([ActiveMQ-CPP library configuration])
  AC_FIND_ACTIVEMQCPP([], [], 1, [3])

  if test $amqcpp_found = "no"; then
    AC_MSG_WARN([ActiveMQ-CPP not found])
    AC_DOWNLOAD_ACTIVEMQCPP
  fi

  AMQCPP_VER_REGEXES=["3\.[3-9]\.[0-9] 3\.[3-9]\.1[0-9] "]

  dnl check ActiveMQ-CPP version number against regex

  AC_MSG_CHECKING([ActiveMQ-CPP version])
  amqcpp_version="`$activemqcpp_config --version`"
  if test $? -ne 0; then
    AC_MSG_ERROR([activemqcpp_config --version failed])
  fi
  AC_MSG_RESULT([$amqcpp_version])

  AMQCPP_WANTED_REGEX_MATCH=0
  for amqcpp_wanted_regex in $AMQCPP_VER_REGEXES; do
    if test `expr $amqcpp_version : $amqcpp_wanted_regex` -ne 0; then
      AMQCPP_WANTED_REGEX_MATCH=1
      break
    fi
  done

  if test $AMQCPP_WANTED_REGEX_MATCH -eq 0; then
    echo "wanted regexes are $AMQCPP_VER_REGEXES"
    AC_MSG_ERROR([invalid activemq-cpp version found, check the README for supported versions.])
  fi

  dnl Get build information from ActiveMQ-CPP

  AMQCPP_CPPFLAGS="`$activemqcpp_config --cflags`"
  if test $? -ne 0; then
    AC_MSG_ERROR([activemqcpp-config --cflags failed])
  fi

  AMQCPP_INCLUDES="`$activemqcpp_config --includes`"
  if test $? -ne 0; then
    AC_MSG_ERROR([activemqcpp-config --includes failed])
  fi

  AMQCPP_LIBS="`$activemqcpp_config --libs`"
  if test $? -ne 0; then
    AC_MSG_ERROR([activemqcpp-config --libs failed])
  fi

  AC_SUBST([AMQCPP_LIBS])
  AC_SUBST([AMQCPP_CPPFLAGS])
  AC_SUBST([AMQCPP_INCLUDES])

])

dnl DECAF_DOWNLOAD_APR()
dnl no apr found, print out a message telling the user what to do
AC_DEFUN([AC_DOWNLOAD_ACTIVEMQCPP],
[
  echo "The Apache ActiveMQ-CPP library cannot be found."
  echo "Please install ActiveMQ-CPP on this system and supply the appropriate"
  echo "--with-activemqcpp option to 'configure'"

  AC_MSG_ERROR([no suitable ActiveMQ-CPP found])
])
