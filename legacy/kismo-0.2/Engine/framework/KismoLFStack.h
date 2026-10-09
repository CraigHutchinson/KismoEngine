/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoLFStack.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_LOCK_FREE_STACK_H
#define _KISMO_LOCK_FREE_STACK_H

#include "KismoTypes.h"

namespace kismo
{
	namespace framework
	{		
		//Lock free stack
		template < class T >
		class KISMOEXPORT LFStack
		{
		public:
			LFStack()
			{
			cmpxchg()
			{
			}

		};

	} //END: framework

} //END: kismo

#endif
