cc_sources = \
  integration/common/AbstractTester.cpp \
  integration/common/IntegrationCommon.cpp \
  integration/durable/DurableTester.cpp \
  integration/simple/SimpleTester.cpp \
  integration/transactional/TransactionTester.cpp \
  integration/various/SimpleRollbackTest.cpp \
  main.cpp

## Compile this as part of make check
check_PROGRAMS = activemq-test-integration

## 
## Compiler/Linker Options
##
activemq_test_integration_SOURCES = $(cc_sources)
activemq_test_integration_CXXFLAGS = -I$(srcdir)/../main @CPPUNIT_CFLAGS@
activemq_test_integration_LDADD= ../main/libactivemq-cpp.la @CPPUNIT_LIBS@

