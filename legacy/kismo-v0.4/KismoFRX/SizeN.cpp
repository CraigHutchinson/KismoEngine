/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\SizeN.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/SizeN.cpp (same name, content differs)
 */
#include "SizeN.h"

namespace kismo
{
	template class SizeT< Int16, 2 >;
	template class SizeT< Int32, 2 >;
	template class SizeT< UInt16, 2 >;
	template class SizeT< UInt32, 2 >;
	template class SizeT< Real, 2 >;
	template class SizeT< Float32, 2 >;
	template class SizeT< Float64, 2 >;
	
	template class SizeT< Int16, 3 >;
	template class SizeT< Int32, 3 >;
	template class SizeT< UInt16, 3 >;
	template class SizeT< UInt32, 3 >;
	template class SizeT< Real, 3 >;
	template class SizeT< Float32, 3 >;
	template class SizeT< Float64, 3 >;

} //END: kismo