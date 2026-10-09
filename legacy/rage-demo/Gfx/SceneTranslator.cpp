/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\SceneTranslator.cpp
 * Status: duplicate of kismo-v0.3/Gfx/SceneTranslator.cpp
 */
#include "SceneTranslator.h"

IMPLEMENT_RTTI( gfx, gfx::SceneTranslator, frx::ResourceTranslator );

namespace gfx
{

	SceneTranslator::SceneTranslator( Bool canRead, Bool canWrite, const String& defaultExtension ) :
		ResourceTranslator( canRead, canWrite, defaultExtension )
	{}

	SceneTranslator::~SceneTranslator()
	{
	}

} // END: gfx