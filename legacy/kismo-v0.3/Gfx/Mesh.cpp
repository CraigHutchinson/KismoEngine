/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Mesh.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Mesh.h"
#include "platform/platform.h"
#include "Framework/Log.h"
#include "gfx/MeshTranslator.h"
#include "gfx/MeshManager.h"
#include "gfx/Renderer.h"

namespace gfx
{
	Mesh::Mesh( frx::ResourceManager* owner, const String& name, MeshTranslator* loader, const String& filename ) :
		frx::Resource( owner, name, loader, filename )
	{
		//setName( filename.subString( filename.findFromEnd( _T('/') ), filename.findFromEnd( _T('.') ) ) );
	}

	Mesh::~Mesh()
	{
	}

	Bool Mesh::load()
	{
		if ( isLoaded() )
			return true;

		if ( frx::isValidPointer( m_loader ) )
		{
			assert( m_loader->isDerived( MeshTranslator::TYPE ) );
			InFStream istream( m_origin.c_str() );
			return m_isLoaded = m_loader->reader( this, istream );
		}		
		else
		{
			logError( "No meshTranslator was specified for loading '" << m_origin.c_str() << "'" );
			return false;
		}
	}

	void Mesh::unload()
	{
		//TODO: WHY CAN@T I DELTE THE BUFFER!
		for ( Mesh::const_buffer_iterator iBuff = bufferBegin(); iBuff != bufferEnd(); ++iBuff )
		{
//			delete( *iBuff );
		}

		m_buffers.clear();

		m_isLoaded = false;
	}

	IMPLEMENT_RTTI( _T("gfx"), MeshNode, Geometry );

	MeshNode::MeshNode( const String& name ) :
		Geometry( name )
	{
	}

	MeshNode::~MeshNode()
	{
	}
	
	Bool MeshNode::loadMesh()
	{
		normal.buffer = uv.buffer = position.buffer= 0;

		if ( m_mesh.isNull() )
		{
			logRecoverable( _T("No mesh has been assigned to meshNode '") << getName() << _T("'. load Failed!") );
			return false;
		}

		if ( !m_mesh->load() )
		{
			logRecoverable( _T("Mesh failed ot load for meshNode '") << getName() << _T("'. load Failed!") );
			return false;
		}

		//Bind all the buffers
		for ( Mesh::const_buffer_iterator iBuff = m_mesh->bufferBegin(); iBuff != m_mesh->bufferEnd(); ++iBuff )
		{
			switch ( (*iBuff)->getPurpose() )
			{
			case gfx::Buffer::Purpose_Vertex:
				{
					gfx::BufferDefinition::ElementIterator iEnd = (*iBuff)->getDefinition().elementEnd();
					for ( gfx::BufferDefinition::ElementIterator iElement = (*iBuff)->getDefinition().elementBegin(); iElement != iEnd; ++iElement )
					{
						switch ( iElement->getElement() )
						{
							case gfx::BufferElement::Element_Position:
							{
								position.buffer = (*iBuff);
								position.stride = (*iBuff)->getDefinition().getSize();
								//position.buffer = (*iBuff)->tempGetBuff();
								//position.end = position.buffer + (*iBuff)->getSize();
								break;
							}
							case gfx::BufferElement::Element_Normal:
							{
								normal.buffer = (*iBuff);
								normal.stride = (*iBuff)->getDefinition().getSize();
								//normal.buffer = (*iBuff)->tempGetBuff();
								//normal.end = normal.buffer + (*iBuff)->getSize();
								break;
							}
							case gfx::BufferElement::Element_TextureCoordinate:
							{
								uv.buffer = (*iBuff);
								uv.stride = (*iBuff)->getDefinition().getSize();
								//uv.buffer = (*iBuff)->tempGetBuff();
								//uv.end = uv.buffer + (*iBuff)->getSize();
								break;
							}
						}
					}
					break;
				}
			case gfx::Buffer::Purpose_Index:
				{		
					IndexElement indices;
					indices.buffer = (*iBuff);
					//indices.buffer = (*iBuff)->tempGetBuff(); 
					indices.stride = (*iBuff)->getDefinition().getSize(); 
					//indices.end = indices.buffer + (*iBuff)->getSize();

					gfx::BufferDefinition::ElementIterator iEnd = (*iBuff)->getDefinition().elementEnd();
					for ( gfx::BufferDefinition::ElementIterator iElement = (*iBuff)->getDefinition().elementBegin(); iElement != iEnd; ++iElement )
					{
						switch ( iElement->getElement() )
						{
							case gfx::BufferElement::Element_Position:
								indices.positionOffset = iElement->getOffset(); 
								break;
							case gfx::BufferElement::Element_Normal:
								indices.normalOffset = iElement->getOffset(); 
								break;
							case gfx::BufferElement::Element_TextureCoordinate:
								indices.uvOffset = iElement->getOffset(); 
								break;
						}
					}

					if ( m_materials.size() && m_materials.size() >= indiceSets.size() )
					{
						indices.material = m_materials[ indiceSets.size() ];		
					}
					else
					{
						logWarning( "No material for faceSet in mesh '" << getName() << "'" );
						indices.material = 0;
					}	
					
					indiceSets.push_back( indices );
					break;
				}
			}
		}
		
		if ( !position.buffer )
			return false;

		return true;
	}

	void MeshNode::doDraw( Renderer& renderer ) const
	{
		if ( m_mesh.isValid() )
			renderer.draw( this );

	}

} //END: gfx