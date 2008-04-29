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

# Since we don't strictly follow the GNU standard of having 'NEWS README AUTHORS ChangeLog' files
AUTOMAKE_OPTIONS = foreign

SUBDIRS = src/main src/examples
if BUILD_CPPUNIT_TESTS
  SUBDIRS += src/test src/test-integration src/test-benchmarks
endif

#Distribute these directories:
DIST_SUBDIRS = src/main 

bin_SCRIPTS = activemqcpp-config 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = activemq-cpp.pc

ACLOCAL_AMFLAGS = -I m4

include doxygen-include.am

EXTRA_DIST=autogen.sh $(DX_CONFIG) doc/html
