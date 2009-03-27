/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <decaf/lang/Math.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Random.h>

using namespace decaf;
using namespace decaf::lang;

#include <cmath>
#include <cstdio>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////
const double Math::E = 2.718281828459045;
const double Math::PI = 3.141592653589793;

////////////////////////////////////////////////////////////////////////////////
float Math::abs( float value ) {
    return Float::intBitsToFloat( Float::floatToIntBits( value ) & 0x7FFFFFFF );
}

////////////////////////////////////////////////////////////////////////////////
double Math::abs( double value ) {
    return Double::longBitsToDouble( Double::doubleToLongBits( value ) & 0x7FFFFFFFFFFFFFFFULL );
}

////////////////////////////////////////////////////////////////////////////////
//double Math::acos( double value ) {
//    return std::acos( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::asin( double value ) {
//    return std::asin( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::atan( double value ) {
//    return std::atan( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::atan2( double x, double y ) {
//    return std::atan2( x, y );
//}

////////////////////////////////////////////////////////////////////////////////
double Math::random() {
    static util::Random random;
    return random.nextDouble();
}

////////////////////////////////////////////////////////////////////////////////
//double Math::cbrt( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( Double::isInfinite( value ) ) {
//        return value;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return value;
//    }
//
//    return ::cbrt( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::cos( double value ) {
//
//    if( Double::isNaN( value ) || Double::isInfinite( value ) ) {
//        return Double::NaN;
//    }
//
//    return std::cos( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::cosh( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( Double::isInfinite( value ) ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( value == 0.0 ) {
//        return 1.0;
//    }
//
//    return std::cosh( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::sin( double value ) {
//
//    if( Double::isNaN( value ) || Double::isInfinite( value ) ) {
//        return Double::NaN;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return value;
//    }
//
//    return std::sin( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::sinh( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( Double::isInfinite( value ) ) {
//        return value;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return value;
//    }
//
//    return std::sinh( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::tan( double value ) {
//
//    if( Double::isNaN( value ) || value < -1.0 ) {
//        return Double::NaN;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return value;
//    }
//
//    return std::tan( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::tanh( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( value == Double::POSITIVE_INFINITY ) {
//        return 1.0;
//    } else if( value == Double::NEGATIVE_INFINITY ) {
//        return -1.0;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return value;
//    }
//
//    return std::tanh( value );
//}

////////////////////////////////////////////////////////////////////////////////
double Math::sqrt( double value ) {

    if( Double::isNaN( value ) || value < 0.0 ) {
        return Double::NaN;
    } else if( value == Double::POSITIVE_INFINITY ) {
        return Double::POSITIVE_INFINITY;
    } else if( value == 0.0 || value == -0.0 ) {
        return Double::NEGATIVE_INFINITY;
    }

    return std::sqrt( value );
}

////////////////////////////////////////////////////////////////////////////////
//double Math::rint( double value ) {
//
//    if( Double::isNaN( value ) || Double::isInfinite( value ) ) {
//        return value;
//    } else if( value == 0.0 || value == -0.0 ) {
//        return value;
//    }
//
//    return ::rint( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::exp( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( value == Double::POSITIVE_INFINITY ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( value == Double::NEGATIVE_INFINITY ) {
//        return 0.0;
//    }
//
//    return std::exp( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::expm1( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( value == Double::POSITIVE_INFINITY ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( value == Double::NEGATIVE_INFINITY ) {
//        return -1.0;
//    } else if( !( value > 0 || value < 0 ) ) {
//        return value;
//    }
//
//    return ::expm1( value );
//}

////////////////////////////////////////////////////////////////////////////////
float Math::min( float a, float b ) {

    if( a > b ) {
        return b;
    } else if( a < b ) {
        return a;
    }

    // if either arg is NaN, return NaN
    if( a != b ) {
        return Float::NaN;
    }

    // min( +0.0,-0.0) == -0.0
    if( a == 0.0f &&
        ( (Float::floatToIntBits( a ) | Float::floatToIntBits( b )) & 0x80000000) != 0) {

        return 0.0f * (-1.0f);
    }

    return a;
}

////////////////////////////////////////////////////////////////////////////////
double Math::min( double a, double b ) {

    if( a > b ) {
        return b;
    } else if( a < b ) {
        return a;
    }

    // if either arg is NaN, return NaN
    if( a != b ) {
        return Double::NaN;
    }

    // min( +0.0,-0.0) == -0.0
    if( a == 0.0 &&
        ( (Double::doubleToLongBits( a ) | Double::doubleToLongBits( b )) & 0x8000000000000000ULL) != 0 ) {
        return 0.0 * (-1.0);
    }

    return a;
}

////////////////////////////////////////////////////////////////////////////////
float Math::max( float a, float b ) {

    if( a > b ) {
        return a;
    } else if( a < b ) {
        return b;
    }

    // if either arg is NaN, return NaN
    if( a != b ) {
        return Float::NaN;
    }
    // max( +0.0,-0.0) == +0.0
    if( a == 0.0f &&
        ( (Float::floatToIntBits( a ) & Float::floatToIntBits( b ) ) & 0x80000000 ) == 0 ) {

        return 0.0f;
    }

    return a;
}

////////////////////////////////////////////////////////////////////////////////
double Math::max( double a, double b ) {

    if( a > b ) {
        return a;
    } else if( a < b ) {
        return b;
    }

    // if either arg is NaN, return NaN
    if( a != b ) {
        return Double::NaN;
    }
    // max( +0.0,-0.0) == +0.0
    if( a == 0.0f &&
        ( (Double::doubleToLongBits( a ) & Double::doubleToLongBits( b ) ) & 0x80000000 ) == 0 ) {

        return 0.0f;
    }

    return a;
}

////////////////////////////////////////////////////////////////////////////////
//double Math::log( double value ) {
//
//    if( Double::isNaN( value ) || value < 0.0 ) {
//        return Double::NaN;
//    } else if( value == Double::POSITIVE_INFINITY ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return Double::NEGATIVE_INFINITY;
//    }
//
//    return std::log( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::log10( double value ) {
//
//    if( Double::isNaN( value ) || value < 0 ) {
//        return Double::NaN;
//    } else if( value == Double::POSITIVE_INFINITY ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return Double::NEGATIVE_INFINITY;
//    }
//
//    return std::log10( value );
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::log1p( double value ) {
//
//    if( Double::isNaN( value ) || value < -1.0 ) {
//        return Double::NaN;
//    } else if( value == Double::POSITIVE_INFINITY ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( value == -1.0 ) {
//        return Double::NEGATIVE_INFINITY;
//    } else if( !( value < 0 || value > 0 ) ) {
//        return value;
//    }
//
//    return ::log1p( value );
//}

////////////////////////////////////////////////////////////////////////////////
double Math::ceil( double value ) {

    if( Double::isNaN( value ) || Double::isInfinite( value ) ) {
        return value;
    } else if( !( value < 0 || value > 0 ) ) {
        return value;
    } else if( value > -1.0 && value < 0.0 ) {
        return -0.0;
    }

    return std::ceil( value );
}

////////////////////////////////////////////////////////////////////////////////
double Math::floor( double value ) {

    if( Double::isNaN( value ) || Double::isInfinite( value ) ) {
        return value;
    } else if( value > 0 || value < 0 ) {
        return std::floor( value );
    }

    // +-0.0
    return value;
}

////////////////////////////////////////////////////////////////////////////////
int Math::round( float value ) {

    if( Float::isNaN( value ) ) {
        return 0;
    }

    return (int)Math::floor( value + 0.5f );
}

////////////////////////////////////////////////////////////////////////////////
long long Math::round( double value ) {

    if( Double::isNaN( value ) ) {
        return 0;
    }

    return (long long)Math::floor( value + 0.5 );
}

////////////////////////////////////////////////////////////////////////////////
//double Math::IEEEremainder( double f1, double f2 ) {
//
//    if( Double::isNaN( f1 ) || Double::isNaN( f2 ) ||
//        Double::isInfinite( f1 ) || !( f2 < 0 || f2 > 0 ) ) {
//        return Double::NaN;
//    } else if( Double::isInfinite( f2 ) ) {
//        return f1;
//    }
//
//    return ::remainder( f1, f2 );
//}

////////////////////////////////////////////////////////////////////////////////
float Math::signum( float value ) {

    if( Float::isNaN( value ) ) {
        return Float::NaN;
    } else if( value > 0 ) {
        return 1.0;
    } else if( value < 0 ) {
        return -1.0;
    }

    return value;
}

////////////////////////////////////////////////////////////////////////////////
double Math::signum( double value ) {

    if( Double::isNaN( value ) ) {
        return Double::NaN;
    } else if( value > 0 ) {
        return 1.0;
    } else if( value < 0 ) {
        return -1.0;
    }

    return value;
}

////////////////////////////////////////////////////////////////////////////////
//double Math::hypot( double x, double y ) {
//
//    if( Double::isInfinite( x ) || Double::isInfinite( y ) ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( Double::isNaN( x ) || Double::isNaN( y ) ) {
//        return Double::NaN;
//    }
//
//    return std::sqrt( ( x * x ) + ( y * y ) );
//}

////////////////////////////////////////////////////////////////////////////////
double Math::pow( double base, double exp ) {

    if( !( exp < 0 || exp > 0 ) ) {
        return 1.0;
    } else if( Double::isNaN( exp ) ) {
        return Double::NaN;
    } else if( Double::isNaN( base ) && ( exp < 0 || exp > 0 ) ) {
        return Double::NaN;
    }

    return std::pow( base, exp );
}

////////////////////////////////////////////////////////////////////////////////
//float Math::ulp( float value ) {
//
//    if( Float::isNaN( value ) ) {
//        return Float::NaN;
//    } else if( Float::isInfinite( value ) ) {
//        return Float::POSITIVE_INFINITY;
//    } else if( value == Float::MAX_VALUE || value == -Float::MAX_VALUE ) {
//        return (float)pow( 2, 104 );
//    } else if( !( value < 0 || value > 0 ) ) {
//        return Float::MIN_VALUE;
//    }
//
//    value = abs( value );
//    return ::nextafterf( value, Float::MAX_VALUE ) - value;
//}

////////////////////////////////////////////////////////////////////////////////
//double Math::ulp( double value ) {
//
//    if( Double::isNaN( value ) ) {
//        return Double::NaN;
//    } else if( Double::isInfinite( value ) ) {
//        return Double::POSITIVE_INFINITY;
//    } else if( value == Double::MAX_VALUE || value == -Double::MAX_VALUE ) {
//        return pow( 2, 971 );
//    } else if( !( value < 0 || value > 0 ) ) {
//        return Double::MIN_VALUE;
//    }
//
//    value = abs( value );
//    return ::nextafter( value, Double::MAX_VALUE ) - value;
//}
