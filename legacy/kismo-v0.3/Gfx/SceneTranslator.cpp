/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\SceneTranslator.cpp
 * Status: current (newest copy of this file name in the archive)
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