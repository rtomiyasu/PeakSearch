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
#ifndef _ControlSmthData_h_
#define _ControlSmthData_h_
// ControlSmthData.hh

#include <list>
#include "../RietveldAnalysisTypes.hh"
#include "../utility_rw_param/I_ReadData.hh"

using namespace std;

class PeakSearchDIV
{
public:
	double endDiv; // Interval
	// Convolution Number of Points for Savitzky and Golay method (5 to 101)
    int convPoints;
	PeakSearchDIV(const double& arg1, const Int4& arg2) : endDiv(arg1), convPoints(arg2){};

	inline int putHalfPoints() const { return convPoints / 2; }
	inline bool operator<(const PeakSearchDIV& rhs) const { return this->endDiv < rhs.endDiv; };
};

class ControlSmthData : public I_ReadData
{
private:
	static const pair< RWParamProperty, RWParamData<bool> > useErrorDataFlag_Data;
//	static const pair< RWParamProperty, RWParamData<bool> > useBGRemovedFlag_Data;
	static const pair< RWParamProperty, RWParamData<void*> > PeakSearchRange_Data;
	static const pair< RWParamProperty, RWParamData<Double> > PeakSearchRangeBegin_Data;
	static const pair< RWParamProperty, RWParamData<Double> > PeakSearchRangeEnd_Data;

	static const pair< RWParamProperty, RWParamData<void*> > PSDivTray_Data;
	static const pair< RWParamProperty, RWParamData<Double> > end_PSDivTray_Data;
	static const pair< RWParamProperty, RWParamData<Int4> > convPoints_Data;

	static const pair< RWParamProperty, RWParamData<Double> > threshold_Data;

	static const pair< RWParamProperty, RWParamData<void*> > Waves_Data;
	static const pair< RWParamProperty, RWParamData<Double> > Kalpha1WaveLength_Data;
	static const pair< RWParamProperty, RWParamData<Double> > Kalpha2WaveLength_Data;
	static const pair< RWParamProperty, RWParamData<bool> > Alpha2Correction_Data;

    bool useErrorDataFlag;
//    bool useBGRemovedFlag;

    range<Double> PeakSearchRange;

    list<PeakSearchDIV> PSDivTray;

    // When this variable > 0, it is used as the threshold.
    // Otherwise, PeakSearch calculates the threshold from the average and the standard derivation of the background noise.
    Double threshold;

	Vec_DP WaveLengths;
    bool Alpha2Correction;

    void setData(const RWParamProperty& parent_prop,
    		vector<RWParam_void>& tray);

protected:
	ZErrorMessage checkData(const RWParam_void& param) const;
	ZErrorMessage checkIfDataAreSet(const RWParam_void& param, const Int4& num) const;

public:
    ControlSmthData();
    virtual ~ControlSmthData();

    const string& putTagLabel() const;

    inline const Double& putPeakSearchRangeBegin() const { return PeakSearchRange.begin; };
    inline const Double& putPeakSearchRangeEnd() const { return PeakSearchRange.end; };

    // Convolution Number of Points for Savitzky and Golay method (5 to 101)
	inline const list<PeakSearchDIV>& putConvPoints() const { return PSDivTray; };

	// Otherwise: the threshold is determined from the member variable below
    inline const bool& putUseErrorDataFlag() const { return useErrorDataFlag; };
//    inline const bool& putUseBackgroundRemovedFlag() const { return useBGRemovedFlag; };

    // When this variable > 0, it is used as the threshold.
    // Otherwise, PeakSearch calculates the threshold from the average and the standard derivation of the background noise.
    inline const Double& putThreshold() const { return threshold; };

    inline const bool& putDoAlpha2CorrectionFlag() const { return Alpha2Correction; };
    inline const Double& putKalpha1WaveLength() const { return WaveLengths[0]; };
    inline const Double& putKalpha2WaveLength() const { return WaveLengths[1]; };
};

#endif
