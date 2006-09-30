# Since we don't strictly follow the GNU standard of having 'NEWS README AUTHORS ChangeLog' files
AUTOMAKE_OPTIONS = foreign

SUBDIRS = src/main src/examples
if BUILD_CPPUNIT_TESTS
  SUBDIRS += src/test src/test-integration
endif

#Distribute these directories:
DIST_SUBDIRS = src/main 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = activemq-cpp.pc

ACLOCAL_AMFLAGS = -I m4

include doxygen-include.am

EXTRA_DIST=autogen.sh $(DX_CONFIG) doc/html
