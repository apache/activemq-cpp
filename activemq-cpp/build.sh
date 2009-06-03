#!/bin/sh -e
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

# load the user supplied build configuration file if present.
if [ -f "./build.conf" ] ; then
  . ./build.conf
fi

# load user activemq-cpp build configuration
if [ -f "$HOME/.activemqcpprc" ] ; then
  . "$HOME/.activemqcpprc"
fi

# This is where we run all the build steps, configure, make, etc.
if [ -n builddir ] ; then
   builddir="./build"
fi

check_Configure() {

    # No Configure script means we have to run autogen, which in turn means
    # we need to run a new Configure on the project.
    if [ -x "./configure" ] ; then

        # even if we have a configure script, if the template has been updated we should
        # run it again to account for the changes, which also means we need to run a autogen
        # to create the actual script and associated autoconf artifacts.
        if [ "./configure.ac" -ot "./configure" ] && [ -d $builddir ] ; then
            runconfigure=false
        else
            runconfigure=true
        fi

    fi

    if [ "true" = $runconfigure ] ; then
        run_Configure
    fi

}

run_Configure() {
    ./autogen.sh

    if ! [ -d $builddir ] ; then
        mkdir -p $builddir
    fi

    pushd $builddir
    ../configure
    popd
    exit
}

run_Clean() {
    rm -rf $builddir
    exit
}

run_Make() {
    check_Configure
    pushd $builddir
    make
    popd
    exit
}

run_Check() {
    check_Configure
    pushd $builddir
    make check
    popd
    exit
}

run_Doxygen() {
    check_Configure
    pushd $builddir
    make doxygen-run
    popd
    exit
}

case "$1" in
configure)
    run_Configure
    ;;
compile)
    run_Make
    ;;
test)
    run_Check
    ;;
clean)
    run_Clean
    ;;
doxygen)
    run_Doxygen
    ;;
*)
    echo "Usage: $0 {configure,compile,clean,test,doxygen}"
    exit 1
esac

exit 0

