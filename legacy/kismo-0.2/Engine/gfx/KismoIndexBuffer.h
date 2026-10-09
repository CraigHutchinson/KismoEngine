/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoIndexBuffer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_HARDWAREINDEXBUFFER_H
#define	_KISMO_HARDWAREINDEXBUFFER_H

#include "gfx\KismoHardwareIndex.h"

namespace kismo
{
	namespace gfx
	{
		//-----------------------------------------------------
		// Hardware Index Buffer
		//-----------------------------------------------------
		class HardwareIndexBuffer : public HardwareBuffer
		{
		public:
			
		    enum ke_IndexType 
			{
			    kc_IndexType_16bit, //Short indexes
			    kc_IndexType_32bit,	//long indexes - for indexing large vertex buffers
		    };

		public:


		protected:

			
		    ke_IndexType m_indexType; ///< The type of index

		};

	} // END: gfx

} // END: kismo


#endif