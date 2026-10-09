/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\MaterialState.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "MaterialState.h"

namespace gfx
{
	IMPLEMENT_RTTI( gfx, MaterialState, RenderState );
		
	MaterialState::Ptr MaterialState::create( const String& name )
	{ return Ptr( new MaterialState( name ) ); }

	MaterialState::MaterialState( const String& name ) :
		RenderState( name ),
		ambient( (Real)0.15, (Real)0.15, (Real)0.15, 1 ),
		diffuse( 1, 0, 1, 1 ),
		specular( 0, 0, 0, 1 ),
		specularPower( 0 )
	{
	}

	MaterialState::~MaterialState()
	{
	}

} //END: gfx