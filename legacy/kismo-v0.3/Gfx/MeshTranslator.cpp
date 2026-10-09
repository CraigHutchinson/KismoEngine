/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\MeshTranslator.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "MeshTranslator.h"

IMPLEMENT_RTTI( gfx, gfx::MeshTranslator, frx::ResourceTranslator );

namespace gfx
{

	MeshTranslator::MeshTranslator( Bool canRead, Bool canWrite, const String& defaultExtension ) :
		ResourceTranslator( canRead, canWrite, defaultExtension )
	{}

	MeshTranslator::~MeshTranslator()
	{
	}


} // END: gfx