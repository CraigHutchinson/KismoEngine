/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Core\Core.cpp
 * Status: duplicate of kismo-v0.3/Core/Core.cpp
 */
#include "Core.h"

IMPL_SINGLETON( core::Core );

namespace core
{
	Core::Core() :
		renderer( 0 ),
		physics( 0 )
	{
#ifdef WIN32
		platform::initialise();
#endif
	}

	


} //END: core