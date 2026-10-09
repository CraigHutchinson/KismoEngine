/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoPlatform\Window.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Window.h (same name, content differs)
 */
#ifndef KISMOPLATFORM_WINDOW_H
#define	KISMOPLATFORM_WINDOW_H

#include "KismoPlatform/Common.h"
#include "KismoPlatform/Keys.h"
#include "KismoFRX/Rect.h"
#include "KismoFRX/Timer.h"

#include <set>

namespace kismo { namespace platform
{	
	/**
		\brief Concept window
	*/	
	class KISMOPLATFORM_EXPORT Window
	{
	public:

		enum eContext
		{
			Context_OpenGL,
			//Context_Direct3D //TODO:
		};

		/**
			\brief Defines the interface for receiving realRT callbacks
			\remarks False should be returned. If true is returned the callback wil not be passed onto the default implementation
		*/
		struct KISMOPLATFORM_EXPORT Callback
		{
			virtual bool onDisplay( /*IWindow* window*/ ) { return true; }
			virtual bool onKeyboard( bool down, eKey key, eKeyModifier modifiers, int x, int y) { return false; }
			virtual bool onMouse( bool down, eButton buttonState, int x, int y) { return false; }
			virtual bool onMotion( UInt32 buttonState, int x, int y, int rx, int ry ) { return false; }
			virtual bool onResize(int width, int height) { return false; }
			virtual bool onIdle() { return false; }		
		};


	public:
		Window( eContext contextType );
		~Window();

		//bool setup( const Options& options )
		//{ return IWindow::setup(options); }

		/**
			\brief Adds a callback for the window to signal events etc
		*/
		//bool addCallback( Callback* callback  );

		/**
			\brief Removes a callback for the window to signal events etc
		*/
		//bool removeCallback( Callback* callback  );

		bool hide();
		bool update();
		void clear();
		void present();
		
		/**
			\brief OPens the window creating any device contexts
		*/
		bool open();

		/**
			\brief Closes the window including any device contexts
		*/
		void close();


		bool setRect( const Rect2s& pos );
		bool setPosition( const Vector2s& size );
		bool setSize( const Size2s& size );

		void setFullscreen();

		void doDisplay();
		
		bool isActive() const
		{ return m_isActive; }

		const Rect2s& rect() const
		{ return m_rect; }
		//UInt32 width() const
		//{ return m_rect.width(); }
		
		//UInt32 height() const
		//{ return m_rect.height(); }

		
		FrameTimer& timer()
		{ return m_timer; }

	protected:

		void onResizeCallbackEx(int width, int height);

		bool createWindow();
		bool createOpenGLRenderContext();
		//bool createDirect3DRenderContext();

		void processWin32Events();

		//IWindow callback functions sends event data to a callback listener
		void onDisplayCallback(void);
		void onKeyboardCallback( bool down, eKey key, eKeyModifier modifiers, int x, int y);
		void onMouseCallback(bool down, eButton buttonState, int x, int y);
		void onMotionCallback( UInt32 buttonState, int x, int y, int rx = 0, int ry = 0) ;
		void onResizeCallback(int width, int height);
		void onIdleCallback(void);

	private:		

		bool createWindowClass();
		bool destroyWindowClass();
		
		bool win32Proc( UInt32 msg, UInt32 wParam, UInt32 lParam);

	protected:
		eContext m_contextType;
		Rect2s m_rect; 
		bool m_isActive;

		struct Win32_SysWMinfo;
		Win32_SysWMinfo& m_win32Info;
		Vector2i m_mousePos; //< mouse posiiton of lasat update to obtain relative motion		
		FrameTimer m_timer;	
		
		typedef std::set<Callback*> CallbackSet;
		CallbackSet m_callbacks; ///< The callback receiver

	private:
		
		struct KeyMap { eKey map[255]; };
		static KeyMap CreateKeyMap();
	private:
		static const KeyMap ms_keys;
		static Count ms_instanceCount; //< IKnstance coutn for registering/unregistering the Win32 class
	};


} /* END: Platform*/ } /*END: Kismo*/

#endif
