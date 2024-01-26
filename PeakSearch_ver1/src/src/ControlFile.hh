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
#ifndef _ControlFile_h_
#define _ControlFile_h_

// ControlFile.hh
#include "utility_rw_param/I_ReadData.hh"
#include "utility_rw_param/find_key_data.hh"

using namespace std;

typedef enum _Conograph_preprocessing
{
	kConograph_prep_Background = 1,
	kConograph_prep_Peaksearch = 2
} Conograph_preprocessing;


class ControlFile : public I_ReadData
{
private:
	static const pair<RWParamProperty, RWParamData<string> > CntParamFname_Data;
    static const pair<RWParamProperty, RWParamData<void*> > HistogramDataFile_Data;
    static const pair<RWParamProperty, RWParamData<string> > HistogramDataFname_Data;
    static const pair<RWParamProperty, RWParamData<string> > OutFname_Data;

    Conograph_preprocessing process_flag;

    void setData(const RWParamProperty& parent_prop,
    		vector<RWParam_void>& tray);

public:
    string CntParamFname;  // File name
    string HistogramDataFname;  // File name
    string OutFname;  // File name

    ControlFile();
    virtual ~ControlFile();

    void setPeakSearchFlag(const Conograph_preprocessing& flag){ process_flag = flag; };
    const string& putTagLabel() const;
};

#endif
