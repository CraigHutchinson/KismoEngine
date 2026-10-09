/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Object.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Object.h (same name, content differs)
 */
#ifndef KISMOFRX_OBJECT_H
#define KISMOFRX_OBJECT_H

//#include "Platform/Platform.h"
#include "KismoFRX/Rtti.h"
#include "KismoFRX/SmartPointer.h"
#include "KismoFRX/Named.h"
#include "KismoFRX/Unique.h"

namespace kismo
{	
	/** \brief Obvject defines the base type for an object
	*/
	class KISMOFRX_EXPORT Object :  public kismo::SmartReferenced, public Named, public Unique< Object >
	{
			DECLARE_RTTI_BASE;
		public:

			typedef kismo::SmartPointer< Object > Ptr;

			/** \brief Finds an object of the supplied name
			*/
			static Object* findObject( const String& name );

		public:

			virtual ~Object();

		protected:

			Object( const String& name );

		private:
			typedef std::map< String, Object* > ObjectNameMap;
			static ObjectNameMap ms_objects;
	};
} //END: kismo

#endif