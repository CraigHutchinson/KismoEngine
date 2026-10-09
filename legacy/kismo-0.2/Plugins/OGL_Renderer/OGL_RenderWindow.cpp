/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\OGL_Renderer\OGL_RenderWindow.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "OGL_RenderWindow.h"
#include "OGL_Renderer.h"

namespace kismo
{
	namespace gfx
	{
		void setVSync( unsigned int interval )
		{
			if ( WGLEW_EXT_swap_control )
				wglSwapIntervalEXT( interval );
		}


		OGL_RenderWindow::OGL_RenderWindow( const k_String& name ) : 
			RenderWindow( name ),
			m_hDC( 0 ), m_hRC( 0 ), m_hWnd( 0 )
		{
		}

		void OGL_RenderWindow::setWindowHandle( void* windowHandle )
		{
			m_hWnd = *(HWND*)windowHandle;			
		}

		bool OGL_RenderWindow::createContext()
		{
			m_hDC = GetDC( m_hWnd ); 
	    
			//Check if we obtained a valid device context
			if (!m_hDC)
			{
				LOG_ERROR( _T("Invalid device context returned from OGL::CreateContext") );
				return false;
			}

			if ( !setupPixelFormat() ) 
			{
				LOG_ERROR( _T("Failed to set render context pixel format") );
				return false; 
			}
		 
			//create the render context
			m_hRC = wglCreateContext( m_hDC ); 

			//set the current render context
			//TODO: Create a context class and manager?
			wglMakeCurrent( m_hDC, m_hRC ); 

			// Initialise OpenGL state to desired values
			//glEnable( GL_DEPTH_TEST );
			//glEnable( GL_TEXTURE_2D );						// Enable Texture Mapping
			//glEnable( GL_CULL_FACE );

			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				LOG_ERROR( _T("An error occured while initialising GLEW(The OpenGL Extension Wrangler Library) - ") << glewGetErrorString(err) );
			}


			setVSync( 0 );
			buildFont( 12 );
			
			glClearColor( 1, 0, 0, 0 );
			m_timer.start();
			m_timer.setSimulationTimeScale( 0.001f );
		
			return true;
		}

		bool OGL_RenderWindow::setupPixelFormat()
		{ 
			PIXELFORMATDESCRIPTOR pfd; 
			int pixelformat; 
		  
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
			pfd.nVersion = 1; 
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
			pfd.dwLayerMask = PFD_MAIN_PLANE; 
			pfd.iPixelType = PFD_TYPE_COLORINDEX; 
			pfd.cColorBits = 8; 
			pfd.cDepthBits = 16; 
			pfd.cAccumBits = 0; 
			pfd.cStencilBits = 0; 
		 		 

			if ( (pixelformat = ChoosePixelFormat( m_hDC, &pfd)) == 0 ) 
			{ 
				LOG_ERROR( _T("ChoosePixelFormat failed") ); 
				return false; 
			} 
		 
			if (SetPixelFormat( m_hDC, pixelformat, &pfd) == FALSE) 
			{ 
				LOG_ERROR( _T("SetPixelFormat failed") ); 
				return false; 
			} 

		 
			return true; 
		} 
		
		void OGL_RenderWindow::update()
		{
			static float acc = 0;
			static k_UInt32 cnt = 0;
			static k_UInt32 lastFPS = 0;
			static k_UInt32 totalFrames = 0;
			static k_UInt32 totalSecs = 0;


			glFlush();
			SwapBuffers(m_hDC);

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //clear The target window
			m_timer.startFrame();
			acc += m_timer.getFrameTime() * (1/1000.0f);
			++cnt;

			if ( acc >= 0.5 )
			{
				LOG_MESSAGE( _T("FPS=") << cnt*2 );
				lastFPS = cnt*2;
				totalFrames += lastFPS;
				acc = 0;
				cnt = 0;
				++totalSecs;
			}
			
			glBegin( GL_QUADS );
			  glColor3f( 0.f, 1.f, 0.f ); // Sets current primary color to green
			  glVertex3f( 0.f, 0.f, 0.f ); // Specify three vertices
			  glVertex3f( 0.5f + 0.5f * sin(m_timer.getSimulationTime()), 0.f, 0.f );
			  glVertex3f( 1.f, 0.5f - 0.5f * cos(m_timer.getSimulationTime()), 0.f );
			  glVertex3f( 0.f, 1.f, 0.f );
			glEnd();

			
			std::stringstream str;
			str << "FPS: " << lastFPS << " AVG_FPS: " << totalFrames / (k_Float32)totalSecs;
			k_String8 ts = str.str();
			drawText( 0,0, ts );


		}



		
		void OGL_RenderWindow::buildFont( k_UInt32 fontHeight )					// Build Our Bitmap Font
		{
		 
			m_fontHeight = fontHeight;

			HFONT	font;						// Windows Font ID
			HFONT	oldfont;					// Used For Good House Keeping

			m_fontListID = glGenLists(96);					// Storage For 96 Characters
			
			font = CreateFont(
				m_fontHeight,				// Height Of Font
				0,				// Width Of Font
				0,				// Angle Of Escapement
				0,				// Orientation Angle
				600,			// Font Weight ( FW_DONTCARE is 0, FW_NORMAL is 400, FW_BOLD is 700 and FW_BLACK is 900)
				FALSE,				// Italic
				FALSE,				// Underline
				FALSE,				// Strikeout
				ANSI_CHARSET,			// Character Set Identifier
				OUT_TT_PRECIS,			// Output Precision (True Type)
				CLIP_DEFAULT_PRECIS,		// Clipping Precision
				ANTIALIASED_QUALITY,		// Output Quality
		        
				/*  [DEFAULT_PITCH, FIXED_PITCH and VARIABLE_PITCH] |
					[FF_DECORATIVE, FF_MODERN, FF_ROMAN, FF_SCRIPT, FF_SWISS, FF_DONTCARE]*/
				FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
				_T("Verdana"));			// Font Name
		    
			if (!font)
				MessageBoxA(0,"font failed","nooo",0);

			oldfont = (HFONT)SelectObject( m_hDC, font );		// Selects The Font We Want

			if (!oldfont)
				MessageBoxA(0,"OLD font failed","nooo",0);

			if ( !wglUseFontBitmaps( m_hDC, 32, 96, m_fontListID) )			// Builds 96 Characters Starting At Character 32
				MessageBoxA(0,"wglUseFontBitmaps failed","nooo",0);


			if (!SelectObject( m_hDC, oldfont))				// Selects The Font We Want
				MessageBoxA(0,"SelectFont 2 font failed","nooo",0);

			DeleteObject(font);

		}

		void OGL_RenderWindow::killFont()						// Delete The Font List
		{
 			glDeleteLists(m_fontListID, 96);				// Delete All 96 Characters ( NEW )
		}
		
		void OGL_RenderWindow::drawText( k_Float32 x, k_Float32 y, const k_String8& text )
		{
			glMatrixMode(GL_PROJECTION);

			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0.0, 800,600, 0.0, 0.0f, 100.0f );

			
			glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
			glLoadIdentity();

			
			glDisable( GL_LIGHTING );
			glBindTexture(GL_TEXTURE_2D, 0);

			glRasterPos2f( x * m_fontHeight, (y + 1) * m_fontHeight ); // Position The Text On The Screen
			glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
			

			// Pushes The Display List Bits
			glPushAttrib( GL_LIST_BIT );			
				

				// Sets The Base Character to 32
				glListBase(m_fontListID - 32);

				
				// Draws The Display List Text
				glCallLists( (GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str() );

			// Pops The Display List Bits
			glPopAttrib();

		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		}
	} //END: gfx

} //END: kismo
