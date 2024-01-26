/*
 * The MIT License

   PeakSearch module for Conograph

Copyright (c) <2012> <Ryoko Oishi-Tomiyasu, KEK>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 *
 */
#ifndef _ZMATH_HH_
#define _ZMATH_HH_

#include"../RietveldAnalysisTypes.hh"

// Gets the solution of the polynomial by Newton-Raphson method. 
bool cal_polynomial_solution(const Vec_DP& poly_coef,	// Coefficients of the polynomial f(x).
								const Double& goal_Y,	// find x s.t. goal_Y = f(x).
								const Double& init_X,	// The initial value of x
								Double& X	// Answer.
							);

// Gets the lattice distance d from the angle
// by solving goal_Y = 2*theta + Z + Ds*cos(theta) + Ts*sin(2*theta)
//                   = 2*sin^{-1}(X) + Z + (Ds + 2*Ts*X)*sqrt(1-X^2).
bool cal_angle_dispersion_d(const Vec_DP& peak_shift_param, const Double& wlength, 
								const Double& goal_Y,	// find x s.t. goal_Y = f(x).
								const Double& init_X,	// The initial value of x
								Double& X	// Answer.
							);

// Returns the value of the first argument raised to the power of the second argument.
template <class T>
inline T uipow(const T& a, const UInt4& k)
{
    T p=1;
    for (unsigned char bit = 0x80; bit > 0; bit >>= 1) {
        p *= p;
        if (k & bit) p *= a;
    }
    return p;
}

// Returns the value of the first argument raised to the power of the second argument.
template <class T>
inline T ipow(const T& a, const Int4& k)
{
	if(k>=0) return uipow(a, (unsigned char)k);
	else return uipow(1/a, (unsigned char)(-k));
}

inline const Double& PI2()
{
	static const Double PI2 = 2.0 * M_PI;  // =2π
	return PI2;
}

inline const CMPX_DP& PI2i()
{
	static const CMPX_DP PI2i(0, 2.0 * M_PI);  // =2πi
	return PI2i;
}

// Return the square value of the argument.
// ( If the argument is  negative, return 0.0.)
inline Double sqrt_d(Double r)
{
	if(r<0.0) return 0.0;
	else return sqrt(r);
}

// Return the polynomial value.
inline Double put_polynomial_value(const Vec_DP& poly_coef, const Double& x)
{
	Int4 k = poly_coef.size()-1;
	Double value = poly_coef[k];
	for(; k>0;) value = value*x + poly_coef[--k];
	return value;
}

// On output, sol = alpha*beta.
/*
inline void transformVectorFromLeft(const Mat_DP& alpha, const Vec_DP& beta, Vec_DP& sol)
{
    const Int4 asize = alpha.size();

    sol.resize( asize );
	for (Int4 j=0;j<asize;j++){
		sol[j] = 0.0;
		for (int k=0;k<asize;k++) sol[j] += alpha[j][k] * beta[k];
    }
}
*/

inline Int4 iceil(const Double& num)
{
	static const double d = 0.01;
	return Int4( ceil(num) + d );
}

inline Int4 ifloor(const Double& num)
{
	static const double d = 0.01;
	return Int4( floor(num) + d );
}

#endif /*MATH_HH_*/
