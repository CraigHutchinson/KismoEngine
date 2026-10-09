/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoTextureManager.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_TEXTUREMANAGER_H
#define	_KISMO_TEXTUREMANAGER_H

#include "framework/KismoPlatform.h"
#include "framework/KismoSingleton.h"
#include "fileIO\KismoResourceManager.h"
#include "gfx\KismoTexture.h"

namespace kismo
{
	namespace gfx
	{
		//-----------------------------------------------------
		// Viewport type
		//-----------------------------------------------------
		class _KismoExport TextureManager : public ResourceManager, public Singleton< TextureManager >
		{
		public:

			virtual Texture* load( const k_String& name, Texture::ke_Type texType = Texture::kc_Texture_2D, bool useAlpha = false ) = 0;

			static TextureManager& getSingleton( void );
			static TextureManager* getSingletonPtr( void );

		protected:

		};

	} // END: gfx

} // END: kismo


#endif