/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoVariant.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_VARIANT_H
#define _KISMO_VARIANT_H

#include "KismoTypes.h"
#include "KismoVector.h"

namespace kismo
{
	namespace frx
	{		
		class _KismoExport Variant
		{
		public:		

			enum ke_DataType
			{
				kc_DataType_Invalid,

				//Built in type [0 to ...<1000]		
				kc_DataType_Bool,
				kc_DataType_Int8,
				kc_DataType_Int16,
				kc_DataType_Int32,
				kc_DataType_Int64,
				kc_DataType_Int8,
				kc_DataType_Int16,
				kc_DataType_Int32,
				kc_DataType_Int64,
				kc_DataType_Float32,
				kc_DataType_Float64,
				kc_DataType_VoidPointer,
				kc_DataType_Vector2i,
				kc_DataType_Vector2,
				kc_DataType_Vector3,
				kc_DataType_Vector4,
				kc_DataType_Vector,
				kc_DataType_Map,
				kc_DataType_Stack,
				kc_DataType_List,
				kc_DataType_String,
				kc_DataType_OStringStream,

				//User types [1000+]
				kc_DataType_UserType = 1000,
			}

		public:

			Variant( const Variant& variant ) : m_type( variant.m_type ), m_data( variant.m_data ) {}

			Variant( void* val ) : m_type( kc_VariantType_VoidPointer ) { m_data.voidPointer = val; }
			Variant( k_Int32 val ) : m_type( kc_VariantType_Integer32 ) { m_data.integer32 = val; }
			Variant( Vector2i val ) : m_type( kc_VariantType_Vector2i ) { memcpy( &m_data.vector2i, &val, sizeof(k_Int32)*2 ); }
			Variant( k_Float32 val ) : m_type( kc_VariantType_Float32 ) { m_data.float32 = val; }
			Variant( k_Bool val ) : m_type( kc_VariantType_Boolean ) { m_data.boolean = val; }

			Variant& operator= ( const Variant& variant )
			{
				m_type = variant.m_type;
				m_data = variant.m_data;
				return *this;
			}

			//Get the type
			ke_VariantType type() { return m_type; }

			k_Bool toBoolean() { return m_data.boolean; }
			k_Void* toVoidPointer() { return m_data.voidPointer; }
			k_Int32 toInteger32() { return m_data.integer32; }
			k_Float32 toFloat32() { return m_data.float32; }
			Vector2i toVector2i() { return m_data.vector2i; }

		protected:

			ke_VariantType m_type;

			void* m_data;

		};

	} //END: frx

} //END: kismo

#endif