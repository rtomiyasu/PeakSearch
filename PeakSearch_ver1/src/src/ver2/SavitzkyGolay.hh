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
#ifndef _SavitzkyGolay_h_
#define _SavitzkyGolay_h_
// SavitzkyGolay.hh
#include <vector>
#include "../OrthoPolynomial.hh"
#include "../RietveldAnalysisTypes.hh"

using namespace std;

class SavitzkyGolay : protected OrthoPolynomial
{
private:
    int NumPoints;
//    vector<double> m_edata;  // propagated errors of smoothing results.

public:
    SavitzkyGolay();
    ~SavitzkyGolay();
    void setParam(int, int, int, int);
    void putSmthData(const Vec_DP& lx, const Vec_DP& ly, vector< vector<double> >& l2data,
    					const Int4& ibegin, const Int4& iend);
//    const vector<double>& putErrorData() const;
};

#endif

