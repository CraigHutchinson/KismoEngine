/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoResourceManager.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_RESOURCEMANAGER_H
#define	_KISMO_RESOURCEMANAGER_H

#include "framework/KismoPlatform.h"
#include "framework/KismoCommon.h"
#include "fileIO\KismoFileIO.h"

namespace kismo
{
	//forward declarations
	class Resource;

	//-----------------------------------------------------
	// ResourceManager type
	//-----------------------------------------------------
	class _KismoExport ResourceManager
	{
	public:
		//-----------------------------------------------------
		// Unload resources - does not remove but saves memory
		//-----------------------------------------------------
		virtual void unload( const k_String& name ); //Unload a resource by name
		virtual void unload( const k_Handle handle ); //Unload a resource by handle
		virtual void unloadUnreferenced( bool reloadableOnly = true );//Unload resources which haven't been 'touched' by a reference
		virtual void unloadAll( bool reloadableOnly = true ); //Unload all resources

		//-----------------------------------------------------
		// Reload resources - reloads a resource making sure it upto date with any file changes
		//-----------------------------------------------------
		virtual void reload( const k_String& name ); //Reload (Unload+reload) a resource by name
		virtual void reload( const k_Handle handle ); //Reload (Unload+reload) a resource by handle
		virtual void reloadAll( bool reloadableOnly = true ); //Reload (Unload+reload) all resources

		//-----------------------------------------------------
		// Remove resources - removes a resource permanately
		//-----------------------------------------------------
		virtual void remove( const k_String& name ); //remove a resource by name
		virtual void remove( const k_Handle handle ); //remove a resource by handle
		virtual void removeUnreferenced();//remove resources which haven't been 'touched' by a reference
		virtual void removeAll(); //remove all resources

		//-----------------------------------------------------
		// Gets
		//-----------------------------------------------------
		virtual k_Handle getHandle( const k_String name );
		virtual Resource* getResource( const k_Handle handle );
		virtual Resource* getResource( const k_String name ) { return getResource( getHandle( name ) ); }

		//Checks whether a resource exists
		bool exists( const k_String name ) { return isValidHandle( getHandle( name ) ); }
		bool exists( const k_Handle handle ) { return isValidPointer( getResource( handle ) ); }

	protected:
		//-----------------------------------------------------
		// Resource storage
		//-----------------------------------------------------
		typedef k_Vector< Resource* > ResourceVector;
		typedef k_Map< k_String, k_Handle > ResourceMap;

		ResourceVector m_resources; //Resource pointers are stored in a vector with quick indexed access by handle
		ResourceMap m_resourceHandleByName; //Map used to associate a Name to a resource handle

		//-----------------------------------------------------
		// Memory usage
		//-----------------------------------------------------
		k_Size m_memoryBudget; //The maximum amount of memory (in bytes) this resource manager can use
        k_Size m_memoryUsage; //The current memory usage for this resource manager

	};

} //END: kismo

#endif