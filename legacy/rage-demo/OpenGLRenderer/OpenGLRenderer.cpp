/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\OpenGLRenderer\OpenGLRenderer.cpp
 * Status: superseded by kismo-v0.3/OpenGLRenderer/OpenGLRenderer.cpp (same name, content differs)
 */
#include "OpenGLRenderer.h"
#include "Framework/Log.h"
#include "OpenGLRenderer/OpenGLWindowTarget.h"
#include "Gfx/Viewport.h"
#include "Gfx/Camera.h"
#include "Gfx/MaterialState.h"
#include "Gfx/RenderModeState.h"

//Include the necessary libraries
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")


//include the opengl headers 
#include <GL/glew.h> 
#include <GL/wglew.h> 
#include <GL/gl.h> 
#include <GL/glu.h>
#include <GL/glut.h>


namespace gfx
{
	gfx::Buffer::Ptr OpenGLRenderer::createBuffer( gfx::Buffer::ePurpose purpose, const gfx::BufferDefinition& definition, Size elementCount, const Byte* data, gfx::Buffer::eUsage usage ) const
	{
		PROFILE;

		//TODO: Check usage and create relevant buffer
		gfx::Buffer::Ptr buff( gfx::SwBuffer::create( purpose, definition, elementCount ) );

		if ( buff && buff->initialise( elementCount * definition.getSize(), data ) )
			return buff;
		else
		{
			logError( "Buffer creation failed!" );
			buff = 0;
			return 0;
		}

	}

	OpenGLRenderer::OpenGLRenderer() : 
		gfx::Renderer( _T("OpenGLRenderer") ),
		m_frameStarted( false ),
		m_hDC( 0 ),
		m_hRC( 0 )
	{}

	OpenGLRenderer::~OpenGLRenderer()
	{
	}
	
	gfx::RenderTarget* OpenGLRenderer::createTarget( const frx::Rtti& type, const String& name, Priority priority )
	{
		PROFILE;

		gfx::RenderTarget* newTarget = 0;

		//For now we only support window target. Latewr texture targets etc can be added
		if ( &type == &gfx::WindowTarget::TYPE )
			newTarget = new OpenGLWindowTarget( name, priority ) ;

		if ( newTarget )
		{
			addRenderTarget( newTarget );
			logSuccess( "Render target '" << name << "' added to the renderer '" << getName() << "'" );
		}
		else
		{
			logError( "Renderer createTarget '" << name << "' or type '" << type.typeFullName() << "' could not be created for the renderer '" << getName() << "'" );
		}

		return newTarget;
	}

	Bool OpenGLRenderer::initialise()
	{
		PROFILE;

		//Find the first render target that is a window
		HWND hWnd = 0; //The window handle	
		for ( RenderTargetMap::const_iterator iTarget = m_renderTargets.begin(); iTarget != m_renderTargets.end(); ++iTarget )
		{
			if ( (iTarget->second)->isDerived( gfx::WindowTarget::TYPE ) )
			{
				gfx::WindowTarget* winTarget = (gfx::WindowTarget*)(iTarget->second);
				if (	!winTarget->getWindow() || 
						!winTarget->getWindow()->create() )
					break;

				hWnd = (HWND)winTarget->getWindow()->getContextHandle();
				m_activeRenderTarget = iTarget->second;
				break;
			}
		}

		if ( hWnd == 0 )
		{
			logCritical( "Initialise was called for a renderer but the renderer has no Windowtarget to use as" \
				"the primary context." );
			return false;
		}

		m_hDC = GetDC( hWnd ); 
	    
		//Check if we obtained a valid device context
		if (!m_hDC)
		{
			logError( _T("Invalid device context returned from OGL::CreateContext") );
			return false;
		}

		if ( !setupPixelFormat() ) 
		{
			logError( _T("Failed to set render context pixel format") );
			return false; 
		}
	 
		//create the render context
		m_hRC = wglCreateContext( m_hDC ); 

		//set the current render context
		//TODO: Create a context class and manager?
		wglMakeCurrent( m_hDC, m_hRC ); 

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			logError( _T("An error occured while initialising GLEW(The OpenGL Extension Wrangler Library) - ") << glewGetErrorString(err) );
		}

		
		glEnable( GL_DEPTH_TEST );
    
		glEnable( GL_BLEND ); //Enable alpha blending
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); //Set the blend function

		//glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
		glEnable( GL_CULL_FACE);


		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f,-1.0f);	

		return 0;
	}

	void OpenGLRenderer::setVSync( UInt32 frameInterval )
	{
		if ( WGLEW_EXT_swap_control )
			wglSwapIntervalEXT( frameInterval );
	}

	Bool OpenGLRenderer::setupPixelFormat()
	{ 
		PROFILE;

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
			logError( _T("ChoosePixelFormat failed") ); 
			return false; 
		} 
	 
		if (SetPixelFormat( m_hDC, pixelformat, &pfd) == FALSE) 
		{ 
			logError( _T("SetPixelFormat failed") ); 
			return false; 
		} 

	 
		return true; 
	} 

	Bool OpenGLRenderer::reinitialise()
	{
		PROFILE;


		return false;
	}

	void OpenGLRenderer::destroy()
	{
		PROFILE;


	}


	Bool OpenGLRenderer::beginFrame()
	{		
		PROFILE;

		m_frameStarted = true; ///< Store that a aframe has begun
		glClear( GL_COLOR_BUFFER_BIT ); //clear The colour bits, depth is cleared in setViewport for now

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();					// Reset The View
		
		glEnable( GL_LIGHTING );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 1 );
		glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
		frx::Colour4 ambient( 1, 1, 1 );
		frx::Colour4 diffuse( 1, 1, 1 );
		frx::Colour4 specular( (Real)0.01, (Real)0.01, (Real)0.01 );
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambient ); //Set the ambient colour
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse ); //Set the diffuse colour
		glLightfv( GL_LIGHT0, GL_SPECULAR, specular ); //set the specular colour
			
		glEnable( GL_LIGHT0 );

		return false;
	}

	Void OpenGLRenderer::endFrame()
	{
		PROFILE;

		m_frameStarted = false; ///< Store that a aframe has ended
		m_activeRenderTarget->update();
		
		SwapBuffers(m_hDC);
	}

	void OpenGLRenderer::setRenderTarget( gfx::RenderTarget* target )
	{
		m_activeRenderTarget = target;

	}

	void OpenGLRenderer::setCamera( gfx::Camera* camera )
	{
		PROFILE;

		m_activeCamera = camera;

		glMatrixMode(GL_PROJECTION);						
		
		//set the projection back to the identity
		glLoadIdentity();

		if ( m_activeCamera )
		{
			//check if the camera is perspective or not
			gluPerspective( m_activeCamera->getFieldOfView(), 
				m_activeRenderTarget->getAspect(),
				m_activeCamera->getNearPlane(), m_activeCamera->getFarPlane() ); 
		}
		else
		{			
			glOrtho( 0.0, m_activeRenderTarget->getWidth(), m_activeRenderTarget->getHeight(), 0.0, 0.0, 1000 );
		}
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();					// Reset The View
		
		setTransform( m_activeCamera->worldTransform(), false );
		
		float pos[4] = { 0,100,100, 1};
		glLightfv( GL_LIGHT0, GL_POSITION, pos );//Set the position of the light

	}

	void OpenGLRenderer::setViewport( gfx::Viewport* viewport )
	{		
		PROFILE;

		m_activeViewport = viewport;

		if ( !m_activeRenderTarget )
		{
			logError( "No active render target has been specified! No viewport can be set!" );
			return;
		}

		GLint x = (GLint)(viewport->getRect().left() * m_activeRenderTarget->getWidth());
		GLint y = (GLint)(viewport->getRect().top() * m_activeRenderTarget->getHeight());
		GLsizei width = (GLsizei)(viewport->getRect().width() * m_activeRenderTarget->getWidth());
		GLsizei height = (GLsizei)(viewport->getRect().height() * m_activeRenderTarget->getHeight());
		glViewport( x, y, width, height );

		if ( m_frameStarted )
			glClear( GL_DEPTH_BUFFER_BIT ); //clear depth bit

	}

	void OpenGLRenderer::pushTransform( const gfx::TransformR& transform )
	{
		glMatrixMode(GL_MODELVIEW);	
		glPushMatrix();	

		setTransform( transform );	
	}

	void OpenGLRenderer::popTransform()
	{		
		glPopMatrix();
	}

	void OpenGLRenderer::draw( const gfx::Geometry* geometry )
	{
		PROFILE;

		//Push the transform matrix
		pushTransform( geometry->worldTransform() );

		//lock the geometry buffers
		const Byte* normalBuffer = (Byte*)(geometry->normal.buffer?geometry->normal.buffer->lock():0);
		const Byte* positionBuffer = (Byte*)(geometry->position.buffer?geometry->position.buffer->lock():0);

		if ( positionBuffer && normalBuffer )
		{			
			UInt16 normalStride = geometry->normal.stride;
			UInt16 positionStride = geometry->position.stride;
			const gfx::Geometry::IndiceArray& indiceSets = geometry->indiceSets;

			for ( gfx::Geometry::IndiceArray::const_iterator iIndices = indiceSets.begin(); iIndices != indiceSets.end(); ++iIndices )
			{
				if ( iIndices->material )
				{
					setMaterialState( iIndices->material );
				}

				//Lock the buffer for reading
				const Byte* indicesBuffer = (Byte*)iIndices->buffer->lock();
				const Byte* indicesBufferEnd = indicesBuffer + iIndices->buffer->getSize();

				UInt16 inormalOffset = iIndices->normalOffset;
				UInt16 ipositionOffset = iIndices->positionOffset;
				UInt16 istride = iIndices->stride;

				glBegin( GL_TRIANGLES );

				for (; indicesBuffer != indicesBufferEnd; indicesBuffer += istride )
				{	
					glNormal3fv( (Real*)(normalBuffer + (normalStride * *(UInt16*)(indicesBuffer + inormalOffset))) );
					glVertex3fv( (Real*)(positionBuffer + (positionStride * *(UInt16*)(indicesBuffer + ipositionOffset))) );
				}

				glEnd();

				//Release the buffer
				iIndices->buffer->release();				
			}			
		}
		else
		{
			assert( false );
		}

		//Release the buffer
		if ( positionBuffer )
			geometry->position.buffer->release();
		if ( normalBuffer )
			geometry->normal.buffer->release();

		popTransform();
	}

	void OpenGLRenderer::drawSphere( Real radius )
	{
		glutWireSphere( radius, 6, 6 );
	}

	void OpenGLRenderer::drawBox( Real width, Real height, Real length )
	{
	}

	void OpenGLRenderer::setTransform( const gfx::TransformR& transform, bool forward )
	{
		PROFILE;

		if ( !transform.isIdentity() )
		{
			frx::Vector3r translate = transform.getTranslate();

			if ( !forward )
			{
				if ( !forward && transform.isRSMatrix() )
					glMultMatrixf( transform.getRotate().inverse() );
				glTranslatef( -translate.x, -translate.y, -translate.z  );
			}
			else
			{
				glTranslatef( translate.x, translate.y, translate.z  );

				if ( transform.isRSMatrix() )
				{
					assert( transform.getRotate() != frx::Matrix4r::Zero );
					glMultMatrixf( transform.getRotate() );
				}

			}

			//if ( transform.isUniformScale() )
			//{
			//	Real scale = transform.getUniformScale();
			//	glScalef( scale, scale, scale );
			//	assert( scale != 0 );
			//}
			//else
			//{
			//	const frx::Vector3r& scale = transform.getScale();
			//	glScalef( scale.x, scale.y, scale.z );
			//	assert( scale.x != 0 && scale.y != 0 && scale.z != 0 );
			//}
			//

		}
	}

	void OpenGLRenderer::setMaterialState( gfx::MaterialState* material )
	{
		PROFILE;

		glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse );
		glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient );
		glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular );
		glMaterialf(GL_FRONT, GL_SHININESS, material->specularPower );
	}
	
	void OpenGLRenderer::draw2DQuad( const frx::Rect2r& rect, const frx::Colour4& colour )
	{
		PROFILE;

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			//set the projection back to the identity
			glLoadIdentity();

			//check if the camera is perspective or not
			glOrtho( 0.0, 1, 0, 1.0, 0.0f, 100.0f );
			
			glMatrixMode(GL_MODELVIEW);

			glPushMatrix();

				glLoadIdentity();
				
				glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT );
					glDisable( GL_DEPTH_TEST );
					glDisable( GL_LIGHTING );
					glDisable( GL_TEXTURE_2D );

					glColor4fv( colour );
					glRectf( rect.left(), rect.top(), rect.right(), rect.bottom() );
					
					glPopAttrib();
				glPopMatrix();		
			glMatrixMode(GL_PROJECTION);

			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	}

	void OpenGLRenderer::pushRenderState()
	{
		PROFILE;

		glPushAttrib( GL_ALL_ATTRIB_BITS );
	}

	void OpenGLRenderer::setRenderState( RenderState* state )
	{
		PROFILE;
		if ( state->getType() == gfx::WireframeState::TYPE )
		{
			if ( ((gfx::WireframeState*)state)->enabled )
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );

			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			}

			if ( ((gfx::WireframeState*)state)->colourOverride )
			{
				glDisable( GL_LIGHTING );
				glColor4fv( ((gfx::WireframeState*)state)->colour );
			}
		}

	}

	void OpenGLRenderer::popRenderState()
	{
		PROFILE;

		glPopAttrib();
	}

} //END: gfx