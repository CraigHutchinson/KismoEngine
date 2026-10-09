/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\RenderModeState.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "RenderModeState.h"

namespace gfx
{	
	IMPLEMENT_RTTI( gfx, WireframeState, RenderState );

	WireframeState::WireframeState( const String& name ) :
		RenderState( name ),
		enabled( true ),
		colourOverride( false ),
		colour( frx::Colour4::Red )
	{}

} //END: gfx