/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Variant.cpp
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



#include "Variant.h"
#include <sstream>


namespace kismo
{
	String Variant::toString( UInt16 defaultVal ) const
	{
		if ( m_type == Type_String )
			return m_data.charArray;
		std::basic_stringstream<Char> ss;
		switch( m_type )
		{				
			case Type_Float32:	ss << m_data.float32; break;
			case Type_Float64:	ss << m_data.float64; break;
			case Type_Int32:	ss << m_data.int32; break;
			case Type_UInt32:	ss << m_data.uint32; break;
			case Type_Int16:	ss << m_data.int16; break;
			case Type_UInt16:	ss << m_data.uint16; break;
		}
		return String( ss.str().c_str(), Count(ss.str().length()) );			
	}
		
	template< class T >
	T Variant::to( T defaultVal ) const
	{
		switch( m_type )
		{				
			case Type_Boolean:	return (T)m_data.boolean; break;
			case Type_Float32:	return (T)m_data.float32; break;
			case Type_Float64:	return (T)m_data.float64; break;
			case Type_Int32:	return (T)m_data.int32; break;
			case Type_UInt32:	return (T)m_data.uint32; break;
			case Type_Int16:	return (T)m_data.int16; break;
			case Type_UInt16:	return (T)m_data.uint16; break;	
			case Type_String:
				Assert( false );
			default:
				return defaultVal;
		}
	}

	bool Variant::toBoolean( bool defaultVal ) const 
	{ if ( m_type != Type_String ) return to<Int32>(defaultVal) != 0; else return (bool)(m_data.charArray ? _tcsicmp(m_data.charArray,_T("yes")) == 0 || _tcsicmp(m_data.charArray,_T("true")) == 0 : defaultVal ); }

	Float32 Variant::toFloat32( Float32 defaultVal ) const 
	{ if ( m_type != Type_String ) return to(defaultVal); else return (Float32)(m_data.charArray ? _tstof(m_data.charArray) : defaultVal ); }

	Float64 Variant::toFloat64( Float64 defaultVal ) const 
	{ if ( m_type != Type_String ) return to(defaultVal); else return (Float64)(m_data.charArray ? _tstof(m_data.charArray) : defaultVal ); }

	Int32 Variant::toInt32( Int32 defaultVal ) const 
	{ if ( m_type != Type_String ) return to(defaultVal); else return (Int32)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

	UInt32 Variant::toUInt32( UInt32 defaultVal ) const 
	{ if ( m_type != Type_String ) return to(defaultVal); else return (UInt32)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

	Int16 Variant::toInt16( Int16 defaultVal ) const 
	{ if ( m_type != Type_String ) return to(defaultVal); else return (Int16)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

	UInt16 Variant::toUInt16( UInt16 defaultVal ) const
	{ if ( m_type != Type_String ) return to(defaultVal); else return (UInt16)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

		
} //END: Kismo