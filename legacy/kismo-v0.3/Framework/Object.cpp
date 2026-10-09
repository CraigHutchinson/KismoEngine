/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Object.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Object.cpp (same name, content differs)
 */
#include "Framework/Object.h"

IMPLEMENT_UNIQUE( frx::Object );

namespace frx
{	
	IMPLEMENT_RTTI_BASE( _T("frx"), Object );

	Object::ObjectNameMap Object::ms_objects;

	Object::Object( const String& name ) :
		Named( name ),
		Unique()
	{
		//Affirm that a duplicate name is not created
		//TOOD: Multimap this?
		//assert( ms_objects.find( name ) == ms_objects.end() );
		if ( !name.isNull() )
			ms_objects[ name ] = this;
	}

	Object::~Object()
	{
		if ( !getName().isNull() )
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

} //END: frx