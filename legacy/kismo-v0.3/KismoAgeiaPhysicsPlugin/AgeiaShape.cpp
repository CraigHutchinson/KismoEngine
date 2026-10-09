/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaShape.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaShape.h"
#include "AgeiaScene.h"
#include <NxShapeDesc.h>
#include <NxPlaneShapeDesc.h>
#include <NxSphereShapeDesc.h>
#include <NxBoxShapeDesc.h>

namespace phys
{
	AgeiaShape::AgeiaShape( const String& name, const Scene::Ptr& owner ) :
		Shape( name, owner ),
		m_shapeDescription( 0 )
	{
	}

	AgeiaShape::~AgeiaShape() 
	{
		if ( m_shapeDescription )
			delete m_shapeDescription;
	}

	NxScene* AgeiaShape::getNxScene() const
	{ return ((AgeiaScene*)getScene())->getNxScene(); }

	void AgeiaShape::setShapeImpl()
	{
		if (  m_shapeDescription )
			frx::safeDelete( m_shapeDescription );

		switch( getShapeType() )
		{
		case Shape_Plane:
			{				
				m_shapeDescription = new NxPlaneShapeDesc;
				break;
			}
		case Shape_Sphere:
			{
				m_shapeDescription = new NxSphereShapeDesc;
				break;
			}
		case Shape_Box:
			{
				m_shapeDescription = new NxBoxShapeDesc;
				break;
			}
		default:
			{
				assert( false );
				break;
			}
		}
	}
	
	bool AgeiaShape::setSize( Real s1, Real s2, Real s3 )
	{
		assert( m_shapeDescription );

		switch( getShapeType() )
		{
		case Shape_Plane:
			{
				break;
			}
		case Shape_Sphere:
			{
				((NxSphereShapeDesc*)m_shapeDescription)->radius = s1;
				break;
			}
		case Shape_Box:
			{
				((NxBoxShapeDesc*)m_shapeDescription)->dimensions.x = s1;
				((NxBoxShapeDesc*)m_shapeDescription)->dimensions.y = s2;
				((NxBoxShapeDesc*)m_shapeDescription)->dimensions.z = s3;
				break;
			}
		default:
			{
				assert( false );
				return false;
			}
		}
		return true;
	}


} //END; phys