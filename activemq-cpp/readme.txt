--------------------------------------------------------------------------
ActiveMQ CPP Library
--------------------------------------------------------------------------

This library provides a JMS like interface to an ActiveMQ broker in c++.

Currently the Library only supports the Stomp protocol, future versions 
will contain support for openwire.

UNIT Tests
--------------------------------------------------------------------------

The package contains a complete set of cppunit tests.  In order for you
to build an run the tests, you will need to download and install the 
cppunit suite.  

http://cppunit.sourceforge.net/cppunit-wiki

or on Fedora type 

yum install cppunit*

Make sure that the path to the installed cpp unit library and includes is 
visible in your current shell before you try building the tets.

Integration Tests
--------------------------------------------------------------------------

The library also contains a set of tests that are run against a real AMQ
broker.  Running these without a broker will result in failed tests.
The tests currently hardcode the broker url to be tcp://127.0.0.1:61613, 
you can change this by changing the declaration in IntegrationCommon.cpp
in the test-integration src tree.

Developers Building on Unix/Linux/OS X/Cygwin from an SVN Checkout
--------------------------------------------------------------------------

To Generate the ./configure script use to create the Makefiles, you need
the following software installed:

 * autoconf version 2.59
 * automake version 1.9.6
 * libtool version 1.5.22
 * cppunit version 1.10.2

Later versions will probably work too with a little tweaking.  Once that's 
installed, run:

  ./autogen.sh

You should run this anytime you chance configure.ac or any of the
Makefile.am files.  Now that the configure script is generated follow 
the directions in the next section to complete the build

Solaris 10 note:  CPP_UNIT might not build until you correct the file
libstdc++.la to contain the correct data, see this discussion.
	http://forum.sun.com/jive/thread.jspa?threadID=73150


Users Building on Unix/Linux/OS X/Cygwin 
--------------------------------------------------------------------------

Use the supplied configure script to configure the software.  The configure
script will customize the way the software is built and installed into 
your system along with detecting the available libraries that have been 
installed.  To use the default configuration just run:

  ./configure

For more help on how to customize the build configuration, run:

  ./configure --help

Once the configure script has run successfully, you are ready to build.  Run

  make

To compile the ActiveMQ libraries and examples.  If you have cppunit installed
on your system you can also run:

  make check

This will verify that the library is functioning correctly and also generate the 
integration tests binary.

To generate the doxygen documentation for the project, just run:

  make doxygen-run

Notes for Windows users
--------------------------------------------------------------------------

The builds support using the GNU compiler on Windows, we used the Cygwin
package.  However we also support using the MSVC compiler on Windows.

There are a couple or things that you will need to setup to ensure that the
MSVC compile succeeds.

* You need to download and install the Platform SDK if you don't have it 
  installed already.
* Ensure that the path to you MSVC install is set in the PATH env variable.
  you can tests this buy typing cl.exe at the command line, if you get an
  error complaining that its not found, then setup you PATH correctly.
* Set the INCLUDE env variable to include the path to your MSVC includes,
  and the platform SDK includes.
  
  i.e. INCLUDE = D:\Program Files\Microsoft Visual Studio 8\VC\include;D:\Program Files\Microsoft Platform SDK\Include

* Set the LIB env variable to include the path to your MSVC libs, and the
  Platform SDK libs.

  i.e. LIB = D:\Program Files\Microsoft Visual Studio 8\VC\lib;D:\Program Files\Microsoft Platform SDK\Lib
