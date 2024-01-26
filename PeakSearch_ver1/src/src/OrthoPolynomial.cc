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
#include "OrthoPolynomial.hh"
#include "zerror_type/error_out.hh"

OrthoPolynomial::OrthoPolynomial()
{
    PolOrder = 3;
    inider = 0;
    endder = 0;
    mdataContents = 1;
}

OrthoPolynomial::~OrthoPolynomial()
{
}

/* 
   PolOrder(num2) : Order of the fitting polynomial which is used 
                    to calculate the the derivative(smoothing) value of original data.(2 or 4)

   inider(num3) : (0 <= inider) The least order of polynomial coefficient.(0th derivative is smoothing)
   
   endder(num4) : (indder <= endder) The greatest order of polynomial coefficient.
*/
void OrthoPolynomial::setParam(int num1, int num2, int num3)
{
    try{
        if (num1 >= 0) PolOrder = num1;
        else throw 1;

        if (num2 >= 0) inider = num2;
        else throw 2;

        endder = num3;

        mdataContents = endder - inider + 1;
        if (mdataContents <= 0) throw 3;
    }
    catch(const int i){
        if(i==1) throw nerror_out_range("Polynomial order", num1, __FILE__, __LINE__, __FUNCTION__);
        if(i==2) throw nerror_out_range("Minimum derivative order", num2, __FILE__, __LINE__, __FUNCTION__);
        if(i==3) throw nerror_out_range("Maximum derivative order", num3, __FILE__, __LINE__, __FUNCTION__);
    }
}


void OrthoPolynomial::calSGCoef(
const vector<Double>& nx, const Double& smthpos, vector< vector<double> >& ps) const
{
    const int numpoint = nx.size();
    ps.clear();
    ps.resize(mdataContents);
    for (int i=0; i<mdataContents; i++) ps[i].resize(numpoint, 0.0);

    vector<double> np(numpoint);
    for (int k=0; k<numpoint; k++) np[k] = nx[k] - smthpos;

    vector< vector<double> > A;
    vector<double> Gamma;
    OrthoPolynomialCoef(np, A, Gamma);

    Double pk;
    for (int k = 0; k<numpoint; k++)
    {
        for(unsigned int j = 0; j < A.size(); j++)
        {
            // pk is the value of the Orthogonal Polynomial PK(X) at np[k].
            pk = A[j][j];
            for (int l = j - 1; l >= 0; l--) pk = pk * np[k] + A[j][l];

            // Savitzky-Golay Coefficients of Yi
            for (int d = 0; d < mdataContents; d++) ps[d][k] += A[j][inider + d] * pk / Gamma[j];
        }
    }
}

void OrthoPolynomial::OrthoPolynomialCoef(const vector<double>& np,
vector< vector<double> >& A, vector<double>& Gamma)
const
{
    const int numpoint = np.size();
    const int polorder2_1 = 2 * PolOrder + 1;
    vector<double> PowerSum(polorder2_1, 0.0);
    // Power Sums of np[i].
    Double x;
    for (int k=0; k<numpoint; k++)
    {
         x = 1.0;
         for (int j = 0; j <polorder2_1; j++)
         {
             PowerSum[j] += x;
             x *= np[k];
         }
    }
      
    const int PolOrder1 = PolOrder + 1;

    A.clear();
    Gamma.clear();
    A.resize(PolOrder1);
    for (int j = 0; j < PolOrder1; j++) A[j].resize(PolOrder1, 0.0);
    Gamma.resize(PolOrder1, 0.0);

    vector<double> B(PolOrder, 0.0);

    // Calculate the value of A[k][j],
    // coefficients of the k'th Orthogonal Polynomial PK(X)=∑A[k][j]x^j
    A[0][0] = 1.0;
    Gamma[0] = PowerSum[0];
    B[0] = PowerSum[1] / Gamma[0];

   	A[1][0] = - B[0];
   	A[1][1] = 1.0;
   	Gamma[1] = PowerSum[1] * A[1][0] + PowerSum[2] * A[1][1];

   	Double CK;
   	for (int k=2, i=1; k<PolOrder1; k++, i++)
    {
        for (int j = 0 ; j <= i ; j++) B[i] += PowerSum[i + j + 1] * A[i][j];  
        B[i] = B[i] / Gamma[i] + A[i][i - 1];

        CK = Gamma[k - 1] / Gamma[k - 2];
        
        A[k][0] = - A[k - 1][0] * B[k - 1] - A[k - 2][0] * CK;  

        for (int j = 1; j < k; j++) A[k][j] = A[k - 1][j - 1]- A[k - 1][j] * B[k - 1] - A[k - 2][j] * CK;  

        A[k][k] = 1.0;  

        for (int j = 0; j <= k; j++) Gamma[k] += PowerSum[k + j] * A[k][j];  
    }
}
