/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DECAF_NET_URLTEST_H_
#define _DECAF_NET_URLTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace net {

    class URLTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( URLTest );
        CPPUNIT_TEST( testConstructor1 );
        CPPUNIT_TEST( testConstructor2 );
        CPPUNIT_TEST( testConstructor3 );
        CPPUNIT_TEST( testConstructor4 );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testSameFile );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testToExternalForm );
        CPPUNIT_TEST( testGetFile );
        CPPUNIT_TEST( testGetHost );
        CPPUNIT_TEST( testGetPort );
        CPPUNIT_TEST( testGetDefaultPort );
        CPPUNIT_TEST( testGetProtocol );
        CPPUNIT_TEST( testGetRef );
        CPPUNIT_TEST( testGetAuthority );
        CPPUNIT_TEST( testToURI );
        CPPUNIT_TEST( testURLStreamHandlerParseURL );
        CPPUNIT_TEST( testUrlParts );
        CPPUNIT_TEST( testFileEqualsWithEmptyHost );
        CPPUNIT_TEST( testNoHost );
        CPPUNIT_TEST( testNoPath );
        CPPUNIT_TEST( testEmptyHostAndNoPath );
        CPPUNIT_TEST( testNoHostAndNoPath );
        CPPUNIT_TEST( testAtSignInUserInfo );
        CPPUNIT_TEST( testUserNoPassword );
        CPPUNIT_TEST( testUserNoPasswordExplicitPort );
        CPPUNIT_TEST( testUserPasswordHostPort );
        CPPUNIT_TEST( testUserPasswordEmptyHostPort );
        CPPUNIT_TEST( testUserPasswordEmptyHostEmptyPort );
        CPPUNIT_TEST( testPathOnly );
        CPPUNIT_TEST( testQueryOnly );
        CPPUNIT_TEST( testFragmentOnly );
        CPPUNIT_TEST( testAtSignInPath );
        CPPUNIT_TEST( testColonInPath );
        CPPUNIT_TEST( testSlashInQuery );
        CPPUNIT_TEST( testQuestionMarkInQuery );
        CPPUNIT_TEST( testAtSignInQuery );
        CPPUNIT_TEST( testColonInQuery );
        CPPUNIT_TEST( testQuestionMarkInFragment );
        CPPUNIT_TEST( testColonInFragment );
        CPPUNIT_TEST( testSlashInFragment );
        CPPUNIT_TEST( testSlashInFragmentCombiningConstructor );
        CPPUNIT_TEST( testHashInFragment );
        CPPUNIT_TEST( testEmptyPort );
        CPPUNIT_TEST( testNonNumericPort );
        CPPUNIT_TEST( testNegativePort );
        CPPUNIT_TEST( testNegativePortEqualsPlaceholder );
        CPPUNIT_TEST( testRelativePathOnQuery );
        CPPUNIT_TEST( testRelativeFragmentOnQuery );
        CPPUNIT_TEST( testPathContainsRelativeParts );
        CPPUNIT_TEST( testRelativePathAndFragment );
        CPPUNIT_TEST( testRelativeParentDirectory );
        CPPUNIT_TEST( testRelativeChildDirectory );
        CPPUNIT_TEST( testRelativeRootDirectory );
        CPPUNIT_TEST( testRelativeFullUrl );
        CPPUNIT_TEST( testRelativeDifferentScheme );
        CPPUNIT_TEST( testRelativeDifferentAuthority );
        CPPUNIT_TEST( testRelativeWithScheme );
        CPPUNIT_TEST( testMalformedUrlsRefusedByFirefoxAndChrome );
        CPPUNIT_TEST( testRfc1808NormalExamples );
        CPPUNIT_TEST( testRfc1808AbnormalExampleTooManyDotDotSequences );
        CPPUNIT_TEST( testRfc1808AbnormalExampleRemoveDotSegments );
        CPPUNIT_TEST( testRfc1808AbnormalExampleNonsensicalDots );
        CPPUNIT_TEST( testRfc1808AbnormalExampleRelativeScheme );
        CPPUNIT_TEST( testRfc1808AbnormalExampleQueryOrFragmentDots );
        CPPUNIT_TEST( testSquareBracketsInUserInfo );
        CPPUNIT_TEST( testComposeUrl );
        CPPUNIT_TEST( testComposeUrlWithNullHost );
        CPPUNIT_TEST( testFileUrlExtraLeadingSlashes );
        CPPUNIT_TEST( testFileUrlWithAuthority );
        CPPUNIT_TEST( testEmptyAuthority );
        CPPUNIT_TEST( testHttpUrlExtraLeadingSlashes );
        CPPUNIT_TEST( testFileUrlRelativePath );
        CPPUNIT_TEST( testFileUrlDottedPath );
        CPPUNIT_TEST( testParsingDotAsHostname );
        CPPUNIT_TEST( testSquareBracketsWithIPv4 );
        CPPUNIT_TEST( testSquareBracketsWithHostname );
        CPPUNIT_TEST( testIPv6WithoutSquareBrackets );
        CPPUNIT_TEST( testIpv6WithSquareBrackets );
        CPPUNIT_TEST( testEqualityWithNoPath );
        CPPUNIT_TEST( testUrlDoesNotEncodeParts );
        CPPUNIT_TEST( testSchemeCaseIsCanonicalized );
        CPPUNIT_TEST( testEmptyAuthorityWithPath );
        CPPUNIT_TEST( testEmptyAuthorityWithQuery );
        CPPUNIT_TEST( testEmptyAuthorityWithFragment );
        CPPUNIT_TEST( testCombiningConstructorsMakeRelativePathsAbsolute );
        CPPUNIT_TEST( testCombiningConstructorsDoNotMakeEmptyPathsAbsolute );
        CPPUNIT_TEST( testPartContainsSpace );
        CPPUNIT_TEST( testUnderscore );
        CPPUNIT_TEST_SUITE_END();

    public:

        URLTest();
        virtual ~URLTest();

        void testConstructor1();
        void testConstructor2();
        void testConstructor3();
        void testConstructor4();
        void testEquals();
        void testSameFile();
        void testToString();
        void testToExternalForm();
        void testGetFile();
        void testGetHost();
        void testGetPort();
        void testGetDefaultPort();
        void testGetProtocol();
        void testGetRef();
        void testGetAuthority();
        void testToURI();
        void testURLStreamHandlerParseURL();
        void testUrlParts();
        void testFileEqualsWithEmptyHost();
        void testOmittedHost();
        void testNoHost();
        void testNoPath();
        void testEmptyHostAndNoPath();
        void testNoHostAndNoPath();
        void testAtSignInUserInfo();
        void testUserNoPassword();
        void testUserNoPasswordExplicitPort();
        void testUserPasswordHostPort();
        void testUserPasswordEmptyHostPort();
        void testUserPasswordEmptyHostEmptyPort();
        void testPathOnly();
        void testQueryOnly();
        void testFragmentOnly();
        void testAtSignInPath();
        void testColonInPath();
        void testSlashInQuery();
        void testQuestionMarkInQuery();
        void testAtSignInQuery();
        void testColonInQuery();
        void testQuestionMarkInFragment();
        void testColonInFragment();
        void testSlashInFragment();
        void testSlashInFragmentCombiningConstructor();
        void testHashInFragment();
        void testEmptyPort();
        void testNonNumericPort();
        void testNegativePort();
        void testNegativePortEqualsPlaceholder();
        void testRelativePathOnQuery();
        void testRelativeFragmentOnQuery();
        void testPathContainsRelativeParts();
        void testRelativePathAndFragment();
        void testRelativeParentDirectory();
        void testRelativeChildDirectory();
        void testRelativeRootDirectory();
        void testRelativeFullUrl();
        void testRelativeDifferentScheme();
        void testRelativeDifferentAuthority();
        void testRelativeWithScheme();
        void testMalformedUrlsRefusedByFirefoxAndChrome();
        void testRfc1808NormalExamples();
        void testRfc1808AbnormalExampleTooManyDotDotSequences();
        void testRfc1808AbnormalExampleRemoveDotSegments();
        void testRfc1808AbnormalExampleNonsensicalDots();
        void testRfc1808AbnormalExampleRelativeScheme();
        void testRfc1808AbnormalExampleQueryOrFragmentDots();
        void testSquareBracketsInUserInfo();
        void testComposeUrl();
        void testComposeUrlWithNullHost();
        void testFileUrlExtraLeadingSlashes();
        void testFileUrlWithAuthority();
        void testEmptyAuthority();
        void testHttpUrlExtraLeadingSlashes();
        void testFileUrlRelativePath();
        void testFileUrlDottedPath();
        void testParsingDotAsHostname();
        void testSquareBracketsWithIPv4();
        void testSquareBracketsWithHostname();
        void testIPv6WithoutSquareBrackets();
        void testIpv6WithSquareBrackets();
        void testEqualityWithNoPath();
        void testUrlDoesNotEncodeParts();
        void testSchemeCaseIsCanonicalized();
        void testEmptyAuthorityWithPath();
        void testEmptyAuthorityWithQuery();
        void testEmptyAuthorityWithFragment();
        void testCombiningConstructorsMakeRelativePathsAbsolute();
        void testCombiningConstructorsDoNotMakeEmptyPathsAbsolute();
        void testPartContainsSpace();
        void testUnderscore();

    };

}}

#endif /* _DECAF_NET_URLTEST_H_ */
