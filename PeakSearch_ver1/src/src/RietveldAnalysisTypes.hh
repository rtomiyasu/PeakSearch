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
#ifndef RietveldAnalysisTypes_HH_
#define RietveldAnalysisTypes_HH_

#if defined(__cplusplus)

#include<vector>
#include<map>
#include<complex>
#include<cmath>

using namespace std;
#endif

typedef char Char;
typedef unsigned char UChar;
typedef short Int2;
typedef unsigned short UInt2;
typedef int Int4;
typedef unsigned int UInt4;
typedef long long Int8;
//typedef unsigned long long UInt8;	//Mac OS X define UInt8 (typedef unsigned char)
typedef double Double;
typedef float Float;

typedef Char ID_type;	// 0(fix), 1(vary) or 2(dependent).
typedef Char ZRietveldID;	// ID For GUI.

typedef enum _ZRietveldAnalysisMode {
	kZRietveldAnalysisModeDefault = 0,
	kZRietveldAnalysisRunModeFitting = 1 << 0,	//!< if 0, Mode is Fitting.
	kZRietveldAnalysisParameterModeFull = 1 << 1,	//!, if 0, Mode is Auto Generate Parameter.
	
	kZRietveldAnalysisModeSimpleSimulationParametersAndConstraintsAreGenerated = kZRietveldAnalysisModeDefault,
	kZRietveldAnalysisModeSimpleSimulationAllParameters = kZRietveldAnalysisParameterModeFull,
	kZRietveldAnalysisModeFitting = kZRietveldAnalysisRunModeFitting,
	kZRietveldAnalysisModeRietveldMethod = kZRietveldAnalysisRunModeFitting + kZRietveldAnalysisParameterModeFull,
}ZRietveldAnalysisMode;

typedef enum {
	kZRietveldTemperatureFactorUnitB = 0,
	kZRietveldTemperatureFactorUnitU = 1,
	kZRietveldTemperatureFactorUnitUndefined = 2
} ZRietveldTemperatureFactorUnit;

#if defined(__cplusplus)
extern "C" {
#endif

/*! @enum ZRietveldBeamType
 FitDataParam.hh m_Beam_Type
 */	
typedef enum _ZRietveldBeamType
{
	kZRietveldBeamTypeNeutron = 0,	//!< Neutron Beam
	kZRietveldBeamTypeXRay = 1	//!< X-ray
}ZRietveldBeamType;

/*! @enum ZRietveldMesurementMethod
 FitDataParam.hh m_TOForAD
 */
typedef enum _ZRietveldMesurementMethod {
	kZRietveldMeasurementMethodUnknown = -1,
	kZRietveldMeasurementMethodTimeOfFlight = 0,	//!< TOF (Time Of Flight)
	kZRietveldMeasurementMethodReactor = 1,	//!< Reactor (Neutron)
	kZRietveldBraggBrentanoGeometryConventionalDivergenceSlit = 2,//!< Bragg-Brentano geometry (conventional divergence slit)
	kZRietveldBraggBrentanoGeometryAutomaticDivergenceSlit = 3	//!<Bragg-Brentano geometry (automatic divergence slit*)
}ZRietveldMesurementMethod;

/*! @enum ZRietveldHistogramFileFormat
 */
typedef enum _ZRietveldHistogramFileFormat {
	kZRietveldHistogramFileFormatUnknown = -1,
	kZRietveldHistogramFileFormatGeneral = 0,	//!< General Format
	kZRietveldHistogramFileFormatIGOR = 1,	//!< IGOR Format
	kZRietveldHistogramFileFormatRietan = 2	//!< Rietan Format
}ZRietveldHistogramFileFormat;

inline map<_ZRietveldHistogramFileFormat, string> putFormatTypeLabel()
{
	map<_ZRietveldHistogramFileFormat, string> ans;
	ans[kZRietveldHistogramFileFormatGeneral] = "XY";
	ans[kZRietveldHistogramFileFormatIGOR] = "IGOR";
	ans[kZRietveldHistogramFileFormatRietan] = "Rietan";

	return ans;
}

typedef enum {	//for switch
	_ZRietveldIDUnknown = -1,
	_ZRietveldIDFixed = 0,
	_ZRietveldIDVary = 1,
	_ZRietveldIDDepend = 2,
	_ZRietveldIDPhase = 3,
} etype_ID;

#if defined(__cplusplus)
}
#endif


#if defined(__cplusplus)

typedef vector<double> Vec_DP;
typedef vector<int> Vec_INT;
typedef vector<char> Vec_CHAR;
typedef vector<bool> Vec_BOOL;
//typedef vector< vector<int> > Mat_INT_iconstr;
typedef vector< vector<double> > Mat_DP_iconstr;

typedef complex<double> CMPX_DP;

struct mconstr_DP{
	ID_type ID;
	map<Int4, Double> constr;
};

// Mat_DP_save are used as a container of flags and constraints for the saving memory.
typedef vector<mconstr_DP> MMat_DP_constr;

typedef vector< vector<double> > each_Vec_DP;
typedef vector< vector<Int4> > each_Vec_INT;

// non-principal axis.
inline Int4 npaxis(const Int4& b, const Int4& i)
{
	static const Int4 npaxis[][3] = { {1,2,0}, {2,0,1} };
	return npaxis[b][i];
}

typedef struct{
	Double Rwp;
	Double Rp;
	Double Re;
	Double DW;
	Vec_DP RB;
	Vec_DP RF;
} statistics_data;
#endif

#endif /*COMMON_HH_*/
