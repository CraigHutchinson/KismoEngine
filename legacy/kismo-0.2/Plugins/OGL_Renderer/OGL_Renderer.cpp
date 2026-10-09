/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\OGL_Renderer\OGL_Renderer.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "OGL_Renderer.h"



#include "OGL_RenderWindow.h"

namespace kismo
{
	namespace gfx
	{
		OGL_Renderer::OGL_Renderer() : 
			Renderer( NAME_SWITCH( OpenGL ) )
		{}

		RenderWindow* OGL_Renderer::createRenderWindow(const k_String& name, const k_KeyValueList* miscParams )
		{
			if ( m_renderTargets.find( name ) != m_renderTargets.end() )
			{
				LOG_ERROR( _T("Render target Window with name '") + name + _T("' already exists") );
				return 0;
			}

			RenderWindow* window = new OGL_RenderWindow( name );	

			return window;
		}

		RenderWindow* OGL_Renderer::initialise( k_Bool autoCreateWindow, const k_String& windowTitle ) //Initialises & creates the device
		{

			return 0;
		}

		k_Bool OGL_Renderer::reinitialise() //Reinitialise the renderer if config has changed
		{

			return false;
		}

		k_Void OGL_Renderer::destroy() //Deletes the device instance freeing memory
		{

		}


		k_Bool OGL_Renderer::beginFrame() //Begins the frame - clears the screen
		{

			m_activeRenderTarget->update();
			return false;
		}

		k_Void OGL_Renderer::endFrame() //Ends the frame & presents the image to the screen
		{


		}



	} //END: gfx
} //END: kismo