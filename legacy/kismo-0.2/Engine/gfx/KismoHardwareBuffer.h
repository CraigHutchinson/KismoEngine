/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoHardwareBuffer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_HARDWAREBUFFER_H
#define	_KISMO_HARDWAREBUFFER_H

//#include "fileIO\KismoResource.h"

namespace kismo
{
	namespace gfx
	{
		//-----------------------------------------------------
		// hardware Buffer
		//-----------------------------------------------------
		class HardwareBuffer
		{
		public:
			enum ke_Usage /** Hardware buffer usage enumerator - Based from D3D10 */
			{
				/** Default: A resource that requires read and write access by the GPU. This is likely 
				to be the most common usage choice. */
				kc_Usage_Default, 					
				/** Immutable: A resource that can only be read by the GPU. It cannot be written by the GPU,
				and cannot be accessed at all by the CPU. This type of resource must be 
				initialized when it is created, since it cannot be changed after creation. */
				kc_Usage_Immutable,
				/** Dynamic: A resource that is accessible by both the GPU (read only) and the 
				CPU (write only). A dynamic resource is a good choice for a resource that will 
				be updated by the CPU at least once per frame. */
				kc_Usage_Dynamic, 
				/** Staging: A resource that supports data transfer (copy) from the GPU to the CPU. */
				//kc_Staging, 
			};


		public:


			//virtual Texture* load( const k_String& name, ke_Type texType = kc_TEXTURE_2D, bool useAlpha = false );

		protected:

		};

	} // END: gfx

} // END: kismo


#endif