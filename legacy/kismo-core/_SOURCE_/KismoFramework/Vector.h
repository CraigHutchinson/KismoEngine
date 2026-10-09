/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Vector.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMO_VECTOR_H
#define KISMO_VECTOR_H

#include "KismoFramework/Common.h"
#include "KismoFramework/Types.h"
#include <vector>

/**
	\brief Defines some basic types used in the rendering of ray
*/
namespace kismo
{
	
	//Typedef the string types
	typedef std::vector<Byte> ByteArray;

	/*typedef std::basic_istream< Byte, std::char_traits< Byte > > IBStream;
	typedef std::basic_ostream< Byte, std::char_traits< Byte > > OBStream;
	typedef std::basic_ifstream< Byte, std::char_traits< Byte > > IBFStream;
	typedef std::basic_ofstream< Byte, std::char_traits< Byte > > OBFStream;
	*/

	//template< class _ValType, class _TCount >
	//IBFStream& operator >> ( IBFStream& stream, typename Vector< _ValType, true, _TCount >& rhs )
	//{
	//	Byte tchar = '\0'; stream >> tchar; Assert( tchar == '\0' );

	//	Size valueSize;
	//	stream >> valueSize >> tchar; Assert( tchar == '\0' ); ///< input the size of the data elements so we can make sure the version of data is compatible at a later date
	//	if ( valueSize != sizeof( _ValType ) )
	//	{
	//		stream.setstate( std::ios_base::failbit );
	//	}
	//	else
	//	{
	//		_TCount size = 0;
	//		stream >> size >> tchar; Assert( tchar == '\0' ); //< input the number of elements in the vector
	//		rhs.resize( size ); //< Resize the vector to store this data
	//		if ( size )
	//		{
	//			std::streamsize bytesRemaining = (size_t)valueSize * size;
	//			Assert( bytesRemaining < Int32_Max );
	//			stream.read( (Byte*)rhs.c_ptr(), bytesRemaining );
	//			Assert( stream.gcount() == valueSize * size );
	//		}
	//	}
	//		
	//	stream >> tchar; Assert( tchar == '\0' );

	//	return stream;
	//}
#if 0
	/**
		\brief Reads the vector from file
		\remarks This slower method must be used if _IntrinsicType == false and will use the stream >> operator of the data type
	*/
	template< class _ValType, class _TCount >
	IBStream& operator >> ( IBStream& stream, typename Vector< _ValType, false, _TCount >& rhs )
	{		
		//Byte tchar = '\0'; stream >> tchar; Assert( tchar == '\0' );

		_TCount size = 0;
		stream.read( (Byte*)&size, sizeof( _TCount ) );
	
		if ( size != 0 )
		{			
			Size valueSize;
			stream.read( (Byte*)&valueSize, sizeof( Size ) ); ///< input the size of the data elements so we can make sure the version of data is compatible at a later date
			if ( valueSize != sizeof( _ValType ) )
			{
				stream.setstate( std::ios_base::failbit );
			}
			else
			{
				rhs.resize( size ); //< Resize the vector to store this data			
				
				//As the data type is not intrinsic we must read each element individually using its stream operator
				for ( typename Vector< _ValType, false, _TCount >::iterator iVal = rhs.begin(); iVal != rhs.end(); ++iVal )
					stream >> *iVal;
			}
		}
			
		//stream >> tchar; Assert( tchar == '\0' );

		return stream;
	}

	/**
		\brief Checks two vectors for equality
		\remarks This fast method is only valid if _IntrinsicType == true which means the data has no pointers and can be memory-compared on its data bytes
	*/
	template< class _ValType, class _TCount >
	bool operator == ( const typename Vector< _ValType, true, _TCount >& lhs, const typename Vector< _ValType, true, _TCount >& rhs )
	{
		if ( lhs.size() != rhs.size() )
			return false;

		//Size is the same so do a memory compare
		return memcmp( lhs.c_ptr(), rhs.c_ptr(), sizeof( _ValType ) * lhs.size() ) == 0;
	}

	/**
		\brief Checks two vectors for equality
		\remarks This slower method must be used if _IntrinsicType == false and will use the datas equality operator
	*/
	template< class _ValType, class _TCount >
	bool operator == ( const typename Vector< _ValType, false, _TCount >& lhs, const typename Vector< _ValType, false, _TCount >& rhs )
	{
		if ( lhs.size() != rhs.size() )
			return false;

		//Size is the same so do a element compare
		typename Vector< _ValType, false, _TCount >::const_iterator iLhsVal = lhs.begin();
		typename Vector< _ValType, false, _TCount >::const_iterator iRhsVal = rhs.begin();
		for (  ; iLhsVal != lhs.end(); ++iLhsVal, ++iRhsVal )
			if ( *iLhsVal != *iRhsVal ) ///< TODO: This does not work with vectors of pointers!
				return false;
		
		return true; //< All elements are the same
	}

	template< class _ValType, bool _IntrinsicType, class _TCount >
	bool operator != ( const typename Vector< _ValType, _IntrinsicType  >& lhs, const typename Vector< _ValType, _IntrinsicType  >& rhs )
	{
		return !(lhs == rhs);
	}
#endif

} //END: Kismo

//#endif
#endif