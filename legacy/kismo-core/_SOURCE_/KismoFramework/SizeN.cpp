/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\SizeN.cpp
 * Status: current (newest copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/



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