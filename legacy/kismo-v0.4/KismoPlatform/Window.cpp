/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoPlatform\Window.cpp
 * Status: superseded by kismo-v0.4/KismoGFX/Window.cpp (same name, content differs)
 */
#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#include "KismoPlatform/Window.h"
//#include "KismoFRX/GLPixelBufferObject.h"

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLFont.h"

namespace kismo { namespace platform
{
	Count l_fontReferences = 0;
	
	struct Window::Win32_SysWMinfo
	{
		HWND hwnd;
		HDC device;
		HGLRC renderContext;
		Window* window;
		
		static LRESULT CALLBACK globalWin32Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

	Window::KeyMap Window::CreateKeyMap()
	{
		KeyMap keys;
		for ( Index i =0; i < 255; ++i )
			keys.map[i] = (eKey)i;
		
		/*From SDL: The keyboard syms have been cleverly chosen to map to ASCII */
		//keys.map[] = Key_UNKNOWN;
		//keys.map[] = Key_FIRST;
		keys.map[VK_BACK] = Key_BACKSPACE;
		keys.map[VK_TAB] = Key_TAB;
		keys.map[VK_CLEAR] = Key_CLEAR;
		keys.map[VK_RETURN] = Key_RETURN;
		keys.map[VK_PAUSE] = Key_Pause;
		keys.map[VK_ESCAPE] = Key_Escape;
		keys.map[VK_SPACE] = Key_SPACE;
		keys.map['!'] = Key_EXCLAIM;
		keys.map['"'] = Key_QUOTEDBL;
		keys.map['#'] = Key_HASH;
		keys.map['$'] = Key_DOLLAR;
		keys.map['&'] = Key_AMPERSAND;
		keys.map['\''] = Key_QUOTE;
		keys.map['('] = Key_LEFTPAREN;
		keys.map[')'] = Key_RIGHTPAREN;
		keys.map['*'] = Key_ASTERISK;
		keys.map['+'] = Key_PLUS;
		keys.map[','] = Key_COMMA;
		keys.map['-'] = Key_MINUS;
		keys.map['.'] = Key_PERIOD;
		keys.map['/'] = Key_SLASH;
		keys.map['0'] = Key_0;
		keys.map['1'] = Key_1;
		keys.map['2'] = Key_2;
		keys.map['3'] = Key_3;
		keys.map['4'] = Key_4;
		keys.map['5'] = Key_5;
		keys.map['6'] = Key_6;
		keys.map['7'] = Key_7;
		keys.map['8'] = Key_8;
		keys.map['9'] = Key_9;
		keys.map[':'] = Key_COLON;
		keys.map[';'] = Key_SEMICOLON;
		keys.map['<'] = Key_LESS;
		keys.map['='] = Key_EQUALS;
		keys.map['>'] = Key_GREATER;
		keys.map['?'] = Key_QUESTION;
		keys.map['@'] = Key_AT;
	
		/* Skip uppercase letters */

		keys.map['['] = Key_LEFTBRACKET;
		keys.map['\\'] = Key_BACKSLASH;
		keys.map[']'] = Key_RIGHTBRACKET;
		keys.map['^'] = Key_CARET;
		keys.map['_'] = Key_UNDERSCORE;
		keys.map['`'] = Key_BACKQUOTE;
		keys.map['A'] = Key_a;
		keys.map['B'] = Key_b;
		keys.map['C'] = Key_c;
		keys.map['D'] = Key_d;
		keys.map['E'] = Key_e;
		keys.map['F'] = Key_f;
		keys.map['G'] = Key_g;
		keys.map['H'] = Key_h;
		keys.map['I'] = Key_i;
		keys.map['J'] = Key_j;
		keys.map['K'] = Key_k;
		keys.map['L'] = Key_l;
		keys.map['M'] = Key_m;
		keys.map['N'] = Key_n;
		keys.map['O'] = Key_o;
		keys.map['P'] = Key_p;
		keys.map['Q'] = Key_q;
		keys.map['R'] = Key_r;
		keys.map['S'] = Key_s;
		keys.map['T'] = Key_t;
		keys.map['U'] = Key_u;
		keys.map['V'] = Key_v;
		keys.map['W'] = Key_w;
		keys.map['X'] = Key_x;
		keys.map['Y'] = Key_y;
		keys.map['Z'] = Key_z;
		keys.map[VK_DELETE] = Key_DELETE;
		/* End of ASCII mapped keysyms */
		
		/* International keyboard syms */
		//keys.map[] = Key_World_0; /* 0xA0 */
		//keys.map[] = Key_World_1;
		//keys.map[] = Key_World_2;
		//keys.map[] = Key_World_3;
		//keys.map[] = Key_World_4;
		//keys.map[] = Key_World_5;
		//keys.map[] = Key_World_6;
		//keys.map[] = Key_World_7;
		//keys.map[] = Key_World_8;
		//keys.map[] = Key_World_9;
		//keys.map[] = Key_World_10;
		//keys.map[] = Key_World_11;
		//keys.map[] = Key_World_12;
		//keys.map[] = Key_World_13;
		//keys.map[] = Key_World_14;
		//keys.map[] = Key_World_15;
		//keys.map[] = Key_World_16;
		//keys.map[] = Key_World_17;
		//keys.map[] = Key_World_18;
		//keys.map[] = Key_World_19;
		//keys.map[] = Key_World_20;
		//keys.map[] = Key_World_21;
		//keys.map[] = Key_World_22;
		//keys.map[] = Key_World_23;
		//keys.map[] = Key_World_24;
		//keys.map[] = Key_World_25;
		//keys.map[] = Key_World_26;
		//keys.map[] = Key_World_27;
		//keys.map[] = Key_World_28;
		//keys.map[] = Key_World_29;
		//keys.map[] = Key_World_30;
		//keys.map[] = Key_World_31;
		//keys.map[] = Key_World_32;
		//keys.map[] = Key_World_33;
		//keys.map[] = Key_World_34;
		//keys.map[] = Key_World_35;
		//keys.map[] = Key_World_36;
		//keys.map[] = Key_World_37;
		//keys.map[] = Key_World_38;
		//keys.map[] = Key_World_39;
		//keys.map[] = Key_World_40;
		//keys.map[] = Key_World_41;
		//keys.map[] = Key_World_42;
		//keys.map[] = Key_World_43;
		//keys.map[] = Key_World_44;
		//keys.map[] = Key_World_45;
		//keys.map[] = Key_World_46;
		//keys.map[] = Key_World_47;
		//keys.map[] = Key_World_48;
		//keys.map[] = Key_World_49;
		//keys.map[] = Key_World_50;
		//keys.map[] = Key_World_51;
		//keys.map[] = Key_World_52;
		//keys.map[] = Key_World_53;
		//keys.map[] = Key_World_54;
		//keys.map[] = Key_World_55;
		//keys.map[] = Key_World_56;
		//keys.map[] = Key_World_57;
		//keys.map[] = Key_World_58;
		//keys.map[] = Key_World_59;
		//keys.map[] = Key_World_60;
		//keys.map[] = Key_World_61;
		//keys.map[] = Key_World_62;
		//keys.map[] = Key_World_63;
		//keys.map[] = Key_World_64;
		//keys.map[] = Key_World_65;
		//keys.map[] = Key_World_66;
		//keys.map[] = Key_World_67;
		//keys.map[] = Key_World_68;
		//keys.map[] = Key_World_69;
		//keys.map[] = Key_World_70;
		//keys.map[] = Key_World_71;
		//keys.map[] = Key_World_72;
		//keys.map[] = Key_World_73;
		//keys.map[] = Key_World_74;
		//keys.map[] = Key_World_75;
		//keys.map[] = Key_World_76;
		//keys.map[] = Key_World_77;
		//keys.map[] = Key_World_78;
		//keys.map[] = Key_World_79;
		//keys.map[] = Key_World_80;
		//keys.map[] = Key_World_81;
		//keys.map[] = Key_World_82;
		//keys.map[] = Key_World_83;
		//keys.map[] = Key_World_84;
		//keys.map[] = Key_World_85;
		//keys.map[] = Key_World_86;
		//keys.map[] = Key_World_87;
		//keys.map[] = Key_World_88;
		//keys.map[] = Key_World_89;
		//keys.map[] = Key_World_90;
		//keys.map[] = Key_World_91;
		//keys.map[] = Key_World_92;
		//keys.map[] = Key_World_93;
		//keys.map[] = Key_World_94;
		//keys.map[] = Key_World_95; /* 0xFF */
		//
		/* Numeric keypad */
		keys.map[VK_NUMPAD0] = Key_Pad0;
		keys.map[VK_NUMPAD1] = Key_Pad1;
		keys.map[VK_NUMPAD2] = Key_Pad2;
		keys.map[VK_NUMPAD3] = Key_Pad3;
		keys.map[VK_NUMPAD4] = Key_Pad4;
		keys.map[VK_NUMPAD5] = Key_Pad5;
		keys.map[VK_NUMPAD6] = Key_Pad6;
		keys.map[VK_NUMPAD7] = Key_Pad7;
		keys.map[VK_NUMPAD8] = Key_Pad8;
		keys.map[VK_NUMPAD9] = Key_Pad9;
		keys.map[VK_DECIMAL] = Key_Pad_Period;
		keys.map[VK_DIVIDE] = Key_Pad_Divide;
		keys.map[VK_MULTIPLY] = Key_Pad_Multiply;
		keys.map[VK_SUBTRACT] = Key_Pad_Subtract;
		keys.map[VK_ADD] = Key_Pad_Add;
		keys.map[VK_EXECUTE] = Key_Pad_Enter;
		//keys.map[VK_EQUAL] = Key_Pad_Equals;
		
		/* Arrows + Home/End pad */
		keys.map[VK_UP] = Key_Up;
		keys.map[VK_DOWN] = Key_Down;
		keys.map[VK_RIGHT] = Key_Right;
		keys.map[VK_LEFT] = Key_Left;
		keys.map[VK_INSERT] = Key_Insert;
		keys.map[VK_HOME] = Key_Home;
		keys.map[VK_END] = Key_End;
		keys.map[VK_PRIOR] = Key_PageUp;
		keys.map[VK_NEXT] = Key_PageDown;

		/* Function keys */
		keys.map[VK_F1] = Key_F1;
		keys.map[VK_F2] = Key_F2;
		keys.map[VK_F3] = Key_F3;
		keys.map[VK_F4] = Key_F4;
		keys.map[VK_F5] = Key_F5;
		keys.map[VK_F6] = Key_F6;
		keys.map[VK_F7] = Key_F7;
		keys.map[VK_F8] = Key_F8;
		keys.map[VK_F9] = Key_F9;
		keys.map[VK_F10] = Key_F10;
		keys.map[VK_F11] = Key_F11;
		keys.map[VK_F12] = Key_F12;
		keys.map[VK_F13] = Key_F13;
		keys.map[VK_F14] = Key_F14;
		keys.map[VK_F15] = Key_F15;

		/* Key state modifier keys */
		keys.map[VK_NUMLOCK] = Key_NumLock;
		keys.map[VK_CAPITAL] = Key_CapsLock;
		keys.map[VK_SCROLL] = Key_ScrollLock;
		keys.map[VK_SHIFT] = Key_LeftShift;
			//keys.map[VK_RSHIFT] = Key_RightShift;
			//keys.map[VK_LSHIFT] = Key_LeftShift;
		keys.map[VK_CONTROL] = Key_LeftCtrl;
			//keys.map[VK_RCONTROL] = Key_RightCtrl;
			//keys.map[VK_LCONTROL] = Key_LeftCtrl;
		//keys.map[] = Key_RightAlt;
		//keys.map[VK_ALT] = Key_LeftAlt;
		//keys.map[] = Key_RMETA;
		//keys.map[] = Key_LMETA;
		keys.map[VK_LWIN] = Key_LeftSystem; /* Left "Windows" key */
		keys.map[VK_RWIN] = Key_RightSystem; /* Right "Windows" key */
		keys.map[VK_MODECHANGE] = Key_MODE; /* "Alt Gr" key */
		keys.map[VK_APPS] = Key_COMPOSE; /* Multi-key compose key */

		//VK_SELECT
		/* Miscellaneous function keys */
		keys.map[VK_HELP] = Key_Help;
		keys.map[VK_PRINT] = Key_Print;
		keys.map[VK_SNAPSHOT] = Key_SYSREQ;
		keys.map[VK_EXECUTE] = Key_Break;
		keys.map[VK_MENU] = Key_Menu;
		keys.map[VK_SLEEP] = Key_POWER; /* Power Macintosh power key */
		//keys.map[] = Key_EURO; /* Some european keyboards */
		//keys.map[] = Key_UNDO; /* Atari keyboard has Undo */

		return keys;
	}

	Count Window::ms_instanceCount = 0;
	const Window::KeyMap Window::ms_keys = Window::CreateKeyMap();

	Window::Window( eContext contextType ) : 
		m_contextType( contextType ),
		m_mousePos( Vector2i( Int32_Max, Int32_Max ) ),
		m_win32Info( *new Win32_SysWMinfo() ),
		m_isActive(false),
		m_rect( ms_instanceCount*10 ,ms_instanceCount*1, UInt16_Max, UInt16_Max )
	{
		m_win32Info.window = this;
		//setStatsName( "Win32: Window" );

		if ( ms_instanceCount == 0 )
			createWindowClass();
		++ms_instanceCount;
	}

	Window::~Window()
	{
		close();
		delete &m_win32Info;

		--ms_instanceCount;
		if ( ms_instanceCount == 0 )
			destroyWindowClass();
	}

	//bool Window::addCallback( Callback* callback  )
	//{ 
	//	if ( !callback ) return false;

	//	m_callbacks.insert(callback);

	//	//Inform new callback of size
	//	//callback->onResize( m_size.w, m_size.h );
	//	return true;
	//}

	//bool Window::removeCallback( Callback* callback  )
	//{ 
	//	if ( !callback ) return false;
	//	CallbackSet::iterator iFind = m_callbacks.find(callback);
	//	if ( iFind == m_callbacks.end() )
	//		return false;
	//	m_callbacks.erase(iFind);
	//	return true;
	//}
	//
	bool Window::destroyWindowClass()
	{	
		if ( !UnregisterClass( _T("KismoWindow"), GetModuleHandle(0) ))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,_T("Could Not Unregister Class."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			return false;
		}

		return true;
	}

	bool Window::createWindowClass()
	{	
		//Step 1: Registering the Window Class
		WNDCLASSEX wc;
		wc.cbSize		 = sizeof(WNDCLASSEX);
		wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc	 = Win32_SysWMinfo::globalWin32Proc;
		wc.cbClsExtra	 = 0;
		wc.cbWndExtra	 = 0;
		wc.hInstance	 = GetModuleHandle(0);
		wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = _T("KismoWindow");
		wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		return true;
	}

	bool Window::createWindow()
	{	
		// Step 2: Creating the Window
		m_win32Info.hwnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			_T("KismoWindow"),
			_T("Kismo Engine (Lead: Craig Hutchinson) {Win32}"),
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,				// Required Window Style								
			m_rect.left(), m_rect.width(), m_rect.width(), m_rect.height(),
			NULL, NULL, GetModuleHandle(0), &m_win32Info );

		if(m_win32Info.hwnd == NULL)
		{
			close();
			MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		if ( !createOpenGLRenderContext() )
			return false;

		glewInit();
		if ( !l_fontReferences )
		{
			BuildFont( m_win32Info.device );

		}
		++l_fontReferences;

		//Setup OpenGL

		//glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		//glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		//glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		
		return true;
	}

	bool Window::createOpenGLRenderContext()
	{	
		PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			32,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		
		if (!(m_win32Info.device=GetDC(m_win32Info.hwnd)))							// Did We Get A Device Context?
		{
			close();								// Reset The Display
			MessageBox(NULL,_T("Can't Create A GL Device Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		GLuint pixelFormat;			// Holds The Results After Searching For A Match
		if ( !(pixelFormat = ChoosePixelFormat(m_win32Info.device,&pfd)) )	// Did Windows Find A Matching Pixel Format?
		{
			close();								// Reset The Display
			MessageBox(NULL,_T("Can't Find A Suitable pixelFormat."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if(!SetPixelFormat(m_win32Info.device,pixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			close();								// Reset The Display
			MessageBox(NULL,_T("Can't Set The pixelFormat."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if (!(m_win32Info.renderContext=wglCreateContext(m_win32Info.device)))				// Are We Able To Get A Rendering Context?
		{
			close();								// Reset The Display
			MessageBox(NULL,_T("Can't Create A GL Rendering Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if(!wglMakeCurrent(m_win32Info.device,m_win32Info.renderContext))					// Try To Activate The Rendering Context
		{
			close();								// Reset The Display
			MessageBox(NULL,_T("Can't Activate The GL Rendering Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return false
		}
		return true;
	}

	void Window::close()
	{
		if ( l_fontReferences )
		{
			--l_fontReferences;
			if ( !l_fontReferences )
				KillFont();
		}

		/*if (fullscreen)										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(TRUE);								// Show Mouse Pointer
		}*/

		if (m_win32Info.renderContext)											// Do We Have A Rendering Context?
		{
			//if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			//{
			//	MessageBox(NULL,_T("Release Of DC And RC Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			//}

			if (!wglDeleteContext(m_win32Info.renderContext) )						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,_T("Release Rendering Context Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			}
		}
		m_win32Info.renderContext = NULL;										// Set RC To NULL

		if (m_win32Info.device && !ReleaseDC(m_win32Info.hwnd, m_win32Info.device ) )					// Are We Able To Release The DC
		{
			MessageBox(NULL,_T("Release Device Context Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		m_win32Info.device = NULL;										// Set DC To NULL

		if (m_win32Info.hwnd && !DestroyWindow(m_win32Info.hwnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,_T("Could Not Release m_win32Info.hwnd."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		m_win32Info.hwnd=NULL;										// Set m_win32Info.hwnd To NULL

		m_isActive = false;		
	}


	bool Window::open()
	{		
		if ( m_win32Info.hwnd == 0 && !createWindow() )
			return false;

		int ret = ShowWindow( m_win32Info.hwnd, SW_SHOWNORMAL );
		return true;
	}
	
	bool Window::hide()
	{
		if ( m_win32Info.hwnd == 0 )
			return false;

		ShowWindow( m_win32Info.hwnd, SW_HIDE );
		return true;
	}
	
	bool Window::update()
	{
		//We need to send a resize event so the PBO can eb created
		//onResizeCallback( m_size.w, m_size.h );

		//We don't call the display callback here and instead call the Idle callback - this allows the Core to decide whether it wishes to diaply a new image or not
		onIdleCallback();

		//Processes the sdl event queue
		processWin32Events();		

		return true;
	}

	void Window::clear()
	{
		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool Window::setRect( const Rect2s& rect )
	{
		m_rect = rect;
		if ( m_win32Info.hwnd != 0 )
		{
			//TODO: Same as setSize() make a updateRect() function
			RECT clientRect, windowRect;
			GetClientRect( m_win32Info.hwnd, &clientRect );
			GetWindowRect( m_win32Info.hwnd, &windowRect );
			int xPad = (windowRect.right-windowRect.left) - (clientRect.right-clientRect.left);
			int yPad = (windowRect.bottom-windowRect.top) - (clientRect.bottom-clientRect.top);
			MoveWindow( m_win32Info.hwnd, m_rect.left(), m_rect.width(), m_rect.width() + xPad, m_rect.height() + yPad, TRUE ); 
			SetWindowLong( m_win32Info.hwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW );
		}

		return true;
	}

	bool Window::setPosition( const Vector2s& pos )
	{ return setRect( Rect2s( pos.x, pos.y, rect().width(), rect().height() ) ); }

	bool Window::setSize( const Size2s& size )
	{ return setRect( Rect2s( rect().left(), rect().top(), size.w, size.h ) ); }
	
	void Window::setFullscreen()
	{
		//LONG style = GetWindowLong( m_win32Info.hwnd, GWL_STYLE);
		SetWindowLong( m_win32Info.hwnd, GWL_STYLE, WS_VISIBLE );

		ShowWindow(m_win32Info.hwnd, SW_MAXIMIZE);
	}

	void Window::present()
	{
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();	

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix		
		glLoadIdentity();
		//glClearColor( 0.5, 0.5, 0.5, 0.5 );
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
		//glLoadIdentity();									// Reset The Current Modelview Matrix
		//glTranslatef(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
		
		// Pulsing Colors Based On Text Position	
#if 0
		srand( int(clock()/CLOCKS_PER_SEC) );// % CLOCKS_PER_SEC)/CLOCKS_PER_SEC);
		glColor3f( rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX) );
#else
		glColor3f( 0.75f, 0, 0 );
		/*glEnable (GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_ONE_MINUS_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);*/
#endif
		
		// Position The Text On The Screen
		glRasterPos2f( -(float)(rect().width() - 12) / rect().width(),  -(float)(rect().height() - 12) / rect().height() );
		glPrint("fps = %f, WxH = %ux%u", m_timer.getFPS(), rect().width(), rect().height() );	// Print GL Text To The Screen
		glColor3f( 1, 1, 1 );

		//glDisable(GL_BLEND);

		// swap buffers to display, since we're double buffered.
		SwapBuffers( m_win32Info.device );				// Swap Buffers (Double Buffering)
		
		m_timer.endFrame();

		//report any errors encountered
		//sdlReportErrors();
	}

	
	void Window::doDisplay()
	{
		if ( m_isActive )
			onDisplayCallback();
	}

	
	void Window::processWin32Events()
	{
		MSG msg;
		while( m_win32Info.hwnd && PeekMessage(&msg, m_win32Info.hwnd, 0, 0, PM_REMOVE )  )
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				close();
				return;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	LRESULT CALLBACK Window::Win32_SysWMinfo::globalWin32Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = 0;

		if( msg == WM_NCCREATE )
		{
			Win32_SysWMinfo* windowInfo = 0;
			// retrieve Window instance from window creation data and associate
			windowInfo = reinterpret_cast<Win32_SysWMinfo *>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			
			// save window handle
			windowInfo->hwnd = hwnd;

			window = windowInfo->window;
//#ifdef _WIN64
			SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)window );
//#else
//			SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<long>(window));
//#endif

		}
		else
		{
			// retrieve associated Window instance
//#ifdef _WIN64
			window = reinterpret_cast<Window*>(::GetWindowLongPtr(hwnd, GWL_USERDATA));
//#else
//			window = reinterpret_cast<Window*>(::GetWindowLongPtr(hwnd, GWL_USERDATA));
//#endif
		}

		if(window)
		{
			if ( window->win32Proc( msg, wParam, lParam ) )
				return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool Window::win32Proc( UInt32 msg, UInt32 wParam, UInt32 lParam)
	{
		switch(msg)
		{
			case WM_SIZE:			/* User resized video mode */
			{		
				onResizeCallbackEx( LOWORD(lParam), HIWORD(lParam) );
				break;
			}

			case WM_SHOWWINDOW:
				m_isActive = (wParam==TRUE);
				break;

			case WM_CREATE:
				m_isActive = true;
				break;

			case WM_CLOSE:
				m_isActive = false;
				hide();
				return true;

			case WM_DESTROY:
				m_isActive = false;
				return true;

			case WM_KEYDOWN:
			{
				eKeyModifier modifiers = (eKeyModifier)0; 
				onKeyboardCallback( true,  ms_keys.map[wParam], modifiers, 0, 0 );
				return true;
			}

			case WM_KEYUP:
			{
				eKeyModifier modifiers = (eKeyModifier)0; 
				onKeyboardCallback( false, ms_keys.map[wParam], modifiers, 0, 0 );
				return true;
			}
			
			case WM_SYSCOMMAND:							// Intercept System Commands
			{
				switch (wParam)							// Check System Calls
				{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;							// Prevent From Happening
				}
				break;									// Exit
			}
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:

			case WM_MOUSEWHEEL:
				break;

			case WM_MOUSEMOVE:			/* Mouse moved */
				{
					UInt32 buttonState = 
						(wParam & MK_LBUTTON ? Button_Left : 0) |
						(wParam & MK_RBUTTON ? Button_Right : 0) |
						(wParam & MK_MBUTTON ? Button_Middle : 0) |
						(wParam & MK_XBUTTON1 ? Button_X1 : 0) |
						(wParam & MK_XBUTTON2 ? Button_X2 : 0);

					Vector2i newMousePos( LOWORD(lParam), HIWORD(lParam) );
					if ( m_mousePos == Vector2i(Int32_Max,Int32_Max) ) //< When we first update we don't want a large relative motion
						m_mousePos = newMousePos;

					onMotionCallback( buttonState, newMousePos.x, newMousePos.y, newMousePos.x-m_mousePos.x, newMousePos.y-m_mousePos.y );
					m_mousePos = newMousePos;
					break;
				}

			case WM_LBUTTONDOWN: /* Mouse button pressed */
				onMouseCallback( true, Button_Left, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_RBUTTONDOWN: /* Mouse button pressed */
				onMouseCallback( true, Button_Right, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_MBUTTONDOWN: /* Mouse button pressed */
				onMouseCallback( true, Button_Middle, LOWORD(lParam), HIWORD(lParam) ); break;

			case WM_LBUTTONUP: 	/* Mouse button released */
				onMouseCallback( false, Button_Left, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_RBUTTONUP:/* Mouse button released */
				onMouseCallback( false, Button_Right, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_MBUTTONUP:	/* Mouse button released */
				onMouseCallback( false, Button_Middle, LOWORD(lParam), HIWORD(lParam) ); break;

		}
		return false;

		//TODO: This could go in a separate function
		//Win32_Event event;
		//while ( Win32_PollEvent( &event ) ) 
		//{
		//	switch ( event.type )
		//	{
		//	case Win32_QUIT:			/* User-requested quit */
		//		{
		//			m_isActive = false;
		//			break;
		//		}

		//	case Win32_ACTIVEEVENT:			/* Application loses/gains visibility */
		//		{
		//			break;
		//		}
		//	case Win32_KEYDOWN:			/* Keys pressed */
		//	case Win32_KEYUP:			/* Keys released */
		//		{
		//			/*if ( scanCode  >= Win32K_a && scanCode <= Win32K_z )
		//				if ( event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT) ) //< if shift we capitalise
		//					scanCode = (scanCode - Win32K_a) + 'A';
		//				else
		//					scanCode = (scanCode - Win32K_a) + 'a'; //< if lowercase*/

		//			eKeyModifier modifiers = (eKeyModifier)event.key.keysym.mod; 

		//			//TODO: conversion from Win32 scan code to eKey enumerator
		//			onKeyboardCallback( event.type == Win32_KEYDOWN, (eKey)event.key.keysym.sym, modifiers, 0, 0 );
		//			break;
		//		}
		//	case Win32_MOUSEMOTION:			/* Mouse moved */
		//		{
		//			onMotionCallback( event.motion.state, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel );					
		//			break;
		//		}
		//	case Win32_MOUSEBUTTONDOWN:		/* Mouse button pressed */
		//		{					
		//			onMouseCallback( event.button.button, event.button.state, event.button.x, event.button.y );
		//			break;
		//		}
		//	case Win32_MOUSEBUTTONUP:		/* Mouse button released */
		//		{					
		//			onMouseCallback( event.button.button, event.button.state, event.button.x, event.button.y );
		//			break;
		//		}
		//	//case Win32_JOYAXISMOTION:		/* Joystick axis motion */
		//	//case Win32_JOYBALLMOTION:		/* Joystick trackball motion */
		//	//case Win32_JOYHATMOTION:		/* Joystick hat position change */
		//	//case Win32_JOYBUTTONDOWN:		/* Joystick button pressed */
		//	//case Win32_JOYBUTTONUP:			/* Joystick button released */
		//	case Win32_SYSWMEVENT:			/* System specific event */
		//		{
		//			break;
		//		}
		//	case Win32_VIDEORESIZE:			/* User resized video mode */
		//		{		
		//			onResizeCallback( event.resize.w, event.resize.h);
		//			break;
		//		}
		//	case Win32_VIDEOEXPOSE:			/* Screen needs to be redrawn  */
		//		{
		//			if ( m_isActive )
		//				onDisplayCallback();
		//			break;
		//		}
		//	}
		//}
		//return false;
	}

	
	void Window::onResizeCallbackEx(int width, int height)
	{
		
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		glViewport( 0, 0, width, height );
		
		// Calculate The Aspect Ratio Of The IWindow
		//gluOrtho2D( 0, width - 1, 0, height - 1 );//45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix

		return onResizeCallback( width, height );
	}

#define DO_CALLBACK( _func ) \
	for ( CallbackSet::const_iterator iCallback = m_callbacks.begin(); iCallback != m_callbacks.end(); ++iCallback ) \
	{ if ( (*iCallback)->_func ) return /*true*/; }

	//IWindow callback functions sends event data to a callback listener
	void Window::onDisplayCallback(void) 
	{ DO_CALLBACK( onDisplay( /*this*/ ) );}

	void Window::onKeyboardCallback( bool down, eKey key, eKeyModifier modifiers, int x, int y)
	{ DO_CALLBACK( onKeyboard( down, key, modifiers, x, y ) ); }

	void Window::onMouseCallback(bool down, eButton buttonState, int x, int y) 
	{ DO_CALLBACK( onMouse( down, buttonState, x, y ) ); }

	void Window::onMotionCallback( UInt32 buttonState, int x, int y, int rx, int ry) 
	{ 
		for ( CallbackSet::const_iterator iCallback = m_callbacks.begin(); iCallback != m_callbacks.end(); ++iCallback ) \
		{
			if ( (*iCallback)->onMotion( buttonState, x, y, rx, ry ) ) return /*true*/;
		}

		/*DO_CALLBACK( onMotion( buttonState, x, y, rx, ry ) );*/ }

	void Window::onResizeCallback(int width, int height) 
	{ 
		m_rect.setSize( Rect2s::Size(width,height) );

		DO_CALLBACK( onResize( width, height ) ); 
	}

	void Window::onIdleCallback(void) 
	{ DO_CALLBACK( onIdle() ); }

} /* END: Platform*/ } /*END: Kismo*/
