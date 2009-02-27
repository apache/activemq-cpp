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

AC_DEFUN([DECAF_CONFIGURE_APR],
[
  AC_MSG_NOTICE([Apache Portable Runtime (APR) library configuration])
  APR_FIND_APR([], [], 1, [1])
  APR_FIND_APU([], [], 1, [1])

  if test $apr_found = "no"; then
    AC_MSG_WARN([APR not found])
    DECAF_DOWNLOAD_APR
  fi

  if test $apu_found = "no"; then
    AC_MSG_WARN([APR Utils not found])
    DECAF_DOWNLOAD_APU
  fi

  APR_VER_REGEXES=["1\.3\.[0-9] 1\.3\.1[0-9] "]
  APU_VER_REGEXES=["1\.3\.[0-9] 1\.3\.1[0-9] "]

  dnl check APR version number against regex

  AC_MSG_CHECKING([APR version])
  apr_version="`$apr_config --version`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apr-config --version failed])
  fi
  AC_MSG_RESULT([$apr_version])

  APR_WANTED_REGEX_MATCH=0
  for apr_wanted_regex in $APR_VER_REGEXES; do
    if test `expr $apr_version : $apr_wanted_regex` -ne 0; then
      APR_WANTED_REGEX_MATCH=1
      break
    fi
  done

  if test $APR_WANTED_REGEX_MATCH -eq 0; then
    echo "wanted regexes are $APR_VER_REGEXES"
    AC_MSG_ERROR([invalid apr version found, check the README for supported versions.])
  fi

  dnl check APU version number against regex

  AC_MSG_CHECKING([APU version])
  apu_version="`$apu_config --version`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apu-config --version failed])
  fi
  AC_MSG_RESULT([$apu_version])

  APU_WANTED_REGEX_MATCH=0
  for apu_wanted_regex in $APU_VER_REGEXES; do
    if test `expr $apr_version : $apu_wanted_regex` -ne 0; then
      APU_WANTED_REGEX_MATCH=1
      break
    fi
  done

  if test $APU_WANTED_REGEX_MATCH -eq 0; then
    echo "wanted regexes are $APU_VER_REGEXES"
    AC_MSG_ERROR([invalid apr-util version found, check the README for supported versions.])
  fi

  dnl Get build information from APR

  APR_CPPFLAGS="`$apr_config --cppflags`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apr-config --cppflags failed])
  fi

  APR_INCLUDES="`$apr_config --includes`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apr-config --includes failed])
  fi
  APU_INCLUDES="`$apu_config --includes`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apu-config --includes failed])
  fi

  APR_LDFLAGS="`$apr_config --ldflags`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apr-config --ldflags failed])
  fi
  APU_LDFLAGS="`$apu_config --ldflags`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apu-config --ldflags failed])
  fi

  APR_LIBS="`$apr_config --link-libtool --libs`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apr-config --link-libtool --libs failed])
  fi
  APU_LIBS="`$apu_config --link-libtool --libs`"
  if test $? -ne 0; then
    AC_MSG_ERROR([apu-config --link-libtool --libs failed])
  fi

  AC_SUBST([APR_LIBS])
  AC_SUBST([APR_LDFLAGS])
  AC_SUBST([APR_CPPFLAGS])
  AC_SUBST([APR_INCLUDES])
  AC_SUBST([APU_LIBS])
  AC_SUBST([APU_LDFLAGS])
  AC_SUBST([APU_INCLUDES])

])

dnl DECAF_DOWNLOAD_APR()
dnl no apr found, print out a message telling the user what to do
AC_DEFUN([DECAF_DOWNLOAD_APR],
[
  echo "The Apache Portable Runtime (APR) library cannot be found."
  echo "Please install APR on this system and supply the appropriate"
  echo "--with-apr option to 'configure'"

  AC_MSG_ERROR([no suitable APR found])
])

dnl DECAF_DOWNLOAD_APU()
dnl no apr-utils found, print out a message telling the user what to do
AC_DEFUN([DECAF_DOWNLOAD_APU],
[
  echo "The Apache Portable Runtime (APR) Utils library cannot be found."
  echo "Please install APR Utils on this system and supply the appropriate"
  echo "--with-apr option to 'configure'"

  AC_MSG_ERROR([no suitable APR Utils found])
])
