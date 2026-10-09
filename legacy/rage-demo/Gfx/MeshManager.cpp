/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\MeshManager.cpp
 * Status: duplicate of kismo-v0.3/Gfx/MeshManager.cpp
 */
#include "MeshManager.h"
#include "IO/DirectoryManager.h"

IMPL_SINGLETON( gfx::MeshManager )

namespace gfx
{
	
	Mesh::Ptr MeshManager::create( const String& name )
	{ 
		//Check if a resource of the same name already exists
		//note: do this here to save the path lookup
		Mesh::Ptr exists( getResource( name ) );
		if ( exists.isValid() )
			return exists;

		String fullName = _DirectoryManager.getFullPath( name, Mesh::TYPE );
		return create( name, (gfx::MeshTranslator *)getTranslator( fullName ), fullName );
	}

	Mesh::Ptr MeshManager::create( const String& name, gfx::MeshTranslator* loader, const String& fullName )
	{ 
		//Check if a resource of the same name already exists
		Mesh::Ptr exists( getResource( name ) );
		if ( exists.isValid() )
			return exists;

		Mesh*newMesh = new Mesh( this, name, loader, fullName );
		
		bool success = false;
		if ( newMesh )
			success = newMesh->load();	
		
		if ( !success )
		{
			frx::safeDelete( newMesh ); //Safe delete
		}
		else
			addImpl( newMesh );

		return Mesh::Ptr( newMesh );
	}

} //END: gfx