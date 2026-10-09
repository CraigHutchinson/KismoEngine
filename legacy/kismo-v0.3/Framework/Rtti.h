/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Rtti.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Rtti.h (same name, content differs)
 */
#ifndef FRX_RTTI_H
#define	FRX_RTTI_H

#include "Platform/Platform.h"
#include "Framework/Common.h"

/** \brief Defines the base functions for an rtti type tree
\remarks Macro should be used in the header of the root base class */
#define DECLARE_RTTI_BASE \
		DECLARE_RTTI \
		inline Bool isExactly ( const frx::Rtti& type ) const { return getType().isExactly( type ); } \
		inline Bool isDerived ( const frx::Rtti& type ) const { return getType().isDerived( type ); } \

/** \brief Defines function for getting an Rtti type
\remarks Macro should be used in the heade rof a class who has a base which uses Rtti */
#define DECLARE_RTTI \
public: \
	static const frx::Rtti TYPE; \
    virtual const frx::Rtti& getType () const { return TYPE; }

/** \brief Used to impliment the Rtti's base static value
\remarks Should be in the cpp file of the class */
#define IMPLEMENT_RTTI_BASE( nsname, classname ) \
	const frx::Rtti classname::TYPE( _T(#nsname), _T(#classname), 0 )

/** \brief Used to impliment the Rtti of a derived class
\remarks Should be in the cpp file of the class */
#define IMPLEMENT_RTTI( nsname, classname, baseclassname ) \
	const frx::Rtti classname::TYPE( _T(#nsname), _T(#classname), &baseclassname::TYPE )
	

namespace frx
{	
	/** \brief  Run-Time Type Information Class */
	class _DllExport Rtti
	{
	public:
		/** \brief Constructor
		\param[in] typeNamespace The namespace of the class e.g. Framework
		\param[in] typeName The name of the class e.g. String
		\param[in] baseType Pointer to the classes base type */
		Rtti( const String& typeNamespace, const String& typeName, const Rtti* baseType = 0 ) : 
			m_typeNamespace( typeNamespace ), m_typeName( typeName ), m_baseType( baseType )
		{}
		/** \brief destructor */
		~Rtti(){}
		/** \brief Gets the name of the type */
		const String& typeName() const;
		/** \brief Gets the namespace of the type */
		const String& typeNamespace() const;
		/** \brief Gets the fulle typename including the type seperated by a '.' */
		String typeFullName() const;

		Bool isExactly( const Rtti& type ) const; //Is exactly a....
		Bool isDerived( const Rtti& type ) const; //Is derived from a...

		Bool operator == ( const Rtti& rhs ) const
		{ return this == &rhs; }

		Bool operator == ( const Rtti* rhs ) const
		{ return this == rhs; }

	private:
		const String m_typeNamespace; ///< Store the namespace of the type
		const String m_typeName;///< stores the name of the type e.g. String
		const Rtti* m_baseType;///< Stores a pointer ot the base type
	};
	
	inline const String& Rtti::typeNamespace() const
	{
		return m_typeNamespace;
	}

	inline const String& Rtti::typeName() const
	{
		return m_typeName;
	}

	inline String Rtti::typeFullName() const
	{
		return typeNamespace() + _T( "." ) + typeName();
	}

	inline Bool Rtti::isExactly( const Rtti& type ) const
	{
		return &type == this;
	}

	inline Bool Rtti::isDerived( const Rtti& type ) const
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

} //END: frx

#endif