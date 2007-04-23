--------------------------------------------------------------------------
C++ Messaging Service (CMS) API
--------------------------------------------------------------------------

CMS is a JMS-like messaging API for C++ applications.  This API is
vendor-independent to encourage the adoption of a standard API
for messaging in C++ applications.


1 Dependencies
--------------------------------------------------------------------------


1.1 GNU Build System (for building on Unix/Linux/OS X/Cygwin)
--------------------------------------------------------------------------

To Generate the ./configure script use to create the Makefiles, you need 
the following software installed:

Tool        Recommended Version
-------------------------------
autoconf    >= 2.59
automake    >= 1.9.6
libtool     >= 1.5.22

On Debian/Ubuntu, multiple versions of autoconf and automake are available
in separate packages. If you have multiple versions of autoconf or automake
installed on your system, you may have to configure the versions to use
using /usr/sbin/update-alternatives.

2 Building on Unix/Linux/OS X/Cygwin
--------------------------------------------------------------------------

This assumes you have all of the project dependencies installed.  We're 
now ready to create the configure script.  To do this, run:

  ./autogen.sh

This should be run the first time and anytime you change configure.ac or 
any of the Makefile.am files.
    
The configure script will customize the way the software is built and 
installed into your system along with detecting the available libraries 
that have been installed.  To use the default configuration just run:

  ./configure

For more help on how to customize the build configuration, run:

  ./configure --help

Once the configure script has run successfully, you can build and 
install the code into the system directories with the command:

  make install

You will have to become the superuser in order to be able to install the 
files.

3 Doxygen
--------------------------------------------------------------------------

To generate the doxygen documentation for the project, just run:

  make doxygen-run


