# ---------------------------------------------------------------------------
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ---------------------------------------------------------------------------

## --------------------------------
## Initialization macros.
## --------------------------------
AC_INIT(decaf, 1, activemq-dev@geronimo.apache.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/main/decaf/lang/Thread.cpp)

## -----------------------------------------------
## Define the Version variables
## -----------------------------------------------
DECAF_LIBRARY_NAME=decaf
DECAF_VERSION=0.1-SNAPSHOT
DECAF_LIBRARY_VERSION=0:0:0
DECAF_RELEASE=0.0
DECAF_API_VERSION=${DECAF_VERSION}

AC_SUBST(DECAF_LIBRARY_NAME)
AC_SUBST(DECAF_VERSION)
AC_SUBST(DECAF_LIBRARY_VERSION)
AC_SUBST(DECAF_RELEASE)
AC_SUBST(DECAF_API_VERSION)

PACKAGE=$DECAF_LIBRARY_NAME
VERSION=$DECAF_VERSION

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)
AM_CONFIG_HEADER(config.h)

# Gives us access to the host_os environment variable
AC_CANONICAL_HOST

## -----------------------------------------------
## Checks for programs.
## -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
## Uncomment to use libtool for shared libs
## AM_PROG_LIBTOOL
AC_PROG_RANLIB
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

AC_C_BIGENDIAN
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)

AC_CHECK_SIZEOF(char, 1)
AC_CHECK_SIZEOF(short, 2)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long long, 8)
AC_CHECK_SIZEOF(float, 4)
AC_CHECK_SIZEOF(double, 8)

AC_CHECK_HEADERS([uuid.h uuid/uuid.h], [AC_DEFINE([HAVE_UUID_T], [1], [Define if uuid_t exists.])])
# AC_CHECK_HEADERS([uuid/uuid.h])
AC_CHECK_HEADERS([objbase.h])
AC_CHECK_HEADERS([repcdce.h])
AC_CHECK_HEADERS([sys/filio.h])
AC_CHECK_HEADERS([sys/ioctl.h])
AC_CHECK_HEADERS([sys/select.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/timeb.h])
AC_CHECK_HEADERS([pthread.h])

AC_CHECK_FUNCS([ioctl select gettimeofday time ftime])

AM_PATH_CPPUNIT(1.10.2, cppunit=yes, cppunit=no; AC_MSG_RESULT([no. Unit and Integration tests disabled]))
AM_CONDITIONAL(BUILD_CPPUNIT_TESTS, test x$cppunit = xyes)

CXXFLAGS="$CXXFLAGS -W -Wall -fstrict-aliasing -Wstrict-aliasing=2 -Wno-long-long"
# CPPUNIT_CXXFLAGS="$CPPUNIT_CXXFLAGS -Wno-non-virtual-dtor -Wno-unused-parameter -Wno-uninitialized"
LIBS="$LIBS"

case "${host_os}" in

  *darwin* ) ## Mac OS X configuration
    CXXFLAGS="-ansi -pedantic $CXXFLAGS"
    LIBS="$LIBS"
    ;;

  *cygwin* ) ## Cygwin configuration
    # LIBS="-lwinmm -lm";
    CXXFLAGS="$CXXFLAGS -Wno-uninitialized"
    LIBS="$LIBS -lm -lpthread -luuid -lrpcrt4"
    ;;

  *solaris* ) ## Solaris configuration
    AC_CHECK_LIB(pthread, pthread_create,[have_pthread="yes"],
                AC_MSG_ERROR([libpthread not found!]))

    CXXFLAGS="-ansi -pedantic  $CXXFLAGS"
    LIBS="$LIBS -lm -lpthread -luuid -lsocket -lrt"
    ;;

  *) ## Unix configuration

    AC_CHECK_LIB(pthread, pthread_create,[have_pthread="yes"],
                AC_MSG_ERROR([libpthread not found!]))

    CXXFLAGS="-ansi -pedantic  $CXXFLAGS"
    LIBS="$LIBS -lm -lpthread -luuid"
esac

##
## Not all platforms define addrinfo and related functions
##
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

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(decaf, doxygen.cfg, doc)


## -----------------------------------------------------
## configuration
## Generates Makefile's, configuration files and scripts
## -----------------------------------------------------

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(decaf.pc)
AC_CONFIG_FILES(src/main/Makefile)

if test x$cppunit = xyes
then
  AC_CONFIG_FILES(src/test/Makefile)
fi

AC_OUTPUT
