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

#ifndef _DECAF_LANG_MATH_H_
#define _DECAF_LANG_MATH_H_

#include <decaf/util/Config.h>

// On some systems there is a min and max macro defined.
#undef min
#undef max

namespace decaf{
namespace lang{

    /**
     * The class <code>Math</code> contains methods for performing basic
     * numeric operations such as the elementary exponential, logarithm,
     * square root, and trigonometric functions.
     */
    class DECAF_API Math {
    public:

        static const double E;
        static const double PI;

    public:

        Math() {}
        virtual ~Math() {}

    public:

        /**
         * Returns the absolute value of an int value. If the argument is not
         * negative, the argument is returned. If the argument is negative, the
         * negation of the argument is returned.
         * @param value - the value to return the abs of
         * @returns the value if positive, otherwise the negative of value
         */
        static int abs( int value ) {
            return value < 0 ? -value : value;
        }

        /**
         * Returns the absolute value of an long long value. If the argument is not
         * negative, the argument is returned. If the argument is negative, the
         * negation of the argument is returned.
         * @param value - the value to return the abs of
         * @returns the value if positive, otherwise the negative of value
         */
        static long long abs( long long value ) {
            return value < 0 ? -value : value;
        }

        /**
         * Returns the absolute value of a float value. If the argument is not
         * negative, the argument is returned. If the argument is negative,
         * the negation of the argument is returned. Special cases:
         *
         * o If the argument is positive zero or negative zero, the result is
         *   positive zero.
         * o If the argument is infinite, the result is positive infinity.
         * o If the argument is NaN, the result is NaN.
         *
         * In other words, the result is the same as the value of the expression:
         *    Float::intBitsToFloat( 0x7fffffff & Float::floatToIntBits( value ) )
         *
         * @param value - the value to return the abs of
         * @returns the value if positive, otherwise the negative of value
         */
        static float abs( float value );

        /**
         * Returns the absolute value of a double value. If the argument is not
         * negative, the argument is returned. If the argument is negative,
         * the negation of the argument is returned. Special cases:
         *
         * o If the argument is positive zero or negative zero, the result is
         *   positive zero.
         * o If the argument is infinite, the result is positive infinity.
         * o If the argument is NaN, the result is NaN.
         *
         * In other words, the result is the same as the value of the expression:
         *    Double::longBitsToDouble( 0x7fffffffffffffffULL &
         *                              Double::doubleToLongBits( value ) )
         *
         * @param value - the value to return the abs of
         * @returns the value if positive, otherwise the negative of value
         */
        static double abs( double value );

        /**
         * Returns the arc cosine of an angle, in the range of 0.0 through pi.
         * Special case:
         *
         *  o If the argument is NaN or its absolute value is greater than 1, then
         *    the result is NaN.
         *
         * @param value - the value to return the arc cosine of.
         * @returns arc cosine of value in radians.
         */
        //static double acos( double value );

        /**
         * Returns the arc sine of an angle, in the range of -pi/2 through pi/2.
         * Special cases:
         *
         *  o If the argument is NaN or its absolute value is greater than 1, then
         *    the result is NaN.
         *  o If the argument is zero, then the result is a zero with the same sign
         *    as the argument.
         *
         * @param value - the value to return the arc cosine of.
         * @returns arc cosine of value in radians.
         */
        //static double asin( double value );

        /**
         * Returns the arc tangent of an angle, in the range of -pi/2 through pi/2.
         * Special cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is zero, then the result is a zero with the same sign
         *    as the argument.
         *
         * @param value - the value to return the arc cosine of.
         * @returns arc tangent of value in radians.
         */
        //static double atan( double value );

        /**
         * Converts rectangular coordinates (x, y) to polar (r, theta). This method
         * computes the phase theta by computing an arc tangent of y/x in the range
         * of -pi to pi. Special cases:
         *
         *  o If either argument is NaN, then the result is NaN.
         *  o If the first argument is positive zero and the second argument is
         *    positive, or the first argument is positive and finite and the second
         *    argument is positive infinity, then the result is positive zero.
         *  o If the first argument is negative zero and the second argument is
         *    positive, or the first argument is negative and finite and the second
         *    argument is positive infinity, then the result is negative zero.
         *  o If the first argument is positive zero and the second argument is
         *    negative, or the first argument is positive and finite and the second
         *    argument is negative infinity, then the result is the double value
         *    closest to pi.
         *  o If the first argument is negative zero and the second argument is
         *    negative, or the first argument is negative and finite and the second
         *    argument is negative infinity, then the result is the double value
         *    closest to -pi.
         *  o If the first argument is positive and the second argument is positive
         *    zero or negative zero, or the first argument is positive infinity and
         *    the second argument is finite, then the result is the double value
         *    closest to pi/2.
         *  o If the first argument is negative and the second argument is positive
         *    zero or negative zero, or the first argument is negative infinity and
         *    the second argument is finite, then the result is the double value
         *    closest to -pi/2.
         *  o If both arguments are positive infinity, then the result is the double
         *    value closest to pi/4.
         *  o If the first argument is positive infinity and the second argument is
         *    negative infinity, then the result is the double value closest to 3*pi/4.
         *  o If the first argument is negative infinity and the second argument is
         *    positive infinity, then the result is the double value closest to -pi/4.
         *  o If both arguments are negative infinity, then the result is the double
         *    value closest to -3*pi/4.
         *
         * @param y - the ordinate coordinate
         * @param x - the abscissa coordinate
         * @returns the theta component of the point (r, theta) in polar coordinates
         * that corresponds to the point (x, y) in Cartesian coordinates.
         */
        //static double atan2( double x, double y );

        /**
         * Returns the cube root of a double value. For positive finite x,
         * cbrt(-x) == -cbrt(x); that is, the cube root of a negative value is the
         * negative of the cube root of that value's magnitude. Special cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is infinite, then the result is an infinity with the
         *    same sign as the argument.
         *  o If the argument is zero, then the result is a zero with the same sign
         *    as the argument.
         *
         * @param value - the double to compute the cube root of
         * @returns the cube root of value
         */
        //static double cbrt( double value );

        /**
         * Returns the trigonometric cosine of an angle. Special cases:
         *
         *   o If the argument is NaN or an infinity, then the result is NaN.
         *
         * @param value - an value in radians
         * @returns the cosine of the argument.
         */
        //static double cos( double value );

        /**
         * Returns the hyperbolic cosine of a double value. The hyperbolic cosine
         * of x is defined to be (ex + e-x)/2 where e is Euler's number.
         * Special cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is infinite, then the result is positive infinity.
         *  o If the argument is zero, then the result is 1.0.
         *
         * @param value - the number whose hyperbolic cosine is to be found
         * @return the hyperbolic cosine of value
         */
        //static double cosh( double value );

        /**
         * Returns the trigonometric sine of an angle. Special case:
         *
         *  o If the argument is NaN or an infinity, then the result is NaN.
         *  o If the argument is zero, then the result is a zero with the same sign
         *    as the argument.
         *
         * @param value - the number whose sin is to be found
         * @return the sine of value
         */
        //static double sin( double value );

        /**
         * Returns the hyperbolic sine of a double value. The hyperbolic sine of x
         * is defined to be (ex - e-x)/2 where e is Euler's number.
         * Special cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is infinite, then the result is an infinity with
         *    the same sign as the argument.
         *  o If the argument is zero, then the result is a zero with the same
         *    sign as the argument.
         *
         * @param value - the number whose hyperbolic sin is to be found
         * @return the hyperbolic sine of value
         */
        //static double sinh( double value );

        /**
         * Returns the trigonometric tangent of an angle. Special cases:
         *
         *  o If the argument is NaN or an infinity, then the result is NaN.
         *  o If the argument is zero, then the result is a zero with the same
         *    sign as the argument.
         *
         * @param value - the number whose tangent is to be found
         * @return the tangent of value
         */
        //static double tan( double value );

        /**
         * Returns the hyperbolic tangent of a double value. The hyperbolic
         * tangent of x is defined to be (ex - e-x)/(ex + e-x), in other words,
         * sinh(x)/cosh(x). Note that the absolute value of the exact tanh is
         * always less than 1. Special cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is zero, then the result is a zero with the same
         *    sign as the argument.
         *  o If the argument is positive infinity, then the result is +1.0.
         *  o If the argument is negative infinity, then the result is -1.0.
         *
         * @param value - the number whose hyperbolic tangent is to be found
         * @return the hyperbolic cosine of value
         */
        //static double tanh( double value );

        /**
         * Returns the correctly rounded positive square root of a double value.
         * Special cases:
         *
         *  o If the argument is NaN or less than zero, then the result is NaN.
         *  o If the argument is positive infinity, then the result is positive infinity.
         *  o If the argument is positive zero or negative zero, then the result is
         *    the same as the argument.
         *
         * Otherwise, the result is the double value closest to the true mathematical
         * square root of the argument value.
         * @param value - the value to find the square root of
         * @param the square root of the argument.
         */
        static double sqrt( double value );

        /**
         * Returns the value of the first argument raised to the power of the second
         * argument. Special cases:
         *
         *  o If the second argument is positive or negative zero, then the result
         *    is 1.0.
         *  o If the second argument is 1.0, then the result is the same as the first
         *    argument.
         *  o If the second argument is NaN, then the result is NaN.
         *  o If the first argument is NaN and the second argument is nonzero,
         *    then the result is NaN.
         *
         * @param base - the base
         * @param exp - the exponent
         * @return the base raised to the power of exp.
         */
        static double pow( double base, double exp );

        /**
         * Returns the double value that is closest in value to the argument and
         * is equal to a mathematical integer. If two double values that are
         * mathematical integers are equally close, the result is the integer
         * value that is even. Special cases:
         *
         *  o If the argument value is already equal to a mathematical integer,
         *    then the result is the same as the argument.
         *  o If the argument is NaN or an infinity or positive zero or negative
         *    zero, then the result is the same as the argument.
         *
         * @param value - the value to round to the nearest integer
         * @returns the rounded value
         */
        //static double rint( double value );

        /**
         * Returns the smaller of two short values. That is, the result is
         * the argument closer to the value of
         * <code>decaf.lang.Short::MIN_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static short min( short a, short b ) {
            return ( a <= b ? a : b );
        }

        /**
         * Returns the smaller of two <code>int</code> values. That is,
         * the result the argument closer to the value of
         * <code>Integer::MIN_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static int min( int a, int b ) {
            return ( a <= b ? a : b );
        }

        /**
         * Returns the smaller of two <code>unsigned int</code> values. That is,
         * the result the argument closer to the value of
         * <code>Integer::MIN_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static unsigned int min( unsigned int a, unsigned int b ) {
            return ( a <= b ? a : b );
        }

        /**
         * Returns the smaller of two <code>long long</code> values. That is,
         * the result the argument closer to the value of
         * <code>Long::MIN_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static long long min( long long a, long long b ) {
            return ( a <= b ? a : b );
        }

        /**
         * Returns the smaller of two float values. That is, the result is the
         * value closer to negative infinity. If the arguments have the same value,
         * the result is that same value. If either value is NaN, then the result
         * is NaN. Unlike the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one argument
         * is positive zero and the other is negative zero, the result is negative
         * zero.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static float min( float a, float b );

        /**
         * Returns the smaller of two double values. That is, the result is the
         * value closer to negative infinity. If the arguments have the same value,
         * the result is that same value. If either value is NaN, then the result
         * is NaN. Unlike the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one argument
         * is positive zero and the other is negative zero, the result is negative
         * zero.
         * @param a - an argument.
         * @param b - another argument.
         * @return the smaller of <code>a</code> and <code>b</code>.
         */
        static double min( double a, double b );

        /**
         * Returns the larger of two <code>short</code> values. That is,
         * the result the argument closer to the value of
         * <code>Short::MAX_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static short max( short a, short b ) {
            return ( a >= b ? a : b );
        }

        /**
         * Returns the larger of two <code>int</code> values. That is,
         * the result the argument closer to the value of
         * <code>Integer::MAX_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static int max( int a, int b ) {
            return ( a >= b ? a : b );
        }

        /**
         * Returns the larger of two <code>long long</code> values. That is,
         * the result the argument closer to the value of
         * <code>Long::MAX_VALUE</code>.  If the arguments have the same
         * value, the result is that same value.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static long long max( long long a, long long b ) {
            return ( a >= b ? a : b );
        }

        /**
         * Returns the greater of two float values. That is, the result is the
         * argument closer to positive infinity. If the arguments have the same
         * value, the result is that same value. If either value is NaN, then the
         * result is NaN. Unlike the numerical comparison operators, this method
         * considers negative zero to be strictly smaller than positive zero. If
         * one argument is positive zero and the other negative zero, the result
         * is positive zero.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static float max( float a, float b );

        /**
         * Returns the greater of two double values. That is, the result is the
         * argument closer to positive infinity. If the arguments have the same value,
         * the result is that same value. If either value is NaN, then the result is
         * NaN. Unlike the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one argument
         * is positive zero and the other negative zero, the result is positive zero.
         * @param a - an argument.
         * @param b - another argument.
         * @return the larger of <code>a</code> and <code>b</code>.
         */
        static double max( double a, double b );

        /**
         * Returns the natural logarithm (base e) of a double  value. Special cases:
         *
         *  o If the argument is NaN or less than zero, then the result is NaN.
         *  o If the argument is positive infinity, then the result is
         *    positive infinity.
         *  o If the argument is positive zero or negative zero, then the result
         *    is negative infinity.
         *
         * @param value the value to compute the natural log of.
         * @returns the natural log of value.
         */
        //static double log( double value );

        /**
         * Returns the base 10 logarithm of a double value. Special cases:
         *
         *   o If the argument is NaN or less than zero, then the result is NaN.
         *   o If the argument is positive infinity, then the result is positive
         *     infinity.
         *   o If the argument is positive zero or negative zero, then the result is
         *     negative infinity.
         *   o If the argument is equal to 10n for integer n, then the result is n.
         *
         * @param value - the value to operate on
         * @returns the long base 10 of value
         */
        //static double log10( double value );

        /**
         * Returns the natural logarithm of the sum of the argument and 1. Note that
         * for small values x, the result of log1p(x) is much closer to the true
         * result of ln(1 + x) than the floating-point evaluation of log(1.0+x).
         *
         * Special cases:
         *
         *   o If the argument is NaN or less than -1, then the result is NaN.
         *   o If the argument is positive infinity, then the result is positive
         *     infinity.
         *   o If the argument is negative one, then the result is negative infinity.
         *   o If the argument is zero, then the result is a zero with the same sign
         *     as the argument.
         *
         * @param value - the value to operate on
         * @returns the the value ln(x + 1), the natural log of x + 1
         */
        //static double log1p( double value );

        /**
         * Returns the smallest (closest to negative infinity) double value that is
         * greater than or equal to the argument and is equal to a mathematical
         * integer. Special cases:
         *
         *   o If the argument value is already equal to a mathematical integer,
         *     then the result is the same as the argument.
         *   o If the argument is NaN or an infinity or positive zero or negative
         *     zero, then the result is the same as the argument.
         *   o If the argument value is less than zero but greater than -1.0, then
         *     the result is negative zero.
         *
         * Note that the value of Math.ceil(x) is exactly the value of -Math.floor(-x).
         * @param value - the value to find the ceiling of
         * @returns the smallest (closest to negative infinity) floating-point value
         * that is greater than or equal to the argument and is equal to a
         * mathematical integer.
         */
        static double ceil( double value );

        /**
         * Returns the largest (closest to positive infinity) double value that is
         * less than or equal to the argument and is equal to a mathematical integer.
         * Special cases:
         *
         *    o If the argument value is already equal to a mathematical integer,
         *      then the result is the same as the argument.
         *    o If the argument is NaN or an infinity or positive zero or negative
         *      zero, then the result is the same as the argument.
         *
         * @param value - the value to find the floor of
         * @returns the largest (closest to positive infinity) floating-point value
         * that less than or equal to the argument and is equal to a mathematical
         * integer.
         */
        static double floor( double value );

        /**
         * Returns the closest int to the argument. The result is rounded to an
         * integer by adding 1/2, taking the floor of the result, and casting the
         * result to type int. In other words, the result is equal to the value
         * of the expression:  (int)Math.floor( a + 0.5f )
         *
         *   o If the argument is NaN, the result is 0.
         *   o If the argument is negative infinity or any value less than or equal
         *     to the value of Integer::MIN_VALUE, the result is equal to the value of
         *     Integer::MIN_VALUE.
         *   o If the argument is positive infinity or any value greater than or equal
         *     to the value of Integer::MAX_VALUE, the result is equal to the value of
         *     Integer::MAX_VALUE.
         *
         * @param value - the value to round
         * @returns the value of the argument rounded to the nearest integral value.
         */
        static int round( float value );

        /**
         * Returns the closest long long to the argument. The result is rounded to
         * an integer by adding 1/2, taking the floor of the result, and casting the
         * result to type long long. In other words, the result is equal to the value
         * of the expression:  (long long)Math.floor(a + 0.5d)
         *
         *   o If the argument is NaN, the result is 0.
         *   o If the argument is negative infinity or any value less than or equal
         *     to the value of Long::MIN_VALUE, the result is equal to the value of
         *     Long::MIN_VALUE.
         *   o If the argument is positive infinity or any value greater than or equal
         *     to the value of Long::MAX_VALUE, the result is equal to the value of
         *     Long::MAX_VALUE.
         *
         * @param value - the value to round
         * @returns the value of the argument rounded to the nearest integral value.
         */
        static long long round( double value );

        /**
         * Computes the remainder operation on two arguments as prescribed by the
         * IEEE 754 standard. The remainder value is mathematically equal to
         * f1 - f2 × n, where n is the mathematical integer closest to the exact
         * mathematical value of the quotient f1/f2, and if two mathematical
         * integers are equally close to f1/f2, then n is the integer that is even.
         * If the remainder is zero, its sign is the same as the sign of the first
         * argument. Special cases:
         *
         *  o If either argument is NaN, or the first argument is infinite, or the
         *    second argument is positive zero or negative zero, then the result is
         *    NaN.
         *  o If the first argument is finite and the second argument is infinite,
         *    then the result is the same as the first argument.
         *
         * @param f1 - the dividend.
         * @param f2 - the divisor
         * @return the IEEE remainder of value
         */
        //static double IEEEremainder( double f1, double f2 );

        /**
         * Returns a double value with a positive sign, greater than or equal to 0.0
         * and less than 1.0. Returned values are chosen pseudorandomly with
         * (approximately) uniform distribution from that range.
         *
         * When this method is first called, it creates a single new pseudorandom-number
         * generator; This new pseudorandom-number generator is used thereafter for all
         * calls to this method and is used nowhere else.
         *
         * This method is properly synchronized to allow correct use by more than
         * one thread. However, if many threads need to generate pseudorandom numbers
         * at a great rate, it may reduce contention for each thread to have its
         * own pseudorandom-number generator.
         * @returns a pseudorandom double greater than or equal to 0.0 and
         * less than 1.0.
         */
        static double random();

        /**
         * Returns Euler's number e raised to the power of a double value.
         * Special cases:
         *
         *  o If the argument is NaN, the result is NaN.
         *  o If the argument is positive infinity, then the result is positive infinity.
         *  o If the argument is negative infinity, then the result is positive zero.
         *
         * @param value - the exponent to raise e to
         * @returns the value e^a, where e is the base of the natural logarithms.
         */
        //static double exp( double value );

        /**
         * Returns e^x - 1. Note that for values of x near 0, the exact sum of
         * expm1(x) + 1 is much closer to the true result of ex than exp(x).
         * Special cases:
         *
         *  o If the argument is NaN, the result is NaN.
         *  o If the argument is positive infinity, then the result is positive infinity.
         *  o If the argument is negative infinity, then the result is -1.0.
         *  o If the argument is zero, then the result is a zero with the same sign as
         *    the argument.
         *
         * @param value - the value to raise e^x - 1
         * @returns the value ex - 1.
         */
        //static double expm1( double value );

        /**
         * Returns sqrt(x^2 + y^2) without intermediate overflow or underflow.
         * Special cases:
         *
         * If either argument is infinite, then the result is positive infinity.
         * If either argument is NaN and neither argument is infinite, then the
         * result is NaN.
         *
         * @param x - an argument
         * @param y - another argument
         * @returns the sqrt(x^2 + y^2) without intermediate overflow or underflow
         */
        //static double hypot( double x, double y );

        /**
         * Returns the signum function of the argument; zero if the argument is zero,
         * 1.0f if the argument is greater than zero, -1.0f if the argument is less
         * than zero.  Special Cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is positive zero or negative zero, then the result is
         *    the same as the argument.
         *
         * @param value - the floating-point value whose signum is to be returned
         * @returns the signum function of the argument
         */
        static float signum( float value );

        /**
         * Returns the signum function of the argument; zero if the argument is zero,
         * 1.0f if the argument is greater than zero, -1.0f if the argument is less
         * than zero.  Special Cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is positive zero or negative zero, then the result is
         *    the same as the argument.
         *
         * @param value - the floating-point value whose signum is to be returned
         * @returns the signum function of the argument
         */
        static double signum( double value );

        /**
         * Returns the measure in radians of the supplied degree angle
         * @param angdeg - an angle in degrees
         * @return the radian measure of the angle.
         */
        static double toRadians( double angdeg ) {
            return angdeg / 180 * PI;
        }

        /**
         * Returns the measure in degrees of the supplied radian angle
         * @param angrad -  an angle in radians
         * @return the degree measure of the angle.
         */
        static double toDegrees( double angrad ) {
            return angrad * 180 / PI;
        }

        /**
         * Returns the size of an ulp of the argument. An ulp of a float value is
         * the positive distance between this floating-point value and the float
         * value next larger in magnitude. Note that for non-NaN x, ulp(-x) == ulp(x).
         * Special Cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is positive or negative infinity, then the result is
         *    positive infinity.
         *  o If the argument is positive or negative zero, then the result is
         *    Float::MIN_VALUE.
         *  o If the argument is ±Float::MAX_VALUE, then the result is equal to 2^104.
         *
         * @param value - the floating-point value whose ulp is to be returned
         * @returns the size of an ulp of the argument
         */
        //static float ulp( float value );

        /**
         * Returns the size of an ulp of the argument. An ulp of a double value is
         * the positive distance between this floating-point value and the double
         * value next larger in magnitude. Note that for non-NaN x, ulp(-x) == ulp(x).
         * Special Cases:
         *
         *  o If the argument is NaN, then the result is NaN.
         *  o If the argument is positive or negative infinity, then the result is
         *    positive infinity.
         *  o If the argument is positive or negative zero, then the result is
         *    Double::MIN_VALUE.
         *  o If the argument is ±Float::MAX_VALUE, then the result is equal to 2^971.
         *
         * @param value - the floating-point value whose ulp is to be returned
         * @returns the size of an ulp of the argument
         */
        //static double ulp( double value );

    };

}}

#endif /*_DECAF_LANG_MATH_H_*/
