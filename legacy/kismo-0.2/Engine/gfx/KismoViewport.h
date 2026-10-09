/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoViewport.h
 * Status: unique (no other copy of this file name in the archive)
 */
//Base render target class//
#ifndef _KISMO_VIEWPORT_H
#define	_KISMO_VIEWPORT_H

//a view port with one camera and 1 render target

#include "framework/KismoCommon.h"
#include "gfx\KismoCamera.h"

namespace kismo
{
	namespace gfx
	{
		//-----------------------------------------------------
		// Viewport type
		//-----------------------------------------------------
		class _KismoExport Viewport
		{
		public:

			//Usefull typedefs
			typedef k_Vector< Viewport* > VECTOR;

			//Friend functions for depth sorting viewports using the STL
			friend const bool sortGreater( Viewport* a, Viewport* b ) { return a->m_zOrder > b->m_zOrder; }
			friend const bool sortLess( Viewport* a, Viewport* b ) { return a->m_zOrder < b->m_zOrder; }

		public:



		protected:
			k_Int32 m_zOrder; //The z order of the viewport
		};

	} // END: gfx

} // END: kismo


#endif