/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaShape.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYS_AGEIA_SHAPE_H
#define	PHYS_AGEIA_SHAPE_H

#include "phys/Shape.h"

//forward declaration
class NxShapeDesc;
class NxScene;

namespace phys
{
	/** Ageia Actor
	*/
	class AgeiaShape : public Shape
	{		
	public:

		AgeiaShape( const String& name, const Scene::Ptr& owner );

		virtual ~AgeiaShape();

		/** Get the Ageia PhysX body description
		*/
		NxShapeDesc* getNxShape()
		{ return m_shapeDescription; }
			
		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene() const;

	protected:
		
		/** Virtual function for implementation of shape to build its shape structure when the shape type
			is specified before the attributes are loaded
		*/
		virtual void setShapeImpl();

		/** Sets the size of the shape by calling the derived implementation
			\remarks Sphere - s1 = radius
			\remarks Plane - s1 = width, s2 = length. Setting to 0 creates infinite plane
			\remarks Box - s1 = x-length, s2 = y-length, s3 = z-length
		*/
		virtual bool setSize( Real s1 = 0, Real s2 = 0, Real s3 = 0 );
		
	private:
		
		NxShapeDesc* m_shapeDescription; ///< The description of the physics shape
	};

} // END: phys

#endif