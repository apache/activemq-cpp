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

#include "URLEncoderTest.h"

#include <decaf/net/URLDecoder.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
URLEncoderTest::URLEncoderTest() {}

////////////////////////////////////////////////////////////////////////////////
void URLEncoderTest::testEncode() {

    const std::string URL = "http://127.0.0.1:80/%24%25";
    const std::string URL2 = "telnet://justWantToHaveFun.com:400";
    const std::string URL3 = "file://myServer.org/a file with spaces.jpg";

    CPPUNIT_ASSERT_MESSAGE(
            "1. Incorrect encoding/decoding",
            URLDecoder::decode( URLEncoder::encode( URL ) ) == URL );
    CPPUNIT_ASSERT_MESSAGE(
            "2. Incorrect encoding/decoding",
            URLDecoder::decode( URLEncoder::encode( URL2 ) ) == URL2 );
    CPPUNIT_ASSERT_MESSAGE(
            "3. Incorrect encoding/decoding",
            URLDecoder::decode( URLEncoder::encode( URL3 ) ) == URL3 );

}
