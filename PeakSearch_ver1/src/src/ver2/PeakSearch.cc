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
#include <fstream>
#include <string>
#include <cmath>
#include "PeakSearch.hh"
#include "SavitzkyGolay.hh"
#include "akima.hh"
#include "mu_sig.hh"


using namespace std;

PeakSearch::PeakSearch()
{
    m_PSDivTray.resize(1, PeakSearchDIV(numeric_limits<Double>::max(), 9));
    m_use_error_data = false;
    m_use_bg_removed = false;
    m_threshold = 0.0;
}


PeakSearch::~PeakSearch()
{
}


// nx : X-coordinate.
// ndata : Original data.
// edata : If m_error != 0, error data.
void PeakSearch::setParam(const ControlSmthData& cont)
{
    m_use_error_data = cont.putUseErrorDataFlag();
    m_threshold = cont.putThreshold();

    m_peak_search_range.begin = cont.putPeakSearchRangeBegin();
    m_peak_search_range.end = cont.putPeakSearchRangeEnd();
    m_PSDivTray.clear();
    m_PSDivTray.insert(m_PSDivTray.end(), cont.putConvPoints().begin(), cont.putConvPoints().end() );
    sort(m_PSDivTray.begin(), m_PSDivTray.end());
    for(vector<PeakSearchDIV>::const_iterator it=m_PSDivTray.begin()+1; it!=m_PSDivTray.end(); it++)
    {
    	if( (it-1)->endDiv >= it->endDiv )
    	{
        	throw ZErrorMessage( "THE END OF INTERVAL IS DOUPLICATED: "+num2str(it->endDiv), __FILE__, __LINE__, __FUNCTION__);
    	}
    }

    if ( m_PSDivTray.rbegin()->endDiv < m_peak_search_range.end )
    {
    	throw ZErrorMessage( "THE END POINT OF SMOOTHING IS LESS THAN THE END POINT OF PEAK SEARCH", __FILE__, __LINE__, __FUNCTION__);
    }

    mdata.clear();
    WaveIParam.clear();
}


// pks_flag = false then SMOOTHING.
// pks_flag = true then 2nd & 1st DERIVATIVE, PEAKSEARCH.
void PeakSearch::execute(const ProfileData &pdata)
{
	mdata.clear();
	mdata.resize(3);
	WaveIParam.clear();

	try{
        const Vec_DP& nx = pdata.putXColumn();   // x-values of profile data
        const Vec_DP& ndata = pdata.putYIntColumn(); // y-values of profile data
        const Vec_DP& edata = pdata.putYErrorColumn(); // y-values of profile data

        if ( m_PSDivTray.empty() ) throw 2;
        if ( (Int4)nx.size() < m_PSDivTray.rbegin()->convPoints ) throw 1;
        if ( m_PSDivTray.rbegin()->endDiv < *( nx.end() - m_PSDivTray.rbegin()->putHalfPoints() - 1 ) ) throw 2;

        const Int4 nsize = nx.size();
    	mdata[0].resize(nsize, 0.0);
    	mdata[1].resize(nsize, 0.0);
    	mdata[2].resize(nsize, 0.0);

       	// Range for peak search.
   		const Vec_DP::const_iterator it_begin_pks = lower_bound(nx.begin(), nx.end(), m_peak_search_range.begin);
   		const Int4 ibegin_pks = max(m_PSDivTray.begin()->putHalfPoints(), distance(nx.begin(), it_begin_pks));
   		const Int4 iend_pks = min(nsize-m_PSDivTray.rbegin()->putHalfPoints(), distance(nx.begin(), upper_bound(it_begin_pks, nx.end(), m_peak_search_range.end)));

	   	Int4 ibegin = m_PSDivTray.begin()->putHalfPoints(), iend;
	   	SavitzkyGolay sg;
   		for(vector<PeakSearchDIV>::const_iterator it=m_PSDivTray.begin(); it<m_PSDivTray.end(); it++, ibegin=iend)
   		{
   			if( nx.begin()+ibegin >= nx.end() - it->putHalfPoints() ) continue;
   			iend = distance(nx.begin(), upper_bound(nx.begin()+ibegin, nx.end() - it->putHalfPoints(), it->endDiv ) );

   	   		sg.setParam(3, it->convPoints, 0, 2);
   	        sg.putSmthData(nx, ndata, mdata, ibegin, iend);

   	   		const Double* p_edata = NULL;
   	   		if( m_use_error_data ) p_edata = &edata[0];

   	   		const Double* p_bgdata = NULL;
   	   		if( m_use_bg_removed ) p_bgdata = &(pdata.putSubtractBGColumn())[0];

   	   		searchPeak(it->convPoints, nx, ndata, p_edata, p_bgdata, mdata[2],
   	   						max(ibegin, ibegin_pks), min(iend, iend_pks),
   	        				m_threshold, WaveIParam);
   		}
	}
    catch(const int num)
    { 
    	if (num == 1) throw ZErrorMessage( "NUMBER OF INTENSITY DATA IS LESS THAN THE NUMBER OF POINTS FOR SG METHOD", __FILE__, __LINE__, __FUNCTION__);
    	if (num == 2) throw ZErrorMessage( "NO DATA ON NUMBER OF POINTS FOR SG METHOD", __FILE__, __LINE__, __FUNCTION__);
    	if (num == 3) throw ZErrorMessage( "THE END POINT OF SMOOTHING IS LESS THAN THE END POINT OF DATA", __FILE__, __LINE__, __FUNCTION__);
    }
}

    
//void PeakSearch::printStatistics(
//const vector<double>& ydata, Double &average, Double &sigma, const string& fname) const
//{
//    string suffix[5] = { "_average", "_sigma", "_average_add_3sigma", "_average_sub_3sigma", "_iter" };
//    Double up3Sigma, low3Sigma;
//    int count;
//    
//    string name = "bg";
//
//    ofstream ofs(fname.c_str());
//    ostream *os;
//    os = &ofs;
//
//    *os << "IGOR" << endl;
//    *os << "WAVES/O ";
//    for (int i = 0; i < 5; i++) *os << name << suffix[i] << ((i!=4) ? ", " : "");
//    *os << endl;
//    *os << "BEGIN" << endl;
//    
//    averageAndSigmaIn3Sigma(ydata, average , sigma, low3Sigma, up3Sigma, count);
//    
//    os->setf(ios::right);
//    os->setf(ios::uppercase);
//    os->setf(ios::showpoint);
//    os->precision(5);
//    os->setf(ios::scientific);
//    os->width(16);
//    *os << average;
//    os->width(16);
//    *os << sigma;
//    os->width(16);
//    *os << up3Sigma;
//    os->width(16);
//    *os << low3Sigma;
//    os->width(12);
//    *os << count << endl;
//    os->unsetf(ios::scientific);
//    *os << "END" << endl;
//
//    ofs.close();
//}



void PeakSearch::searchPeak(const Int4& points,
		const Vec_DP& lx, const Vec_DP& ly,
		const Double* edata,
		const Double* subtractBG, const Vec_DP& l2data,
		const Int4& ibegin, const Int4& iend,
		const Double& thred,
		vector<PeakData>& peak_data)
{
    static const Double H_FACTOR = -0.25*exp(0.5); //-0.412180317675;
	static const Double W_FACTOR = sqrt(2.0*log(2.0));
	static const int AKIMA_POINTS = 4;

	if (ibegin >= iend) return;
    assert( ibegin >= points / 2 );
    assert( (Int4)lx.size() >= iend + points / 2 );
    assert( (Int4)ly.size() >= iend + points / 2 );
    assert( (Int4)l2data.size() >= iend );

    vector< vector<double> > k34data(2);
    k34data[0].resize(iend, 0.0);
    k34data[1].resize(iend, 0.0);
    const Vec_DP& k3data = k34data[0];
    const Vec_DP& k4data = k34data[1];

    Double Area_under0, pkheight_area, xp1, xp2, hpk;
    SavitzkyGolay sg1;
	// 1st derivative of l2data(2nd derivative)
	sg1.setParam(3, points, 1, 2);
    
    int i = ibegin;
    for(; i < iend; i++)
    {
    	if( l2data[i] >= 0.0 ) break;
    }

    PeakData pdata;
    for(; i < iend; i++)
    {
        if (l2data[i] >= 0.0) continue;

        int i0 = i;	// i0 > m_half, l2data[i0-1] >= 0 and l2data[i0] < 0.
        Area_under0 = 0.0;

        for(; i < iend; i++)
        {
            if (l2data[i] >= 0.0) break;
            // Calculate the area surrounded by x-axe and the 2nd derivative under 0.
            Area_under0 += l2data[i] * (lx[i + 1] - lx[i - 1]) * 0.5;
        }
        // datanum - m_half > i > i0, l2data[i-1] < 0 and l2data[i] >= 0.
        if (i >= iend) break;

	    // Calculate x-value when 2nd derivative y=0.
	    xp1 = (l2data[i0] * lx[i0 - 1] - l2data[i0 - 1] * lx[i0]) / (l2data[i0] - l2data[i0 - 1]);
	    xp2 = (l2data[i] * lx[i - 1] - l2data[i - 1] * lx[i]) / (l2data[i] - l2data[i - 1]);
	    const Double dev2 = xp2 - xp1;

        // Peak height using the values of the 2nd derivative.
        pkheight_area = H_FACTOR * dev2 * Area_under0;

        if( edata == NULL )
        {
        	if ( pkheight_area < thred ) continue;
        }
        else
        {
         	Int4 j;
         	for (j = i0; j < i; j++)
         		if (pkheight_area < thred*edata[j]) break;
         	if( j < i ) continue;
        }

	    // Search peaktops in the range from lx[i0 - 1] to lx[i].
	    sg1.putSmthData(lx, l2data, k34data, i0-1, i+1);

	    const int i1 = i + 1;
	    int d = i0-1, d0;
	    while (d < i1)
	    {
	        while ( d < i1 && k4data[d] <= 0.0 ) d++;
	        if( d >= i1 ) break;

	        d0 = d;
	        while ( d < i1 && k4data[d] > 0.0 ) d++;

	        Double max_xp0 = 0.0;
	        Double max_hpk = -1.0;
	        for(Int4 m=d0; m < d; m++)
	        {
		        while ( m < d && k3data[m] >= 0.0 ) m++;
		        while ( m < d && k3data[m] < 0.0 ) m++;
		        if( m >= d ) break;

	            // xp0 : zero points of the 3rd derivative.
		        const Double xp0 = (k3data[m] * lx[m - 1] - k3data[m - 1] * lx[m])
	                                                         / (k3data[m] - k3data[m - 1]);

		        const int m3 = m - 2;
		        if( subtractBG != NULL )
		        {
			        akima(&lx[m3], &subtractBG[m3], AKIMA_POINTS, xp0, hpk);

		            if( edata == NULL )
		            {
		            	if ( hpk < thred ) continue;
		            }
		            else
		            {
		            	if( hpk < thred*edata[m-1] || hpk < thred*edata[m] ) continue;
		            }
		        }
		        akima(&lx[m3], &ly[m3], AKIMA_POINTS, xp0, hpk);
	            if( hpk <= max_hpk ) continue;

	            max_xp0 = xp0;
	            max_hpk = hpk;
	        }
	        if( max_hpk < 0.0 ) continue;

	        // i_pkt_pos[j] : The number i s.t. the peaktop of j'th peak is in the interval [ lx[i], lx[i+1] ).
	        pdata.PeakPos = max_xp0;
	        pdata.PeakTopHeight = max_hpk;
	        pdata.FWHM = W_FACTOR * dev2;
	        pdata.Flag = true;

	        peak_data.push_back(pdata);
	    }
    }
}


// For simulation process.
// Output histogram in PAT_FILE.
void PeakSearch::printPeakSearchData(const ProfileData& pData,
		const string& fname)
{
	static const Int4 PrecisionOutput = 6;

	const Vec_DP& nx = pData.putXColumn();
    const Vec_DP& ndata = pData.putYIntColumn();
    const Vec_DP& edata = pData.putYErrorColumn();

    ofstream ofs(fname.c_str());
    ostream * const os = &ofs;

    const string& x_phase_title = pData.putXColumnTitle();
    const string& y_phase_title = pData.putYIntColumnTitle();
    const string& y_phase_smth_title = "smth_" + y_phase_title;

    *os << "IGOR" << endl;
    *os << "WAVES/O ";
    *os << x_phase_title <<", " << y_phase_title;
    *os << ", " << pData.putYErrorColumnTitle();
	*os << ", " << y_phase_smth_title;
//    *os << ", error_of_" << y_phase_smth_title;
    *os << endl;

    *os << "BEGIN\n";

    os->setf(ios::right);
    os->setf(ios::uppercase);
    os->setf(ios::showpoint);
   	os->precision(PrecisionOutput);

   	const Int4 isize = nx.size();
   	assert( !m_PSDivTray.empty() );
   	assert( isize == (Int4)ndata.size() );
   	assert( isize == (Int4)edata.size() );
   	const Int4 half = m_PSDivTray.begin()->putHalfPoints();

   	for (Int4 k=0; k<isize; k++)
    {
	   	os->unsetf(ios::scientific);
       	os->width(10);
       	*os << nx[k];

       	os->setf(ios::scientific);
   		os->width(16);
    	*os << ndata[k];

   		os->width(16);
    	*os << edata[k];

       	if( k < half || k + half >= isize )
       	{
       		os->width(16);
        	*os << "NAN";
       	}
       	else
       	{
       		os->width(16);
        	*os << mdata[0][k];
       	}
    	*os << endl;
    }

   	*os << "END" << endl;

    const Int4 peak_num = WaveIParam.size();
    *os << "WAVES/O peak, peakpos, height, FWHM, Flag" << endl;
    *os << "BEGIN" << endl;

    for (Int4 j = 0; j < peak_num; j++)
    {
        // OUTPUT
        os->precision();
        os->width(5);
        *os << j + 1;

        os->width(15);
        *os << WaveIParam[j].PeakPos;
        os->width(15);
        *os << WaveIParam[j].PeakTopHeight;
        os->width(15);
        *os << WaveIParam[j].FWHM;

        os->precision();
        os->width(5);
        if( WaveIParam[j].Flag ) *os << 1;
        else *os << 0;
        *os << endl;
    }
    *os << "END" << endl;

    *os << "X Display " << y_phase_title << ", " << pData.putYErrorColumnTitle() << ", ";
    *os << y_phase_smth_title << " vs " << x_phase_title << endl;
    *os << "X ModifyGraph mirror(left)=2" << endl;
    *os << "X ModifyGraph mirror(bottom)=2" << endl;
    *os << "X ModifyGraph mode(" << y_phase_title << ")=0, msize(" << y_phase_title << ")=2" << endl;
    *os << "X ModifyGraph mode(" << pData.putYErrorColumnTitle() << ")=0, msize(" << pData.putYErrorColumnTitle() << ")=2" << endl;
    *os << "X ModifyGraph mode(" << y_phase_smth_title << ")=0, msize(" << y_phase_smth_title << ")=2" << endl;
    *os << "X ModifyGraph rgb(" << pData.putYErrorColumnTitle() << ")=(65280,43520,0)" << endl;
    *os << "X ModifyGraph rgb(" + y_phase_smth_title + ")=(0,15872,65280)" << endl;

    *os << "X AppendToGraph height vs peakpos" << endl;
    *os << "X ModifyGraph mode(height)=3,marker(height)=17" << endl;
    *os << "X ModifyGraph rgb(height)=(0,65280,65280)" << endl;
	ofs.close();
}
