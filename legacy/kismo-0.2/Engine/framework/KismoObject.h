/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoObject.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_OBJECT_H
#define	_KISMO_OBJECT_H

#include "framework/KismoRtti.h"
#include "framework/KismoSmartPtr.h"

namespace kismo
{
	namespace frx
	{	
		//-----------------------------------------------------
		// Unknown data type with Rtti
		//-----------------------------------------------------
		class _KismoExport Object : public IRefCountImpl< Object >
		{
				KISMO_DECLARE_RTTI_BASE;
			public:

				typedef SmartPtr< Object > Ptr;

			public:

				virtual ~Object();

			protected:

				Object();

			public:
		};
	}
}

#endif