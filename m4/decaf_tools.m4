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
dnl  dependance on the Apache Decaf Library.
dnl
dnl  DECAF_CONFIGURE_DECAF()
dnl
dnl  Check configure options and assign variables related to
dnl  the Apache Decaf Library.
dnl

AC_DEFUN([DECAF_CONFIGURE_DECAF],
[
  AC_MSG_NOTICE([Apache Decaf Library configuration])
  DECAF_FIND_DECAF([$srcdir/decaf], [./decaf], 1, [0 1])

  if test $decaf_found = "no"; then
    AC_MSG_WARN([Decaf not found])
    DECAF_DOWNLOAD_DECAF
  fi

  if test $decaf_found = "reconfig"; then
    AMQ_EXTERNAL_PROJECT([decaf])
  fi

  dnl Get build information from Decaf

  DECAF_CPPFLAGS="`$decaf_config --cppflags`"
  if test $? -ne 0; then
    AC_MSG_ERROR([decaf-config --cppflags failed])
  fi

  DECAF_INCLUDES="`$decaf_config --includes`"
  if test $? -ne 0; then
    AC_MSG_ERROR([decaf-config --includes failed])
  fi

  DECAF_LIBS="`$decaf_config --libs`"
  if test $? -ne 0; then
    AC_MSG_ERROR([decaf-config --libs failed])
  fi

  AC_SUBST([DECAF_LIBS])
  AC_SUBST([DECAF_INCLUDES])
  AC_SUBST([DECAF_CPPFLAGS])
])

dnl DECAF_DOWNLOAD_DECAF()
dnl no decaf found, print out a message telling the user what to do
AC_DEFUN([DECAF_DOWNLOAD_DECAF],
[
  echo "The Apache Decaf library cannot be found."
  echo "Please install Decaf on this system and supply the appropriate"
  echo "--with-decaf option to 'configure'"

  AC_MSG_ERROR([no suitable Decaf Install found])
])
