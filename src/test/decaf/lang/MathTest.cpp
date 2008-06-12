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

#include "MathTest.h"

#include <decaf/lang/Math.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
MathTest::MathTest() {
    this->HYP = Math::sqrt( 2.0 );
    this->OPP = 1.0;
    this->ADJ = 1.0;
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_absD() {
    // Test for method double decaf.lang.Math.abs(double)

    CPPUNIT_ASSERT_MESSAGE( "Incorrect double abs value",
            ( Math::abs(-1908.8976) == 1908.8976));
    CPPUNIT_ASSERT_MESSAGE( "Incorrect double abs value",
            ( Math::abs(1908.8976) == 1908.8976));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_absF() {
    // Test for method float decaf.lang.Math::abs(float)
    CPPUNIT_ASSERT_MESSAGE("Incorrect float abs value",
            (Math::abs(-1908.8976f) == 1908.8976f));
    CPPUNIT_ASSERT_MESSAGE("Incorrect float abs value",
            (Math::abs(1908.8976f) == 1908.8976f));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_absI() {
    // Test for method int decaf.lang.Math::abs(int)
    CPPUNIT_ASSERT_MESSAGE("Incorrect int abs value", (Math::abs(-1908897) == 1908897));
    CPPUNIT_ASSERT_MESSAGE("Incorrect int abs value", (Math::abs(1908897) == 1908897));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_absJ() {
    // Test for method long decaf.lang.Math::abs(long)
    CPPUNIT_ASSERT_MESSAGE("Incorrect long abs value",
            (Math::abs(-19088976000089LL) == 19088976000089LL));
    CPPUNIT_ASSERT_MESSAGE("Incorrect long abs value",
            (Math::abs(19088976000089LL) == 19088976000089LL));
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_acosD() {
//    // Test for method double decaf.lang.Math::acos(double)
//    double r = Math::cos(Math::acos(ADJ / HYP));
//    long long lr = Double::doubleToLongBits(r);
//    long long t = Double::doubleToLongBits(ADJ / HYP);
//    CPPUNIT_ASSERT_MESSAGE("Returned incorrect arc cosine", lr == t || (lr + 1) == t
//            || (lr - 1) == t);
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_asinD() {
//    // Test for method double decaf.lang.Math::asin(double)
//    double r = Math::sin(Math::asin(OPP / HYP));
//    long long lr = Double::doubleToLongBits(r);
//    long long t = Double::doubleToLongBits(OPP / HYP);
//    CPPUNIT_ASSERT_MESSAGE("Returned incorrect arc sine", lr == t || (lr + 1) == t
//            || (lr - 1) == t);
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_atanD() {
//    // Test for method double decaf.lang.Math::atan(double)
//    double answer = Math::tan(Math::atan(1.0));
//    CPPUNIT_ASSERT_MESSAGE("Returned incorrect arc tangent: ",
//            answer <= 1.0 && answer >= 9.9999999999999983E-1);
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_atan2DD() {
//    // Test for method double decaf.lang.Math::atan2(double, double)
//    double answer = Math::atan(Math::tan(1.0));
//    CPPUNIT_ASSERT_MESSAGE("Returned incorrect arc tangent: ",
//            answer <= 1.0 && answer >= 9.9999999999999983E-1);
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_cbrt_D() {
//    //Test for special situations
//    CPPUNIT_ASSERT_MESSAGE("Should return Double::NaN",
//            Double::isNaN(Math::cbrt(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return Double::POSITIVE_INFINITY",
//            Math::cbrt(Double::POSITIVE_INFINITY), Double::POSITIVE_INFINITY );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return Double::NEGATIVE_INFINITY",
//            Math::cbrt(Double::NEGATIVE_INFINITY), Double::NEGATIVE_INFINITY);
//    CPPUNIT_ASSERT_EQUAL(
//            Double::doubleToLongBits(0.0),
//            Double::doubleToLongBits(Math::cbrt(0.0)));
//    CPPUNIT_ASSERT_EQUAL( Double::doubleToLongBits(+0.0),
//                          Double::doubleToLongBits(Math::cbrt(+0.0)));
//    CPPUNIT_ASSERT_EQUAL( Double::doubleToLongBits(-0.0),
//                          Double::doubleToLongBits(Math::cbrt(-0.0)));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 3.0", Math::cbrt(27.0), 3.0);
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 5.643803094122362E102",
//            5.643803094122362E102, Math::cbrt(Double::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 0.01", 0.01, Math::cbrt(0.000001));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -3.0", -3.0, Math::cbrt(-27.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.7031839360032603E-108",
//            1.7031839360032603E-108, Math::cbrt(Double::MIN_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -0.01", -0.01, Math::cbrt(-0.000001));
//}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_ceilD() {
    // Test for method double decaf.lang.Math::ceil(double)
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Incorrect ceiling for double",
                                  79.0, Math::ceil(78.89) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect ceiling for double",
                         -78.0, Math::ceil(-78.89) );
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_cosD() {
//    // Test for method double decaf.lang.Math::cos(double)
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect answer", 1.0, Math::cos(0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect answer", 0.5403023058681398, Math::cos(1));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_cosh_D() {
//    // Test for special situations
//    CPPUNIT_ASSERT(Double::isNaN(Math::cosh(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::cosh(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::cosh(Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0", 1.0, Math::cosh(+0.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0", 1.0, Math::cosh(-0.0));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::cosh(1234.56));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::cosh(-1234.56));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0000000000005",
//            1.0000000000005, Math::cosh(0.000001));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0000000000005",
//            1.0000000000005, Math::cosh(-0.000001));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 5.212214351945598",
//            5.212214351945598, Math::cosh(2.33482));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::cosh(Double::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0",
//            1.0, Math::cosh(Double::MIN_VALUE));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_expD() {
//    // Test for method double decaf.lang.Math::exp(double)
//    CPPUNIT_ASSERT_MESSAGE("Incorrect answer returned for simple power",
//            Math::abs(Math::exp(4.0) -
//            Math::E * Math::E * Math::E * Math::E) < 0.1 );
//    CPPUNIT_ASSERT_MESSAGE("Incorrect answer returned for larger power",
//            Math::log( Math::abs( Math::exp(5.5) ) - 5.5) < 10.0 );
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_expm1_D() {
//
//    // Test for special cases
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Double::isNaN(Math::expm1(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::expm1(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -1.0", -1.0,
//            Math::expm1(Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL( Double::doubleToLongBits(0.0),
//                          Double::doubleToLongBits(Math::expm1(0.0)) );
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(+0.0),
//            Double::doubleToLongBits(Math::expm1(+0.0)));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(-0.0),
//            Double::doubleToLongBits(Math::expm1(-0.0)));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -9.999950000166666E-6",
//            -9.999950000166666E-6, Math::expm1(-0.00001));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0145103074469635E60",
//            1.0145103074469635E60, Math::expm1(138.16951162));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY,
//            Math::expm1(123456789123456789123456789.4521584223));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::expm1(Double::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return MIN_VALUE", Double::MIN_VALUE,
//            Math::expm1(Double::MIN_VALUE));
//}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_floorD() {
    // Test for method double decaf.lang.Math::floor(double)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect floor for double",
                         78.0, Math::floor(78.89));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect floor for double",
                         -79.0, Math::floor(-78.89));
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_hypot_DD() {
//    // Test for special cases
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::hypot(Double::POSITIVE_INFINITY,
//                    1.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::hypot(Double::NEGATIVE_INFINITY,
//                    123.324));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY,
//            Math::hypot(-758.2587,Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY,
//            Math::hypot(5687.21, Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::hypot(Double::POSITIVE_INFINITY,
//                    Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::hypot(Double::NEGATIVE_INFINITY,
//                    Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_MESSAGE("Should be NaN", Double::isNaN(Math::hypot(Double::NaN,
//            2342301.89843)));
//    CPPUNIT_ASSERT_MESSAGE("Should be NaN", Double::isNaN(Math::hypot(-345.2680,
//            Double::NaN)));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 2396424.905416697",
//            2396424.905416697, Math::hypot(12322.12, -2396393.2258));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 138.16958070558556", 138.16958070558556,
//            Math::hypot(-138.16951162, 0.13817035864));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.7976931348623157E308",
//            1.7976931348623157E308, Math::hypot(Double::MAX_VALUE, 211370.35));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 5413.7185", 5413.7185, Math::hypot(
//            -5413.7185, Double::MIN_VALUE));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_IEEEremainderDD() {
//    // Test for method double decaf.lang.Math::IEEEremainder(double, double)
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect remainder returned",
//            0.0, Math::IEEEremainder(1.0, 1.0));
//    CPPUNIT_ASSERT_MESSAGE("Incorrect remainder returned",
//            Math::IEEEremainder(1.32,89.765) >= 1.4705063220631647E-2 ||
//            Math::IEEEremainder(1.32, 89.765) >= 1.4705063220631649E-2);
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_logD() {
//    // Test for method double decaf.lang.Math::log(double)
//    for( double d = 10; d >= -10; d -= 0.5 ) {
//        double answer = Math::log( Math::exp(d) );
//
//        CPPUNIT_ASSERT_MESSAGE(
//                "Answer does not equal expected answer for d",
//                Math::abs( answer - d ) <= Math::abs(d * 0.00000001) );
//    }
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_log10_D() {
//    // Test for special cases
//    CPPUNIT_ASSERT(Double::isNaN(Math::log10(Double::NaN)));
//    CPPUNIT_ASSERT(Double::isNaN(Math::log10(-2541.05745687234187532)));
//    CPPUNIT_ASSERT(Double::isNaN(Math::log10(-0.1)));
//    CPPUNIT_ASSERT_EQUAL(Double::POSITIVE_INFINITY, Math::log10(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL(Double::NEGATIVE_INFINITY, Math::log10(0.0));
//    CPPUNIT_ASSERT_EQUAL(Double::NEGATIVE_INFINITY, Math::log10(+0.0));
//    CPPUNIT_ASSERT_EQUAL(Double::NEGATIVE_INFINITY, Math::log10(-0.0));
//
//    CPPUNIT_ASSERT_EQUAL(3.0, Math::log10(1000.0));
//    CPPUNIT_ASSERT_EQUAL(14.0, Math::log10(Math::pow(10, 14)));
//
//    CPPUNIT_ASSERT_EQUAL(3738956126954LL, (long long)(Math::log10(5482.2158)*1000000000000.0));
//    CPPUNIT_ASSERT_EQUAL(14661551142893LL, (long long)(Math::log10(458723662312872.125782332587)*1000000000000.0));
//    CPPUNIT_ASSERT_EQUAL(-908382862219LL, (long long)(Math::log10(0.12348583358871)*1000000000000.0));
//    CPPUNIT_ASSERT_EQUAL(308254715559916LL, (long long)(Math::log10(Double::MAX_VALUE)*1000000000000.0));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_log1p_D() {
//
//	// Test for special cases
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Double::isNaN(Math::log1p(Double::NaN)));
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Double::isNaN(Math::log1p(-32.0482175)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::log1p(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL( Double::doubleToLongBits(0.0),
//                          Double::doubleToLongBits(Math::log1p(0.0)));
//    CPPUNIT_ASSERT_EQUAL( Double::doubleToLongBits(+0.0),
//                          Double::doubleToLongBits(Math::log1p(+0.0)));
//    CPPUNIT_ASSERT_EQUAL( Double::doubleToLongBits(-0.0),
//                          Double::doubleToLongBits(Math::log1p(-0.0)));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -0.2941782295312541", -0.2941782295312541,
//            Math::log1p(-0.254856327));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 7.368050685564151", 7.368050685564151,
//            Math::log1p(1583.542));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 0.4633708685409921", 0.4633708685409921,
//            Math::log1p(0.5894227));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 709.782712893384", 709.782712893384,
//            Math::log1p(Double::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return Double::MIN_VALUE", Double::MIN_VALUE,
//            Math::log1p(Double::MIN_VALUE));
//}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_maxDD() {
    // Test for method double decaf.lang.Math::max(double, double)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect double max value", 1908897.6000089,
            Math::max(-1908897.6000089, 1908897.6000089));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect double max value",
            1908897.6000089, Math::max(2.0, 1908897.6000089));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect double max value", -2.0,
            Math::max(-2.0, -1908897.6000089));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_maxFF() {
    // Test for method float decaf.lang.Math::max(float, float)
    CPPUNIT_ASSERT_MESSAGE("Incorrect float max value", Math::max(-1908897.600f,
            1908897.600f) == 1908897.600f);
    CPPUNIT_ASSERT_MESSAGE("Incorrect float max value",
            Math::max(2.0f, 1908897.600f) == 1908897.600f);
    CPPUNIT_ASSERT_MESSAGE("Incorrect float max value",
            Math::max(-2.0f, -1908897.600f) == -2.0f);
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_maxII() {
    // Test for method int decaf.lang.Math::max(int, int)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect int max value",
            19088976, Math::max(-19088976, 19088976));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect int max value",
            19088976, Math::max(20, 19088976));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect int max value", -20, Math::max(-20, -19088976));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_maxJJ() {
    // Test for method long long decaf.lang.Math::max(long long, long long)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect long max value",
            19088976000089LL, Math::max(-19088976000089LL, 19088976000089LL));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect long max value",
            19088976000089LL, Math::max(20LL, 19088976000089LL));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect long max value",
            -20LL, Math::max(-20LL, -19088976000089LL));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_minDD() {
    // Test for method double decaf.lang.Math::min(double, double)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect double min value",
            -1908897.6000089, Math::min(-1908897.6000089, 1908897.6000089));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect double min value",
            2.0, Math::min(2.0, 1908897.6000089));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect double min value",
            -1908897.6000089, Math::min(-2.0,-1908897.6000089));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_minFF() {
    // Test for method float decaf.lang.Math::min(float, float)
    CPPUNIT_ASSERT_MESSAGE("Incorrect float min value", Math::min(-1908897.600f,
            1908897.600f) == -1908897.600f);
    CPPUNIT_ASSERT_MESSAGE("Incorrect float min value",
            Math::min(2.0f, 1908897.600f) == 2.0f);
    CPPUNIT_ASSERT_MESSAGE("Incorrect float min value",
            Math::min(-2.0f, -1908897.600f) == -1908897.600f);
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_minII() {
    // Test for method int decaf.lang.Math::min(int, int)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect int min value",
            -19088976, Math::min(-19088976, 19088976));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect int min value", 20, Math::min(20, 19088976));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect int min value",
            -19088976, Math::min(-20, -19088976));

}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_minJJ() {
    // Test for method long long decaf.lang.Math::min(long long, long long)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect long min value",
            -19088976000089LL, Math::min(-19088976000089LL, 19088976000089LL));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect long min value",
            20LL, Math::min(20LL, 19088976000089LL));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect long min value",
            -19088976000089LL, Math::min(-20LL, -19088976000089LL));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_powDD() {
    // Test for method double decaf.lang.Math::pow(double, double)
    CPPUNIT_ASSERT_MESSAGE("pow returned incorrect value",
            (long) Math::pow(2, 8) == 256l);
    CPPUNIT_ASSERT_MESSAGE("pow returned incorrect value",
            Math::pow(2, -8) == 0.00390625);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect root returned1",
                         2, (int)Math::sqrt(Math::pow(Math::sqrt(2), 4)));
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_rintD() {
//    // Test for method double decaf.lang.Math::rint(double)
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to round properly - up to odd",
//            3.0, Math::rint(2.9));
//    CPPUNIT_ASSERT_MESSAGE("Failed to round properly - NaN",
//            Double::isNaN(Math::rint(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to round properly down to even",
//            2.0, Math::rint(2.1));
//    CPPUNIT_ASSERT_MESSAGE("Failed to round properly to even",
//            Math::rint(2.5) == 2.0);
//}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_roundD() {
    // Test for method long decaf.lang.Math::round(double)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect rounding of a float",
            -91LL, Math::round(-90.89));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_roundF() {
    // Test for method int decaf.lang.Math::round(float)
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect rounding of a float",
            -91, Math::round(-90.89f));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_signum_D() {
    CPPUNIT_ASSERT(Double::isNaN(Math::signum(Double::NaN)));
    CPPUNIT_ASSERT(Double::isNaN(Math::signum(Double::NaN)));
    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(0.0),
            Double::doubleToLongBits(Math::signum(0.0)));
    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(+0.0),
            Double::doubleToLongBits(Math::signum(+0.0)));
    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(-0.0),
            Double::doubleToLongBits(Math::signum(-0.0)));

    CPPUNIT_ASSERT_EQUAL(1.0, Math::signum(253681.2187962));
    CPPUNIT_ASSERT_EQUAL(-1.0, Math::signum(-125874693.56));
    CPPUNIT_ASSERT_EQUAL(1.0, Math::signum(1.2587E-308));
    CPPUNIT_ASSERT_EQUAL(-1.0, Math::signum(-1.2587E-308));

    CPPUNIT_ASSERT_EQUAL(1.0, Math::signum(Double::MAX_VALUE));
    CPPUNIT_ASSERT_EQUAL(1.0, Math::signum(Double::MIN_VALUE));
    CPPUNIT_ASSERT_EQUAL(-1.0, Math::signum(-Double::MAX_VALUE));
    CPPUNIT_ASSERT_EQUAL(-1.0, Math::signum(-Double::MIN_VALUE));
    CPPUNIT_ASSERT_EQUAL(1.0, Math::signum(Double::POSITIVE_INFINITY));
    CPPUNIT_ASSERT_EQUAL(-1.0, Math::signum(Double::NEGATIVE_INFINITY));
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_signum_F() {
    CPPUNIT_ASSERT(Float::isNaN(Math::signum(Float::NaN)));
    CPPUNIT_ASSERT_EQUAL(Float::floatToIntBits(0.0f),
            Float::floatToIntBits(Math::signum(0.0f)));
    CPPUNIT_ASSERT_EQUAL(Float::floatToIntBits(+0.0f),
            Float::floatToIntBits(Math::signum(+0.0f)));
    CPPUNIT_ASSERT_EQUAL(Float::floatToIntBits(-0.0f),
            Float::floatToIntBits(Math::signum(-0.0f)));

    CPPUNIT_ASSERT_EQUAL(1.0f, Math::signum(253681.2187962f));
    CPPUNIT_ASSERT_EQUAL(-1.0f, Math::signum(-125874693.56f));
    CPPUNIT_ASSERT_EQUAL(1.0f, Math::signum(1.2587E-11f));
    CPPUNIT_ASSERT_EQUAL(-1.0f, Math::signum(-1.2587E-11f));

    CPPUNIT_ASSERT_EQUAL(1.0f, Math::signum(Float::MAX_VALUE));
    CPPUNIT_ASSERT_EQUAL(1.0f, Math::signum(Float::MIN_VALUE));
    CPPUNIT_ASSERT_EQUAL(-1.0f, Math::signum(-Float::MAX_VALUE));
    CPPUNIT_ASSERT_EQUAL(-1.0f, Math::signum(-Float::MIN_VALUE));
    CPPUNIT_ASSERT_EQUAL(1.0f, Math::signum(Float::POSITIVE_INFINITY));
    CPPUNIT_ASSERT_EQUAL(-1.0f, Math::signum(Float::NEGATIVE_INFINITY));
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_sinD() {
//    // Test for method double decaf.lang.Math::sin(double)
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect answer",
//        0.0, Math::sin(0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect answer",
//        0.8414709848078965, Math::sin(1));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_sinh_D() {
//    // Test for special situations
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Double::isNaN(Math::sinh(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::sinh(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return NEGATIVE_INFINITY",
//            Double::NEGATIVE_INFINITY, Math::sinh(Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(0.0),
//            Double::doubleToLongBits(Math::sinh(0.0)));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(+0.0),
//            Double::doubleToLongBits(Math::sinh(+0.0)));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(-0.0),
//            Double::doubleToLongBits(Math::sinh(-0.0)));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::sinh(1234.56));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return NEGATIVE_INFINITY",
//            Double::NEGATIVE_INFINITY, Math::sinh(-1234.56));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0000000000001666E-6",
//            1.0000000000001666E-6, Math::sinh(0.000001));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -1.0000000000001666E-6",
//            -1.0000000000001666E-6, Math::sinh(-0.000001));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 5.11538644196386",
//             5.11538644196386, Math::sinh( 2.33482 ) );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return POSITIVE_INFINITY",
//            Double::POSITIVE_INFINITY, Math::sinh(Double::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 4.9E-324", 4.9E-324,
//            Math::sinh(Double::MIN_VALUE));
//}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_sqrtD() {
    // Test for method double decaf.lang.Math::sqrt(double)
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Incorrect root returned2", 7.0, Math::sqrt(49.0) );
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_tanD() {
//    // Test for method double decaf.lang.Math::tan(double)
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect answer",
//            0.0, Math::tan(0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect answer",
//            1.5574077246549023, Math::tan(1));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_tanh_D() {
//    // Test for special situations
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Double::isNaN(Math::tanh(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return +1.0", +1.0,
//            Math::tanh(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -1.0", -1.0,
//            Math::tanh(Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(0.0),
//            Double::doubleToLongBits(Math::tanh(0.0)));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(+0.0),
//            Double::doubleToLongBits(Math::tanh(+0.0)));
//    CPPUNIT_ASSERT_EQUAL(Double::doubleToLongBits(-0.0),
//            Double::doubleToLongBits(Math::tanh(-0.0)));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0", 1.0, Math::tanh(1234.56));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return -1.0", -1.0, Math::tanh(-1234.56));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 9.999999999996666E-7",
//            9.999999999996666E-7, Math::tanh(0.000001));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 0.981422884124941", 0.981422884124941,
//            Math::tanh(2.33482));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 1.0",
//            1.0, Math::tanh(Double::MAX_VALUE));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should return 4.9E-324", 4.9E-324,
//            Math::tanh(Double::MIN_VALUE));
//}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_random() {
    // There isn't a place for these tests so just stick them here
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong value E",
        4613303445314885481LL, Double::doubleToLongBits(Math::E));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong value PI",
        4614256656552045848LL, Double::doubleToLongBits(Math::PI));

    for (int i = 500; i >= 0; i--) {
        double d = Math::random();
        CPPUNIT_ASSERT_MESSAGE("Generated number is out of range: ",
                d >= 0.0 && d < 1.0);
    }
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_toRadiansD() {
    for (double d = 500; d >= 0; d -= 1.0) {
        double converted = Math::toDegrees(Math::toRadians(d));
        CPPUNIT_ASSERT_MESSAGE("Converted number not equal to original. d ",
                converted >= d * 0.99999999 && converted <= d * 1.00000001);
    }
}

////////////////////////////////////////////////////////////////////////////////
void MathTest::test_toDegreesD() {
    for (double d = 500; d >= 0; d -= 1.0) {
        double converted = Math::toRadians(Math::toDegrees(d));
        CPPUNIT_ASSERT_MESSAGE("Converted number not equal to original. d ",
                converted >= d * 0.99999999 && converted <= d * 1.00000001);
    }
}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_ulp_D() {
//    // Test for special cases
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Double::isNaN(Math::ulp(Double::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Double::POSITIVE_INFINITY,
//            Math::ulp(Double::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Double::POSITIVE_INFINITY,
//            Math::ulp(Double::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Double::MIN_VALUE, Math::ulp(0.0) );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Double::MIN_VALUE,
//            Math::ulp(+0.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Double::MIN_VALUE,
//            Math::ulp(-0.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Math::pow(2, 971),
//            Math::ulp(Double::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Math::pow(2, 971),
//            Math::ulp(-Double::MAX_VALUE));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Double::MIN_VALUE, Math::ulp(Double::MIN_VALUE) );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Double::MIN_VALUE, Math::ulp(-Double::MIN_VALUE) );
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", 2.220446049250313E-16,
//            Math::ulp(1.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", 2.220446049250313E-16,
//            Math::ulp(-1.0));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", 2.2737367544323206E-13,
//            Math::ulp(1153.0));
//}

////////////////////////////////////////////////////////////////////////////////
//void MathTest::test_ulp_f() {
//    // Test for special cases
//    CPPUNIT_ASSERT_MESSAGE("Should return NaN", Float::isNaN(Math::ulp(Float::NaN)));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Float::POSITIVE_INFINITY, Math::ulp(Float::POSITIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Float::POSITIVE_INFINITY, Math::ulp(Float::NEGATIVE_INFINITY));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Float::MIN_VALUE, Math::ulp(0.0f));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", Float::MIN_VALUE, Math
//            ::ulp(+0.0f));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            Math::ulp(-0.0f), Float::MIN_VALUE );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            2.028241E31f, Math::ulp(Float::MAX_VALUE));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            2.028241E31f, Math::ulp(-Float::MAX_VALUE));
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            1.4E-45f, Math::ulp( Float::MIN_VALUE ) );
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            1.4E-45f, Math::ulp( -Float::MIN_VALUE ) );
//
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            1.1920929E-7f, Math::ulp(1.0f));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value", 1.1920929E-7f,
//            Math::ulp(-1.0f));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            1.2207031E-4f, Math::ulp(1153.0f));
//    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value",
//            5.6E-45f, Math::ulp(9.403954E-38f) );
//}
