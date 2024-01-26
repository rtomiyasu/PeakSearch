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
#include "utility_rw_param/find_key_data.hh"
#include "ControlFile.hh"

using namespace std;

const pair< RWParamProperty, RWParamData<string> > ControlFile::CntParamFname_Data(
	RWParamProperty(STRVALUE, "ControlParamFile"),
	RWParamData<string>("", REPLACE_NONE<string>, NULL, "", NULL, "", -1, -1) );
const pair< RWParamProperty, RWParamData<void*> > ControlFile::HistogramDataFile_Data(
	RWParamProperty(VOIDDATA, "HistogramDataFile"),
	RWParamData<void*>(NULL, REPLACE_NONE<void*>, NULL, NULL, NULL, NULL, -1, -1) );
const pair< RWParamProperty, RWParamData<string> > ControlFile::HistogramDataFname_Data(
	RWParamProperty(STRVALUE, "FileName"),
	RWParamData<string>("", REPLACE_NONE<string>, NULL, "", NULL, "", -1, -1) );
const pair< RWParamProperty, RWParamData<string> > ControlFile::OutFname_Data(
	RWParamProperty(STRVALUE, "Outfile"),
	RWParamData<string>("", REPLACE_NONE<string>, NULL, "", NULL, "", -1, -1) );

ControlFile::ControlFile()
{
	process_flag = kConograph_prep_Peaksearch;
}

ControlFile::~ControlFile()
{
}


const string& ControlFile::putTagLabel() const
{
	static const string label = "PeakSearchInputFile";
	return label;
}


void ControlFile::setData(const RWParamProperty& parent_prop,
		vector<RWParam_void>& tray)
{
	if( parent_prop.putLabel() == this->putTagLabel() )
	{
		tray.push_back( RWParam_void( CntParamFname_Data, &CntParamFname ) );
		tray.push_back( RWParam_void( HistogramDataFile_Data ) );
		tray.push_back( RWParam_void( OutFname_Data, &OutFname ) );
	}
	else if( IsEqualTag(parent_prop, HistogramDataFile_Data.first ) )
	{
		tray.push_back( RWParam_void(HistogramDataFname_Data, &HistogramDataFname) );
	}
}
