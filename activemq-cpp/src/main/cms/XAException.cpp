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

#include "XAException.h"

using namespace cms;

////////////////////////////////////////////////////////////////////////////////
const int XAException::XA_RBBASE = 100;
const int XAException::XA_RBROLLBACK = 100;
const int XAException::XA_RBCOMMFAIL = 101;
const int XAException::XA_RBDEADLOCK = 102;
const int XAException::XA_RBINTEGRITY = 103;
const int XAException::XA_RBOTHER = 104;
const int XAException::XA_RBPROTO = 105;
const int XAException::XA_RBTIMEOUT = 106;
const int XAException::XA_RBTRANSIENT = 107;
const int XAException::XA_RBEND = 107;
const int XAException::XA_NOMIGRATE = 9;
const int XAException::XA_HEURHAZ = 8;
const int XAException::XA_HEURCOM = 7;
const int XAException::XA_HEURRB = 6;
const int XAException::XA_HEURMIX = 5;
const int XAException::XA_RETRY = 4;
const int XAException::XA_RDONLY = 3;
const int XAException::XAER_ASYNC = -2;
const int XAException::XAER_RMERR = -3;
const int XAException::XAER_NOTA = -4;
const int XAException::XAER_INVAL = -5;
const int XAException::XAER_PROTO = -6;
const int XAException::XAER_RMFAIL = -7;
const int XAException::XAER_DUPID = -8;
const int XAException::XAER_OUTSIDE = -9;

////////////////////////////////////////////////////////////////////////////////
XAException::XAException() : CMSException(), errorCode(0) {
}

////////////////////////////////////////////////////////////////////////////////
XAException::XAException(int errorCode) : CMSException(), errorCode(errorCode) {
}

////////////////////////////////////////////////////////////////////////////////
XAException::XAException(const XAException& ex) : CMSException(ex), errorCode(ex.errorCode) {
}

////////////////////////////////////////////////////////////////////////////////
XAException::XAException(const std::string& message) : CMSException(message, NULL), errorCode(0) {
}

////////////////////////////////////////////////////////////////////////////////
XAException::XAException(const std::string& message, const std::exception* cause) :
    CMSException(message, cause), errorCode(0) {
}

////////////////////////////////////////////////////////////////////////////////
XAException::XAException(const std::string& message, const std::exception* cause,
                         const std::vector<std::pair<std::string, int> >& stackTrace) :
    CMSException(message, cause, stackTrace), errorCode(0) {
}

////////////////////////////////////////////////////////////////////////////////
XAException::~XAException() throw() {
}

////////////////////////////////////////////////////////////////////////////////
XAException* XAException::clone() {
    return new XAException(*this);
}
