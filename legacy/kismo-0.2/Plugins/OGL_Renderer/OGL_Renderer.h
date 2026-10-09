/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\OGL_Renderer\OGL_Renderer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_OGL_RENDERER_H
#define _KISMO_OGL_RENDERER_H

#include "gfx\KismoRenderer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//include the opengl headers 
#include <GL/glew.h> 
#include <GL/wglew.h> 
#include <GL/gl.h> 
#include <GL/glu.h>

namespace kismo
{
	namespace gfx
	{
		//forward declaration
		class RenderWindow;

		class OGL_Renderer : public Renderer
		{
		public:
			OGL_Renderer();

			virtual RenderWindow* initialise( k_Bool autoCreateWindow = kc_TRUE, const k_String& windowTitle = _T("Kismo OpenGL") ); //Initialises & creates the device
			virtual k_Bool reinitialise(); //Reinitialise the renderer if config has changed
			virtual k_Void destroy(); //Deletes the device instance freeing memory

			virtual k_Bool beginFrame(); //Begins the frame - clears the screen
			virtual k_Void endFrame(); //Ends the frame & presents the image to the screen

			virtual RenderWindow* createRenderWindow(const k_String& name, const k_KeyValueList* miscParams = 0);


		protected:

			/** \brief Process a message */
			virtual void processMessage( Message::Ptr& msg ){};


		};
	
	} //END: gfx

} //END: kismo

#endif