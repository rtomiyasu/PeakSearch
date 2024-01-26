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
#include <cmath>
#include "../zerror_type/error_out.hh"
#include "akima.hh"

/**
 *         calculate yy (is interpolated value at xx).
 *         c are coefficients of quasi-Hermite polynomials for interpolation.
 *         x, y are original data.
 *
 *         I : n, x(n), y(n), c(4,n), xx
 *         O : yy
 */
static void stval(const Double *x, const Double *y, const int& n, Vec_DP c[4], const Double& xx, Double &yy)
{
    int i;
    Double  h;

    if (n > 1)
      {
        i = n - 2;
        for (int j = 0; j < n; j++)
            if (xx < x[j])
              {
                i = j - 1; break;
              }
        h = xx - x[i];
        yy = c[0][i] + h*(c[1][i]+h*(c[2][i]+h*c[3][i]));
      }
    else
      yy = 0.0;
}

/**
 *         I : n, x(n), y(n)
 *         O : c(4, n), ier
 */
static void qherm(const Double *x, const Double *y, const int& n, Vec_DP c[4], int &ier)
{
    int i;
    Double cm43, cm21, cm1, cm2, cm3, cm4;

    ier = 0;
    if (n <= 2) ier = 1;
    if (ier != 0) return;

    for (int i = 0; i < n-1; i++)
        c[0][i] = (y[i+1]-y[i])/(x[i+1]-x[i]);

    // ----- first order coefficients -----
    for (i = 2; i < n-2; i++)
      {
        cm43 = fabs(c[0][i+1] - c[0][i]);
        cm21 = fabs(c[0][i-1] - c[0][i-2]);
        if ((cm43 != 0.0) || (cm21 != 0.0))
            c[1][i] = (cm43*c[0][i-1] + cm21*c[0][i]) / (cm43 + cm21);
        else
            c[1][i] = (c[0][i] + c[0][i-1]) * 0.5;
      }

    i = n-1;
    cm3 = 2.0 * c[0][i-1] - c[0][i-2];
    cm4 = 3.0 * c[0][i-1] - 2.0*c[0][i-2];
    cm43 = fabs(cm4 - cm3);
    cm21 = fabs(c[0][i-1] - c[0][i-2]);
    if ((cm43 != 0.0) || (cm21 != 0.0))
        c[1][i] = (cm43*c[0][i-1] + cm21*cm3) / (cm43+cm21);
    else
        c[1][i] = (cm3 + c[0][i-1]) * 0.5;

    i = n-2;
    cm4 = cm3;
    cm43 = fabs(cm4 - c[0][i]);
    cm21 = fabs(c[0][i-1] - c[0][i-2]);
    if ((cm43 != 0.0) || (cm21 != 0.0))
        c[1][i] = (cm43*c[0][i-1] + cm21*c[0][i]) / (cm43+cm21);
    else
        c[1][i] = (c[0][i] + c[0][i-1]) * 0.5;

    i = 0;
    cm1 = 3.0 * c[0][i] - 2.0 * c[0][i+1];
    cm2 = 2.0 * c[0][i] - c[0][i+1];
    cm43 = fabs(c[0][i+1] - c[0][i]);
    cm21 = fabs(cm2 - cm1);
    if ((cm43 != 0.0) || (cm21 != 0.0))
        c[1][i] = (cm43*cm2 + cm21*c[0][i]) / (cm43+cm21);
    else
        c[1][i] = (c[0][i] + cm2) * 0.5;

    i = 1;
    cm1 = cm2;
    cm43 = fabs(c[0][i+1] - c[0][i]);
    cm21 = fabs(c[0][i-1] - cm1);
    if ((cm43 != 0.0) || (cm21 != 0.0))
        c[1][i] = (cm43*c[0][i-1] + cm21*c[0][i]) / (cm43+cm21);
    else
        c[1][i] = (c[0][i] + c[0][i-1]) * 0.5;

    // ----- second and third order coefficients -----
    Double dx;
    for (int i = 0; i < n-1; i++)
      {
        dx = x[i+1] - x[i];
        c[2][i] = (3.0*c[0][i] - 2.0*c[1][i] - c[1][i+1]) / dx;
        c[3][i] = (c[1][i] + c[1][i+1] - 2.0*c[0][i]) / (dx*dx);
      }

    // ----- constant term -----
    for (int i = 0; i < n; i++) c[0][i] = y[i];
}


/** Interpolate by using AKIMA method for 3rd Hermitian polynomial.
 *  Subroutine qherm : calculate coefficients of 3rd Hermitian polynomial.
 *  Subroutine stval : interpolates it for xx.
 */
void akima(const Double *x, const Double *y, const int& n, const Double& xx, Double &yy)
{
    int ier;
    Vec_DP c[4];
    for (int i = 0; i < 4; i++) c[i].resize(n);
    
    qherm(x, y, n, c, ier);
    if (ier != 0) { throw ZErrorMessage( "ERROR in akima\n", __FILE__, __LINE__, __FUNCTION__ ); }
    stval(x, y, n, c, xx, yy);
}
