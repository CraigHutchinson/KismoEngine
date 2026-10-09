/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Query.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_QUERY_H
#define GFX_QUERY_H

#include "Gfx/Geometry.h"
#include "Framework/SmartPointer.h"
#include "Framework/LockFreeQueue.h"
#include "Framework/Thread.h"

namespace gfx
{
	/** \brief Base query interface for passing to a query class
	*/
	class _DllExport QueryType : public frx::SmartReferenced
	{
	public:
		typedef frx::SmartPointer< QueryType > Ptr;
	public:
		QueryType(){}

	};

	/** \brief Base query interface which takes a queryType to perform a query
		\remarks The query derives from Threaded to allow it to be run on a seperate thread.
		\par A reference to a locklessQueue may eb supplied which will be given a reference to the query when 
		the query has completed. This allows a managing process to apply the query results
	*/
	class _DllExport Query : public frx::Threaded
	{
	public:

		typedef frx::LockFreeQueue< Query* > PtrLFQueue;

	public:
		Query( QueryType* queryType = 0 ) :
			m_query( queryType ),
			m_threadResultQueue( 0 ),
			m_threadQueryAttemts( 0 )
		{}

		/** \brief Sets the current query type. 
			\remarks The query is stored via smart pointer so will be automatically destroyed
			if no other refrences exist so it is safe to call 'setQueryType( new QueryType() )'
		*/
		void setQueryType( QueryType* query )
		{ m_query = query; }

		/** \brief Get a pointer to the current query
		*/
		QueryType* getQueryType() const
		{ return m_query.getPtr(); }

		/** \brief Executes the query and returns whether the query was able to succeed.
		*/
		virtual Bool doQuery() = 0;
		
		/** \brief Performs the final stages of the query clearing the query results so ti may eb run again
		*/
		virtual Bool closeQuery() = 0;

		/** \brief Sets the results queue which the query will be added to once it has finished processing
			\remarks Used when using the Threaded model
		*/
		void setThreadResultQueue( PtrLFQueue* queue )
		{ m_threadResultQueue = queue; }
	
	protected:
		
		virtual Bool threadIteration()
		{
			if ( !doQuery() ) //Run the query
				return ++m_threadQueryAttemts <= 5; ///< Try again 5 times

			if ( m_threadResultQueue ) /// If a results queue was supplied we should add this
				m_threadResultQueue->push( (gfx::Query*)this );

			m_threadQueryAttemts = 0;			
			return false; //< We are finished so remove from teh thread
		}

	protected:
		QueryType::Ptr m_query; ///< The query that is used to obtain the working data

		//Threaded query paramaters
		PtrLFQueue* m_threadResultQueue; //< Pointer to a queue where the completed queries will be added
		Count m_threadQueryAttemts; ///< Counts the numbert of attempts made to perform the query. TODO: needed?
	};



} //END: gfx

#endif