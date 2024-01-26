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
#ifndef FIND_KEY_DATA_HH_
#define FIND_KEY_DATA_HH_

#include <map>
#include <string>

using namespace std;

template<class T>
inline string find_data(const map<T, string>& tray, const T& arg)
{
	typename map<T, string>::const_iterator it=tray.find(arg);
	if( it == tray.end() ) return "Undefined";
	else return it->second;
}

template<class T>
inline T find_key(const map<T, string>& tray, const string& arg)
{
	for(typename map<T, string>::const_iterator it=tray.begin(); it!=tray.end(); it++)
	{
		if( it->second == arg ) return it->first;
	}
	return T(-1);
}

#endif /* BEAM_TYPE_MEASUREMENT_METHOD_LABEL_HH_ */
