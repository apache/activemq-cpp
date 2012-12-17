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

#ifndef _DECAF_LANG_MATHTEST_H_
#define _DECAF_LANG_MATHTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf{
namespace lang{

    class MathTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( MathTest );
        CPPUNIT_TEST( test_absD );
        CPPUNIT_TEST( test_absF );
        CPPUNIT_TEST( test_absI );
        CPPUNIT_TEST( test_absJ );
//        CPPUNIT_TEST( test_acosD );
//        CPPUNIT_TEST( test_asinD );
//        CPPUNIT_TEST( test_atanD );
//        CPPUNIT_TEST( test_atan2DD );
//        CPPUNIT_TEST( test_cbrt_D );
        CPPUNIT_TEST( test_ceilD );
//        CPPUNIT_TEST( test_cosD );
//        CPPUNIT_TEST( test_cosh_D );
//        CPPUNIT_TEST( test_expD );
//        CPPUNIT_TEST( test_expm1_D );
        CPPUNIT_TEST( test_floorD );
//        CPPUNIT_TEST( test_hypot_DD );
//        CPPUNIT_TEST( test_IEEEremainderDD );
//        CPPUNIT_TEST( test_logD );
//        CPPUNIT_TEST( test_log10_D );
//        CPPUNIT_TEST( test_log1p_D );
        CPPUNIT_TEST( test_maxDD );
        CPPUNIT_TEST( test_maxFF );
        CPPUNIT_TEST( test_maxII );
        CPPUNIT_TEST( test_maxJJ );
        CPPUNIT_TEST( test_minDD );
        CPPUNIT_TEST( test_minFF );
        CPPUNIT_TEST( test_minII );
        CPPUNIT_TEST( test_minJJ );
        CPPUNIT_TEST( test_powDD );
//        CPPUNIT_TEST( test_rintD );
        CPPUNIT_TEST( test_roundD );
        CPPUNIT_TEST( test_roundF );
        CPPUNIT_TEST( test_signum_D );
        CPPUNIT_TEST( test_signum_F );
//        CPPUNIT_TEST( test_sinD );
//        CPPUNIT_TEST( test_sinh_D );
        CPPUNIT_TEST( test_sqrtD );
//        CPPUNIT_TEST( test_tanD );
//        CPPUNIT_TEST( test_tanh_D );
        CPPUNIT_TEST( test_random );
        CPPUNIT_TEST( test_toRadiansD );
        CPPUNIT_TEST( test_toDegreesD );
//        CPPUNIT_TEST( test_ulp_D );
//        CPPUNIT_TEST( test_ulp_f );
        CPPUNIT_TEST_SUITE_END();

    private:

        double HYP;
        double OPP;
        double ADJ;

    public:

        MathTest();
        virtual ~MathTest() {}

        virtual void test_absD();
        virtual void test_absF();
        virtual void test_absI();
        virtual void test_absJ();
//        virtual void test_acosD();
//        virtual void test_asinD();
//        virtual void test_atanD();
//        virtual void test_atan2DD();
//        virtual void test_cbrt_D();
        virtual void test_ceilD();
//        virtual void test_cosD();
//        virtual void test_cosh_D();
//        virtual void test_expD();
//        virtual void test_expm1_D();
        virtual void test_floorD();
//        virtual void test_hypot_DD();
//        virtual void test_IEEEremainderDD();
//        virtual void test_logD();
//        virtual void test_log10_D();
//        virtual void test_log1p_D();
        virtual void test_maxDD();
        virtual void test_maxFF();
        virtual void test_maxII();
        virtual void test_maxJJ();
        virtual void test_minDD();
        virtual void test_minFF();
        virtual void test_minII();
        virtual void test_minJJ();
        virtual void test_powDD();
//        virtual void test_rintD();
        virtual void test_roundD();
        virtual void test_roundF();
        virtual void test_signum_D();
        virtual void test_signum_F();
//        virtual void test_sinD();
//        virtual void test_sinh_D();
        virtual void test_sqrtD();
//        virtual void test_tanD();
//        virtual void test_tanh_D();
        virtual void test_random();
        virtual void test_toRadiansD();
        virtual void test_toDegreesD();
//        virtual void test_ulp_D();
//        virtual void test_ulp_f();

    };

}}

#endif /*_DECAF_LANG_MATHTEST_H_*/
