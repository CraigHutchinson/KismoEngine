/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Core\Core.cpp
 * Status: current (newest copy of this file name in the archive)
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