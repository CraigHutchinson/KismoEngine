/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Object.cpp
 * Status: current (newest copy of this file name in the archive)
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



#include "KismoFramework/Object.h"

IMPLEMENT_UNIQUE( kismo::Object );

namespace kismo
{	
	Object::ObjectNameMap Object::ms_objects;

	Object::Object( const Char* name ) :
		Named( name ),
		Unique()
	{
		//Affirm that a duplicate name is not created
		//TOOD: Multimap this?
		//Assert( ms_objects.find( name ) == ms_objects.end() );
		if ( name )
			ms_objects[ name ] = this;
	}

	Object::~Object()
	{
		if ( !getName().empty() )
		{
			ObjectNameMap::iterator iFind = ms_objects.find( getName() );
			if ( iFind != ms_objects.end() )
				ms_objects.erase( iFind );
		}
	}


	Object* Object::findObject( const String& name )
	{
		ObjectNameMap::iterator iFind = ms_objects.find( name );
		if ( iFind == ms_objects.end() )
			return 0;
		else
			return iFind->second;

	}

} //END: kismo