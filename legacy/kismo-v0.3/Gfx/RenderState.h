/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\RenderState.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_RENDERSTATE_H
#define	GFX_RENDERSTATE_H

#include "Platform/Platform.h"
#include "Framework/Rtti.h"
#include "Framework/Object.h"
#include "Framework/SmartPointer.h"

namespace gfx
{	
	/** \brief The renderState defines the base interface for render states such as foggig, culling, materials
		etc that can be applied a spatial node
	ref. Game engine architecture p204

		\remarks RenderState is NOT USED IN CURRENT VERSION

	*/
	class _DllExport RenderState : public frx::Object
	{
		DECLARE_RTTI;
	public:

		typedef frx::SmartPointer< RenderState > Ptr;

	protected:

		RenderState( const String& name );
		virtual ~RenderState();

	public:


	};

} //END: gfx

#endif