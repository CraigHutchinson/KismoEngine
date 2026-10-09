/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\BaseTypes.h
 * Status: duplicate of kismo-v0.3/Framework/BaseTypes.h
 */
#ifndef BASETYPES_H
#define BASETYPES_H

#include <tchar.h>
#include <float.h>
#include <limits>//Include some constants defning limits of variable types

///Here for looped header dependancies
#include <vector>
#define Vector std::vector
#include <iostream>
#define OutStream std::basic_ostream< Char >
#define InStream std::basic_istream< Char >
#define Stream std::basic_iostream< Char >
#include <fstream>
#define OutFStream std::basic_ofstream< Char >
#define InFStream std::basic_ifstream< Char >
#define FStream std::basic_fstream< Char >
#include <sstream>
#define StringStream std::basic_stringstream< Char >

#define Stream_SizeMax std::numeric_limits<std::streamsize>::max()

typedef void Void;
typedef bool Bool;

typedef float Float32; 
typedef double Float64;

typedef char Char8;
typedef wchar_t Char16;
typedef TCHAR Char;

//signed
typedef signed char Byte;
typedef signed char Int8;
typedef short Int16;
typedef long Int32;
typedef __int64 Int64;

//Unsigned
typedef unsigned char UByte;
typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned long UInt32;
typedef unsigned __int64 UInt64;

//types
typedef UInt32 Count;
typedef UInt32 Index; //'0 base' index
typedef UInt32 Handle; //'1 base' index where 0 is invalid
typedef size_t Size;
typedef UInt32 Rgba32; //RGBA colour value packed into 32bit integer
typedef Int8 Priority; //Priority value

//Limits
const UInt8 UInt8_MAX = UCHAR_MAX;
const UInt16 UInt16_MAX = USHRT_MAX;
const UInt32 UInt32_MAX = ULONG_MAX;
const UInt64 UInt64_MAX = ULLONG_MAX;
const Int8 Int8_MAX = SCHAR_MAX;
const Int16 Int16_MAX = SHRT_MAX;
const Int32 Int32_MAX = LONG_MAX;
const Int64 Int64_MAX = LLONG_MAX;
const UInt8 UInt8_MIN = 0;
const UInt16 UInt16_MIN = 0;
const UInt32 UInt32_MIN = 0;
const UInt64 UInt64_MIN = 0;
const Int8 Int8_MIN = SCHAR_MIN;
const Int16 Int16_MIN = SHRT_MIN;
const Int32 Int32_MIN = LONG_MIN;
const Int64 Int64_MIN = LLONG_MIN;

const Index Index_MIN = UInt32_MIN;
const Index Index_MAX = UInt32_MAX;
const Count Count_MIN = UInt32_MIN;
const Count Count_MAX = UInt32_MAX;
const Handle Handle_MIN = UInt32_MIN;
const Handle Handle_MAX = UInt32_MAX;
const Priority Priority_MIN = Int8_MIN;
const Priority Priority_MAX = Int8_MAX;

const Float32 Float32_MIN = FLT_MIN;
const Float64 Float64_MIN = DBL_MIN;
const Float32 Float32_MAX = FLT_MAX;
const Float64 Float64_MAX = DBL_MAX;
const Float32 Float32_EPSILON = FLT_EPSILON;
const Float64 Float64_EPSILON = DBL_EPSILON;

#ifndef REAL_64 ///Check if 64bit-real has been defined or not
	typedef Float32 Real;
	const Real Real_MIN = Float32_MIN;
	const Real Real_MAX = Float32_MAX;
	const Real Real_EPSILON = Float32_EPSILON;
#else
	typedef Float64 Real;
	const Real Real_MIN = Float64_MIN;
	const Real Real_MAX = Float64_MAX;
	const Real Real_EPSILON = Float64_EPSILON;
#endif

/** \brief Framework namespace contains all framework/utility classes
*/
namespace frx{}

#endif