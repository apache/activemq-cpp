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
autoconf    >= 2.61
automake    >= 1.10
libtool     >= 1.5.24
APR         >= 1.3*
CPPUnit     >= 1.10.2* ( 1.12.1 is recommended )
OpenSSL     >= 0.9.8m* ( 1.0.0 or higher is recommended, this is an optional dependency)

* Requires that the Development package also be installed.

1.1 CppUnit
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

1.2 APR
-------------------------------------------------------------------------

The build requires the APR library.  This library can be obtained from the APR
site at: http://apr.apache.org/  See the documentation there on how to build
this libraries for your platform.  On many Unix system the library can be
installed using the systems package installer.

The Library has been tested using version v1.3 and higher of the APR libs,
the older 0.9 version will definitely not work with this library.

Many of the Unix type OS'es currently shipping include APR 1.2.x and
APR-Util 1.2.x, this implies that you will need to build and install APR from
the source download at Apache.

    -----------------------------------------------------------------------
    |MacOS X Note:                                                        |
    | If you have installed APR via MacPorts                              |
    | (http://www.macports.org/) and you are building ActiveMQ-CPP from   |
    | source, you will need to specify the location of the APR library    |
    | to the configure script. This is easy to do and only requires one   |
    | extra option for compilation. An example of this is shown below     |
    | using the standard location for this library as installed by        |
    | MacPorts:                                                           |
    |                                                                     |
    | $ ./configure \                                                     |
    | --with-apr=/opt/local/var/macports/software/apr/1.3.3_0/opt/local/  |
    |                                                                     |
    | Please notice that this is simply a single option to the            |
    | configure script that has been broken into two lines by escaping    |
    | the newline character to fit the formatting of this file.           |
    |                                                                     |
    | If you have manually compiled and installed the APR libraries into  |
    | a custom location, then you will need to point to that location     |
    | using the argument above.                                           |
    -----------------------------------------------------------------------

1.3 OpenSSL
--------------------------------------------------------------------------

If you wish to use the SSL Transport then you will need to have OpenSSL and
its includes installed on your system.  We recommend that you use version 1.0.0
or higher for best performance and security, but version from 0.9.8 are also
known to work.  The autoconf script will search for the library and enabled support
automatically if it is found.

You can disable the OpenSSL checks in the configure script with the --disable-ssl
option or you can specify a custom location for the OpenSSL package with the
--with-openssl option.  See the --help option on the configure script for more
information.

On Windows you need to obtain an OpenSSL binary package and place the libraries on
the system path or in the System32 directory.  In the Visual Studio project you must
add the HAVE_OPENSSL flag to the preprocessor directives and add the paths for the
includes and libraries so that the compiler and linker can find them.

1.4 GNU Build System (for building on Unix/Linux/OS X)
--------------------------------------------------------------------------

To Generate the ./configure script use to create the Makefiles, you need
the following software installed:

Tool        Recommended Version
-------------------------------
autoconf    >= 2.61
automake    >= 1.10
libtool     >= 1.5.24

On Debian/Ubuntu, multiple versions of autoconf and automake are available
in separate packages. If you have multiple versions of autoconf or automake
installed on your system, you may have to configure the versions to use
using /usr/sbin/update-alternatives.

    -----------------------------------------------------------------------
    |Ubuntu Note:                                                         |
    |                                                                     |
    |In order to build you will need the Build Essentials Package         |
    |                                                                     |
    | sudo apt-get install build-essential                                |
    -----------------------------------------------------------------------

2 Building on Unix/Linux/OS X
--------------------------------------------------------------------------

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

** A Note For ActiveMQ-CPP Developers **

If you need to make any changes to the configure.ac or any of the included
m4 files then you need to regenerate the configure script.

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

However we do support using the MSVC compiler on Windows, we provide a set of
Visual Studio project files for v2010, newer version should be able to upgrade
these files to the newest format.

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

6.1 Project Settings

The Visual Studio 2010 Project files included with the code have been configured to look
for the various headers and library files of the APR, OpenSSL and CPPUnit projects using
a predefined directory structure and a set of environment variables which can be used to
define the exact location on disk.

The directory structure for each dependency is prefixed with an environment variable, the
set of variables is as follows:

    Library        Recommended Version     Windows Env Var
    ------------------------------------------------------------
      APR             >= 1.3                 ${APR_DIST}
      CPPUNIT         >= 1.10.2              ${CPPUNIT_DIST}
      OpenSSL         >= 1.5.24              ${OPENSSL_DIST}
      Platform SDK    {varies}               ${PLATFORM_SDK}

Under each of the library dependencies the structure should be layed out as follows:

      ${APR_DIST}\
          win32\
             include\
             lib\
             bin\
          x64\
             include\
             lib\
             bin\
      ${CPPUNIT_DIST}\
          win32\
             include\
             lib\
          x64\
             include\
             lib\
      ${OPENSSL_DIST}\
          win32\
             include\
             lib\
             bin\
             exp\
          x64\
             include\
             lib\
             bin\
             exp\

6.2  Obtaining and building the dependent libraries

Before you can build the ActiveMQ-CPP library you need to get builds of the required libraries
installed on you system.  This is not always simple on windows, we will offer a few tips to try
and make getting a copy of each here but you might also want to consult Google and ask on the
mailing list for more help.

6.2.1  APR library.

APR is provided in source form only from the Apache Software Foundation.  You will need to build
a library from source by downloading the latest release from:

    http://apr.apache.org/

At the time of this writing the latest version was v1.4.8 and is recommended since its build
support files are working with the Visual Studio 2010 tools.

APR is built from the command line using its provided Make files.  When building the library you
need to ensure you are in the right environment to produce the desired architecture builds (x64 or
win32).  You can open a command line that's correct by using the shortcuts under the Visual Studio
start menu location (ex. Visual Studio 2010 / Visual Studio Tools / Visual Studio x64 win 64 Command Prompt

Once you are in the proper command prompt change to the directory where your APR source code is
located (ex: C:\APR) and then run the build for the library to produce the desired ARCH build.

For a 32 bit library which installs into a proper distribution directory run:

   nmake -f Makefile.win ARCH="win32 Release" PREFIX=C:\dist\APR\x64 buildall install clean

and for a 64 bit build of the library use the command.

   nmake -f Makefile.win ARCH="x64 Release" PREFIX=C:\dist\APR\x64 buildall install clean

6.2.2  CPPUnit

To build the CPPUnit library for later Visual Studio versions such as 2010+ you need to download the
most up to date code for CPPUnit v1.12.1+ from its SVN location.  The current location is:

   https://svn.code.sf.net/p/cppunit/code/trunk/cppunit

Once you have checked out the code you can use the included Visual Studio 2010 project files to
build the various target (Debug, Release, Debug DLL, Release DLL) and copy each to the location
you have chosen, just ensure you follow the directory layout as specified above.

6.2.3   OpenSSL

OpenSSL can be the harder one to obtain on Windows.  If you choose to use SSL on Windows you need
to locate a build of OpenSSL that matches your Visual Stidio version (2010 at the time of this
writing) and install it into the location matching the directory layout configured in the ActiveMQ-CPP
project files.   Builds for visual studio 2008 were available online at this location:

   http://slproweb.com/products/Win32OpenSSL.html

Before attempting to build OpenSSL yourself you should check to see if newer 2010 versions are
available.  If you find you need to build it yourself you should consult Google to find useful
Blog postings that detail how to get started with the build process for OpenSSL on Windows.




