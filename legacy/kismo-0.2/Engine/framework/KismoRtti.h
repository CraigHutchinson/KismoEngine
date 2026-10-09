/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoRtti.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_RTTI_H
#define	_KISMO_RTTI_H

#include "framework/KismoCommon.h"

#define KISMO_DECLARE_RTTI \
public: \
	static const frx::Rtti TYPE; \
    virtual const frx::Rtti& getType () const { return TYPE; }

//#define KISMO_DECLARE_TEMPLATE_RTTI \
//public: \
//    KISMO_EXPORT static const frx::Rtti TYPE; \
//    virtual const frx::Rtti& getType () const { return TYPE; }

#define KISMO_IMPLEMENT_RTTI( nsname, classname, baseclassname ) \
	const frx::Rtti classname::TYPE( _T(#nsname)##_T(".")##_T(#classname), &baseclassname::TYPE)

//#define KISMO_IMPLEMENT_TEMPLATE_RTTI(nsname,classname,baseclassname) \
//    template <> \
//    const frx::Rtti classname::TYPE(#nsname"."#classname,&baseclassname::TYPE)

#define KISMO_DECLARE_RTTI_BASE \
		KISMO_DECLARE_RTTI \
		inline k_Bool isExactly ( const frx::Rtti& type ) const { return getType().isExactly( type ); } \
		inline k_Bool isDerived ( const frx::Rtti& type ) const { return getType().isDerived( type ); } \

namespace kismo
{
	namespace frx
	{	
		//-----------------------------------------------------
		// Real Time Type Information
		//-----------------------------------------------------
		class _KismoExport Rtti
		{
		public:

			Rtti( const k_String& typeName, const Rtti* baseType ) : 
				m_typeName( typeName ), m_baseType( baseType )
			{}

			~Rtti(){}

			const k_String& typeName() const;

			k_Bool isExactly( const Rtti& type ) const; //Is exactly a....
			k_Bool isDerived( const Rtti& type ) const; //Is derived from a...

		private:
			const k_String m_typeName;
			const Rtti* m_baseType;
		};
		
		inline const k_String& Rtti::typeName() const
		{
			return m_typeName;
		}

		inline k_Bool Rtti::isExactly( const Rtti& type ) const
		{
			return &type == this;
		}

		inline k_Bool Rtti::isDerived( const Rtti& type ) const
		{
			const Rtti* search = this;
			while ( search )
			{
				if ( search == &type )
					return true;
				search = search->m_baseType;
			}
			return false;
		}
	}
}

#endif