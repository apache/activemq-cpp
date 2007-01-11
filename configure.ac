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
AC_INIT(activemq-cpp, 1, activemq-dev@geronimo.apache.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/main/activemq/core/ActiveMQConnection.cpp)

## -----------------------------------------------
## Define the Version variables
## -----------------------------------------------
ACTIVEMQ_LIBRARY_NAME=activemq-cpp
ACTIVEMQ_VERSION=1.x-incubator-SNAPSHOT
ACTIVEMQ_LIBRARY_VERSION=0:0:0
ACTIVEMQ_RELEASE=0.0
ACTIVEMQ_API_VERSION=${ACTIVEMQ_VERSION}

AC_SUBST(ACTIVEMQ_LIBRARY_NAME)
AC_SUBST(ACTIVEMQ_VERSION)
AC_SUBST(ACTIVEMQ_LIBRARY_VERSION)
AC_SUBST(ACTIVEMQ_RELEASE)
AC_SUBST(ACTIVEMQ_API_VERSION)

PACKAGE=$ACTIVEMQ_LIBRARY_NAME
VERSION=$ACTIVEMQ_VERSION

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

AC_CHECK_HEADERS([uuid.h])
AC_CHECK_HEADERS([uuid/uuid.h])
AC_CHECK_HEADERS([objbase.h])
AC_CHECK_HEADERS([repcdce.h])
AC_CHECK_HEADERS([sys/filio.h])
AC_CHECK_HEADERS([sys/ioctl.h])
AC_CHECK_HEADERS([sys/select.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/timeb.h])

AC_CHECK_FUNCS([ioctl select gettimeofday time ftime])

AM_PATH_CPPUNIT(1.10.2, cppunit=yes, cppunit=no; AC_MSG_RESULT([no. Unit and Integration tests disabled])) 
AM_CONDITIONAL(BUILD_CPPUNIT_TESTS, test x$cppunit = xyes)

case "${host_os}" in

  *darwin* ) ## Mac OS X configuration
    CXXFLAGS="$CXXFLAGS -fstrict-aliasing -Wstrict-aliasing" 
    LIBS=""
    ;;

  *cygwin* ) ## Cygwin configuration
    # LIBS="-lwinmm -lm";
    CXXFLAGS="$CXXFLAGS -fstrict-aliasing -Wstrict-aliasing" 
    LIBS="$LIBS -lm -lpthread -luuid -lrpcrt4"
    ;;
    
  *solaris* ) ## Solaris configuration
    AC_CHECK_LIB(pthread, pthread_create,[have_pthread="yes"],
                AC_MSG_ERROR([libpthread not found!]))

    CXXFLAGS="$CXXFLAGS -fstrict-aliasing -Wstrict-aliasing"
    LIBS="$LIBS -lm -lpthread -luuid -lsocket -lrt"
    ;;
    
  *) ## Unix configuration

    AC_CHECK_LIB(pthread, pthread_create,[have_pthread="yes"],
                AC_MSG_ERROR([libpthread not found!]))

    CXXFLAGS="$CXXFLAGS -fstrict-aliasing -Wstrict-aliasing" 
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
DX_INIT_DOXYGEN(activemq-cpp, doxygen.cfg, doc)


## -----------------------------------------------------
## configuration
## Generates Makefile's, configuration files and scripts
## -----------------------------------------------------

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(activemq-cpp.pc)
AC_CONFIG_FILES(src/main/Makefile)
AC_CONFIG_FILES(src/examples/Makefile)

if test x$cppunit = xyes
then
  AC_CONFIG_FILES(src/test/Makefile)
  AC_CONFIG_FILES(src/test-integration/Makefile)
fi

AC_OUTPUT
