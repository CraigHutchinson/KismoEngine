/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Rtti.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Rtti.h
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

#ifndef KISMOFRAMEWORK_RTTI_H
#define KISMOFRAMEWORK_RTTI_H

#include "KismoFramework/String.h"
#include "KismoFramework\Common.h"

/**	\brief		Defines the base functions for an rtti type tree
	\remarks	Macro should be used in the header of the root base class 
	\warning	This macro modifies the private/protected/public access of the scope. Access definition MUST be specified after the macro for this reason.
*/
#define DECLARE_ROOT_RTTI \
	DECLARE_RTTI \
		inline kismo::Bool isExactly ( const kismo::Rtti& type ) const { return getType().isExactly( type ); } \
		inline kismo::Bool isKindOf ( const kismo::Rtti& type ) const { return getType().isKindOf( type ); } \

/**	Defines function for getting an Rtti type
	\remarks Macro should be used in the heade of a class who has a base which uses Rtti
	\warning This macro modifies the private/protected/public access of the scope. Access definition MUST be specified after the macro for this reason. */
#define DECLARE_RTTI \
	static const kismo::Rtti Type; \
    virtual const kismo::Rtti& getType() const { return Type; } \

/** \brief Used to impliment the Rtti's base static value
\remarks Should be in the cpp file of the class */
#define IMPLEMENT_BASE_RTTI( nsname, classname ) \
	const kismo::Rtti classname::Type( _T(#nsname) ## _T("::") ## _T(#classname), (Count)strlen(#nsname)+2,  0 );

#define IMPLEMENT_BASE_RTTI_PROPS( nsname, classname ) \
	const kismo::Rtti classname::Type( _T(#nsname) ## _T("::") ## _T(#classname), (Count)strlen(#nsname)+2,  0, &ms_rttiProperties );

/** \brief Used to impliment the Rtti of a derived class
\remarks Should be in the cpp file of the class */
#define IMPLEMENT_RTTI( nsname, classname, baseclassname ) \
	const kismo::Rtti classname::Type( _T(#nsname ## "::" ## #classname), strlen(#nsname)+2, &baseclassname::Type );

#define IMPLEMENT_RTTI_PROPS( nsname, classname, baseclassname ) \
	const kismo::Rtti classname::Type( _T(#nsname ## "::" ## #classname), strlen(#nsname)+2, &baseclassname::Type, &ms_rttiProperties );
	
/** Define this at the top of a class to declare the use of RttiMeta properties	
	\code DECLARE_PROPERTIES( YourClassName, RttiMeta );
*/
#define DECLARE_PROPERTIES( _className ) \
	static kismo::PropertyInterface& propertyInterface() { return ms_rttiProperties; } \
	bool setProperty( kismo::Index prop, kismo::Variant& value ); \
	bool getProperty( kismo::Index prop, kismo::Variant& value ) const; \
private: \
	static kismo::Bool defineProperties( kismo::PropertyDefinitionList& list ); \
	static kismo::PropertyInterface ms_rttiProperties;
		

/** Define this in the class cpp file to implement the properties using the defineProperties() method of the class
	\code IMPLEMENT_PROPERTIES( YourClassName );
*/
#define IMPLEMENT_PROPERTIES(_className )  \
	kismo::PropertyInterface _className::ms_rttiProperties( _className::defineProperties, (PropertyInterface::Fn_setProperty)&_className::setProperty, (PropertyInterface::Fn_getProperty)&_className::getProperty);
	//_metaClassName _className::ms_rttiMeta( _className::defineProperties,&_className::setProperty, &_className::getProperty )

namespace kismo
{	
	//Forward declarations
	class RttiMeta;
	class PropertyInterface;

	/** \brief  Run-Time Type Information Class */
	class Rtti
	{
	public:
		/** Constructor
			\param[in] typeName The name of the class. e.g. core::Scene
			//\param[in] typeNameLength (optional) The length of typeName excluding null terminator. Defaults to 0, will cause strlen() to be called to calculate length.
			\param[in] namespaceLength (optional) The length of typeNames namespace upto and including the last ':'. Defaults to 0, will cause strlen() to be called to calculate length.
			\param[in] baseType Pointer to the classes base type 
			\remarks typeName must exist for the lifetime of the application
			\note  If no namespace exists '::' must still be prepended to the class name in typeName i.e ::Scene
			\note All strings are compared in case-sensitive
		*/
		Rtti( const Char* typeName, /*Count typeNameLength = 0,*/ Count namespaceLength = 0, const Rtti* baseType = 0, PropertyInterface* propertyInterface = 0, RttiMeta* metaData = 0 );

		/** \brief destructor */
		~Rtti(){}

		Bool isRootType() const
		{ return m_baseType == 0; }

		const Rtti* getBaseType() const
		{ return m_baseType; }

		const PropertyInterface* propertyInterface() const
		{ return m_propertyInterface; }

		const RttiMeta* metaData() const
		{ return m_metaData; }

		///** Gets the name of the type
		//*/
		//inline const Char* className() const
		//{ return m_typeName + m_namespaceLength ; }

		// Removed for memory/performance. Add if really needed!
		///** Gets the namespace of the type 
		//*/
		//inline const String& namespaceName() const
		//{ return m_typeName; }

		/** \brief Gets the full typename including the type seperated by a '.' */
		inline const Char* name() const
		{ return m_typeName; }

		/** Whether the this is, or is derived, from type
			\param[in] type The type to compare against
		*/
		inline Bool isKindOf( const Rtti& type ) const
		{
			const Rtti* search = this;
			while ( search && search != &type ) { search = search->m_baseType; }
			return search != 0; //< If we didn't get to the base then we are derived from this type
		}

		/** Whether the this is exactly equal to type
			\param[in] type The type to compare against
		*/
		inline Bool isExactly( const Rtti& type ) const
		{ return this == &type; }

		/** Whether the this is derived from type but IS NOT the type
			\remarks Does not test if this == type, use isKindOf instead
			\param[in] type The type to compare against
		*/
		inline Bool isDerived( const Rtti& type ) const
		{
			const Rtti* search = this;
			do { search = search->m_baseType; } while ( search && search != &type );
			return search != 0; //< If we didn't get to the base then we are derived from this type
		}

		Bool operator == ( const Rtti& rhs ) const
		{ return this == &rhs; }

		Bool operator == ( const Rtti* rhs ) const
		{ return this == rhs; }

	private:
		const Char* const m_typeName; ///< Store the namespace of the type
		const Rtti* const m_baseType;///< Stores a pointer ot the base type
		PropertyInterface* const m_propertyInterface; //< Type property information
		RttiMeta* const m_metaData; //, Type meta information for properties etc
	};
	
} //END: kismo

#endif