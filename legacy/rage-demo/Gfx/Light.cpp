/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Light.cpp
 * Status: duplicate of kismo-v0.3/Gfx/Light.cpp
 */
#include "Light.h"

namespace gfx
{
	
	IMPLEMENT_RTTI( _T("gfx"), Light, Spatial );

	Light::Light( const String& name, eLightType type ) :
		Spatial( name ),
		type( type )
	{
	}

	Light::~Light()
	{
	}

	void Light::updateWorldBound()
	{
	}
	
	void Light::doDraw( Renderer& renderer ) const
	{
	}

	void Light::updateState( RenderStateStack& states, LightStack& lights )
	{
	}

} //END: gfx