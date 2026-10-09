/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\TypeInfo.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/TypeInfo.h (same name, content differs)
 */
#ifndef FRX_TYPEINFO_H
#define FRX_TYPEINFO_H

#include "Framework/BaseTypes.h"

/** \brief Enumerator for the data types
*/
enum eDataType
{
	Type_Float32, Type_Float64,
	Type_Int8, Type_Int16, Type_Int32, Type_Int64,
	Type_UInt8, Type_UInt16, Type_UInt32, Type_UInt64,
	Type_Char16, Type_Size,

	Type_Unknown = 255,///< Enumerator fo unknown types!

	Type_Char8 = Type_UInt8, Type_Byte = Type_Int8, Type_UByte = Type_UInt8,
};

/** \brief Global template function for obtaining the data type enumerator of a basic data type
\remarks if an unknown type is passed in Type_Unknown will be returned
\par
	Example usage -		eDataType type = typeOf< Real >()
*/
template< class _Type > inline eDataType typeOf() { return Type_Unknown; }
template<> inline eDataType typeOf< Char8 >() { return Type_Char8; }
template<> inline eDataType typeOf< Char16 >() { return Type_Char16; }
template<> inline eDataType typeOf< Float32 >() { return Type_Float32; }
template<> inline eDataType typeOf< Float64 >() { return Type_Float64; }
template<> inline eDataType typeOf< UInt8 >() { return Type_Int8; }
template<> inline eDataType typeOf< UInt16 >() { return Type_Int16; }
template<> inline eDataType typeOf< UInt32 >() { return Type_Int32; }
template<> inline eDataType typeOf< UInt64 >() { return Type_Int64; }
template<> inline eDataType typeOf< Int8 >() { return Type_UInt8; }
template<> inline eDataType typeOf< Int16 >() { return Type_UInt16; }
template<> inline eDataType typeOf< Int32 >() { return Type_UInt32; }
template<> inline eDataType typeOf< Int64 >() { return Type_UInt64; }
template<> inline eDataType typeOf< Size >() { return Type_Size; }

/** \brief Global template function for obtaining the size of a data type from its enum
\remarks if an unknown type is passed in Type_Unknown will be returned
*/
inline UInt8 sizeOf( const eDataType type )
{ 
	switch ( type ) {
	case Type_Int8:		case Type_UInt8:	return 1;
	case Type_Int16:	case Type_UInt16:	case Type_Char16:	return 2;
	case Type_Float32:	case Type_Int32:	case Type_UInt32:	return 4;	 
	case Type_Int64:	case Type_UInt64:	case Type_Float64:	return 8;
	case Type_Size:		return sizeof( Size );
	default:			return 0;
	}
}


#endif