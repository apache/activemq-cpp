#!/bin/sh
# ------------------------------------------------------------------------
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
# ------------------------------------------------------------------------

# default settings
runautogen=true
runconfigure=true
maketarget=all

# load the user supplied build configuration file if present.
if [ -f "./build.conf" ] ; then
  . ./build.conf
fi

# No Configure script means we have to run autogen, which in turn means
# we need to run a new Configure on the project.
if [ -x "./configure" ] ; then

    # even if we have a configure script, if the template has been updated we should
    # run it again to account for the changes, which also means we need to run a autogen
    # to create the actual script and associated autoconf artifacts.
    if [ "./configure.ac" -ot "./configure" ] ; then
        echo "No need to run autogen.sh or configure"
        runautgen=false
        runconfigure=false
    else
        echo "We need to run autogen.sh and configure"
        runautgen=true
        runconfigure=true
    fi

fi

run_Configure {
    autogen.sh
    pushd build
    ../configure
    exit
}

run_Clean() {
    pushd build
    make clean
    exit
}

run_Make() {
    pushd build
    make
    exit
}

run_Check() {
    pushd build
    make check
    exit
}

run_Doxygen() {
    pushd build
    make doxygen-run
    exit
}

case "$1" in
configure)
    run_Configure
    ;;
all)
    run_Make
    ;;
check)
    run_Check
    ;;
clean)
    run_Clean
    ;;
doxygen)
    run_Doxygen
    ;;
*)
    echo "Usage: $0 {clean}"
    exit 1
esac

exit 0

