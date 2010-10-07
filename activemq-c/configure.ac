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

AC_PREREQ(2.61)

## -----------------------------------------------------------------------------
## Initialization macros.
## -----------------------------------------------------------------------------
AC_INIT(activemq-c, 1, dev@activemq.apache.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/main/c/cms.cpp)
AC_CONFIG_MACRO_DIR([m4])

## -----------------------------------------------------------------------------
## Define the Version variables
## -----------------------------------------------------------------------------
ACTIVEMQ_LIBRARY_NAME=activemq-c
ACTIVEMQ_MAJOR_VERSION=3
ACTIVEMQ_MINOR_VERSION=3
ACTIVEMQ_PATCH_VERSION=0
ACTIVEMQ_VERSION=${ACTIVEMQ_MAJOR_VERSION}.${ACTIVEMQ_MINOR_VERSION}.${ACTIVEMQ_PATCH_VERSION}
ACTIVEMQ_API_VERSION=${ACTIVEMQ_VERSION}

##
## When releasing a new version the ACTIVEMQ_LIBRARY_VERSION needs to be updated.
## The following rules apply to the numbers in the version.
##
## MAJOR: a change in the MAJOR number is necessary for anything that changes
##  binary compatibility (e.g. changing a function prototype or class definition),
##  or when major internal changes occur
## MINOR: a change in the MINOR number is necessary whenever new functionality is
##  added without breaking binary compatibility (e.g. adding a new class), or when
##  anything other than very minor internal changes occur
## REVISION: a change in the REVISION number is made for any other type of change
##  that does not justify a change to MINOR or MAJOR
##
ACTIVEMQ_LIBRARY_VERSION=1:0:0

AC_SUBST(ACTIVEMQ_LIBRARY_NAME)
AC_SUBST(ACTIVEMQ_VERSION)
AC_SUBST(ACTIVEMQ_MAJOR_VERSION)
AC_SUBST(ACTIVEMQ_MINOR_VERSION)
AC_SUBST(ACTIVEMQ_PATCH_VERSION)
AC_SUBST(ACTIVEMQ_LIBRARY_VERSION)
AC_SUBST(ACTIVEMQ_API_VERSION)

PACKAGE=$ACTIVEMQ_LIBRARY_NAME
VERSION=$ACTIVEMQ_VERSION

AM_INIT_AUTOMAKE([subdir-objects])
AM_CONFIG_HEADER(config.h)

# Gives us access to the host_os environment variable
AC_CANONICAL_HOST

echo "Configuring ActiveMQ-C library"
echo "   Platform: $host_os"
echo "   CPU: $host_cpu"
echo "   Vendor: $host_vendor"

## -----------------------------------------------------------------------------
## Checks for programs.
## -----------------------------------------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS])

AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

AC_FUNC_MEMCMP
AC_FUNC_VPRINTF
AC_HEADER_STDC
AC_C_CONST
AC_TYPE_SIZE_T
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

AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([sys/time.h])

AMQ_FIND_CPPUNIT( 1.10.2, cppunit=yes, cppunit=no;
    AC_MSG_RESULT([no. Unit and Integration tests disabled])
)
AM_CONDITIONAL(BUILD_CPPUNIT_TESTS, test x$cppunit = xyes)

# Absolute source/build directory
decaf_srcdir=`(cd $srcdir && pwd)`
decaf_builddir=`pwd`
AC_SUBST(decaf_srcdir)
AC_SUBST(decaf_builddir)

if test "$decaf_builddir" != "$decaf_srcdir"; then
  USE_VPATH=1
  DECAF_CONFIG_LOCATION=build
else
  DECAF_CONFIG_LOCATION=source
fi

AC_SUBST(DECAF_CONFIG_LOCATION)

PLAT_CXXFLAGS=
PLAT_LIBS=

case "${host_os}" in

  *darwin* ) ## Mac OS X configuration

    ;;

  *solaris* ) ## Solaris configuration

    PLAT_LIBS="-lm -luuid -lsocket -lrt"

    if ! test "$GCC" = "yes"; then
       # Assuming Sun Compiler and adding in optimization
       # because autoconf can't seem to figure it out.
       PLAT_CXXFLAGS="-mt -w -O5 -library=stlport4"
    fi
    ;;

  *hpux10* )

    if ! test "$GCC" = "yes"; then
       # Assuming aCC Compiler unknown yet if any additional settings are needed.
       PLAT_CXXFLAGS=""
    fi
    ;;

  *hpux11* )

    if ! test "$GCC" = "yes"; then
       # Assuming aCC Compiler unknown yet if any additional settings are needed.
       PLAT_CXXFLAGS="-Wc,-ansi_for_scope,on"
    fi
    ;;

  *linux*) ## Unix configuration

    PLAT_LIBS="-lm -luuid"
    ;;

esac

if test "$GCC" = "yes"; then
   PLAT_CXXFLAGS="-ansi -pedantic"
fi

## Find and configure the cflags and libs needed to build against the
## ActiveMQ-CPP library.
AMQ_CONFIGURE_ACTIVEMQCPP

## find and configure the Apache Decaf Library
## DECAF_CONFIGURE_DECAF - Future use for decaf as a standalone lib
## Flags for building the decaf cpp library
## find and configure the APR, and APR Utils
DECAF_CONFIGURE_APR
DECAF_CXXFLAGS="$APR_CPPFLAGS $APR_INCLUDES $APU_INCLUDES"
DECAF_LIBS="$APR_LIBS $APR_LDFLAGS $APU_LIBS $APU_LDFLAGS"

AC_SUBST([DECAF_CXXFLAGS])
AC_SUBST([DECAF_LIBS])

## Flags for building the test applications.
DECAF_TEST_CXXFLAGS="$DECAF_CXXFLAGS $APR_CPPFLAGS $APR_INCLUDES $APU_INCLUDES"
AC_SUBST([DECAF_TEST_CXXFLAGS])

## Flags for building the activemq-c library
AC_SUBST([AMQ_CXXFLAGS])
AC_SUBST([AMQ_CFLAGS])
AC_SUBST([AMQ_LIBS])

if test "$GCC" = "yes"; then
   AMQ_CXXFLAGS="$PLAT_CXXFLAGS $AMQCPP_INCLUDES $AMQCPP_CPPFLAGS $DECAF_INCLUDES $DECAF_CXXFLAGS -W -Wall -Wextra -Wconversion -fPIC -fstrict-aliasing -Wstrict-aliasing=2 -Wno-long-long"
else
   AMQ_CXXFLAGS="$PLAT_CXXFLAGS $AMQCPP_INCLUDES $AMQCPP_CPPFLAGS $DECAF_INCLUDES $DECAF_CXXFLAGS -PIC"
fi

AMQ_LIBS="$PLAT_LIBS $AMQCPP_LIBS $DECAF_LIBS"

if test "$GCC" = "yes"; then
   AMQ_TEST_CXXFLAGS="$AMQ_CXXFLAGS $AMQCPP_INCLUDES $AMQCPP_CPPFLAGS $DECAF_INCLUDES $DECAF_CXXFLAGS -Wno-non-virtual-dtor -Wno-unused-parameter -Wno-uninitialized"
else
   AMQ_TEST_CXXFLAGS="$AMQ_CXXFLAGS $AMQCPP_INCLUDES $AMQCPP_CPPFLAGS $DECAF_INCLUDES $DECAF_CXXFLAGS"
fi

AMQ_TEST_CFLAGS="$AMQ_CXXFLAGS $AMQCPP_INCLUDES $AMQCPP_CPPFLAGS $DECAF_INCLUDES $DECAF_CXXFLAGS"
AMQ_TEST_LIBS="../main/c/libactivemq-c.la $AMQCPP_LIBS $DECAF_LIBS"

## Flags for building the test applications.
AC_SUBST([AMQ_TEST_CXXFLAGS])
AC_SUBST([AMQ_TEST_LIBS])

# Check to see if the compiler supports the visibility flags.
# If so, define HAVE_VISIBILITY_OPTIONS
CHECK_VISIBILITY()

## Not all platforms define addrinfo and related functions.  If this platform
## supports addrinfo, defines HAVE_STRUCT_ADDRINFO.
CHECK_ADDRINFO()

## Execute Doxygen macros
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(activemq-c, doxygen.cfg, doc)

## -----------------------------------------------------------------------------
## configuration
## Generates Makefile's, configuration files and scripts
## -----------------------------------------------------------------------------

AC_OUTPUT_COMMANDS([chmod +x activemqc-config],[])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(activemq-c.pc)
AC_CONFIG_FILES(src/main/c/Makefile)
AC_CONFIG_FILES(src/examples/Makefile)
AC_CONFIG_FILES(activemqc-config)

if test x$cppunit = xyes
then
  AC_CONFIG_FILES(src/test/c/Makefile)
fi

AC_OUTPUT
