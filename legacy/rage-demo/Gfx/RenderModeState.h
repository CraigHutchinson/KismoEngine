/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\RenderModeState.h
 * Status: duplicate of kismo-v0.3/Gfx/RenderModeState.h
 */
#ifndef GFX_RENDERMODESTATE_H
#define	GFX_RENDERMODESTATE_H

#include "Gfx/RenderState.h"
#include "Framework/Colour.h"

namespace gfx
{	
	/** \brief Wireframe render state
	*/
	class _DllExport WireframeState : public RenderState
	{
		DECLARE_RTTI;
	public:
		WireframeState( const String& name );
		Bool enabled; ///< Whether the wire frame is enabled
		frx::Colour4 colour; ///< Wireframer colour ovveride
		Bool colourOverride; //< Wheter the materials shoudl be ovveridden to us  the specified colour
	};

} //END: gfx

#endif