/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\MeshShape.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "MeshShape.h"
#include "Gfx/Renderer.h"
#include "Phys/Physics.h"

namespace phys
{	
	IMPLEMENT_RTTI( phys, MeshShape, Shape );

	MeshShape::MeshShape( const String& name, const Scene::Ptr& owner ) :
		Shape( name, owner ),
		m_meshNode( name + _T("_ShapeMesh") )
	{}

	void MeshShape::setShapeImpl()
	{ assert( false ); }

	bool MeshShape::setSize( Real s1, Real s2, Real s3 )
	{ assert( false ); return false; }

	bool MeshShape::setMesh( const gfx::Mesh::Ptr& mesh )
	{ 
		m_mesh = mesh;
		m_meshNode.setMesh( m_mesh );
		return meshSetImpl();
	}
	
	void MeshShape::updateWorldData( Real tDelta, Real tApp )
	{
		if ( m_meshNode.getMesh().isValid() )
		{
			m_meshNode.worldTransform() = getParent()->worldTransform();
		}
	}

	void MeshShape::doDraw( gfx::Renderer& renderer ) const
	{
		if ( !getPhysics()->getWireframe() )
			return;

		if ( m_meshNode.getMesh().isValid() )
		{
			//Check if we need to set a new render state
			if ( m_renderStates.size() )
			{
				renderer.pushRenderState();

				for ( RenderStateMap::const_iterator iState = m_renderStates.begin();
					iState != m_renderStates.end();
					++iState )
				{
					renderer.setRenderState( iState->second );
				}
				
				renderer.draw( &m_meshNode );

				renderer.popRenderState();
			}
			else
			{				
				renderer.draw( &m_meshNode );
			}
		}		
	}

} //END: phys