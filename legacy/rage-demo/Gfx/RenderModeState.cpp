/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\RenderModeState.cpp
 * Status: duplicate of kismo-v0.3/Gfx/RenderModeState.cpp
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