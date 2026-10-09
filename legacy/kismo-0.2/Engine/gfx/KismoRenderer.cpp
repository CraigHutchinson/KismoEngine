/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoRenderer.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gfx/KismoRenderer.h"
#include "gfx/KismoRenderTarget.h"
#include "framework/KismoLog.h"

namespace kismo
{
	namespace gfx
	{
		Renderer::Renderer( const k_String& name ) :
			m_initialised( false )
		{
			_Gfx.addRenderer( name, this );
		}

		Renderer::~Renderer()
		{

		}

		bool Renderer::addRenderTarget( RenderTarget& target )
		{
			if ( m_renderTargets.find( target.getName() ) != m_renderTargets.end() )
			{
				LOG_ERROR( _T("A render target of the name '") + target.getName() + _T("' already added!") );
				return false;
			}

			m_renderTargets.insert( RenderTargetMap::value_type( target.getName(), &target ) );
			m_prioritisedRenderTargets.insert( RenderTargetPriorityMap::value_type( target.getPriority(), &target ) );

			m_activeRenderTarget = &target;

			return true;
		}
		
	} // END: gfx

} // END: kismo
