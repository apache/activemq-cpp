--------------------------------------------------------------------------
ActiveMQ CPP Library
--------------------------------------------------------------------------

ActiveMQ CPP is a messaging library that can use multiple protocols to
talk to a MOM (e.g. ActiveMQ).


1 Dependencies
--------------------------------------------------------------------------

1.1 libuuid
--------------------------------------------------------------------------

The build requires the *libuuid* library that is part of the e2fsprogs 
package and is available from http://e2fsprogs.sourceforge.net/ which is 
not always installed by default.

1.2 cppunit
--------------------------------------------------------------------------

The package contains a complete set of cppunit tests.  In order for you to 
build an run the tests, you will need to download and install the cppunit 
suite.  See http://cppunit.sourceforge.net/cppunit-wiki

or on Fedora type the following:

  sudo yum install cppunit

Make sure that the paths to the installed cppunit library and includes are 
visible in your current shell before you try building the tests.

Windows users will need to build the cppunit library using the CPPUnit 
MSVC project files. A discussion of the build process can be found 
on the CPPUnit wiki under 
http://cppunit.sourceforge.net/cppunit-wiki/BuildingCppUnit1 this covers 
both MSVC along with many other platforms and tool suites.


1.3 GNU Build System (for building on \*nix)
--------------------------------------------------------------------------

To Generate the ./configure script use to create the Makefiles, you need 
the following software installed:

Tool        Recommended Version
-------------------------------
autoconf    >= 2.59
automake    >= 1.9.6
libtool     >= 1.5.22

2 Building on \*nix (Unix/Linux/OS X/Cygwin)
--------------------------------------------------------------------------

This assumes you have all of the project dependencies installed.  We're 
now ready to create the configure script.  To do this, run:

  ./autogen.sh

    -----------------------------------------------------------------------
    |NOTE: You may see the following warnings when running this command:  |
    |                                                                     |
    |  src/test-integration/Makefile.am:44: `CXXFLAGS' is a user variable,| 
    |you should not override it;                                          |
    |  src/test-integration/Makefile.am:44: use `AM_CXXFLAGS' instead.    |
    |  src/test/Makefile.am:104: `CXXFLAGS' is a user variable, you       |
    |should not override it;                                              |
    |  src/test/Makefile.am:104: use `AM_CXXFLAGS' instead.               |
    |                                                                     |
    |These can be ignored.  We override CXXFLAGS in the makefiles for     |
    |the unit and integration tests in order to suppress compiler         |
    |warnings.                                                            |
    -----------------------------------------------------------------------

This should be run the first time and anytime you change configure.ac or 
any of the Makefile.am files.

    -----------------------------------------------------------------------
    |Solaris 10 Note:  CPP_UNIT might not build until you correct the file| 
    |  libstdc++.la to contain the correct data, see this discussion:     |
    |  http://forum.sun.com/jive/thread.jspa?threadID=73150               |
    -----------------------------------------------------------------------
    
The configure script will customize the way the software is built and 
installed into your system along with detecting the available libraries 
that have been installed.  To use the default configuration just run:

  ./configure

For more help on how to customize the build configuration, run:

  ./configure --help

Once the configure script has run successfully, you are ready to build. 
Run:

  make
  
This will build all of the core ActiveMQ CPP source code.  To build and 
install the code into the system directories, run:

  make install

You will have to become the superuser in order to be able to install the 
files.

3 Doxygen
--------------------------------------------------------------------------

To generate the doxygen documentation for the project, just run:

  make doxygen-run


4 Running Tests
--------------------------------------------------------------------------

4.1 Unit Tests
--------------------------------------------------------------------------
In order to build and run the suite of unit tests, run:

  make check

This will verify that the library is functioning correctly on the target 
platform. In addition, it will generate the integration tests binary.

4.2 Integration Tests
--------------------------------------------------------------------------
The library also contains a set of tests that are run against a real AMQ 
broker.  These allow you to validate this distribution of ActiveMQ CPP 
against your broker.  Running these without a broker will result in failed 
tests.  The tests currently hard-code the broker url to be 
tcp://localhost:61613 for stomp and tcp://localhost:61616 for openwire.

The integration tests are built via "make check".  To run them, first 
start a broker and then

  cd src/test-integration
  ./activemq-test-integration

This will take quite some time to complete, so be patient.

5 Example
--------------------------------------------------------------------------
There is an example application that ships with the distribution in 
src/examples.   The example is compiled by default with the "make" 
command, but can easily be compiled manually using the command:

  g++ -o main -pthread -I ../main main.cpp ../../out/libactivemq-cpp-0_0_2.a -luuid

6 Notes for Windows users
--------------------------------------------------------------------------
We support using the GNU compiler on Windows, using the Cygwin package.  
However we also support using the MSVC compiler on Windows.

There are a couple or things that you will need to setup to ensure that 
the MSVC compile succeeds.

* You need to download and install the Platform SDK if you don't have it 
  installed already.
  
* Ensure that the path to you MSVC install is set in the PATH env variable.   
  You can test this by typing cl.exe at the command line, if you get an 
  error complaining that its not found, then you'll need to fix your PATH.
* Set the INCLUDE env variable to include the path to your MSVC includes, 
  and the platform SDK includes. For example:

    INCLUDE = D:\Program Files\Microsoft Visual Studio 8\VC\include;D:\Program Files\Microsoft Platform SDK\Include\*

* Set the LIB env variable to include the path to your MSVC libs, and the 
  Platform SDK libs. For example:

    LIB = D:\Program Files\Microsoft Visual Studio 8\VC\lib;D:\Program Files\Microsoft Platform SDK\Lib

* The Project files reference the CPPUnit libraries for the Integration and 
  Unit tests builds. In order for these to build correctly you must 
  either place the CPPUnit libraries in a directory listed in the project 
  settings, or add a new location for your install of CPPUnit.

