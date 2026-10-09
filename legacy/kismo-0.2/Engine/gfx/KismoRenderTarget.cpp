/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoRenderTarget.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoRenderTarget.h"

namespace kismo
{
	namespace gfx
	{

		bool RenderTarget::isEnabled() const
		{
			return m_enabled;
		}

		void RenderTarget::setEnabled( bool b )
		{
			m_enabled = b;
		}

		bool RenderTarget::isPrimary() const //RenderWindow should return true for the primary window
		{    
			return false;
		}
	}
}