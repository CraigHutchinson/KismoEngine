/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Object.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Object.h (same name, content differs)
 */
#ifndef FRX_OBJECT_H
#define	FRX_OBJECT_H

#include "Platform/Platform.h"
#include "Framework/Rtti.h"
#include "Framework/SmartPointer.h"
#include "Framework/Named.h"
#include "Framework/Unique.h"

namespace frx
{	
	/** \brief Obvject defines the base type for an object
	*/
	class _DllExport Object :  public frx::SmartReferenced, public Named, public Unique< Object >
	{
			DECLARE_RTTI_BASE;
		public:

			typedef frx::SmartPointer< Object > Ptr;

			/** \brief Finds an object of the supplied name
			*/
			static Object* findObject( const String& name );

		public:

			virtual ~Object();

		protected:

			Object( const String& name );

		private:
			typedef Map< String, Object* > ObjectNameMap;
			static ObjectNameMap ms_objects;
	};
} //END: frx

#endif