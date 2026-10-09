/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoResource.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_RESOURCE_H
#define	_KISMO_RESOURCE_H

#include "framework/KismoCommon.h"

namespace kismo
{
	//forward declarations
	class ResourceManager;

	//-----------------------------------------------------
	// Resource type
	//-----------------------------------------------------
	class Resource
	{
	public:

		virtual bool isLoaded() { return m_isLoaded; }
		virtual bool isTouched() { return m_isTouched; } //Is the resource touched/used by something

		
		const k_String& name() const { return m_name; }
		void setName( const k_String& name ) { m_name = name; }

	protected:
		ResourceManager* m_creator; //The resource manager that created the resource

		k_String m_origin; //Source of the resource - e.g. filename/path

		bool m_isLoaded; //Is the resource loaded?
		bool m_isTouched; //Is the resource touched/used by something
		k_Size m_size; //The size of the created resource in bytes

	private:

		k_String m_name;

	};

} //END: kismo

#endif