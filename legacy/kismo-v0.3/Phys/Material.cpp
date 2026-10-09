/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Material.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Material.h"

namespace phys
{

	Material::Material( Scene* owner ) :
		m_owner( owner )
	{}

	Material::~Material()
	{
	}

} //END: phys