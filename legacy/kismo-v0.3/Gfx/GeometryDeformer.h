/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\GeometryDeformer.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_DEOMETRYDEFORMER_H
#define GFX_DEOMETRYDEFORMER_H

#include "Gfx/GeometryQuery.h"
#include "Math/Plane.h"
#include "Framework/Log.h"

namespace gfx
{

	/** \brief base geometry defromer type
		\remarks Requires a GeometryQuery instance qhich is used to query the geometry object.
		\par Once the query and deform is complete it will be added to m_threadResultQueue if the
		threaded model is in use and m_threadResultQueue has been specified

	*/
	class _DllExport Deformer : public frx::Threaded
	{
	public:

		/** \brief Lockless queue type
		*/
		typedef frx::LockFreeQueue< Deformer* > PtrLFQueue;

		typedef Queue< Deformer* > PtrQueue;
		typedef Stack< Deformer* > PtrStack;
		typedef Vector< Deformer* > PtrArray;

	public:

		/** \brief Constructor passes a query object for the deformer to use
			\remarks The object is managed by smart pointer so will be destroyed 
			automatically with the deformer
		*/
		Deformer( GeometryQuery* query = 0 );

		/** \brief Sets the current query type. 
			\remarks The query is stored via smart pointer so will be automatically destroyed
			if no other refrences exist so it is safe to call 'setQueryType( new QueryType() )'
		*/
		void setQuery( GeometryQuery* query )
		{ m_query = query; }

		/** \brief Get a pointer to the current query
		*/
		GeometryQuery* getQuery() const
		{ return m_query.getPtr(); }

		/** \brief Executes the query and returns whether the deformer was able to succeed.
		*/
		virtual Bool doDeformer() = 0;
		
		/** \brief Performs the final stages of the deformer clearing the query results so it may be run again
		*/
		virtual Bool closeDeformer();

		/** \brief Sets the results queue which the deformers will be added to once it has finished processing
			\remarks Used when using the Threaded model
		*/
		void setThreadResultQueue( PtrLFQueue* queue )
		{ m_threadResultQueue = queue; }
	
	protected:

		/** \brief helper function for defomer implementation and should be called at the start of any call to
			doDeformer()
		*/
		Bool doQuery();
		
		virtual Bool threadIteration();

	protected:
		GeometryQuery::Ptr m_query; ///< The query that is used to obtain the deformer working data

		//Threaded query paramaters
		PtrLFQueue* m_threadResultQueue; //< Pointer to a queue where the completed deformer queries will be added
		Count m_threadQueryAttemts; ///< Counts the numbert of attempts made to perform the query. TODO: needed?
	};


	/** \brief Demo deformer simply creates an indent on the surface and nothing special
		//TODO: Deform factory so this doens't need to be exported across dll boundaries
	*/
	class _DllExport IndentDeformer : public Deformer
	{
	public:

		/** \brief Executes the query and returns whether the deformer was able to succeed.
		*/
		virtual Bool doDeformer();
	};

} //END: gfx

#endif