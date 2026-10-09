/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoMeshManager.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_MESHMANAGER_H
#define	_KISMO_MESHMANAGER_H

#include "framework/KismoPlatform.h"
#include "framework/KismoSingleton.h"
#include "fileIO\KismoResourceManager.h"
#include "fileIO\KismoFile.h"
//#include "gfx\KismoMesh.h"

namespace kismo
{
	namespace gfx
	{
		//forward declaration
		class MeshTranslator;

		//-----------------------------------------------------
		// Manager for mesh resources
		//-----------------------------------------------------
		class _KismoExport MeshManager : public ResourceManager, public Singleton< MeshManager >
		{
		public:
			class _KismoExport Translators : public Singleton< Translators >
			{
			public:
				/** Add a new translator */
				bool addTranslator( MeshTranslator* trans );
				/** Find a translator for the specified file */
				MeshTranslator* findTranslator( const fio::File& file );
			};

		public:

			//virtual Mesh* load( const k_String& name, Mesh::ke_Type texType = Mesh::kc_MESH_2D, bool useAlpha = false ) = 0;

			static MeshManager& getSingleton( void );
			static MeshManager* getSingletonPtr( void );

		protected:

		private:
		};

		/** Singleton mesh translator instance */
		extern MeshManager::Translators meshTranslators;		

	} // END: gfx

} // END: kismo


#endif