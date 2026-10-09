/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Rtti.cpp
 * Status: unique (no other copy of this file name in the archive)
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



#include "Rtti.h"
#include "RttiMeta.h"

namespace kismo
{
	Rtti::Rtti( const Char* typeName, /*Count typeNameLength = 0,*/ Count namespaceLength, const Rtti* baseType, PropertyInterface* propertyInterface, RttiMeta* metaData ) : 
		m_typeName( typeName ), 
		m_baseType( baseType ), 
		m_propertyInterface( propertyInterface ),
		m_metaData(metaData )/*, m_namespaceLength(namespaceLength)*/
	{
		AssertEx( namespaceLength > 2, "RTTI Type should preferrably have a namespace defined to prevent name clashes later on" );
		//TODO: sort out calculating namespace length efficiently when necessary!
		//if ( m_namespaceLength == 0 ) 
		//{ 
		//	Count length = String::measure(typeName);
		//	Assert( length >= 2 ); //< Sanity check
		//	m_namespaceLength = length;
		//	while ( m_typeName[m_namespaceLength-1] != ':' && m_typeName[m_namespaceLength-2] != ':' && m_namespaceLength != 2 )
		//		--m_namespaceLength;
		//}
		
	}
} //END: kismo