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
#ifndef _ProfileData_h_
#define _ProfileData_h_
// ProfileData.hh
#include <assert.h>
#include <vector>
#include <string>
#include "RietveldAnalysisTypes.hh"
#include "zerror_type/error_out.hh"


using namespace std;

class ProfileData
{
private:
    int NumContents;	// x-phase, y-phase, error(, subtractbg).

	vector<Vec_DP> m_Wave;
	vector<string> m_Wave_title;

	ZErrorMessageReadingFile readIGORFile(const string& fname, const bool& err_flag);
	ZErrorMessageReadingFile readGeneralFile(const string& fname, const bool& err_flag);
	ZErrorMessageReadingFile readRietanFile(const string& fname);

public:
    ProfileData();
    ~ProfileData();

	const Vec_DP& putXColumn() const { return m_Wave.at(0); };
	const Vec_DP& putYIntColumn() const { return m_Wave.at(1); };
	const Vec_DP& putYErrorColumn() const { return m_Wave.at(2);};
	const Vec_DP& putSubtractBGColumn() const { return m_Wave.at(3); };

	const string& putXColumnTitle() const { return m_Wave_title[0]; };
	const string& putYIntColumnTitle() const { return m_Wave_title[1]; };
	const string& putYErrorColumnTitle() const { return m_Wave_title[2];};
	const string& putSubtractBGColumnTitle() const { assert( NumContents > 3 ); return m_Wave_title[3]; };

	inline ZErrorMessageReadingFile readFile(const string& fname,
											ZRietveldHistogramFileFormat& file_format,
											bool& err_flag);
};


inline ZErrorMessageReadingFile ProfileData::readFile(const string& fname,
		ZRietveldHistogramFileFormat& file_format,
		bool& err_flag)
{
	file_format = kZRietveldHistogramFileFormatIGOR;
	err_flag = true;
	ZErrorMessageReadingFile zerr = readIGORFile(fname, err_flag);

	if( zerr.putErrorType() == ZErrorNotIGORFile )
	{
		file_format = kZRietveldHistogramFileFormatRietan;
		err_flag = false;
		zerr = readRietanFile(fname);

		if( zerr.putErrorType() == ZErrorFileFormatBroken )
		{
			file_format = kZRietveldHistogramFileFormatGeneral;
			err_flag = true;
			zerr = readGeneralFile(fname, err_flag);
			if( zerr.putErrorType() == ZErrorErrorsAreNotContained )
			{
				err_flag = false;
				return readGeneralFile(fname, err_flag);
			}
		}
	}
	else if( zerr.putErrorType() == ZErrorErrorsAreNotContained )
	{
		err_flag = false;
		return readIGORFile(fname, err_flag);
	}
	return zerr;
}

#endif
