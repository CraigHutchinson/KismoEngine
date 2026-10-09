/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaMeshShape.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYS_AGEIA_MESHSHAPE_H
#define	PHYS_AGEIA_MESHSHAPE_H

#include "AgeiaShape.h"
#include "phys/MeshShape.h"

//forward delcarations
class NxConvexMeshDesc;
class NxTriangleMeshDesc;
class NxShapeDesc;

namespace phys
{
	/** Ageia Actor
	*/
	class AgeiaMeshShape : public MeshShape
	{		
	public:

		AgeiaMeshShape( const String& name, const Scene::Ptr& owner );

		virtual ~AgeiaMeshShape();
		
		/** Initialise the mesh shape
		*/
		bool intitialise();

		/** Gets whetther the mesh is convex or a triangle mesh
		*/
		Bool isConvex() const
		{ return m_convex; }

		/** Get the Ageia PhysX mesh description
		*/
		const NxConvexMeshDesc* getNxConvexMeshDescription() const
		{ return m_meshConvexDescription; }
			
		/** Get the Ageia PhysX mesh description
		*/
		const NxTriangleMeshDesc* getNxTriangleMeshDescription() const
		{ return m_meshTriangleDescription; }

		/** Get the Ageia PhysX shape description
		*/
		NxShapeDesc* getNxShape()
		{ return (NxShapeDesc*)m_shapeDescription; }

		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene() const;
		
		/** Initialise and create the scene
		*/
		virtual Bool initialise();
		
		/** Destroy the scene
		*/
		virtual void destroy();

	protected:
		
		/** Virtual function called whent eh mesh is set in the base impl
		*/
		virtual Bool meshSetImpl();

	private:		
		bool m_convex; //< Whether the mesh is a convex or triangle mesh
		union
		{
			NxConvexMeshDesc* m_meshConvexDescription; //< The mesh shape description
			NxTriangleMeshDesc* m_meshTriangleDescription; //< The mesh shape description
		};
		NxShapeDesc* m_shapeDescription; //< The mesh shape description which we actually use to create the collision hull
	};

} // END: phys

#endif