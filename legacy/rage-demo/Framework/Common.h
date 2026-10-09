/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef COMMON_H
#define COMMON_H

#include "Framework/Types.h"
#include "Framework/Profiler.h"

//helper macros
#ifdef _DEBUG
#	define DEBUG_ONLY( fn ) fn
#	define RELEASE_ONLY( fn )
#	define COMPILE_SWITCH( dbg, rls ) dbg
#	define BUILD_MODE_TYPE _T("Debug")
#else
#	define DEBUG_ONLY( fn )
#	define RELEASE_ONLY( fn ) fn
#	define COMPILE_SWITCH( dbg, rls ) rls
#	define BUILD_MODE_TYPE _T("Release")
#endif

#ifndef METRICS
#	define BUILD_MODE BUILD_MODE_TYPE
#else
#	define BUILD_MODE BUILD_MODE_TYPE _T("-Metrics")
#endif

#ifdef WIN32
#	define BUILD_PLATFORM _T("WIN32")
#	define _WIN( fn ) fn
#else
#	define _WIN( fn )
#endif

#define BUILD_MODE_FULL BUILD_PLATFORM _T("-") BUILD_MODE

namespace frx
{
	/** \brief Checks that a handle is valid */
	template< class T > inline bool isValidHandle( const T h ) 
	{ return (h != 0); }
	/** \brief Checks that a pointer is valid */
	inline bool isValidPointer( const void* p ) 
	{ 
		assert(p != (const void*)0xcccccccccccccccc); //Check if the variable is not initialised
		return (p != 0); 
	}
	/** \brief Safely deletes memory pointed to by a pointer and sets it to 0 */
	template<class T>
	inline void safeDelete( T& p ) 
	{ 
		if ( isValidPointer( p ) )
			delete (p);
		p = 0;
	}
	/** \brief Safely deletes memory array pointed to by a pointer and sets it to 0 */
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
	inline bool equal( const Real& a, const Real& b ) 
	{ return (a > (b - Real_EPSILON)) && ( a < ( b + Real_EPSILON )); }


	/** \brief Fast fill array function using power of 2 memcpy
	\param[in] arr Pointer to the target array of type T
	\param[in] length The number of elements in the traget array
	\param[in] fillValue A value to fill the array with 
	\remarks This function uses memcpy for data duplication, therefore a class/struct 
	which contains pointers to data should not be used */
	template <class T>
	inline void fillArray( T* arr, Count length, const T& fillVal )
	{
		if ( length == 0 ) //Nothing to do!
			return;
		T* dest = arr + 1; //the destination point for the copy ( +1 as we write the first value now)
		*arr = fillVal; //Set the first value
		--length; //Deduct the first value
		Count copySize = 1; //Start the copy size at 1 value
		while ( length )
		{
			if ( length < copySize ) //Don't write past the end of the array
				copySize = length;
			memcpy( dest, arr, sizeof(fillVal) * copySize );
			dest += copySize; //Increment the destination pointer
			length -= copySize; //Decrement the length remaining
			copySize <<= 1; //Next copy can be twice the size			
		}
	}
	//FillArray specialisation for char sizes which will be faster with a plain memset which works on a Byte basis
	template<> inline void fillArray( char* arr, Count length, const char& fillVal ) { memset( arr, fillVal, length ); }
	template<> inline void fillArray( unsigned char* arr, Count length, const unsigned char& fillVal ) { memset( arr, fillVal, length ); }	
	template<> inline void fillArray( signed char* arr, Count length, const signed char& fillVal ) { memset( arr, fillVal, length ); }	


} //END: frx

#endif