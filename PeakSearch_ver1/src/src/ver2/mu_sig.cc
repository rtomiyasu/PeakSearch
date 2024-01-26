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
#include <vector>
#include <cmath>
#include "mu_sig.hh"

using namespace std;

// If data is empty, return average = 0.0, sigma = -1.0.
// Otherwise, calculate average (mu) and standard deviation (sigma) by saving data
// within 3sigma and rejecting data out of 3sigma.
void averageAndSigmaIn3Sigma(
    const vector<double>& data,        // Number of data, data
    Double & average,           // Average (Mu)
    Double & sigma,             // Standard deviation (Sigma)
    Double & low3Sigma,         // 3 sigma lower limit (Mu - 3*Sigma)
    Double & up3Sigma,          // 3 sigma upper limit (Mu + 3*Sigma)
    int & count)
{
    const int ITMAX = 1000;
    const int n = data.size();
    Double average0, sigma0, low3Sigma0, up3Sigma0;
    
    count = 0;
    averageAndSigma(n, data, average0, sigma0, low3Sigma0, up3Sigma0);
    
    average = average0;
    sigma = sigma0;
    low3Sigma = low3Sigma0;
    up3Sigma = up3Sigma0;

    if (n == 0) return;
    vector<double> data1(n);

    do {
        count++;
        average0   = average;
        sigma0     = sigma;
        low3Sigma0 = low3Sigma;
        up3Sigma0  = up3Sigma;
        
        int n1 = 0;
        for (int i = 0; i < n; i++)
          {
            if ((data[i] >= low3Sigma0) && (data[i] <= up3Sigma0))
              {
                data1[n1] = data[i];
                n1++;
              }
          }
        averageAndSigma(n1, data1, average, sigma, low3Sigma, up3Sigma);
        
    } while(count < ITMAX && 
           (fabs(sigma - sigma0) > 1.0e-5 * sigma0 || fabs(average - average0) > 1.0e-5 * average0) );

}

//  For n > 0, calculate the average and the standard deviation of data[i](0 <= i < n).
//  If n = 0, return average = 0.0, sigma = -1.0.
void averageAndSigma(
    int n, const vector<double>& data,        // Number of data, data
    Double & average,           // Average (Mu)
    Double & sigma,             // Standard deviation (Sigma)
    Double & low3Sigma,         // 3 sigma lower limit (Mu - 3*Sigma)
    Double & up3Sigma)          // 3 sigma upper limit (Mu + 3*Sigma)
{
    Double sum, diff;
    if (n > 1)
      {
        sum = 0.0;
        for (int i = 0; i < n; i++) sum += data[i];
        average = sum / (double)n;
    
        sum = 0.0;
        for (int i = 0; i < n; i++)
          {
            diff = data[i] - average;
            sum += diff * diff;
            sigma = sqrt(sum / (double)(n-1));
          }
       }
    else if (n == 1)
      { average = data[0]; sigma = 0.0; }
    else
      { average = 0.0; sigma = -1.0; }

    low3Sigma = average - 3.0 * sigma;
    up3Sigma  = average + 3.0 * sigma;
}
