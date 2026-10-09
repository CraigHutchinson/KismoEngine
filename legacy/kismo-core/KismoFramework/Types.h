/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Types.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Types.h (same name, content differs)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_TYPES_H
#define KISMOFRAMEWORK_TYPES_H

/*
// Dll export interface
#if KISMOFRAMEWORK_DLL
#ifdef KISMOFRAMEWORK_EXPORTS	
#	define KISMOFRAMEWORK_EXPORT __declspec(dllexport)
#else	
#	define KISMOFRAMEWORK_EXPORT__declspec(dllimport)
#endif 
#else
#define KISMOFRAMEWORK_EXPORT
#endif
*/

#ifndef UNICODE
#define tcout std::cout
#define tostream std::ostream
#define tofstream std::ofstream
#else
#define tcout std::wcout
#define tostream std::wostream
#define tofstream std::wofstream
#endif

/**
	\brief Specifies the Real/float accuracy of most math as Float64 if REAL_DOUBLE != 0
*/
#define REAL_DOUBLE 0


/**
	\brief Specifies the timing calculations should be performed in 64bit math where possible
*/
#define DTIME_DOUBLE 0

#include <cAssert>
#include <TCHAR.h>
//#include <bitset>
#include <limits.h>
#include <float.h>
#include <string>
//#include <map>
//#include <set>
//#include <bitset>
//#include <list>
//#include <vector>
//#include <ios>
//#include <fstream>

#define USE_TCHAR //< Set this to enable project specific unicode/utf8 build
#ifdef USE_TCHAR
#	define RTCHAR TChar
#else
#	define RTCHAR Char8
#endif

namespace kismo
{
	typedef void Void;
	typedef bool Bool;

	typedef float Float32; 
	typedef double Float64; //Long Float32 = double

#if REAL_DOUBLE
	typedef Float64 Real; //Real
#else
	typedef Float32 Real; //Real
#endif

#if DTIME_DOUBLE
	typedef Float64 DTime; //Simulation time format
#else
	typedef Float32 DTime; //Simulation time format
#endif

	typedef char Char8;
	typedef wchar_t Char16;
	typedef TCHAR TChar;
	typedef RTCHAR Char; //< Specified via USE_TCHAR precompiler to be either Char* or TCHAR

	//typedef std::basic_string< Char > String; //String

	//signed
	typedef char Byte;
	typedef signed char Int8;
	typedef short Int16;
	typedef int Int32;
	typedef __int64 Int64;

	//Unsigned
	typedef unsigned char UByte;
	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef unsigned __int64 UInt64;

	//types
	typedef UInt32 Count; ///< A counter variable
	typedef size_t Size; ///< Stores the size of an array etc
	typedef UInt16 Size16; ///< Stores the size of an array etc
	typedef UInt32 Index; //'0 base' index
	typedef UInt16 Index16; //'0 base' short index
	typedef UInt32 Handle; //'1 base' index where 0 is invalid
	typedef UInt8 Priority; //Priority value

	//Streams
	//typedef std::basic_iostream< Char, std::char_traits< Char > > Stream;
	//typedef std::basic_istream< Char, std::char_traits< Char > > IStream;
	//typedef std::basic_ostream< Char, std::char_traits< Char > > OStream;
	//typedef std::basic_ifstream< Char, std::char_traits< Char > > IFStream;
	//typedef std::basic_ofstream< Char, std::char_traits< Char > > OFStream;
	//typedef std::basic_fstream< Char, std::char_traits< Char > > FStream;

	//#define BitSet std::bitset

	const Float32 Float32_Max = FLT_MAX;
	const Float64 Float64_Max = DBL_MAX;
	const Float32 Float32_Epsilon = FLT_EPSILON;
	const Float32 Float32_Min = -FLT_MAX;
	const Float64 Float64_Min = -DBL_MAX;
	const Float64 Float64_Epsilon = DBL_EPSILON;
#if REAL_DOUBLE
	const Real Real_Max = Float64_Max;
	const Real Real_Min = Float64_Min;
	const Real Real_Epsilon = Float64_Epsilon;
#else
	const Real Real_Max = Float32_Max;
	const Real Real_Min = Float32_Min;
	const Real Real_Epsilon = Float32_Epsilon;
#endif
	const Int8 Int8_Max = SCHAR_MAX;
	const Int8 Int8_Min = SCHAR_MIN;
	const UInt8 UInt8_Max = CHAR_MAX;
	const UInt8 UInt8_Min = CHAR_MIN;
	const Int16 Int16_Max = SHRT_MAX;
	const Int16 Int16_Min = SHRT_MIN;
	const UInt16 UInt16_Max = USHRT_MAX;
	const UInt16 UInt16_Min = 0;
	const Int32 Int32_Max = INT_MAX;
	const Int32 Int32_Min = INT_MIN;
	const UInt32 UInt32_Max = UINT_MAX;
	const UInt32 UInt32_Min = 0;
	const Count Count_Max = UInt32_Max;
	const Count Count_Min = UInt32_Min;
	const Count Size_Max = UInt32_Max;
	const Count Size_Min = UInt32_Min;
	const Handle Handle_Max = UInt32_Max;
	const Handle Handle_Min = UInt32_Min+1;
	const Handle Handle_Invalid = UInt32_Min;
	const Index Index_Max = UInt32_Max-1;
	const Index Index_Min = UInt32_Min;
	const Index Index_Invalid = UInt32_Max;
	const Index16 Index16_Max = UInt16_Max;
	const Index16 Index16_Min = UInt16_Min;
	const Size16 Size16_Max = UInt16_Max;
	const Size16 Size16_Min = UInt16_Min;

} //END: kismo

#endif