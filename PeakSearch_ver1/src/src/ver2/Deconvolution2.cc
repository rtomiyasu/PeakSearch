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
#include "Deconvolution2.hh"

using namespace std;

Deconvolution2::Deconvolution2()
{
}



Deconvolution2::~Deconvolution2()
{
}



void Deconvolution2::setParam(const ControlSmthData& csmthdata)
{
	m_wave_length_ratio = csmthdata.putKalpha2WaveLength() / csmthdata.putKalpha1WaveLength();
//	m_intensity_ratio = csmthdata.putIntensityRatioOfMaxWaveLength() / csmthdata.putIntensityRatioOfMinWaveLength();
}

void Deconvolution2::execute(vector<PeakData>& WaveIParam)
{
	static const Double DegRad = 180.0 / M_PI;
	static const Double RadDeg = M_PI / 180.0;
	static const Double INV_W_FACTOR = 1.0/sqrt(8.0*log(2.0));

    const Int4 peak_num = WaveIParam.size();

    sort(WaveIParam.begin(), WaveIParam.end());

    Double nx0, nx1;
    PeakData wave_param_cp;
    vector<PeakData>::const_iterator it_begin, it_end;
    vector<PeakData>::const_iterator it0 = WaveIParam.begin();
    for(Int4 i=0; i<peak_num; i++, it0++)
    {
    	WaveIParam[i].Flag = true;
    	wave_param_cp = WaveIParam[i];

    	const Double gauss_err = WaveIParam[i].FWHM * INV_W_FACTOR;

    	nx0 = asin( sin( WaveIParam[i].PeakPos * RadDeg * 0.5 ) / m_wave_length_ratio ) * DegRad * 2.0;
    	wave_param_cp.PeakPos = nx0 - gauss_err;
    	it_begin = lower_bound(vector<PeakData>::const_iterator(WaveIParam.begin()), it0, wave_param_cp);

    	wave_param_cp.PeakPos = nx0 + gauss_err;
    	it_end = lower_bound(it_begin, it0, wave_param_cp);

    	if( it_begin >= it_end ) continue;

    	nx1 = asin( sin( WaveIParam[i].PeakPos * RadDeg * 0.5 ) * m_wave_length_ratio ) * DegRad * 2.0;
    	wave_param_cp.PeakPos = nx1 + gauss_err;
    	it_end = upper_bound(it0+1, vector<PeakData>::const_iterator(WaveIParam.end()), wave_param_cp);

    	wave_param_cp.PeakPos = nx1 - gauss_err;
    	it_begin = upper_bound(it0+1, it_end, wave_param_cp);

    	if( it_begin >= it_end )
    	{
    		WaveIParam[i].Flag = false;
    	}
    }
}
