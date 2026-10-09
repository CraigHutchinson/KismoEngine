/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_TestApp\DeformerControllerAction.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "DeformerControllerAction.h"
#include "Framework/Window.h"
#include "Math/Plane.h"
#include "Gfx/Camera.h"
#include <windows.h>

DeformerControllerAction::DeformerControllerAction( frx::IWindow* window, gfx::Camera* camera, gfx::Geometry* geometry ) :
	EventAction( false ),
	m_camera( camera ),
	m_geometry( geometry ),
	m_on( false )
{
	assert( m_camera );
	assert( m_geometry );
	window->onKey().registerAction( this );

	m_deformerThread = frx::Thread::create( _T("deformer"), frx::Thread::ThreadPriority_Medium );

}

DeformerControllerAction::~DeformerControllerAction()
{
	terminate();
}

void DeformerControllerAction::terminate()
{
	m_deformerThread->stopThread();

	for ( gfx::Deformer::PtrArray::iterator iDeform = m_deformers.begin(); iDeform != m_deformers.end(); ++iDeform )
	{
		delete *iDeform;
	}
	m_deformers.clear();
}

void DeformerControllerAction::exec( frx::Event* event, Real tDelta, Real tApp )
{
	if ( event->getType() == frx::KeyEvent::TYPE )
	{
		frx::KeyEvent* key = (frx::KeyEvent*)event;
		
		if ( key->keyCode == 'F' )
		{
			m_on = key->pressed;
		}
		else
		if (  key->keyCode == 'T' && key->pressed )
		{
			if ( !m_deformerThread->isThreadRunning() )
			{
				logComment( "Deform thread started" );
				m_deformerThread->startThread();
			}
			else
			{
				logComment( "Deform thread stopped" );
				m_deformerThread->stopThread();
			}
		}
	}

}

void DeformerControllerAction::update( Real tDelta, Real tApp )
{

	if ( m_on )
	{
		m_on = false;
		gfx::Deformer* deformer = 0;

		//Check if there is a deformer in the free list instead or re-creating one
		if ( m_deformerResultQueue.empty() )
		{
			//logComment(  "New deformer created" );
			//We need a new deformer
			deformer = new gfx::IndentDeformer();
			m_deformers.push_back( deformer );
		}
		else
		{
			//logComment(  "Deformer re-used" );
			//re-use the deformer in the free list to save creating a new one
			gfx::Deformer::PtrLFQueue::Node* deformerNode;
			deformerNode = m_deformerResultQueue.pop();
			assert( deformerNode );
			deformer = deformerNode->value;
			delete deformerNode; //Remove the queue node
		}
		assert( deformer);

		//Specify that we want the results of the query to be passed to our result queue
		deformer->setThreadResultQueue( &m_deformerResultQueue );
		
		// get the query for the deformer and set it up
		gfx::GeometryQuery* geometryQuery = deformer->getQuery();

		//If the deformer has no query we need to add one
		if ( !geometryQuery )
		{
			geometryQuery = new gfx::GeometryQuery( m_geometry );
			deformer->setQuery( geometryQuery ); //Add the query to the deformer
		}
		assert( geometryQuery );

		gfx::QueryType* queryType = geometryQuery->getQueryType();

		// If the query has no type or should be changed then it should be set here
		if ( !queryType )
		{
			queryType = new gfx::RayGeometryQueryType(); 
			geometryQuery->setQueryType( queryType ); //Set the queries type object
		}

		//Setup the ray query type to eb in the position & direction fo the camera
		((gfx::RayGeometryQueryType*)queryType)->start = m_camera->worldTransform().getTranslate();			
		((gfx::RayGeometryQueryType*)queryType)->direction =
			m_camera->worldTransform().getRotate() * math::Vector3( 0, 0, -1 );
		//((gfx::SweptSphereGeometryQueryType*)queryType)->radius = 1.5;

		if ( m_deformerThread->isThreadRunning() )
		{
			//logComment( "Threaded" );
			//Attach the deformer to a thread
			deformer->attachToThread( m_deformerThread );
		}
		else
		{
			//Run the deformer on this thread
			if ( !deformer->doDeformer() )
			{
				assert( false );
			}

			//Close the deformer whihc will update the render buffers
			if ( !deformer->closeDeformer() )
			{
				assert( false );
			}
		}
	}
}