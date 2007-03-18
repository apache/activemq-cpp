#include "connector/openwire/OpenwireAsyncSenderTest.h"
#include "connector/openwire/OpenwireDurableTest.h"
#include "connector/openwire/OpenwireExpirationTest.h"
#include "connector/openwire/OpenwireSimpleRollbackTest.h"
#include "connector/openwire/OpenwireSimpleTest.h"
#include "connector/openwire/OpenwireSlowListenerTest.h"
#include "connector/openwire/OpenwireTempDestinationTest.h"
#include "connector/openwire/OpenwireTransactionTest.h"

#include "connector/stomp/AsyncSenderTest.h"
#include "connector/stomp/DurableTest.h"
#include "connector/stomp/ExpirationTest.h"
#include "connector/stomp/SimpleRollbackTest.h"
#include "connector/stomp/SimpleTest.h"
#include "connector/stomp/TransactionTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireAsyncSenderTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireDurableTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireExpirationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireSimpleRollbackTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireSimpleTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireSlowListenerTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireTempDestinationTest );
//CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::openwire::OpenwireTransactionTest );

CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::AsyncSenderTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::DurableTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::ExpirationTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::SimpleRollbackTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::SimpleTest );
CPPUNIT_TEST_SUITE_REGISTRATION( integration::connector::stomp::TransactionTest );
