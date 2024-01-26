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
#include <ctime>
#include <getopt.h>
#include <stdexcept>
#include "../ProfileData.hh"
#include "../ControlFile.hh"
#include "../utility_func/zstring.hh"
#include "../zerror_type/error_out.hh"
#include "../zlog/zlog.hh"
#include "ControlSmthData.hh"
#include "Deconvolution2.hh"
#include "PeakSearch.hh"

using namespace std;

int main(int argc, char* argv[])
{
		static string controlFile = "cntl.inp.xml";
		static const string InputFileLabel = "ZCodeParameters";
		static const Conograph_preprocessing process_flag = kConograph_prep_Peaksearch;


	try{
	    CRLog::append(new CCoutListner());
	    CRLog::append(new FileoutListner("LOG_PEAKSEARCH.txt", zListnerID(1)));

	    clock_t start;
		start = clock();    /* Set the initial time */

		ZErrorMessageReadingFile zerr;
		
		// read input_cntrol_file
	    ControlFile cf;
    	cf.setPeakSearchFlag(process_flag);
   		zerr = cf.readFile(controlFile, InputFileLabel);
   		if( zerr.putErrorType() != ZErrorNoError ) throw zerr;

		// smoothing or peaksearch
		// Profile data
		ProfileData pData;
		ZRietveldHistogramFileFormat file_format;
		bool is_error_contained;
		zerr = pData.readFile(cf.HistogramDataFname, file_format, is_error_contained);

ZLOG_INFO("Histogram format : " + find_data(putFormatTypeLabel(), file_format) + "\n" );
ZLOG_INFO("Errors of y-values : " + string(is_error_contained?"Contained in third column":"Square roots of y-values") + "\n\n" );
		if( zerr.putErrorType() != ZErrorNoError ) throw zerr;

		    // Control parameters
    	ControlSmthData csmthData;
    	zerr = csmthData.readFile(cf.CntParamFname, InputFileLabel);
    	if( zerr.putErrorType() != ZErrorNoError ) throw zerr;

    	PeakSearch ps;
    	ps.setParam(csmthData);
    	ps.execute(pData);

    	if( csmthData.putDoAlpha2CorrectionFlag() )
    	{
    		Deconvolution2 dcv;
    		dcv.setParam(csmthData);
    		dcv.execute(ps.putPeakData());
    	}

    	ps.printPeakSearchData(pData, cf.OutFname);

ZLOG_INFO( "PeakSeach Program has finished.\nCPU time : "
			+ num2str( (clock() - start) / CLOCKS_PER_SEC ) + "[sec.]\n" );
	}
	catch(bad_alloc& ball){
		ZErrorMessage zerr = nerror(ball, __FILE__, __LINE__, __FUNCTION__);
		ZLOG_ERROR( zerr.printErrorLog() );
		return 0;
	}
	catch(out_of_range&)
	{
		ZErrorMessage zerr("out_of_range exception has occurred", __FILE__, __LINE__, __FUNCTION__);
		ZLOG_ERROR( zerr.printErrorLog() );
		return 0;
	}
	catch(const ZErrorMessage& etype)
	{
		ZLOG_ERROR( etype.printErrorLog() );
		return 0;
	}
    return 1;
}
