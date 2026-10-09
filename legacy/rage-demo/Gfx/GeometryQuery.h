/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\GeometryQuery.h
 * Status: superseded by kismo-v0.3/Gfx/GeometryQuery.h (same name, content differs)
 */
#ifndef GFX_GEOMETRYQUERY_H
#define GFX_GEOMETRYQUERY_H

#include "Gfx/Geometry.h"
#include "Gfx/Query.h"

namespace gfx
{
	/** \brief Base geometry query ttype that can be passed to a GeometryQuery.
		\remarks This function class is used to checmk whether a supplied face matches the query and
		allows any different types of query to be formed from simple ray/range queries top complex 
		polygon shape intersections
	*/
	class _DllExport GeometryQueryType : public QueryType
	{
	public:

		typedef frx::SmartPointer< GeometryQueryType > Ptr;

	public:
		GeometryQueryType(){}

		/** \brief Virtual query function should be overrident o perform the query as required
			\param[in] point1 XYZ tuples for 3 vertice positions making up a triangle
			\param[in] point2 XYZ tuples for 3 vertice positions making up a triangle
			\param[in] point3 XYZ tuples for 3 vertice positions making up a triangle
			\return Whether the triangle passes the query test
		*/
		virtual bool runQuery( const frx::Vector3r& point1, const frx::Vector3r& point2, const frx::Vector3r& point3 ) = 0;
	};

	/** \brief A ray query type for passing to a geometry query
	*/
	class _DllExport RayGeometryQueryType : public GeometryQueryType
	{
	public:
		/** \brief performs the ray query on the polugon gheometry
		*/
		virtual bool runQuery( const frx::Vector3r& point1, const frx::Vector3r& point2, const frx::Vector3r& point3 );
	
		frx::Vector3r start;
		frx::Vector3r direction;
	};

	/** \brief A swept where queery. i.e a line on which a sphere exists at every point along the line
	*/
	class _DllExport SweptSphereGeometryQueryType : public GeometryQueryType
	{
	public:
		/** \brief performs the ray query on the polugon gheometry
		*/
		virtual bool runQuery( const frx::Vector3r& point1, const frx::Vector3r& point2, const frx::Vector3r& point3 );
	
		frx::Vector3r start;
		frx::Vector3r direction;
		Real radius;
	};

	/** \brief Base query interface for a query to be applied to a geomety object
	*/
	class _DllExport GeometryQuery : public Query, public frx::SmartReferenced
	{
	public:

		typedef frx::SmartPointer< GeometryQuery > Ptr;

		typedef Queue< GeometryQuery* > PtrQueue;

		/** \brief Stores the reference information for the working data so it can be re-inserted into the
			source data when necessary
		*/
		struct FaceElement
		{
			// //TODO: Face editing These should not be modified atm
			UInt16 iposition[3]; ///< Index of the positions qithin the queries positions.  NOTE;: not the original index
			UInt16 inormal[3]; //< The indexes of the normals for the face. NOTE: not the original index
			//UInt16 sourceIndex; ///< Index of the face within the source buffer so we can update it later 
		};
		typedef Vector< FaceElement > FaceElementArray;

		/** \brief Vector element for storing a position or normal element
		*/
		struct Vector3Element
		{
			frx::Vector3r v;
			UInt16 isource; ///< Index of the vertex within the source buffer so we can update it later
		};
		typedef Vector< Vector3Element > Vector3ElementArray;

	public:
		GeometryQuery( Geometry* geometry, GeometryQueryType* query = 0 );


		/** \brief Executes the query and returns whether the query was able to succeed. If false is returned 
			it is possible the geometry buffers were locked and subsequent calls should be attempted
			\remarks This function is virtual so that it can be specialised by a geometry node to perform
			a more specific query operation for the type of data that is stored
			\par The default implementation performs a face by face query
		*/
		virtual Bool doQuery();

		/** \brief Gets a reference to the query results
			\remarks To update the geometry call
			putGeometry() which will enter the geometry data into the visual buffers
		*/
		FaceElementArray& getFaces()
		{ return m_faces; }

		/** \brief Get the position results which are indexed by the faces
		*/
		Vector3ElementArray& getPositions()
		{ return m_positions; }

		/** \brief Get the position results which are indexed by the faces
		*/
		Vector3ElementArray& getNormals()
		{ return m_normals; }

		/** \brief Calls putGeometry() and Resets the query results emtrying the buffers 
			for the results etc
		*/
		virtual Bool closeQuery();

	protected:

		/** \brief Re-enter the faces into the geometry structure which is used for the visual display 
			of the geometry
			\remarks Called by closeQuery();
		*/
		virtual Bool putGeometry();

	protected:
		Geometry::Ptr m_geometry; ///< pointer to the geometry node that the query is operating on
		FaceElementArray m_faces; ///< Vector of faces that passed the geometry query
		Vector3ElementArray m_positions; ///< Vector of positions that passed the geometry query
		Vector3ElementArray m_normals; ///< Vector of normals that passed the geometry query
		//TODO: add uv's
	};

} //END: gfx

#endif