/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\MacroManager.cpp
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


#include "MacroManager.h"
#include "MacroNamespace.h"

namespace kismo
{	
	MacroManager::MacroManager() :
		m_defaultNamespace( createNamespace( _T("Default"), (MacroNamespace*)0 ) )
	{
	}

	MacroNamespace* MacroManager::createNamespace( const String& name, const String& parentNamespace  )
	{
		MacroNamespace* pParentNamespace = findNamespace( parentNamespace );
		if ( !pParentNamespace )
		{ 
			Assert(false);
			return 0; //< Indicate error by returning 0. TODO: expected bahaviour!?
		}
		return createNamespace( name, pParentNamespace );
	}

	MacroNamespace* MacroManager::createNamespace( const String& name, MacroNamespace* parentNamespace  ) 
	{ 
		Assert( findNamespace( name ) == 0 );

		//TODO: allocator
		//Insert the lookup leaving it blank so we only create the namespace when it doens't already exist
		NamespaceLookup::_Pairib iInsertion = m_namespaces.insert( NamespaceLookup::value_type( name, (MacroNamespace*)0 ) );
		if ( iInsertion.second )
			iInsertion.first->second = new MacroNamespace(parentNamespace);

		assert( iInsertion.first->second->parentNamespace() == parentNamespace);
		return iInsertion.first->second; //< Return the existing or new namespace
	}
} //END: kismo