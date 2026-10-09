/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\RenderState.cpp
 * Status: duplicate of kismo-v0.3/Gfx/RenderState.cpp
 */
#include "RenderState.h"


namespace gfx
{	
	IMPLEMENT_RTTI( _T("gfx"), RenderState, Object );

	RenderState::RenderState( const String& name ) :
		Object( name )
	{
	}

	RenderState::~RenderState()
	{
	}
	
} //END: gfx