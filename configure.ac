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

## -----------------------------------------------
## Checks for programs.
## -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

## -----------------------------------------------
## Generates Makefile's, configuration files and scripts
## -----------------------------------------------

AC_OUTPUT(\
   Makefile \
   activemq-cpp.pc \
   src/main/Makefile \
   src/examples/Makefile \
)

