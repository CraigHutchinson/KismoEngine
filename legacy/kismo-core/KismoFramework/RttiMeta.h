/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\RttiMeta.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/RttiMeta.h
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

#ifndef KISMOFRAMEWORK_RTTIMETA_H
#define KISMOFRAMEWORK_RTTIMETA_H

#include "KismoFramework/Properties.h"
#include "KismoFramework/Rtti.h"

namespace kismo
{
	/** Define this at the top of a class to declare the use of RttiMeta properties
		\code DECLARE_PROPERTIES( YourClassName, RttiMeta );
	*/
	//#define DECLARE_META( _className, _metaClassName) \
	//public: \
	//	static _metaClassName& metaData() { return ms_rttiMeta; } \
	//public: \
	//	bool setProperty( kismo::Index prop, kismo::Variant& value ); \
	//	bool getProperty( kismo::Index prop, kismo::Variant& value ) const; \
	//private: \
	//	static kismo::Bool defineProperties( kismo::PropertyDefinitionList& list ); \
	//private: \
	//	static _metaClassName ms_rttiMeta;
	//		

	///** Define this in the class cpp file to implement the properties using the defineProperties() method of the class
	//	\code IMPLEMENT_META( YourClassName );
	//*/
	//#define  DECLARE_META(_className,_metaClassName)  \
	//	_metaClassName _className::ms_rttiMeta( _className::Type, _className::defineProperties, (_metaClassName::Fn_setProperty)&_className::setProperty, (_metaClassName::Fn_getProperty)&_className::getProperty);
	//	//_metaClassName _className::ms_rttiMeta( _className::defineProperties,&_className::setProperty, &_className::getProperty )

	///** Base Rtti meta information class
	//	\remarks May be extended to store any arbistrary information along with a class type
	//*/
	//class RttiMeta : public PropertyInterface
	//{
	//	friend class Rtti;
	//	DECLARE_ROOT_RTTI;

	//public:
	//	/*template < class _Type >
	//	RttiMeta( Fn_defineProperties fnDefineProperties ) :
	//		m_fnSetProperty( (Fn_setProperty)_Type::setProperty ),
	//		m_fnGetProperty( (Fn_getProperty)_Type::getProperty )
	//	{ if(fnDefineProperties) (*fnDefineProperties)( m_properties ); }*/

	//	/** Type Meta constructor taking 3 function pointers
	//		\param[in] fnDefineProperties Static/C-style function pointer called to populate the types property list
	//		\param[in] fnSetProperty Class-member-function called to SET a property values for a class instance
	//		\param[in] fnGetProperty Class-member-function called to GET a property values for a class instance
	//		\remarks Cast member function pointers to RttiMeta::Fn_setProperty, RttiMeta::Fn_getProperty, etc.
	//	*/
	//	RttiMeta( const Rtti& owner, Fn_defineProperties fnDefineProperties, Fn_setProperty fnSetProperty = 0, Fn_getProperty fnGetProperty = 0 );
	//
	//	/** Virtual destructor to allow for specialised type Meta implementations
	//	*/
	//	virtual ~RttiMeta(void);

	//	inline const Rtti& owner() const
	//	{ return m_owner; }

	//private:
	//	const Rtti& m_owner;
	//};

} //END: kismo

#endif