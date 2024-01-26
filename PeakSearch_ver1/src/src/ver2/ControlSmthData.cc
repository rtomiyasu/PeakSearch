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
#include <string>
#include <sstream>
#include <cmath>
#include "ControlSmthData.hh"
#include "../utility_func/zstring.hh"

using namespace std;

const pair< RWParamProperty, RWParamData<bool> > ControlSmthData::useErrorDataFlag_Data(
		RWParamProperty(BOOLFLAG, "UseErrorData"),
		RWParamData<bool>(true, REPLACE_NONE<bool>, NULL, false, NULL, false, -1, -1) );
//const pair< RWParamProperty, RWParamData<bool> > ControlSmthData::useBGRemovedFlag_Data(
//		RWParamProperty(BOOLFLAG, "UseBGRemoved"),
//		RWParamData<bool>(true, REPLACE_NONE<bool>, NULL, false, NULL, false, -1, -1) );

const pair< RWParamProperty, RWParamData<void*> > ControlSmthData::PeakSearchRange_Data(
		RWParamProperty(VOIDDATA, "PeakSearchRange"),
		RWParamData<void*>(NULL, REPLACE_NONE<void*>, NULL, NULL, NULL, NULL, -1, -1) );
const pair< RWParamProperty, RWParamData<Double> > ControlSmthData::PeakSearchRangeBegin_Data(
		RWParamProperty(DVALUE, "Begin"),
		RWParamData<Double>(0.0, REPLACE_MIN2ZERO<Double>, NULL, 0.0, NULL, MAX_DP(), -1, -1) );
const pair< RWParamProperty, RWParamData<Double> > ControlSmthData::PeakSearchRangeEnd_Data(
		RWParamProperty(DVALUE, "End"),
		RWParamData<Double>(MAX_DP(), REPLACE_MAX<Double>, NULL, 0.0, NULL, MAX_DP(), -1, -1) );

const pair< RWParamProperty, RWParamData<void*> > ControlSmthData::PSDivTray_Data(
		RWParamProperty(VOIDDATA, "ParametersForSmoothingDevision", NULL, MAX_INT(), 1),
		RWParamData<void*>(NULL, REPLACE_NONE<void*>, NULL, NULL, NULL, NULL, -1, -1) );
const pair< RWParamProperty, RWParamData<Double> > ControlSmthData::end_PSDivTray_Data(
		RWParamProperty(DVALUE, "EndOfRegion"),
		RWParamData<Double>(MAX_DP(), REPLACE_MAX<Double>, NULL, -MAX_DP(), NULL, MAX_DP(), 0, -1) );
const pair< RWParamProperty, RWParamData<Int4> > ControlSmthData::convPoints_Data(
		RWParamProperty(INT4VALUE, "NumberOfPointsForSGMethod"),
		RWParamData<Int4>(9, REPLACE_NONE<Int4>, GE<Int4>, 5, NULL, MAX_INT(), -1, -1) );

const pair< RWParamProperty, RWParamData<Double> > ControlSmthData::threshold_Data(
		RWParamProperty(DVALUE, "Threshold"),
		RWParamData<Double>(5.0, REPLACE_NONE<Double>, NULL, -MAX_DP(), NULL, MAX_DP(), -1, -1) );
const pair< RWParamProperty, RWParamData<void*> > ControlSmthData::Waves_Data(
		RWParamProperty(VOIDDATA, "Waves"),
		RWParamData<void*>(NULL, REPLACE_NONE<void*>, NULL, NULL, NULL, NULL, 2, 2) );
const pair< RWParamProperty, RWParamData<Double> > ControlSmthData::Kalpha1WaveLength_Data(
		RWParamProperty(DVALUE, "Kalpha1WaveLength"),
		RWParamData<Double>(1.54056, REPLACE_NONE<Double>, GT<Double>, 0.0, NULL, MAX_DP(), -1, -1) );
const pair< RWParamProperty, RWParamData<Double> > ControlSmthData::Kalpha2WaveLength_Data(
		RWParamProperty(DVALUE, "Kalpha2WaveLength"),
		RWParamData<Double>(1.54439, REPLACE_NONE<Double>, GT<Double>, 0.0, NULL, MAX_DP(), -1, -1) );
const pair< RWParamProperty, RWParamData<bool> > ControlSmthData::Alpha2Correction_Data(
		RWParamProperty(BOOLFLAG, "Alpha2Correction"),
		RWParamData<bool>(false, REPLACE_NONE<bool>, NULL, false, NULL, false, -1, -1) );

ControlSmthData::ControlSmthData()
	:
	useErrorDataFlag(useErrorDataFlag_Data.second.initial_value),
//	useBGRemovedFlag(useBGRemovedFlag_Data.second.initial_value),
	PeakSearchRange(PeakSearchRangeBegin_Data.second.initial_value, PeakSearchRangeEnd_Data.second.initial_value),
	PSDivTray(1, PeakSearchDIV(end_PSDivTray_Data.second.initial_value, convPoints_Data.second.initial_value)),
	threshold(threshold_Data.second.initial_value),
	WaveLengths(2),
	Alpha2Correction(Alpha2Correction_Data.second.initial_value)
{
	WaveLengths[0] = Kalpha1WaveLength_Data.second.initial_value;
	WaveLengths[1] = Kalpha2WaveLength_Data.second.initial_value;
}

ControlSmthData::~ControlSmthData()
{
}


const string& ControlSmthData::putTagLabel() const
{
	static const string label = "PeakSearchPSParameters";
	return label;
}


void ControlSmthData::setData(const RWParamProperty& parent_prop,
				vector<RWParam_void>& tray)
{
	if( parent_prop.putLabel() == this->putTagLabel() )
	{
		PSDivTray.clear();
		tray.push_back( RWParam_void(PSDivTray_Data) );

		tray.push_back( RWParam_void(PeakSearchRange_Data) );
		tray.push_back( RWParam_void(useErrorDataFlag_Data, &useErrorDataFlag) );
//		tray.push_back( RWParam_void(useBGRemovedFlag_Data, &useBGRemovedFlag) );
		tray.push_back( RWParam_void(threshold_Data, &threshold) );

		tray.push_back( RWParam_void(Alpha2Correction_Data, &Alpha2Correction) );
		tray.push_back( RWParam_void(Waves_Data) );
	}
	else if( IsEqualTag(parent_prop, PeakSearchRange_Data.first ) )
	{
		tray.push_back( RWParam_void(PeakSearchRangeBegin_Data, &PeakSearchRange.begin) );
		tray.push_back( RWParam_void(PeakSearchRangeEnd_Data, &PeakSearchRange.end) );
	}
	else if( IsEqualTag(parent_prop, PSDivTray_Data.first ) )
	{
		PSDivTray.push_back( PeakSearchDIV(end_PSDivTray_Data.second.initial_value, convPoints_Data.second.initial_value) );

		tray.push_back( RWParam_void(end_PSDivTray_Data, &(PSDivTray.rbegin()->endDiv) ) );
		tray.push_back( RWParam_void(convPoints_Data, &(PSDivTray.rbegin()->convPoints) ) );
	}
	else if( IsEqualTag(parent_prop, Waves_Data.first ) )
	{
		tray.push_back( RWParam_void(Kalpha1WaveLength_Data, &WaveLengths[0] ) );
		tray.push_back( RWParam_void(Kalpha2WaveLength_Data, &WaveLengths[1] ) );
	}
}



ZErrorMessage ControlSmthData::checkData(const RWParam_void& param) const
{
	if( IsEqualTag(param.putProperty(), convPoints_Data.first ) )
	{
	    for(list<PeakSearchDIV>::const_iterator it=PSDivTray.begin(); it!=PSDivTray.end(); it++)
	    {
			if( it->convPoints != 2*( (it->convPoints-1)/2)+1 )
		    {
				return nerror_out_range(param.putLabel(), "Input an odd number",
											__FILE__, __LINE__, __FUNCTION__);
		    }
	    }
	}
	else if( IsEqualTag(param.putProperty(), PeakSearchRangeEnd_Data.first ) )
	{
		if( PeakSearchRange.end <= PeakSearchRange.begin )
		{
			return ZErrorMessage( "\"" + PeakSearchRangeEnd_Data.first.putLabel() + "\" is not larger than \"" + PeakSearchRangeBegin_Data.first.putLabel() + "\"",
									__FILE__, __LINE__, __FUNCTION__);
		}
	}
	else if( IsEqualTag(param.putProperty(), end_PSDivTray_Data.first ) )
	{
		list<PeakSearchDIV>::const_iterator it=PSDivTray.begin();
	    for(; it!=PSDivTray.end(); it++)
	    {
			if( it->endDiv >= PeakSearchRange.end ) break;
	    }
	    if( it==PSDivTray.end() )
	    {
			return nerror_out_range(param.putLabel(), "Input a number not smaller than " + num2str(PeakSearchRange.end),
										__FILE__, __LINE__, __FUNCTION__);
	    }
	}
	else if( IsEqualTag(param.putProperty(), Kalpha2WaveLength_Data.first ) )
	{
		if( WaveLengths[1] <= WaveLengths[0] )
		{
			return ZErrorMessage( "\"" + Kalpha2WaveLength_Data.first.putLabel() + "\" is not larger than \"" + Kalpha1WaveLength_Data.first.putLabel() + "\"",
									__FILE__, __LINE__, __FUNCTION__);
		}
	}
	return I_ReadData::checkData(param);
}


ZErrorMessage ControlSmthData::checkIfDataAreSet(const RWParam_void& param, const Int4& num) const
{
	if( IsEqualTag(param.putProperty(), Waves_Data.first )
			|| IsEqualTag(param.putProperty(), Kalpha1WaveLength_Data.first )
			|| IsEqualTag(param.putProperty(), Kalpha2WaveLength_Data.first ) )
	{
		if( !Alpha2Correction )
		{
			return ZErrorMessage();
		}
	}
	return I_ReadData::checkIfDataAreSet(param, num);
}
