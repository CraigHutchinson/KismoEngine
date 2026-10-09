/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Properties.cpp
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


#include "Properties.h"

namespace kismo
{
	PropertyDefinitionList::PropertyDefinitionList() : 
		m_properties(0),
		m_propertyCount(0)
	{}

	PropertyDefinitionList::~PropertyDefinitionList()
	{
		//Delete all the properties
		// TODO: use an allocator for the properties so we can do safe cross-module add/delete
		for ( Index i =0; i < m_propertyCount; ++i )
		{
			if ( m_properties[i] != 0 ) 
				delete m_properties[i];			
		}
		delete[] m_properties;

		m_propertyNameLookup.clear();
	}

	bool PropertyDefinitionList::setPropertyCount( Count count )
	{
		Assert( m_properties == 0 );
		Assert( m_propertyCount == 0 );
		if ( m_properties != 0 )
		{
			AssertEx( false, "Number of properties must be defines once to the correct number and cannot be resized!" );
			return false;
		}
		if ( count == 0 ) //< nothing to do!
			return true; 

		m_properties = new PropertyDefinition*[count];
		AssertEx( m_properties, "Memory allocation failed! Fatal error of doom ha occured, run for the hills!" );
		if ( m_properties == 0 )
			return false;
		
		memset( m_properties, 0, sizeof(PropertyDefinition*) * count);
		m_propertyCount = count;
		return true;
	}

	PropertyDefinition* PropertyDefinitionList::setPropertyDefinition( Index index,PropertyDefinition* prop )
	{
		Assert( prop );
		AssertEx( index < m_propertyCount, "FATAL: Index is out of range! Ensure setPropertyCount() ahs been called on the PropertyDefinitionList" );
		AssertEx( getPropertyDefinition( index ) == 0, "FATAL: The property has already been defined at the specified index" );
		AssertEx( getPropertyIndex(prop->name()) == Index_Max, "FATAL: The property name is not unique" );

		assert( m_properties[ index ] == 0 );
		m_properties[ index ] = prop;
		m_propertyNameLookup[ prop ] = index;
		return prop;// m_properties[ prop->index() ];
	}

	Index PropertyDefinitionList::getPropertyIndex( const Char* name, Count nameLength ) const
	{
		PropertyDefinition temp( name, nameLength, 0, 0, 0 );
		PropertyDefinitionNameLookup::const_iterator iFind = m_propertyNameLookup.find( &temp );
		if ( iFind == m_propertyNameLookup.end() )
			return Index_Max;

		return iFind->second;
	}

	PropertyInterface::PropertyInterface(  Fn_defineProperties fnDefineProperties, Fn_setProperty fnSetProperty, Fn_getProperty fnGetProperty ) :
		m_fnSetProperty( fnSetProperty ),
		m_fnGetProperty( fnGetProperty )
	{
		if(fnDefineProperties)
			(*fnDefineProperties)( *this );
	}
		
	PropertyInterface::~PropertyInterface()
	{
	}

} //END: kismo