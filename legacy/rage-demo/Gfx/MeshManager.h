/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\MeshManager.h
 * Status: duplicate of kismo-v0.3/Gfx/MeshManager.h
 */
#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Framework/Singleton.h"
#include "Framework/ResourceManager.h"
#include "Gfx/Mesh.h"
#include "Platform/Platform.h"

#define _MeshManager gfx::MeshManager::getSingleton()
#define _MeshManagerPtr gfx::MeshManager::getSingletonPtr()

namespace gfx
{
	/** \brief Manages mesh resources and MeshTranslator instances
	*/
	class _DllExport MeshManager : public frx::ResourceManager, public frx::Singleton< MeshManager >
	{
		friend class Mesh;
	public:
		SINGLETON( MeshManager );

		/** \brief Create a mesh resource
			\param[in] name The name of the resource
		*/
		Mesh::Ptr create( const String& name );

		/** \brief Create a mesh resource
			\param[in] name The name of the resource
			\param[in] loader the loader used to create the mesh
			\param[in] fullName The full path name for the resource if required by the loader
		*/
		Mesh::Ptr MeshManager::create( const String& name, gfx::MeshTranslator* loader, const String& fullName = String() );
	

	protected:

	};

} //END: gfx

#endif