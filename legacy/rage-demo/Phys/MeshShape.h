/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\MeshShape.h
 * Status: duplicate of kismo-v0.3/Phys/MeshShape.h
 */
#ifndef PHYS_MESHSHAPE_H
#define	PHYS_MESHSHAPE_H

#include "phys/Shape.h"
#include "gfx/Mesh.h"

namespace phys
{
	/** \brief Mesh shape
		\remarks Takes a mesh and constructs a collision hull from the polygons
	*/
	class _DllExport MeshShape : public Shape
	{		
		DECLARE_RTTI;
	public:
				
		/** \brief Smart pointer type keeps track of and auto destroys the shape when not in use
		*/
		typedef frx::SmartPointer< MeshShape > Ptr;

	protected:

		MeshShape( const String& name, const Scene::Ptr& owner );

		virtual ~MeshShape() {}
		
	public:
		/** \brief Set the mesh that will be used by the shape when the actor it is attached to is initialised
		*/
		bool setMesh( const gfx::Mesh::Ptr& mesh );

		/** \brief Get a pointer to the mesh that this shape is using
		*/
		gfx::Mesh* getMesh() const
		{ return m_mesh; }

		/** \brief Initialise and create the scene
		*/
		virtual Bool initialise() = 0;
		
		/** \brief Destroy the scene
		*/
		virtual void destroy() = 0;

		/** \brief Set the attributes for the shape node
		*/
		virtual bool setAttributes( const AttributeMap& attributes )
		{ return Spatial::setAttributes( attributes ); }
	
	protected:
		
		virtual void updateWorldData( Real tDelta, Real tApp );
		virtual void doDraw( gfx::Renderer& renderer ) const;

		/** \brief Virtual function called whent eh mesh is set in the base impl
		*/
		virtual Bool meshSetImpl() = 0;

		/** \brief These aren't valid for a mesh */
		virtual void setShapeImpl();
		virtual bool setSize( Real s1 = 0, Real s2 = 0, Real s3 = 0 );

	private:
		gfx::Mesh::Ptr m_mesh; //< The mesh object used to create the shape		
		gfx::MeshNode m_meshNode; ///< The mesh node used for drawing the shape if needed
	};

} // END: phys

#endif