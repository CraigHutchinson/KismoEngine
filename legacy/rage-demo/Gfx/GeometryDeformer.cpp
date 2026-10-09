/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\GeometryDeformer.cpp
 * Status: superseded by kismo-v0.3/Gfx/GeometryDeformer.cpp (same name, content differs)
 */
#include "GeometryDeformer.h"

namespace gfx
{

	template class frx::LockFreeQueue< Deformer* >;

	Deformer::Deformer( GeometryQuery* query ) :
		m_query( query ),
		m_threadResultQueue( 0 ),
		m_threadQueryAttemts( 0 )
	{}

	Bool Deformer::closeDeformer()
	{
		if ( m_query.isValid() )
		{
			return m_query->closeQuery(); //Close the query whihc will update the render buffers with the deformed geometry
		}	
		else
		{
			assert( false );
			return false;
		}
	}

	Bool Deformer::doQuery()
	{
		assert( m_query.isValid() );
		if ( m_query.isNull() )
		{
			assert( false );
			m_threadQueryAttemts = Count_MAX; //< Make sure we don't get a repeat
			logError( "Failed to run deformer using threaded model as no query has been supplied!" );
			return false;
		}

		return m_query->doQuery(); //Run the query
	}
		
	Bool Deformer::threadPhase()
	{
		//logComment(  "Deformer::threadPhase called" );
		if ( !doDeformer() )
		{
			logError( "Failed to run deformer with threaded model!" );
			assert( false );
			return ++m_threadQueryAttemts <= 5; ///< Try again 5 times
		}

		//So we have managed to do the deform calc so we should close the deformer which will update the geometry
		if ( !closeDeformer() )
		{
			logError( "Failed to close deformer with threaded model!" );
			assert( false );
			return false;
		}

		assert( m_threadResultQueue );
		if ( m_threadResultQueue ) /// If a results queue was supplied we should add this
		{
			//logComment(  "Deformer::threadPhase added to result queue" );
			m_threadResultQueue->push( this );
		}
		else
		{
		}

		//logComment(  "Deformer::threadPhase completed" );
		m_threadQueryAttemts = 0;			
		return false; //< We are finished so remove from the thread
	}


	Bool IndentDeformer::doDeformer()
	{
		//logComment(  "Deformer::doDeformer called" );
		if ( !doQuery() )
		{
			//logComment(  "Deformer::doQuery failed" );
			return false;
		}

		if ( m_query->getPositions().size() )
		{
			//Peform the deformer
			gfx::GeometryQuery::FaceElementArray::iterator iFace = m_query->getFaces().begin();
			for ( ;iFace != m_query->getFaces().end(); ++iFace )
			{
				for ( int i = 0; i < 3; ++i )
				{
					m_query->getPositions()[ iFace->iposition[i] ].v
						+= ((gfx::RayGeometryQueryType*)m_query->getQueryType())->direction * (Real)0.01;
				}

				frx::Planer plane( 
					m_query->getPositions()[ iFace->iposition[0] ].v,
					m_query->getPositions()[ iFace->iposition[1] ].v, 
					m_query->getPositions()[ iFace->iposition[2] ].v );

				frx::Vector3r mid = m_query->getPositions()[ iFace->iposition[0] ].v 
					+ m_query->getPositions()[ iFace->iposition[1] ].v 
					+ m_query->getPositions()[ iFace->iposition[2] ].v;
				mid /= 3;

				for ( int i = 0; i < 3; ++i )
				{
					m_query->getPositions()[ iFace->iposition[i] ].v
						+= ((gfx::RayGeometryQueryType*)m_query->getQueryType())->direction;

					frx::Vector3r norm = (mid - plane.n)
						- m_query->getPositions()[ iFace->iposition[i] ].v;
					norm.normalise();
					m_query->getNormals()[ iFace->inormal[i] ].v = norm;							

				}
			}
			//Simulate processing of complex geometry calculations
			sleep( 20 );
		}
		else
		{
			//logComment(  "Deformer:: no faces foudn from query" );
		}

		return true;
	}

} //END: gfx