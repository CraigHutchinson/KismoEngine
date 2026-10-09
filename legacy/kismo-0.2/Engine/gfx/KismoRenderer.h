/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoRenderer.h
 * Status: unique (no other copy of this file name in the archive)
 */
//Base renderer class//
#ifndef _KISMO_RENDERER_H
#define	_KISMO_RENDERER_H

#include "gfx\KismoGfx.h"
#include "gfx\KismoTextureManager.h"
#include "framework/KismoMessageQueue.h"

namespace kismo
{
	namespace gfx
	{
		class RenderWindow;
		class RenderTarget;

		typedef k_Map< k_String, RenderTarget* > RenderTargetMap;
		typedef k_Map< k_Priority, RenderTarget* > RenderTargetPriorityMap;

		class _KismoExport Renderer : public frx::MessageQueue
		{		

		public:
			Renderer( const k_String& name );
			virtual ~Renderer();

			//Gets a list of all available options and their current values for the renderer
			//TODO: Option list
			//virtual void getConfigOptions() = 0;

			//Sets a configurable option for this renderer
			//virtual void setConfigOption(const k_String& name, const k_String& value) = 0;

			virtual RenderWindow* initialise( k_Bool autoCreateWindow = kc_TRUE, const k_String& windowTitle = _T( "Kismo Render Window" ) ) = 0; //Initialises & creates the device
			virtual k_Bool reinitialise() = 0; //Reinitialise the renderer if config has changed
			virtual k_Void destroy() = 0; //Deletes the device instance freeing memory

			/** \brief Add a new render target for the renderers use */
			virtual bool addRenderTarget( RenderTarget& target );

			virtual RenderWindow* createRenderWindow(const k_String& name, const k_KeyValueList* miscParams = 0) = 0;

			//virtual k_Void setAmbientLight( k_Float32 r, k_Float32 g, k_Float32 b );
			//virtual k_Void setLightingEnabled( k_Bool enabled );
			

			virtual k_Bool beginFrame() = 0; ///< Begins the frame - clears the screen

			virtual k_Void endFrame() = 0; ///< Ends the frame & presents the image to the screen
		
			
			/** \brief Check if the renderer is initialised and is worth updating
			\return True if initialised*/
			k_Bool initialised(){ return m_initialised; }
			void setInitialised(){ m_initialised = true; }

	
		protected:

			RenderTargetMap m_renderTargets; ///< The render targets
			RenderTargetPriorityMap m_prioritisedRenderTargets; ///< The render targets ordered by priority
			RenderTarget* m_activeRenderTarget; ///< The active render target

		private:
			k_Bool m_initialised; ///< Whether the renderer is initialised
		};

	} // END: gfx

} // END: kismo

#endif