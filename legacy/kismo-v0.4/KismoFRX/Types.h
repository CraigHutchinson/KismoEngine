/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Types.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Types.h (same name, content differs)
 */
#ifndef KISMOFRX_TYPES_H
#define KISMOFRX_TYPES_H

// Dll export interface
#ifdef KISMOFRX_EXPORTS	
#	define KISMOFRX_EXPORT __declspec(dllexport)
#else	
#	define KISMOFRX_EXPORT __declspec(dllimport)
#endif 

/**
	\brief Specifies the Real/float accuracy of most math as Float64 if REAL_DOUBLE != 0
*/
#define REAL_DOUBLE 0


/**
	\brief Specifies the timing calculations should be performed in 64bit math where possible
*/
#define DTIME_DOUBLE 0

#include <cassert>
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

	typedef std::basic_string< Char > String; //String

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
	typedef UInt32 Size; ///< Stores the size of an array etc
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

	#define Float32_Max FLT_MAX
	#define Float64_Max DBL_MAX
	#define Float32_Epsilon FLT_EPSILON
	#define Float32_Min -FLT_MAX
	#define Float64_Min -DBL_MAX
	#define Float64_Epsilon DBL_EPSILON
#if REAL_DOUBLE
	#define Real_Max Float64_Max
	#define Real_Min Float64_Min
	#define Real_Epsilon Float64_Epsilon
#else
	#define Real_Max Float32_Max
	#define Real_Min Float32_Min
	#define Real_Epsilon Float32_Epsilon
#endif
	#define Int32_Max INT_MAX
	#define Int32_Min INT_MIN
	#define Int16_Max SHRT_MAX
	#define Int16_Min SHRT_MIN
	#define UInt32_Max UINT_MAX
	#define UInt32_Min 0
	#define UInt16_Max USHRT_MAX
	#define UInt16_Min 0
	#define Count_Max UInt32_Max
	#define Count_Min UInt32_Min
	#define Handle_Max UInt32_Max
	#define Handle_Min UInt32_Min
	#define Index_Max UInt32_Max
	#define Index_Min UInt32_Min
	#define Index16_Max UInt16_Max
	#define Index16_Min UInt16_Min

} //END: kismo

#endif