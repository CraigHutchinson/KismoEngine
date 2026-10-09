/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoTexture.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_TEXTURE_H
#define	_KISMO_TEXTURE_H

#include "fileIO\KismoResource.h"

namespace kismo
{
	namespace gfx
	{
		//-----------------------------------------------------
		// Texture type
		//-----------------------------------------------------
		class Texture : public Resource
		{
		public:
			enum ke_Type
			{
				kc_Texture_1D,
				kc_Texture_2D,
				kc_Texture_3D,
			};

		public:

		protected:

		};

	} // END: gfx

} // END: kismo


#endif