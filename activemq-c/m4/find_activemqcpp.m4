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
dnl find_decaf.m4 : locate the decaf include files and libraries
dnl
dnl This macro file can be used by applications to find and use the Decaf
dnl library. It provides a standardized mechanism for using Decaf. It supports
dnl embedding Decaf into the application source, or locating an installed
dnl copy of Decaf.
dnl
dnl AC_FIND_ACTIVEMQCPP(srcdir, builddir, implicit-install-check, acceptable-majors)
dnl
dnl   where srcdir is the location of the bundled ActiveMQ-CPP source directory, or
dnl   empty if source is not bundled.
dnl
dnl   where builddir is the location where the bundled ActiveMQ-CPP will will be built,
dnl   or empty if the build will occur in the srcdir.
dnl
dnl   where implicit-install-check set to 1 indicates if there is no
dnl   --with-decaf option specified, we will look for installed copies.
dnl
dnl   where acceptable-majors is a space separated list of acceptable major
dnl   version numbers. Often only a single major version will be acceptable.
dnl   If multiple versions are specified, and --with-decaf=PREFIX or the
dnl   implicit installed search are used, then the first (leftmost) version
dnl   in the list that is found will be used.  Currently defaults to [0 1].
dnl
dnl Sets the following variables on exit:
dnl
dnl   amqcpp_found : "yes", "no", "reconfig"
dnl
dnl   activemqcpp_config : If the activemqcpp-config tool exists, this refers to it. If
dnl                        amqcpp_found is "reconfig", then the bundled directory
dnl                        should be reconfigured *before* using activemqcpp_config.
dnl
dnl Note: this macro file assumes that activemqcpp_config has been installed; it
dnl       is normally considered a required part of an ActiveMQ-CPP installation.
dnl
dnl If a bundled source directory is available and needs to be (re)configured,
dnl then amqcpp_found is set to "reconfig". The caller should reconfigure the
dnl (passed-in) source directory, placing the result in the build directory,
dnl as appropriate.
dnl
dnl If amqcpp_found is "yes" or "reconfig", then the caller should use the
dnl value of activemqcpp_config to fetch any necessary build/link information.
dnl

AC_DEFUN([AC_FIND_ACTIVEMQCPP], [
  amqcpp_found="no"

  if test "$target_os" = "os2-emx"; then
    # Scripts don't pass test -x on OS/2
    TEST_X="test -f"
  else
    TEST_X="test -x"
  fi

  ifelse([$4], [], [
         ifdef(AC_WARNING,AC_WARNING([$0: missing argument 4 (acceptable-majors): Defaulting to ActiveMQ-CPP 3.x then ActiveMQ-CPP 4.x]))
         acceptable_majors="3 4"],
         [acceptable_majors="$4"])

  ac_temp_acceptable_amqcpp_config="activemqcpp-config"

  AC_MSG_CHECKING(for ActiveMQ-CPP)
  AC_ARG_WITH(activemqcpp,
  [  --with-activemqcpp=PATH         prefix for installed ActiveMQ-CPP, path to ActiveMQ-CPP build tree,
                                     or the full path to activemqcpp-config],
  [
    if test "$withval" = "no" || test "$withval" = "yes"; then
      AC_MSG_ERROR([--with-activemqcpp requires a directory or file to be provided])
    fi

    for ac_temp_acceptable_amqcpp_config_file in $ac_temp_acceptable_amqcpp_config
    do
      for lookdir in "$withval/bin" "$withval"
      do
        if $TEST_X "$lookdir/$ac_temp_acceptable_amqcpp_config_file"; then
          amqcpp_found="yes"
          activemqcpp_config="$lookdir/$ac_temp_acceptable_amqcpp_config_file"
          break 2
        fi
      done
    done

    if test "$amqcpp_found" != "yes" && $TEST_X "$withval" && $withval --help > /dev/null 2>&1 ; then
      amqcpp_found="yes"
      activemqcpp_config="$withval"
    fi

    dnl if --with-activemqcpp is used, it is a fatal error for its argument
    dnl to be invalid
    if test "$amqcpp_found" != "yes"; then
      AC_MSG_ERROR([the --with-activemqcpp parameter is incorrect. It must specify an install prefix, a build directory, or an activemqcpp-config file.])
    fi
  ],[
    dnl If we allow installed copies, check those before using bundled copy.
    if test -n "$3" && test "$3" = "1"; then
      for ac_temp_acceptable_amqcpp_config_file in $ac_temp_acceptable_amqcpp_config
      do
        if $ac_temp_acceptable_amqcpp_config_file --help > /dev/null 2>&1 ; then
          amqcpp_found="yes"
          activemqcpp_config="$ac_temp_acceptable_amqcpp_config_file"
          break
        else
          dnl look in some standard places
          for lookdir in /usr /usr/local /usr/local/activemq-cpp /opt/activemq-cpp; do
            if $TEST_X "$lookdir/bin/$ac_temp_acceptable_amqcpp_config_file"; then
              amqcpp_found="yes"
              activemqcpp_config="$lookdir/bin/$ac_temp_acceptable_amqcpp_config_file"
              break 2
            fi
          done
        fi
      done
    fi
    dnl if we have not found anything yet and have bundled source, use that
    if test "$amqcpp_found" = "no" && test -d "$1"; then
      amqcpp_temp_abs_srcdir="`cd $1 && pwd`"
      amqcpp_found="reconfig"
      ac_temp_acceptable_amqcpp_config_file="activemqcpp_config"

      if test -n "$2"; then
        activemqcpp_config="$2/$ac_temp_acceptable_amqcpp_config_file"
      else
        activemqcpp_config="$1/$ac_temp_acceptable_amqcpp_config_file"
      fi
    fi
  ])

  AC_MSG_RESULT($amqcpp_found)
])
