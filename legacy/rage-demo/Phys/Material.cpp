/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Material.cpp
 * Status: duplicate of kismo-v0.3/Phys/Material.cpp
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