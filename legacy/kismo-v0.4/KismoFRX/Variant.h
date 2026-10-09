/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Variant.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Variant.h (same name, content differs)
 */
/**
	This source code is provided on an “as-is” basis under licence agreement reference LMINSYS/SLA/ST/019, 
	no warranty is implied or conferred. Original source code copyright XPI Simulation Ltd
*/

#ifndef VARIANT_H
#define VARIANT_H

//#include "KismoFRX/Platform.h"
#include "KismoFRX/Types.h"

namespace kismo
{
	class /*Kismo_EXPORT*/ Variant
	{

	public:

		enum eType
		{
			Type_Null,

			Type_Boolean,
			Type_Float32,
			Type_Float64,
			Type_Int32,
			Type_UInt32,
			Type_Int16,
			Type_UInt16,
			Type_String,
		};

	private:

		union Data
		{
			bool boolean;
			Float32 float32;
			Float64 float64;
			Int32 int32;
			UInt32 uint32;
			Int16 int16;
			UInt16 uint16;
			Char* charArray;
		};

	public:

		Variant() : m_type(Type_Null) {}

		Variant( const Variant& rhs ) : m_type(rhs.m_type) 
		{ 
			if (m_type !=Type_String) 
				m_data = rhs.m_data;
			else 
				m_data.charArray = _tcsdup(rhs.m_data.charArray);
		}
		Variant( bool value )	: m_type(Type_Boolean) { m_data.boolean = value; }
		Variant( Float32 value )	: m_type(Type_Float32) { m_data.float32 = value; }
		Variant( Float64 value )	: m_type(Type_Float64) { m_data.float64 = value; }
		Variant( Int32 value )		: m_type(Type_Int32) { m_data.int32 = value; }
		Variant( UInt32 value )		: m_type(Type_UInt32) { m_data.uint32 = value; }
		Variant( Int16 value )		: m_type(Type_Int16) { m_data.int16 = value; }
		Variant( UInt16 value )		: m_type(Type_UInt16) { m_data.uint16 = value; }
		Variant( const String& value ) : m_type(Type_Null) { set(value); } 
		Variant( const Char* value )	: m_type(Type_Null) { set(value); }

		~Variant() 
		{
			clear(); ///< make sure any char string is erased
		}
		
		void clear()
		{
			if ( m_type == Type_String ) 
				delete[] m_data.charArray;
			memset( &m_data, 0, sizeof(Data) );			
			m_type = Type_Null;
		}

		bool isNull()
		{ return m_type == Type_Null; }

		eType type() const
		{ return m_type; }

		void set( bool value ) { clear(); m_type = Type_Boolean; m_data.boolean = value; }
		void set( Float32 value ) { clear(); m_type = Type_Float32; m_data.float32 = value; }
		void set( Float64 value ) { clear(); m_type = Type_Float64; m_data.float64 = value; }
		void set( Int32 value )	 { clear(); m_type = Type_Int32; m_data.int32 = value; }
		void set( UInt32 value ) { clear(); m_type = Type_UInt32; m_data.uint32 = value; }
		void set( Int16 value )	 { clear(); m_type = Type_Int16; m_data.int16 = value; }
		void set( UInt16 value ) { clear(); m_type = Type_UInt16; m_data.uint16 = value; }
		
		void set( const String& value )
		{ set(value.c_str()); }

		void set( const Char* value )
		{ 
			clear();
			m_type = Type_String;
			m_data.charArray = (value? _tcsdup(value) : 0 );
		}

		/**
			\brief Convert the current value into another data type using the inbuilt type conversions
		*/
		bool toBoolean( bool defaultVal = 0) const;
		Float32 toFloat32( Float32 defaultVal = 0) const;
		Float64 toFloat64( Float64 defaultVal = 0) const;
		Int32 toInt32( Int32 defaultVal = 0) const;
		UInt32 toUInt32( UInt32 defaultVal = 0) const ;
		Int16 toInt16( Int16 defaultVal = 0) const;
		UInt16 toUInt16( UInt16 defaultVal = 0) const;
		String toString( UInt16 defaultVal = 0) const;
				
		/**
			\brief Read the variant as a type
			\remarks No conversion is performed from the current stored type. This is a 'union' type conversion as such.
			\warning Data may be corrupt using this method and requesting a different type than that that is stored. us toTYPE() functions instead for this
		*/
		bool asBoolean( bool defaultVal = 0) const 
		{ if ( m_type != Type_String ) return m_data.boolean; else return (bool)(m_data.charArray ? _tcsicmp(m_data.charArray,_T("yes")) == 0 || _tcsicmp(m_data.charArray,_T("true")) == 0: defaultVal ); }

		Float32 asFloat32( Float32 defaultVal = 0) const 
		{ if ( m_type != Type_String ) return m_data.float32; else return (Float32)(m_data.charArray ? _tstof(m_data.charArray) : defaultVal ); }

		Float64 asFloat64( Float64 defaultVal = 0) const 
		{ if ( m_type != Type_String ) return m_data.float64; else return (Float64)(m_data.charArray ? _tstof(m_data.charArray) : defaultVal ); }

		Int32 asInt32( Int32 defaultVal = 0) const 
		{ if ( m_type != Type_String ) return m_data.int32; else return (Int32)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

		UInt32 asUInt32( UInt32 defaultVal = 0) const 
		{ if ( m_type != Type_String ) return m_data.uint32; else return (UInt32)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

		Int16 asInt16( Int16 defaultVal = 0) const 
		{ if ( m_type != Type_String ) return m_data.int16; else return (Int16)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

		UInt16 asUInt16( UInt16 defaultVal = 0) const
		{ if ( m_type != Type_String ) return m_data.uint16; else return (UInt16)(m_data.charArray ? _tstoi(m_data.charArray) : defaultVal ); }

		String asString( const String& defaultVal = String() ) const
		{ return m_data.charArray; }

		Variant& operator = ( const Variant& rhs )
		{
			clear();
			if ( rhs.m_type == Type_String )
				set( rhs.m_data.charArray );
			else
				memcpy_s( &m_data, sizeof(Data), &rhs.m_data, sizeof(Data) );
			m_type = rhs.m_type;
			return *this;
		}

	private:
		/**
			\brief Tmeplates to() to simplify toFloat32() etc calls
		*/
		template< class T > T to( T defaultValue = 0 ) const;

	private:
		eType m_type;
		Data m_data;
	};

} //END: Kismo

#endif