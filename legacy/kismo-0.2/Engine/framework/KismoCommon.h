/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoCommon.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_COMMON_H
#define	_KISMO_COMMON_H

#include "framework/KismoTypes.h"
#include "framework/KismoMath.h"
#include "framework/KismoSingleton.h"
#include "framework/KismoPlatform.h"
#include "framework/KismoString.h"

namespace kismo
{		
	//------------------------------
	// Common types
	//------------------------------
	typedef frx::StringT k_String; //String of current build type
	typedef frx::String8 k_String8; //String
	typedef frx::String16 k_String16; //String
	
	typedef k_Map< k_String, k_String > k_KeyValueList;

	namespace frx
	{			


		//------------------------------
		// Invalid values
		//------------------------------	
		const k_String kc_NOTSETNAME = _T("<notset>"); //Constant value not set string
		const k_Int32 kc_NOTSETInt32 = 0xefefefef; //Not set integer value (not really to be used if this value is likely!)

		//------------------------------------------
		// Bool-string conversions
		//		boolToString - outputs descriptor 'TRUE' or 'FALSE'
		//		stringToBool - converts 'TRUE', or '1', to true and 'FALSE', '0', & all zero-value strings to false
		//------------------------------------------
		const k_String kc_TRUE_string = _T("TRUE");
		const k_String kc_FALSE_string = _T("FALSE");

		template <class T>
		k_String toString( const T& t )
		{
			k_OStringStream oss; // create a stream
			oss << t;       // insert value to stream 
			return oss.str();   // return as a string
		}

		/** Fast fill array function using power of 2 memcpy
		\param[in] arr Pointer to the target array of type T
		\param[in] length The number of elements in the traget array
		\param[in] fillValue A value to fill the array with 
		\remarks This function uses memcpy for data duplication, therefore a class/struct 
		which contains pointers to data should not be used */
		template <class T>
		inline void fillArray( T* arr, k_Count length, const T& fillVal )
		{
			if ( length == 0 ) //Nothing to do!
				return;
			T* dest = arr + 1; //the destination point for the copy ( +1 as we write the first value now)
			*arr = fillVal; //Set the first value
			--length; //Deduct the first value
			k_Count copySize = 1; //Start the copy size at 1 value
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
		//FillArray specialisation
		template<> inline void fillArray( char* arr, k_Count length, const char& fillVal ) { memset( arr, fillVal, length ); }
		template<> inline void fillArray( unsigned char* arr, k_Count length, const unsigned char& fillVal ) { memset( arr, fillVal, length ); }	
		template<> inline void fillArray( signed char* arr, k_Count length, const signed char& fillVal ) { memset( arr, fillVal, length ); }	

		//inline k_Count stringLength( const k_Char8* str ) { return (k_Count)strlen( str ); }
		//inline k_Count stringLength( const k_Char16* str ) { return (k_Count)wcslen( str ); }


		inline k_String boolToString( bool b ) { return ( b == kc_TRUE ? kc_TRUE_string : kc_FALSE_string ); }
		inline bool stringToBool( const k_String& b ) { return ( b == kc_TRUE_string ? kc_TRUE : (_tstoi( b.c_str() ) ? kc_TRUE : kc_FALSE) ); }

		//structure for sorting string pointers
		template < class _Ty > 
		struct dereferenceSortLess : public std::binary_function<_Ty, _Ty, bool>
		{ bool operator()(const _Ty* _Left, const _Ty* _Right) const { return ( (*_Left) < (*_Right) ); } };
		template < class _Ty > 
		struct dereferenceSortGreater : public std::binary_function<_Ty, _Ty, bool>
		{ bool operator()(const _Ty* _Left, const _Ty* _Right) const { return ( (*_Left) > (*_Right) ); } };

		//enum for the axis'
		enum ke_Axis{
			kc_X = 0, kc_Y, kc_Z, _kc_Axis
		};

		//enum for the order that rotations are performed
		enum ke_AxisOrder{
			kc_XYZ = 0, kc_YZX, kc_ZXY, kc_XZY, kc_YXZ, kc_ZYX, _kc_AxisOrder
		};

		//Array holds the index to the x/y/z for the axis order
		const ke_Axis kc_AxisOrders[ _kc_AxisOrder ][ _kc_Axis ] =
		{
			{ kc_X, kc_Y, kc_Z }, 
			{ kc_Y, kc_Z, kc_X }, 
			{ kc_Z, kc_X, kc_Y }, 
			{ kc_X, kc_Z, kc_Y }, 
			{ kc_Y, kc_X, kc_Z }, 
			{ kc_Z, kc_Y, kc_X }
		};


		//Version number
		struct Version 
		{ 
			k_UInt8 major;
			k_UInt8 minor; 
			k_UInt16 build; 

			friend k_String& operator << (k_String& o, const Version& ver )
			{
				o.grow( 8 );
				o << _T('v') << ver.major << _T('.') << ver.minor << _T('.') << ver.build;
				return o;
			}
		};
	
	} // END: frx

} // END: kismo

#endif
