/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Mesh.h
 * Status: duplicate of kismo-v0.3/Gfx/Mesh.h
 */
#ifndef GFX_MESH_H
#define	GFX_MESH_H

#include "Framework/Named.h"
#include "Framework/Resource.h"
#include "Gfx/Geometry.h"
#include "Gfx/Buffer.h"
#include "Gfx/MaterialState.h"

namespace gfx
{
	//Forward declaration
	class MeshTranslator;

	/** \brief Mesh geometry type
	*/
	class _DllExport Mesh : public frx::Resource
	{
		friend MeshTranslator;

	public:
		typedef frx::SmartPointer< Mesh > Ptr;
		typedef Buffer::PtrArray::const_iterator const_buffer_iterator;
	public:

		/** \brief Mesh constructor
			\param[in] name Unique name given to the resource
			\param[in] filename The full path for the resource
			\param[in] loader Pointer to the meshTranslator used to load the mesh
		*/
		Mesh( frx::ResourceManager* owner, const String& name, MeshTranslator* loader, const String& filename = String() );

		/** \brief Virtual destructor
		*/
		virtual ~Mesh();

		/** \brief \copydoc frx::Resource::load()
		*/
		virtual bool load();

		/** \brief \copydoc frx::Resource::destroy()
		*/
		virtual void unload();

		/** \brief gets whether the mesh has a valid vertex buffer
		*/
		Bool isValidBuffer( Index index ) const
		{ return (index < m_buffers.size()? m_buffers[ index ].isValid(): false ); }

		/** \brief get a reference to the vertex buffer for this mesh
		*/
		const Buffer* getBuffer( Index index ) const
		{ return ( index < m_buffers.size()? m_buffers[ index ] : 0); }
		Buffer* getBuffer( Index index )
		{ return ( index < m_buffers.size()? m_buffers[ index ] : 0); }
	
		/** \brief Sets the buffers for the mesh
			\remarks Used by tranalators when loading the mesh
		*/
		void setBuffers( Buffer::PtrArray& buffers )
		{ m_buffers = buffers; }

		/** \brief Helper iterators for the renderer to get access to the buffers
		*/
		const_buffer_iterator bufferBegin() const
		{ return m_buffers.begin(); }
		
		const_buffer_iterator bufferEnd() const
		{ return m_buffers.end(); }

		/** \brief Sets the materials for the mesh
			\remarks Used by tranalators when loading the mesh
		*/
		void setMaterials( String::Array& materials )
		{ m_materials = materials; }

		/** \brief Gets a const reference to the default materials for this object
		*/
		const String::Array& getMaterials() const
		{ return m_materials; }

	protected:

		Buffer::PtrArray m_buffers; ///< The mesh geometry and index buffers
		String::Array m_materials; ///< The mesh materials names that are used by default

	};

	/** \brief Mesh geometry scene node
	\remarks
		Derives from Spatial as a mesh cannot directly contain a child
	*/
	class _DllExport MeshNode : public Geometry
	{
		DECLARE_RTTI;

	public:

		typedef frx::SmartPointer< MeshNode > Ptr;
		typedef Vector< Ptr > PtrArray;
		typedef Stack< Ptr > PtrStack;

	public:

		MeshNode( const String& name );
		virtual ~MeshNode();

		/** \brief Set the mesh that the node contains
			\remarks This will automatically call loadmesh()
		*/
		Bool setMesh( const Mesh::Ptr& mesh )
		{
			m_mesh = mesh;
			return loadMesh();
		}

		/** \brief Gets a pointer to the mesh
		*/
		const Mesh::Ptr& getMesh() const
		{ return m_mesh; }

		/** \brief Sets the materials for the mesh
			\remarks Used by tranalators when loading the mesh
		*/
		void setMaterials( MaterialState::PtrArray& materials )
		{ m_materials = materials; }

	protected:		
		
		/** \brief Loads the mesh that is assigned to the node
		*/
		Bool loadMesh();

		/** \brief Performs the actual drawing of this node
		*/
		virtual void doDraw( Renderer& renderer ) const;


	protected:
		Mesh::Ptr m_mesh; ///< The mesh
		MaterialState::PtrArray m_materials; ///< The materials for this instance
		
	};

} // END: gfx


#endif