# Since we don't strictly follow the GNU standard of having 'NEWS README AUTHORS ChangeLog' files
AUTOMAKE_OPTIONS = foreign

#Build in these directories:
SUBDIRS = src/main src/examples

#Distribute these directories:
DIST_SUBDIRS = src/main src/examples

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = activemq-cpp.pc

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST=autogen.sh
