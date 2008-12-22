--------------------------------------------------------------------------
ActiveMQ CPP Library
--------------------------------------------------------------------------

ActiveMQ CPP is a messaging library that can use multiple protocols to
talk to a MOM (e.g. ActiveMQ).

1 Dependencies
--------------------------------------------------------------------------

There are several dependencies that need to be met in order to build and
install ActiveMQ-CPP on a Unix type system, the short list is shown below,
read the sections that follow for more detailed information.  On Windows
you will not need the Auto Tools since the library is built using Microsft's
Visual Studio product.

Tool        Recommended Version
-------------------------------
autoconf    >= 2.60
automake    >= 1.10
libtool     >= 1.5.24
APR         >= 1.3*
APR-Util    >= 1.3*
CPPUnit     >= 1.10.2*
libuuid     >= ?*

* Requires that the Development package also be installed.

1.1 libuuid
--------------------------------------------------------------------------

The build requires the *libuuid* library that is part of the e2fsprogs
package and is available from http://e2fsprogs.sourceforge.net/ which is
not always installed by default.

On Fedora, type the following:

  sudo yum install e2fsprogs-devel

On Debian/Ubuntu, type the following:

  sudo apt-get install uuid-dev

1.2 CppUnit
--------------------------------------------------------------------------

The package contains a complete set of CppUnit tests.  In order for you to
build an run the tests, you will need to download and install the CppUnit
suite.  See http://cppunit.sourceforge.net/cppunit-wiki

On Fedora, type the following:

  sudo yum install cppunit cppunit-devel

On Debian/Ubuntu, type the following:

  sudo apt-get install libcppunit-dev

Make sure that the paths to the installed CppUnit library and includes are
visible in your current shell before you try building the tests.

Windows users will need to build the CppUnit library using the CPPUnit
MSVC project files. A discussion of the build process can be found
on the CPPUnit wiki under:

http://cppunit.sourceforge.net/cppunit-wiki/BuildingCppUnit1

This covers both MSVC along with many other platforms and tool suites.
The included Visual Studio projects are configured with the assumption
that you will configure Visual Studio with the locations of the Platform
SDK and the CPPUnit and APR libraries and headers.

1.3 APR and APR Util
-------------------------------------------------------------------------

The build requires the APR library and the APR-Util.  These libraries can
be obtained from the APR site at: http://apr.apache.org/  See the
documentation there on how to build these libraries for your platform.  On
many Unix system the libraries can be installed using the systems package
installer.

The Library has been tested using version v1.3 and higher of the APR libs,
the older 0.9 version will definitely not work with this library.

Many of the Unix type OS'es currently shipping include APR 1.2.x and
APR-Util 1.2.x, this implies that you will need to build and install APR from
the source download at Apache.

1.4 GNU Build System (for building on Unix/Linux/OS X)
--------------------------------------------------------------------------

To Generate the ./configure script use to create the Makefiles, you need
the following software installed:

Tool        Recommended Version
-------------------------------
autoconf    >= 2.60
automake    >= 1.10
libtool     >= 1.5.24

On Debian/Ubuntu, multiple versions of autoconf and automake are available
in separate packages. If you have multiple versions of autoconf or automake
installed on your system, you may have to configure the versions to use
using /usr/sbin/update-alternatives.

2 Building on Unix/Linux/OS X
--------------------------------------------------------------------------

This assumes you have all of the project dependencies installed.  We're
now ready to create the configure script.  To do this, run:

  ./autogen.sh

This should be run the first time and any time you change configure.ac or
any of the Makefile.am files.

    -----------------------------------------------------------------------
    |MacOS X Note:                                                        |
    | Make sure to set the LIBTOOLIZE environment variable to point to    |
    | /usr/bin/glibtoolize for the build to complete successfully. Below  |
    | is an example:                                                      |
    |                                                                     |
    | $ export LIBTOOLIZE=/usr/bin/glibtoolize                            |
    |                                                                     |
    | If you do not use this environment variable you will encounter an   |
    | error stating:                                                      |
    |                                                                     |
    | Can't exec "libtoolize": No such file or directory at               |
    | /opt/local/share/autoconf/Autom4te/FileUtils.pm line 290...         |
    -----------------------------------------------------------------------

    -----------------------------------------------------------------------
    |Solaris 10 Note:  CppUnit might not build until you correct the file |
    |  libstdc++.la to contain the correct data, see this discussion:     |
    |  http://forum.sun.com/jive/thread.jspa?threadID=73150               |
    |  Also you must pass --enable-shared=no for Solaris GCC builds       |
    |  For Solaris builds using the Sun Compiler you must set the env     |
    |  values CC and CXX to point to the cc and CC commands respectively. |
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

To generate the Doxygen documentation for the project, just run:

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
tcp://localhost:61613 for Stomp and tcp://localhost:61616 for Openwire.

The integration tests are built via "make check".  To run them, first
start a broker and then

  cd src/test-integration
  ./activemq-test-integration

This will take quite some time to complete, so be patient.  It is recommended
that you restart the broker between successive runs of the integration tests.

5 Example
--------------------------------------------------------------------------
There are example applications that ship with the distribution in
src/examples.   The examples are compiled by default with the "make"
command on Unix systems.  Only one sample is included in the Visual Studio
projects supplied, the others can be easily added by examining the settings
of the one supplied.

6 Notes for Windows users
--------------------------------------------------------------------------
We do not support using the GNU compiler on Windows, using the Cygwin package
or the MinGW platform, several issues with sockets and threading were found to
exist when trying to use these solutions.

However we do support using the MSVC compiler on Windows.

There are a couple or things that you will need to setup to ensure that
the MSVC compile succeeds.

* When linking your application to the DLL version of the ActiveMQ-CPP library
  you must link your app the the same runtime version that the DLL is linked to,
  otherwise your application will cause heap corruption when you delete objects
  that are created in the ActiveMQ-CPP DLL's heap.

* You need to download and install the Platform SDK if you don't have it
  installed already.  On machines where you intend to use the built libraries
  and executable you will also need to install the MS Redistributable for the
  version of Visual Studio which you used to build the library.

* Ensure that the path to you MSVC install is set in the PATH env variable.
  You can test this by typing cl.exe at the command line, if you get an
  error complaining that its not found, then you'll need to fix your PATH.

* The Project files reference the CPPUnit libraries for the Integration and
  Unit tests builds. In order for these to build correctly you must
  either configure the global settings in Visual Studio for include and library
  folders or add new settings to each of the projects in the solution to point
  to these locations.

* The Project files reference the APR libraries for the Integration and
  Unit tests builds. In order for these to build correctly you must
  either configure the global settings in Visual Studio for include and library
  folders or add new settings to each of the projects in the solution to point
  to these locations.
