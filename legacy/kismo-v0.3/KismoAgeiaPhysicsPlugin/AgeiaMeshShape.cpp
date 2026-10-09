/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaMeshShape.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaMeshShape.h"
#include "AgeiaScene.h"
#include "AgeiaPhysics.h"
#include <NxConvexMeshDesc.h>
#include <NxConvexShapeDesc.h>
#include <NxTriangleMeshDesc.h>
#include <NxTriangleMeshShapeDesc.h>
#include <NxStream.h>
#include <NxCooking.h>
#include "Gfx/Geometry.h"
#include "Framework/Log.h"
#include "phys/Actor.h"

namespace phys
{
	AgeiaMeshShape::AgeiaMeshShape( const String& name, const Scene::Ptr& owner ) :
		MeshShape( name, owner ),
		m_meshConvexDescription( 0 ),
		m_shapeDescription( 0 ),
		m_convex( true )
	{
	}

	AgeiaMeshShape::~AgeiaMeshShape() 
	{
		destroy();
	}

	NxScene* AgeiaMeshShape::getNxScene() const
	{ return ((AgeiaScene*)getScene())->getNxScene(); }

	
	Bool AgeiaMeshShape::initialise()
	{
		assert( getActor() );
		m_convex = getActor()->getBody() != 0; //< If the body is active we make a convex mesh

		if ( m_convex )
		{
			m_meshConvexDescription = new NxConvexMeshDesc();		
			m_shapeDescription = new NxConvexShapeDesc();
		}
		else
		{
			m_meshTriangleDescription = new NxTriangleMeshDesc();		
			m_shapeDescription = new NxTriangleMeshShapeDesc();
		}


		gfx::Geometry::VertexElement position;
		gfx::Geometry::IndiceArray indiceSets;

		Count indiceCount = 0;
		Count verticeCount = 0;

		//Get a bind to the buffers for the indices and the position  of the mesh vertices
		for ( gfx::Mesh::const_buffer_iterator iBuff = getMesh()->bufferBegin(); iBuff != getMesh()->bufferEnd(); ++iBuff )
		{
			if ( (*iBuff)->getPurpose() == gfx::Buffer::Purpose_Vertex )
			{
				//Get the vertices of the mesh
				gfx::BufferDefinition::ElementIterator iEnd = (*iBuff)->getDefinition().elementEnd();
				for ( gfx::BufferDefinition::ElementIterator iElement = (*iBuff)->getDefinition().elementBegin(); iElement != iEnd; ++iElement )
				{
					if( iElement->getElement() == gfx::BufferElement::Element_Position )
					{
						position.buffer = (*iBuff);
						position.stride = (*iBuff)->getDefinition().getSize();
						verticeCount = (Count)((*iBuff)->getSize() / position.stride);
						break;
					}
				}
			}
			else
			if ( (*iBuff)->getPurpose() == gfx::Buffer::Purpose_Index )
			{		
				//Get the indices of the mesh
				gfx::Geometry::IndexElement indices;
				indices.buffer = (*iBuff);
				indices.stride = (*iBuff)->getDefinition().getSize(); 				
				indiceCount += (Count)((*iBuff)->getSize() / indices.stride);

				//TODO: ass a findElement function to BufferDefinition
				gfx::BufferDefinition::ElementIterator iEnd = (*iBuff)->getDefinition().elementEnd();
				for ( gfx::BufferDefinition::ElementIterator iElement = (*iBuff)->getDefinition().elementBegin(); iElement != iEnd; ++iElement )
				{
					if( iElement->getElement() ==  gfx::BufferElement::Element_Position )
					{
						indices.positionOffset = iElement->getOffset();			
						break;
					}
				}
				indiceSets.push_back( indices );
			}
		}

		// Chekc if we foudn any vertices
		if ( indiceSets.size() == 0 || indiceCount == 0 )
		{
			logError( "No Indices were found, collision mesh cannot be constructed!" );
			return false;
		}

		//Create the memory for the collision mesh vertices and indices
		NxVec3* vertices = new NxVec3[ verticeCount ];
		NxU16* faces = new NxU16[ indiceCount ];

		//Try and lock the position buffer if it exists			
		const Byte* positionBuffer = (Byte*)(position.buffer? position.buffer->lock() : 0 );
		assert( positionBuffer );
		if ( !positionBuffer || verticeCount == 0 )
		{
			logError( "No position buffer was found while trying to create a collision mesh or the mesh is locked!" );
			return false;
		}

		//Get all the vertice positions from the buffer
		const Byte* positionBufferEnd = positionBuffer + position.buffer->getSize();
		NxVec3* currentVertice = vertices;
		for (; positionBuffer != positionBufferEnd; positionBuffer += position.stride )
		{
			(*currentVertice).x = *(((Float32*)positionBuffer));
			(*currentVertice).y = *(((Float32*)positionBuffer)+1);
			(*currentVertice).z = *(((Float32*)positionBuffer)+2);
			++currentVertice; ///< Get the next indice
		}
		position.buffer->release();

		//get all the triangle indices from the indice sets for each sub mesh
		NxU16* currentIndice = faces;
		for ( gfx::Geometry::IndiceArray::const_iterator iIndices = indiceSets.begin(); iIndices != indiceSets.end(); ++iIndices )
		{
			//Lock the buffer for reading
			const Byte* indicesBuffer = (Byte*)iIndices->buffer->lock();
			assert( indicesBuffer );
			if ( !indicesBuffer ) continue; //TODO: POssibly error checking but we shouldn't get here
			const Byte* indicesBufferEnd = indicesBuffer + iIndices->buffer->getSize();
			
			for (; indicesBuffer != indicesBufferEnd; indicesBuffer += iIndices->stride )
			{
				*currentIndice = (NxU16)*(UInt16*)(indicesBuffer + iIndices->positionOffset);
				++currentIndice; ///< Get the next indice
			}
			iIndices->buffer->release();//Release the buffer
		}


		//Count materialIndices = new NxMaterialIndex[ indiceCount / 3 ];

		// Add the mesh material data
		//m_meshDescription->materialIndexStride = sizeof(NxMaterialIndex);
		//m_meshDescription->materialIndices = materialIndices;

		// Cooking from memory
		MemoryWriteBuffer buf;
		if ( m_convex )
		{
			//Build the mesh to pass to Ageia
			m_meshConvexDescription->numVertices = verticeCount;
			m_meshConvexDescription->numTriangles = indiceCount / 3;
			m_meshConvexDescription->pointStrideBytes = sizeof(NxVec3);
			m_meshConvexDescription->triangleStrideBytes = 3*sizeof(NxU16);
			m_meshConvexDescription->points = vertices;
			m_meshConvexDescription->triangles = faces;							
			m_meshConvexDescription->flags = NX_MF_16_BIT_INDICES;

			
			if ( !NxCookConvexMesh( *m_meshConvexDescription, buf ) )
			{
				logError( "Failed to cook convex mesh shape" );
				return false;
			}

			//TODO: The AgeiaPhysics class could manage and instance this
			((NxConvexShapeDesc*)m_shapeDescription)->meshData =
				((AgeiaPhysics*)getScene()->getPhysics())->getNxSDK()->createConvexMesh( MemoryReadBuffer( buf.data ) );
			
			return (((NxConvexShapeDesc*)m_shapeDescription)->meshData != 0);
		}
		else
		{
			//Build the mesh to pass to Ageia
			m_meshTriangleDescription->numVertices = verticeCount;
			m_meshTriangleDescription->numTriangles = indiceCount / 3;
			m_meshTriangleDescription->pointStrideBytes = sizeof(NxVec3);
			m_meshTriangleDescription->triangleStrideBytes = 3*sizeof(NxU16);
			m_meshTriangleDescription->points = vertices;
			m_meshTriangleDescription->triangles = faces;							
			m_meshTriangleDescription->flags = NX_MF_16_BIT_INDICES;
					
			if ( !NxCookTriangleMesh( *m_meshTriangleDescription, buf ) )
			{
				logError( "Failed to cook convex mesh shape" );
				return false;
			}

			//TODO: The AgeiaPhysics class could manage and instance this
			((NxTriangleMeshShapeDesc*)m_shapeDescription)->meshData =
				((AgeiaPhysics*)getScene()->getPhysics())->getNxSDK()->createTriangleMesh( MemoryReadBuffer( buf.data ) );

			return (((NxTriangleMeshShapeDesc*)m_shapeDescription)->meshData != 0);
		}



		

	}
	
	void AgeiaMeshShape::destroy()
	{
		//Release our memory
		if ( m_convex && m_meshConvexDescription )
		{
			frx::safeDelete( m_meshConvexDescription->points );
			frx::safeDelete( m_meshConvexDescription->triangles );
			delete m_meshConvexDescription;
		}
		else
		if ( !m_convex && m_meshTriangleDescription )
		{
			frx::safeDelete( m_meshTriangleDescription->points );
			frx::safeDelete( m_meshTriangleDescription->triangles );
			delete m_meshTriangleDescription;
		}

		if ( m_shapeDescription )
		{			
			if ( m_convex )
			{
				//Release the Triangle mesh from the physics
				if ( ((NxConvexShapeDesc*)m_shapeDescription)->meshData )
					((AgeiaPhysics*)getScene()->getPhysics())->getNxSDK()->releaseConvexMesh(
						*(((NxConvexShapeDesc*)m_shapeDescription)->meshData) );
			}
			else
			{
				//Release the Triangle mesh from the physics
				if ( ((NxTriangleMeshShapeDesc*)m_shapeDescription)->meshData )
					((AgeiaPhysics*)getScene()->getPhysics())->getNxSDK()->releaseTriangleMesh( 
						*(((NxTriangleMeshShapeDesc*)m_shapeDescription)->meshData) );
			
			}

			delete m_shapeDescription;
		}
	}

	Bool AgeiaMeshShape::meshSetImpl()
	{
		PROFILE;

		if ( !getMesh() )
		{
			return false;
		}
		else
		{
			return true; //< We don't do anythign yet!
		}

	}

} //END: phys