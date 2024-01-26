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
#ifndef _PeakSearch_h_
#define _PeakSearch_h_
// PeakSearch.h
#include <vector>
#include <string>
#include "ControlSmthData.hh"
#include "PeakData.hh"
#include "../ProfileData.hh"
#include "../RietveldAnalysisTypes.hh"

class PeakSearch
{
private:
    bool m_use_error_data;
    bool m_use_bg_removed;
    Double m_threshold;

    range<Double> m_peak_search_range;
    vector<PeakSearchDIV> m_PSDivTray;

    // Results.
    // m_pks_flag = false -> mdata[0] : Smoothing.
    // m_pks_flag = true -> mdata[0] : Smoothing,  mdata[1] : 1-st derivative,  mdata[2] : 2-nd derivative.
    vector< vector<double> > mdata;
    vector<PeakData> WaveIParam;	// WaveIParam[0]:Peak-positions, WaveIParam[1]:Peak-heights, WaveIParam[2]:half-widths.

    void printPeakParam(const vector< vector<double> >&, const string&) const;

    static void searchPeak(const Int4& points, const Vec_DP& lx,
    	    		const Vec_DP& ly,
    	    		const Double* edata,
    	    		const Double* subtractBG,
    	    		const Vec_DP& l2data, const Int4& ibegin, const Int4& iend,
       	    		const Double& thred,
    	    		vector<PeakData>& peak_data);

public:
    PeakSearch();
    ~PeakSearch();
    void setParam(const ControlSmthData&);
    void execute(const ProfileData&);

    // Peaksearch Output.
    void printPeakSearchData(const ProfileData&, const string& fname);

    inline vector<PeakData>& putPeakData() { return WaveIParam; };
    inline const vector<PeakData>& putPeakData() const { return WaveIParam; };
};

#endif
