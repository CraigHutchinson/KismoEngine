/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\GeometryQuery.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "GeometryQuery.h"
#include "Math/Plane.h"
#include "Framework/Log.h"
#include "Gfx/GeometryMath.h"

namespace gfx
{
	bool RayGeometryQueryType::runQuery( const math::Vector3& point1, const math::Vector3& point2, const math::Vector3& point3 )
	{
		return rayInTriangle( start, direction, point1, point2, point3 );
	}

	bool SweptSphereGeometryQueryType::runQuery( const math::Vector3& point1, const math::Vector3& point2, const math::Vector3& point3 )
	{
		math::Vector3 intersect;
		if ( intersectSweeptSpherePlane( start, direction, radius, point1, point2, point3, 0, &intersect ) )
		{
			if ( pointInTriangle( intersect,  point1, point2, point3 ) )
				return true;
		}
		return false;
	}


	GeometryQuery::GeometryQuery( Geometry* geometry, GeometryQueryType* query ) :
		Query( query ),
		m_geometry( geometry )
	{
	}

	Bool GeometryQuery::doQuery()
	{
		assert( m_geometry.isValid() );
		if ( m_geometry.isNull() )
		{
			//Log a critical error as the caller may decide to attempt the query again because it assumes the buffers were locked
			// when actually there is no geometry object. TODO: fix this, currently bug should be caught before we get to this point
			logCritical( "No geometry node has been supplied to a geometryQuery. POSSIBLE DEADLOCK MAY OCCUR!" );
			return false;
		}

		typedef Map< UInt16, UInt16 > SourceToQueryMap; ///< Map to take a source index to get the query index if it is already found
		SourceToQueryMap ipositionsAdded; ///< Map for the indexes of positions that have been added already due to being shared between faces
		SourceToQueryMap inormalsAdded; ///< Map for the indexes of normals that have been added already due to being shared between faces

		const Byte* normalBuffer = (Byte*)(m_geometry->normal.buffer? m_geometry->normal.buffer->lock(): 0 );
		const Byte* positionBuffer = (Byte*)(m_geometry->position.buffer? m_geometry->position.buffer->lock(): 0 );

		Bool success = positionBuffer && normalBuffer;
		if ( success )	
		{
			UInt16 normalStride = m_geometry->normal.stride;
			UInt16 positionStride = m_geometry->position.stride;
			const gfx::Geometry::IndiceArray& indiceSets = m_geometry->indiceSets;

			for ( gfx::Geometry::IndiceArray::const_iterator iIndices = indiceSets.begin(); iIndices != indiceSets.end(); ++iIndices )
			{
				//Lock the buffer for reading
				const Byte* indicesBuffer = (Byte*)iIndices->buffer->lock();
				const Byte* indicesBufferEnd = indicesBuffer + iIndices->buffer->getSize();

				UInt16 inormalOffset = iIndices->normalOffset;
				UInt16 ipositionOffset = iIndices->positionOffset;
				UInt16 istride = iIndices->stride;

				for (; indicesBuffer != indicesBufferEnd; indicesBuffer += istride*3 )
				{	
					//Get the position indices
					UInt16 iposition[3] = { 
						*(UInt16*)(indicesBuffer + ipositionOffset),
						*(UInt16*)(indicesBuffer + istride + ipositionOffset),
						*(UInt16*)(indicesBuffer + (istride*2) + ipositionOffset) };

					//Get the vertice positions
					const math::Vector3* position[3] = { (math::Vector3*)(positionBuffer + (positionStride * iposition[0])),
						(math::Vector3*)(positionBuffer + (positionStride * iposition[1])),
						(math::Vector3*)(positionBuffer + (positionStride * iposition[2])) };

					//Perform the query to check if the triangle passes the check
					if ( ((GeometryQueryType*)m_query)->runQuery( *position[0], *position[1], *position[2] ) )
					{
						FaceElement face;
						//face.sourceIndex = .... //TODO:

						//Check and lookup the positions for the face vertices making sure we don't
						// duplicate existing buffered entities
						for ( Index i =0; i < 3; ++i )
						{
							SourceToQueryMap::const_iterator iFind = ipositionsAdded.find( iposition[i] );
							if ( iFind == ipositionsAdded.end() ) //If the position is not yet added we need to add it!
							{
								Vector3Element element = { *position[i], iposition[i] }; ///Create the new position element
								face.iposition[i] = (UInt16)m_positions.size(); //Update the index for the face
								ipositionsAdded[ element.isource ] = face.iposition[i]; //Store the lookup int eh map
								m_positions.push_back( element ); ///Add the element
							}
							else //The position is already in the query buffer so just store the new index for the face
								face.iposition[i] = iFind->second;
						}

						//Get the normal indices
						UInt16 inormal[3] = { 
							*(UInt16*)(indicesBuffer + inormalOffset),
							*(UInt16*)(indicesBuffer + istride + inormalOffset),
							*(UInt16*)(indicesBuffer + (istride*2) + inormalOffset) };

						//Check and lookup the normals for the face vertices making sure we don't
						// duplicate existing buffered entities
						for ( Index i =0; i < 3; ++i )
						{
							SourceToQueryMap::const_iterator iFind = inormalsAdded.find( inormal[i] );
							if ( iFind == inormalsAdded.end() ) //If the normal is not yet added we need to add it!
							{
								Vector3Element element = { 
									*(math::Vector3*)(normalBuffer + (normalStride * inormal[i])),
									inormal[i] }; ///Create the new normal element
								face.inormal[i] = (UInt16)m_normals.size(); //Update the index for the face
								inormalsAdded[ element.isource ] = face.inormal[i]; //Store the lookup int eh map
								m_normals.push_back( element ); ///Add the element
							}
							else //The normal is already in the query buffer so just store the new index for the face
								face.inormal[i] = iFind->second;
						}

						//Store the face in the query buffer
						m_faces.push_back( face );						
					}
				}

				//Release the buffer
				iIndices->buffer->release();				
			}
			
			//Release the buffer
			if ( positionBuffer )
				m_geometry->position.buffer->release();
			if ( normalBuffer )
				m_geometry->normal.buffer->release();
		}
		else
		{
			assert( false );
		}

		return success;
	}
	
	Bool GeometryQuery::closeQuery()
	{
		bool ret  = putGeometry();
		
		m_positions.clear();
		m_normals.clear();
		m_faces.clear();

		return ret;
	}

	Bool GeometryQuery::putGeometry()
	{
		if ( !m_geometry->position.buffer )
		{
			logError( "Geometry object has no position buffer" );
			return false;
		}

		if ( !m_geometry->normal.buffer )
		{
			logError( "Geometry object has no normal buffer" );
			return false;
		}

		//Lock the buffers for writing (ReadWrite)
		Byte* normalBuffer = (Byte*)m_geometry->normal.buffer->lock( gfx::Buffer::LockFlag_ReadWrite );
		Byte* positionBuffer = (Byte*)m_geometry->position.buffer->lock( gfx::Buffer::LockFlag_ReadWrite );


		Bool success = positionBuffer && normalBuffer;
		if ( success )
		{	
			//Write the new normals into the render buffers
			UInt16 normalStride = m_geometry->normal.stride;
			for ( Vector3ElementArray::const_iterator iNormal = m_normals.begin(); 
				iNormal != m_normals.end(); ++iNormal )
			{
				*(math::Vector3*)(normalBuffer + (normalStride * iNormal->isource)) = iNormal->v;
			}

			//Write the new positions into the render buffers
			UInt16 positionStride = m_geometry->position.stride;
			for ( Vector3ElementArray::const_iterator iPosition = m_positions.begin(); 
				iPosition != m_positions.end(); ++iPosition )
			{
				*(math::Vector3*)(positionBuffer + (positionStride * iPosition->isource)) = iPosition->v;
			}


		}
		else
		{
			logError( "Faield to lock geometry buffers for writing" );
		}

		//Release the buffer
		if ( positionBuffer )
			m_geometry->position.buffer->release();
		if ( normalBuffer )
			m_geometry->normal.buffer->release();

		return success;
	}

} //END: gfx