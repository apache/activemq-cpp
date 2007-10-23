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
AC_INIT(cms, 1, dev@activemq.apache.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/main/cms/BytesMessage.h)

## -----------------------------------------------
## Define the Version variables
## -----------------------------------------------
CMS_LIBRARY_NAME=cms
CMS_VERSION=1.2
CMS_LIBRARY_VERSION=0:0:0
CMS_RELEASE=0.0
CMS_API_VERSION=${CMS_VERSION}

AC_SUBST(CMS_LIBRARY_NAME)
AC_SUBST(CMS_VERSION)
AC_SUBST(CMS_LIBRARY_VERSION)
AC_SUBST(CMS_RELEASE)
AC_SUBST(CMS_API_VERSION)

PACKAGE=$CMS_LIBRARY_NAME
VERSION=$CMS_VERSION

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)
AM_CONFIG_HEADER(config.h)

## -----------------------------------------------
## Checks for programs.
## -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
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

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(cms, doxygen.cfg, doc)


## -----------------------------------------------------
## configuration
## Generates Makefile's, configuration files and scripts
## -----------------------------------------------------

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(cms.pc)
AC_CONFIG_FILES(src/main/Makefile)

AC_OUTPUT
