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
#include <assert.h>
#include <cmath>
#include "SavitzkyGolay.hh"
#include "../zerror_type/error_out.hh"

using namespace std;

SavitzkyGolay::SavitzkyGolay()
{
    NumPoints = 9;
//    m_edata.resize(mdataContents);
}

SavitzkyGolay::~SavitzkyGolay()
{
}

/* 
   NumPoints(num1) : The number of data which is used for calculation of a derivative(smoothing) 
                     value at some point.
                     (numPoins must be a odd number; 
                      from 5 to 101 when PolOrder = 2, from 7 to 101 when PolOrder = 4)

   PolOrder(num2) : Order of the fitting polynomial which is used 
                    to calculate the the derivative(smoothing) value of original data.(2 or 4)

   inider(num3) : (0 <= inider < 3) The least order of derivative.(0th derivative is smoothing)
   
   endder(num4) : (0 <= endder < 3) The greatest order of derivative.
*/
void SavitzkyGolay::setParam(int num1, int num2, int num3, int num4)
{
    try{
        if ( 0 < num1 || num1 <= 4 ) PolOrder = num1;
        else throw 1;

        if (num2 > PolOrder + 1  && num2 % 2 == 1) NumPoints = num2;
        else throw 2;

        if (num3 >= 0) inider = num3;
        else throw 3;

        if (num4 < 4) endder = num4;
        else throw 4;

        mdataContents = endder - inider + 1;
        if (mdataContents <= 0) throw 4;
    }
    catch(const int i){
        if( i == 1 ) throw nerror_out_range("Polynomial order", num1, __FILE__, __LINE__, __FUNCTION__);
        else if( i == 2 ) throw nerror_out_range("Number of points", num2, __FILE__, __LINE__, __FUNCTION__);
        else if( i == 3 ) throw nerror_out_range("Min derivative order", num3, __FILE__, __LINE__, __FUNCTION__);
        else if( i == 4 ) throw nerror_out_range("Max derivative order", num4, __FILE__, __LINE__, __FUNCTION__);
    }
}

//const vector<double>& SavitzkyGolay::putErrorData() const
//{
//    return m_edata;
//}

/* nsize : the number of points of nx(ny) that is used for calculation.
   nx : an arry of x_values. (All bin widths needs to be greater than 0.)
   ny : an array of y_values.
   ey : an array of error_values.
   if ey is not NULL, the propageted error values are calculated using ey.
   If the data is not proper, return empty mdata.
*/
// When the 3rd argument is NULL, the size of m_edata equals to mdataContents, mdata[j] is empty for each j. 
// Otherwise, m_edata[j][i] is the propageted error of the smoothed(derivation) mdata[j][i] for each i, j.
void SavitzkyGolay::putSmthData(
const Vec_DP& lx, const Vec_DP& ly, vector< vector<double> >& mdata,
    					const Int4& ibegin, const Int4& iend)
{
    if (ibegin >= iend) return;
    const Int4 NumHalf = NumPoints / 2;

    assert(ibegin >= NumHalf);
	assert((Int4)lx.size() >= iend + NumHalf);
	assert((Int4)ly.size() >= iend + NumHalf);
	assert((Int4)mdata.size() == mdataContents);
    for (int i = 0; i < mdataContents; i++)
    {
    	assert((Int4)mdata[i].size() >= iend);
    }

    
    const int ib = (NumPoints - 1) / 2;

    vector< vector<double> > ps_con, ps;

    vector<double> np(NumPoints);
    // Savitzky-Golay Coefficients for fixed bin widths.
    for (int k = 0; k < NumPoints; k++) np[k] = k;
    calSGCoef(np, ib, ps_con);

    const Double* nx_p = &lx[ibegin - NumHalf];
    const Double* ny_p = &ly[ibegin - NumHalf];

    bool const_bin_flag = false;
    int checker = 0;

    Double bin0 = 1.0;
    Double bin = 1.0;

    // SMOOTHING LOOP
    Double t, t2;
    for (int i = ibegin; i < iend; i++, nx_p++, ny_p++, checker--)
    {
        // Check whether the bin widths are same or not.
        if ( checker == 0 )
	    {
    	    bin0 = nx_p[1] - nx_p[0];
            do
            {
            	checker++;
            	bin = nx_p[checker + 1] - nx_p[checker];
            } while(checker < NumPoints - 1 && fabs(bin - bin0) < 0.01 * bin0);
        }
    	else if (checker == NumPoints - 2)
        {
            bin = nx_p[checker + 1] - nx_p[checker];
            if (fabs(bin - bin0) < 0.01 * bin0) checker++;
        }

        if(checker == NumPoints - 1)
        { // const bin widths.
            if(!const_bin_flag)
            {
                ps = ps_con;
                t = 1.0/bin;
                if (endder >1)
                {
                	t2 = t * t;
                    for (int k = 0; k < NumPoints; k++) ps[mdataContents - 1][k] *= t2;
                    if (mdataContents > 1)
                    {
                    	for (int k = 0; k < NumPoints; k++) ps[mdataContents - 2][k] *= t;
                    }
                }
                else if (endder > 0)
                {
                    for (int k = 0; k < NumPoints; k++) ps[mdataContents - 1][k] *= t;
                }
                const_bin_flag = true;
            }
        }
        else
        {
            const_bin_flag = false;
			for (int k = 0; k < NumPoints; k++) np[k] = nx_p[k];
            calSGCoef(np, nx_p[ib], ps); // variable bin widths.
        }

        for (int j = 0; j < mdataContents; j++)
        {
        	for (int k = 0; k < NumPoints; k++) mdata[j][i] += ps[j][k] * ny_p[k];
        }

        if (endder >= 2)
        {
        	mdata[mdataContents - (endder-1)][i] *= 2.0;
            if (endder >= 3) mdata[mdataContents - (endder-2)][i] *= 6.0;
        }
    }

}
