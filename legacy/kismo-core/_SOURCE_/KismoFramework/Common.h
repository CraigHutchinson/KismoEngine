/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
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

#ifndef KISMOFRAMEWORK_COMMON_H
#define KISMOFRAMEWORK_COMMON_H

#include "KismoFramework/Types.h"
//#include "KismoFramework/Profiler.h"


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

#if defined(_WIN64)
#	define BUILD_PLATFORM _T("WIN64")
#	define _WIN( fn ) fn
#elif defined(WIN32)
#	define BUILD_PLATFORM _T("WIN32")
#	define _WIN( fn ) fn
#else
#	define _WIN( fn )
#endif

#define BUILD_MODE_FULL BUILD_PLATFORM _T("-") BUILD_MODE

/// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#if defined( _DEBUG )
#define Assert( exp ) \
   (void)( !(exp) && kismo::kismoAssertFunction( _T(#exp),  _T(#exp),  __LINE__, _T(__FILE__), 0 ) )

#define AssertEx( exp, description ) \
   {  static bool ignoreAlways = false; \
     if (!ignoreAlways && !(exp)) { \
	 if( kismo::kismoAssertFunction( _T(#exp), _T(description),  __LINE__, _T(__FILE__), &ignoreAlways ) ) \
         { __debugbreak(); } \
      } \
   }
#else
#define Assert( exp ) (void)(false)
#define AssertEx( exp, description )
#endif

namespace kismo
{
	bool kismoAssertFunction( const Char* expression, const Char* description, UInt32 line, const Char* file, bool* alwaysIgnore );
	//bool kismoAssertFunction( bool boolTest, Char8* description, UInt32 line, Char8* file, bool& alwaysIgnore );



	//Forwards declare some common classes
	class Log;

	/** \brief Checks that a handle is valid */
	template< class T > inline bool isValidHandle( const T h ) 
	{ return (h != 0); }
	/** \brief Checks that a pointer is valid */
	inline bool isValidPointer( const void* p ) 
	{ 
		AssertEx(p != (const void*)0xcccccccccccccccc, "Invalid Pointer" ); //Check if the variable is not initialised
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

	/** 
		\brief Shouldn't use the == operator for floats, this function compares the floats
			using the epsilon to check for mathematical equality 
		\return If the two values are identical 
	*/
	inline bool equal( const Real a, const Real b, const Real epsilon = Real_Epsilon ) 
	{ return (a > (b - epsilon)) && ( a < ( b + epsilon )); }


	/** 
		\brief Fast fill array function using power of 2 memcpy
		\param[in] arr Pointer to the target array of type T
		\param[in] length The number of elements in the traget array
		\param[in] fillValue A value to fill the array with 
		\remarks This function uses memcpy for data duplication, therefore a class/struct 
		which contains pointers to data should not be used 
	*/
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

	/**
		\brief May be faster if it known that the memory is an alligned 64bit type
	*/
	/*
	void memset64( void *_dest, uint64_t _value, uintptr_t _size )
	{
	  uintptr_t i;
	  for( i = 0; i < (_size & (~7)); i+=8 )
	  {
		memcpy( ((char*)_dest) + i, &_value, 8 );
	  }  
	  for( ; i < _size; i++ )
	  {
		((char*)_dest)[i] = ((char*)&_value)[i&7];
	  }  
	}
	*/

} //END: kismo

#endif