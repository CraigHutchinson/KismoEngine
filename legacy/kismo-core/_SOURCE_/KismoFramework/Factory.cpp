/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Factory.cpp
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


#include "KismoFramework/Factory.h"


namespace kismo
{	
	const Rtti* Factory::getType( const Char* name, Count length ) const
	{
		Assert( name != 0 );
		Rtti tempType( name, Count_Max );
		TypeLookup::const_iterator iFind = m_types.find( &tempType );
		if ( iFind == m_types.end() ) return 0;
		return *iFind;
	}

	Factory::Factory()
	{
	}

	Factory::~Factory()
	{
	}

	bool Factory::hasType( const kismo::Rtti* type ) const
	{
		Assert( type != 0 );
		TypeLookup::const_iterator iFind = m_types.find( type );
		return ( iFind != m_types.end() );
	}

	bool Factory::addProduct( IFactoryProduct* instance )
	{
		AssertEx( instance->factory() == 0, "A IFactoryProduct can currently only be registered with one factory! TODO: check if this is still necessary!?" );
		if ( hasType( instance->type() ) )
		{
			AssertEx( false, "IFactoryProduct which constructs the supplied type already exists! TODO: support replace/override existing constructor etc?" );
			return false;
		}
		m_constructors.insert( instance ); ///< Store the constructor to lookup the constructor by type
		m_types.insert( instance->type() ); ///< Store the type for name lookup to obtain type
		return true;
	}

	bool Factory::removeProduct( IFactoryProduct* instance )
	{
		Assert( instance != 0 );

		ProductMap::const_iterator iFind = m_constructors.find( instance );
		if ( iFind == m_constructors.end() ) return false;
		m_constructors.erase( iFind );

		TypeLookup::const_iterator itypeFind = m_types.find( instance->type() );
		Assert( itypeFind != m_types.end() ); //< Sanity check as if this was found in the m_constructors it must be here too!
		m_types.erase( itypeFind );
		return true;
	}

	IFactoryProduct* Factory::getProduct( const Rtti* type ) const
	{
		if( type == 0 )
			return 0;

		IFactoryProduct tempProduct(type);
		ProductMap::const_iterator iFind = m_constructors.find( &tempProduct );
		if ( iFind == m_constructors.end() ) return 0;
		return *iFind;
	}
} //END: kismo