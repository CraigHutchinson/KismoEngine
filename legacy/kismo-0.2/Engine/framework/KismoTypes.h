/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoTypes.h
 * Status: unique (no other copy of this file name in the archive)
 */
//defines global types
#ifndef _KISMO_TYPES_H
#define	_KISMO_TYPES_H
#include <crtdbg.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <list>
#include <queue>
#include <fstream>

//Include some constants defning limits of variable types
#include <limits>

#ifdef _DEBUG
#	define DEBUG_ONLY( fn ) fn
#	define COMPILE_SWITCH( dbg, rls ) dbg
#	define NAME_SWITCH( nameString ) _T("DEBUG:") ## _T(#nameString)
#else
#	define DEBUG_ONLY( fn )
#	define COMPILE_SWITCH( dbg, rls ) rls
#	define NAME_SWITCH( nameString ) _T(#nameString)
#endif

#ifdef KISMO_REAL64
#	define REAL_TYPE_SWITCH( using32, using64 ) using64
#else
#	define REAL_TYPE_SWITCH( using32, using64 ) using32
#endif

namespace kismo
{		
	enum k_Status
	{
		kc_FALSE = false,
		kc_FAIL = kc_FALSE,
		kc_TRUE = true,
		kc_SUCCESS = kc_TRUE,
		kc_INVALID_PARAM = 2,
	};
	
	typedef void k_Void;
	typedef bool k_Bool;

	typedef float k_Float32; 
	typedef double k_Float64;

	typedef char k_Char8;
	typedef wchar_t k_Char16;
	typedef TCHAR k_Char;

#	define k_Vector std::vector
#	define k_List std::list
#	define k_Map std::map
#	define k_MultiMap std::multimap
#	define k_PriorityQueue std:: priority_queue
#	define k_Set std::set
#	define k_Queue std::queue

	//signed
	typedef signed char k_Byte;
	typedef signed char k_Int8;
	typedef short k_Int16;
	typedef long k_Int32;
	typedef __int64 k_Int64;

	//Unsigned
	typedef unsigned char k_UByte;
	typedef unsigned char k_UInt8;
	typedef unsigned short k_UInt16;
	typedef unsigned long k_UInt32;
	typedef unsigned __int64 k_UInt64;

	//types
	typedef k_UInt32 k_Count;
	typedef k_UInt32 k_Index; //'0 base' index
	typedef k_UInt32 k_Handle; //'1 base' index where 0 is invalid
	typedef size_t k_Size;
	typedef k_UInt32 k_RGBA32; //RGBA colour value packed into 32bit integer
	typedef k_Int8 k_Priority; //Priority value

	//Limits
	const k_UInt8 kc_UInt8_MAX = UCHAR_MAX;
	const k_UInt16 kc_UInt16_MAX = USHRT_MAX;
	const k_UInt32 kc_UInt32_MAX = ULONG_MAX;
	const k_UInt64 kc_UInt64_MAX = ULLONG_MAX;
	const k_Int8 kc_Int8_MAX = SCHAR_MAX;
	const k_Int16 kc_Int16_MAX = SHRT_MAX;
	const k_Int32 kc_Int32_MAX = LONG_MAX;
	const k_Int64 kc_Int64_MAX = LLONG_MAX;
	const k_UInt8 kc_UInt8_MIN = 0;
	const k_UInt16 kc_UInt16_MIN = 0;
	const k_UInt32 kc_UInt32_MIN = 0;
	const k_UInt64 kc_UInt64_MIN = 0;
	const k_Int8 kc_Int8_MIN = SCHAR_MIN;
	const k_Int16 kc_Int16_MIN = SHRT_MIN;
	const k_Int32 kc_Int32_MIN = LONG_MIN;
	const k_Int64 kc_Int64_MIN = LLONG_MIN;
	
	const k_Index kc_Index_MIN = kc_UInt32_MIN;
	const k_Index kc_Index_MAX = kc_UInt32_MAX;
	const k_Count kc_Count_MIN = kc_UInt32_MIN;
	const k_Count kc_Count_MAX = kc_UInt32_MAX;
	const k_Handle kc_Handle_MIN = kc_UInt32_MIN;
	const k_Handle kc_Handle_MAX = kc_UInt32_MAX;
	const k_Priority kc_Priority_MIN = kc_Int8_MIN;
	const k_Priority kc_Priority_MAX = kc_Int8_MAX;

	const k_Float32 kc_Float32_MIN = FLT_MIN;
	const k_Float64 kc_Float64_MIN = DBL_MIN;
	const k_Float32 kc_Float32_MAX = FLT_MAX;
	const k_Float64 kc_Float64_MAX = DBL_MAX;
	const k_Float32 kc_Float32_EPSILON = FLT_EPSILON;
	const k_Float64 kc_Float64_EPSILON = DBL_EPSILON;

	typedef REAL_TYPE_SWITCH( k_Float32, k_Float64 ) k_Real;
	const k_Real kc_Real_MIN = REAL_TYPE_SWITCH( kc_Float32_MIN, kc_Float64_MIN );
	const k_Real kc_Real_MAX = REAL_TYPE_SWITCH( kc_Float32_MAX, kc_Float64_MAX );
	const k_Real kc_Real_EPSILON = REAL_TYPE_SWITCH( kc_Float32_EPSILON, kc_Float64_EPSILON );

	//Streams
	typedef std::basic_ofstream< k_Char > k_OFStream;
	typedef std::basic_ifstream< k_Char > k_iFStream;
	typedef std::basic_fstream< k_Char > k_FStream;
	typedef std::basic_istream< k_Char > k_IStream;
	typedef std::basic_istream< k_UByte > k_UByteIStream;
	typedef std::basic_ostream< k_Char > k_OStream;
	typedef std::basic_ostringstream< k_Char > k_OStringStream;
	typedef std::basic_istringstream< k_Char > k_IStringStream;

	//Some common arrays
	typedef k_Vector< k_Byte > k_ByteArray;

	//------------------------------
	// common checks - cannot be in Common because of circular dependancies
	//------------------------------
	template< class T > inline bool isValidHandle( const T h ) { return (h != 0); }
	inline bool isValidPointer( const void* p ) { assert(p != (const void*)0xcccccccccccccccc); return (p != 0) DEBUG_ONLY( && p != (const void*)0xcccccccccccccccc ); }
	#define succeeded( fn ) ( fn == kc_SUCCESS )
	#define failed( fn ) ( fn == kc_FAIL )

	template<class T>
	inline void safeDelete( T p ) 
	{ 
		if ( isValidPointer( p ) )
			delete (p);
		p = 0;
	}

	template<class T>
	inline void safeDeleteArray( T& p ) 
	{ 
		if ( isValidPointer( p ) )
			delete[] (p);
		p = 0;
	}

	/** \brief Shouldn't use the == operator for floats, this function compares the floats
	using the epsilon to check for mathematical equality 
	\return If the two values are identical */
	inline bool fEqual( const k_Float32& a, const k_Float32& b ) 
	{ return (a > (b - kc_Float32_EPSILON)) && ( a < ( b + kc_Float32_EPSILON )); }

	/** \brief Shouldn't use the == operator for floats, this function compares the floats
	using the epsilon to check for mathematical equality 
	\return If the two values are identical */ 
	inline bool fEqual( const k_Float64& a, const k_Float64& b ) 
	{ return (a > (b - kc_Float64_EPSILON)) && ( a < ( b + kc_Float64_EPSILON )); }

} //END: kismo

#endif
