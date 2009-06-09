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

# AMQ_EXTERNAL_PROJECT_SETUP()
# Internal helper for AMQ_EXTERNAL_PROJECT.
AC_DEFUN([AMQ_EXTERNAL_PROJECT_SETUP], [
  do_subdir_config="yes"
  AC_ARG_ENABLE([subdir-config],
    AC_HELP_STRING([--disable-subdir-config],
                   [do not reconfigure packages in subdirectories]),
    [if test "$enableval" = "no"; then do_subdir_config="no"; fi])
  AC_SUBST([AMQ_EXTERNAL_PROJECT_SUBDIRS], [""])
])

# AMQ_EXTERNAL_PROJECT(SUBDIR [, ADDITIONAL-CONFIGURE-ARGS])
# Setup SUBDIR as an external project. This means:
# - Execute the configure script immediately at the point of macro invocation.
# - Add SUBDIR to the substitution variable AMQ_EXTERNAL_PROJECT_SUBDIRS,
#   for the Makefile.in to arrange to execute make in the subdir.
#
# Derived from APR_SUBDIR_CONFIG
AC_DEFUN([AMQ_EXTERNAL_PROJECT], [
  AC_REQUIRE([AMQ_EXTERNAL_PROJECT_SETUP])
  AMQ_EXTERNAL_PROJECT_SUBDIRS="$AMQ_EXTERNAL_PROJECT_SUBDIRS $1"
  if test "$do_subdir_config" = "yes" ; then
    # save our work to this point; this allows the sub-package to use it
    AC_CACHE_SAVE

    AC_MSG_NOTICE([configuring package in $1 now])
    ac_popdir=`pwd`
    ac_abs_srcdir=`(cd $srcdir/$1 && pwd)`
    apr_config_subdirs="$1"
    AS_MKDIR_P( $1 )
    cd $1

    # A "../" for each directory in /$config_subdirs.
    ac_dots=[`echo $apr_config_subdirs|sed -e 's%^\./%%' -e 's%[^/]$%&/%' -e 's%[^/]*/%../%g'`]

    # Make the cache file name correct relative to the subdirectory.
    case "$cache_file" in
    /*) ac_sub_cache_file=$cache_file ;;
    *) # Relative path.
      ac_sub_cache_file="$ac_dots$cache_file" ;;
    esac

    # The eval makes quoting arguments work.
    if eval $SHELL $ac_abs_srcdir/configure $ac_configure_args --cache-file=$ac_sub_cache_file --srcdir=$ac_abs_srcdir $2
    then :
      echo "$1 configured properly"
    else
      echo "configure failed for $1"
      exit 1
    fi
    cd $ac_popdir

    # grab any updates from the sub-package
    AC_CACHE_LOAD
  else
    AC_MSG_WARN([not running configure in $1])
  fi
])
