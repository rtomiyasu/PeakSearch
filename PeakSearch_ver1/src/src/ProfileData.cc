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
#include <sstream>
#include "utility_data_structure/range.hh"
#include "utility_func/zstring.hh"
#include "utility_func/zmath.hh"
#include "ProfileData.hh"

using namespace std;

ProfileData::ProfileData()
{
	NumContents = 3;
	m_Wave.resize(NumContents);
	m_Wave_title.resize(NumContents);
}

ProfileData::~ProfileData()
{
}

ZErrorMessageReadingFile ProfileData::readIGORFile(const string& filename, const bool& error_flag)
{
	NumContents = 3;

	m_Wave.clear();
	m_Wave.resize(NumContents);
	m_Wave_title.resize(NumContents);

	ifstream ifs( filename.c_str() );
    if (!ifs)
    {
    	return	ZErrorMessageReadingFile( filename, ZErrorMessage( ZErrorFileNotFound, __FILE__ , __LINE__, __FUNCTION__) );
    }


    int flag = 0;
    string s;

	try{
    	//  read headers while flag <= 0.
		string header;
    	while( flag <= 0 && getnewline(ifs, s) == ZErrorNoError )
    	{
        	if(is_blank(s)) continue;
	       	replace(s.begin(),s.end(),',',' ');
        	istringstream iss(s);
        	iss >> header;
        	
        	if(flag == 0)
        	{
        		if (header != "IGOR") return ZErrorMessageReadingFile(filename, ZErrorMessage(ZErrorNotIGORFile, "Not Igor File", __FILE__, __LINE__, __FUNCTION__));
        		flag = -1;
        	}
        	else if (header.substr(0,5) == "WAVES")
        	{
        		iss >> m_Wave_title[0];
        		if( iss.fail() ) throw 1;

        		iss >> m_Wave_title[1];
        		if( iss.fail() ) throw 1;

        		if( error_flag )
        		{
            		iss >> m_Wave_title[2];
            		if( iss.fail() ) return ZErrorMessageReadingFile(filename, ZErrorMessage(ZErrorErrorsAreNotContained, "The number of columns is less than 3", __FILE__, __LINE__, __FUNCTION__));
        		}
        		else m_Wave_title[2] = "sqrt_of_"+m_Wave_title[1];
        		
        		flag = 1;
        	}
        	else
        	{
        		flag = 0;
        		break;
        	}
    	}
		if( flag <= 0 ) return ZErrorMessageReadingFile(filename, ZErrorMessage("There is no headers", __FILE__, __LINE__, __FUNCTION__) );
	

   		//read Profile Data.
    	Double t;
    	while (getnewline(ifs, s) == ZErrorNoError)
    	{
        	if(is_blank(s)) continue;
        	if(s.substr(0,5) == "BEGIN") continue;
       		if(s.substr(0,3) == "END" ) break;

        	istringstream iss(s);

    		iss >> t;
    		if( iss.fail() ) throw 1;
    		m_Wave[0].push_back(t);

    		iss >> t;
    		if( iss.fail() ) throw 1;
    		m_Wave[1].push_back(t);
    		
    		if( error_flag )
    		{
        		iss >> t;
    			if( iss.fail() ) throw 1;
    			m_Wave[2].push_back(t);
    		}
    		else m_Wave[2].push_back(sqrt_d(t));
    	}
    
	}
	catch(const Int4& num)
	{
       	return ZErrorMessageReadingFile(filename, nerror(ZErrorFileFormatBroken, __FILE__, __LINE__, __FUNCTION__));
	}

    return ZErrorMessageReadingFile();
}


ZErrorMessageReadingFile ProfileData::readGeneralFile(const string& filename, const bool& error_flag)
{
	NumContents = 3;

	m_Wave.clear();
	m_Wave.resize(NumContents);
	m_Wave_title.resize(NumContents);

    ifstream ifs( filename.c_str() );
    if (!ifs)
    {
   		return ZErrorMessageReadingFile(filename, ZErrorMessage( ZErrorFileNotFound, __FILE__ , __LINE__, __FUNCTION__));
    }


    int flag = 0;
    string s;

	try{
    	//  read headers while flag <= 0.
    	while( flag <= 0 && getnewline(ifs, s) == ZErrorNoError )
    	{
        	if(is_blank(s)) continue;

	       	replace(s.begin(),s.end(),',',' ');
        
        	istringstream iss(s);

    		iss >> m_Wave_title[0];
    		if( iss.fail() ) throw 1;
    		iss >> m_Wave_title[1];
    		if( iss.fail() ) throw 1;
    		if( error_flag )
    		{
        		iss >> m_Wave_title[2];
        		if( iss.fail() ) return ZErrorMessageReadingFile(filename, ZErrorMessage(ZErrorErrorsAreNotContained, "The number of columns is lass than 3", __FILE__, __LINE__, __FUNCTION__));
    		}
    		else m_Wave_title[2] = "sqrt_of_"+m_Wave_title[1];

       		flag = 1;
    	}
		if( flag <= 0 )
		{
			return ZErrorMessageReadingFile(filename, ZErrorMessage("There is no headers", __FILE__, __LINE__, __FUNCTION__));
		}
	

   		//read Profile Data.
    	Double t;
    	while ( getnewline(ifs, s) == ZErrorNoError )
    	{
        	if(is_blank(s)) continue;

        	istringstream iss(s);

    		iss >> t;
    		if( iss.fail() ) throw 1;
    		m_Wave[0].push_back(t);

    		iss >> t;
    		if( iss.fail() ) throw 2;
    		m_Wave[1].push_back(t);
    		
    		if( error_flag )
    		{
        		iss >> t;
    			if( iss.fail() ) throw 3;
    			m_Wave[2].push_back(t);
    		}
    		else m_Wave[2].push_back(sqrt_d(t));
    	}
    
	}
	catch(const Int4& num)
	{
        if( num == 1 )
        {
        	return ZErrorMessageReadingFile(filename, ZErrorMessage("Failed to read the first column", __FILE__, __LINE__, __FUNCTION__));
        }
        else if( num == 2 )
        {
        	return ZErrorMessageReadingFile(filename, ZErrorMessage("Failed to read the second column", __FILE__, __LINE__, __FUNCTION__));
        }
        else // if( num == 3 )
        {
        	return ZErrorMessageReadingFile(filename, ZErrorMessage("Failed to read the third column", __FILE__, __LINE__, __FUNCTION__));
        }
    }

    return ZErrorMessageReadingFile();
}


ZErrorMessageReadingFile ProfileData::readRietanFile(const string& filename)
{
	NumContents = 3;

	m_Wave_title.clear();
    m_Wave.clear();

    m_Wave_title.resize(NumContents);
    m_Wave_title[0] = "xphase";
    m_Wave_title[1] = "yphase";
    m_Wave_title[2] = "err_yphase";

	m_Wave.resize(NumContents);

//	const Int4 file_index = setReadingFileName( filename );
    ifstream ifs( filename.c_str() );
    if (!ifs){
   		return ZErrorMessageReadingFile(filename, nerror( ZErrorFileNotFound, __FILE__ , __LINE__, __FUNCTION__));
    }

    int flag = 0, num_data;
    string s;
   	Double min_x, step_width;

	try{
    	//  read the number of data, the starting of x-phase and step width.
    	while( flag <= 0 && getnewline(ifs, s) == ZErrorNoError )
    	{
//        	addLineCounter(file_index);
        	if(s.empty()) continue;
    		if ( s.at(0) == '*' ) continue;

        	istringstream iss(s);

			iss >> num_data;
       		if( iss.fail() ) throw 1;
       		if( num_data <= 0 )
       		{
            	return ZErrorMessageReadingFile(filename, nerror_out_range("Number of Data points", num_data, __FILE__, __LINE__, __FUNCTION__));
      		}

			iss >> min_x;
       		if( iss.fail() ) throw 1;
       		if( min_x <= 0.0 )
       		{
            	return ZErrorMessageReadingFile(filename, nerror_out_range("Minimum of x", min_x, __FILE__, __LINE__, __FUNCTION__));
      		}

			iss >> step_width;
       		if( iss.fail() ) throw 1;
       		if( min_x <= 0.0 )
       		{
            	return ZErrorMessageReadingFile(filename, nerror_out_range("Step width", step_width, __FILE__, __LINE__, __FUNCTION__));
      		}

    		flag = 1;
    	}
		if( flag <= 0 ) throw 1;


   		//read Profile Data.
	   	Double t;
    	while(getnewline(ifs, s) == ZErrorNoError)
    	{
//        	addLineCounter(file_index);
        	if(s.empty()) continue;

	       	replace(s.begin(),s.end(),',',' ');

        	istringstream iss(s);

			iss >> t;
			while( !iss.fail() )
			{
            	m_Wave[0].push_back( min_x );
            	m_Wave[1].push_back( t );
            	m_Wave[2].push_back( sqrt(max(0.0, t) ) );

            	min_x += step_width;

            	iss >> t;
        	}
    	}

    	if( Int4(m_Wave[0].size()) != num_data )  return ZErrorMessageReadingFile(filename, nerror_out_range("Number of Data points", num_data, __FILE__, __LINE__, __FUNCTION__));
	}
	catch(const Int4& num)
	{
		return ZErrorMessageReadingFile(filename, nerror(ZErrorFileFormatBroken, __FILE__, __LINE__, __FUNCTION__));
    }

    return ZErrorMessageReadingFile();
}
