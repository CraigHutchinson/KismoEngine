/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoResourceManager.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoResourceManager.h"

namespace kismo
{
	//-----------------------------------------------------
	// Unload resources - does not remove but saves memory
	//-----------------------------------------------------
	void ResourceManager::unload( const k_String& name ) //Unload a resource by name
	{
	}
	void ResourceManager::unload( const k_Handle handle ) //Unload a resource by handle
	{
	}
	void ResourceManager::unloadUnreferenced( bool reloadableOnly )//Unload resources which haven't been 'touched' by a reference
	{
	}
	void ResourceManager::unloadAll( bool reloadableOnly ) //Unload all resources
	{
	}

	//-----------------------------------------------------
	// Reload resources - reloads a resource making sure it upto date with any file changes
	//-----------------------------------------------------
	void ResourceManager::reload( const k_String& name ) //Reload (Unload+reload) a resource by name
	{
	}
	void ResourceManager::reload( const k_Handle handle ) //Reload (Unload+reload) a resource by handle
	{
	}
	void ResourceManager::reloadAll( bool reloadableOnly ) //Reload (Unload+reload) all resources
	{
	}

	//-----------------------------------------------------
	// Remove resources - removes a resource permanately
	//-----------------------------------------------------
	void ResourceManager::remove( const k_String& name ) //remove a resource by name
	{
	}
	void ResourceManager::remove( const k_Handle handle ) //remove a resource by handle
	{
	}
	void ResourceManager::removeUnreferenced()//remove resources which haven't been 'touched' by a reference
	{
	}
	void ResourceManager::removeAll() //remove all resources		
	{
	}

	//-----------------------------------------------------
	// Gets
	//-----------------------------------------------------
	k_Handle ResourceManager::getHandle( const k_String name )
	{
		return 0;
	}
	Resource* ResourceManager::getResource( const k_Handle handle )
	{ 
		return  0; 
	}

}