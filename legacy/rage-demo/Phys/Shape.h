/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Shape.h
 * Status: superseded by kismo-v0.3/Phys/Shape.h (same name, content differs)
 */
#ifndef PHYS_PHYSICSSHAPE_H
#define	PHYS_PHYSICSSHAPE_H

#include "Framework/Named.h"
#include "Framework/Rtti.h"
#include "Gfx/Spatial.h"
#include "Phys/Scene.h"

namespace phys
{
	//Forward declaration
	class Scene;
	class Actor;

	/** \brief Physics base class
	\remarks 
		Base physics interface which can be specialised in a plugin to use a specific engine
	*/
	class _DllExport Shape : public gfx::Spatial
	{		
		DECLARE_RTTI;	
	public:
		
		/** \brief Smart pointer type keeps track of and auto destroys the shape when not in use
		*/
		typedef frx::SmartPointer< Shape > Ptr;

		/** \brief Cector style set class
		*/
		typedef frx::SmallSet< Ptr > PtrLookup;

		/** \brief Enumerator for the type of shape this shape defines
		*/
		enum eShape
		{
			Shape_Invalid, ///< No shape is set and is therefore invalid!
			Shape_Plane, ///< Flat plane. If 'size' is 0,0 then it is classed as infinite
			Shape_Sphere, ///< Sphere defined by a 'radius'
			Shape_Box, ///< A box/cube shape defined by a 'size' with x,y,z

		};

	protected:

		Shape( const String& name, const Scene::Ptr& owner );
		virtual ~Shape();

	public:

		/** \brief Get the physics scene the actor belongs to
		*/
		Scene::Ptr getScene() const
		{ return m_owner; }	

		Physics* getPhysics() const
		{ return m_owner->getPhysics(); }	

		/** \brief set the Actor that this body belongs to
		*/
		virtual void setActor( Actor* actor );

		/** \brief Get the actor the body belongs to if any
		*/
		Actor* getActor() const
		{ return m_actor; }

		/** \brief Set the type of shjape that is defined
		*/
		void setShape( eShape shapeType );

		/** \brief Get the type of shape that has been defined by this shape
		*/
		eShape getShapeType() const
		{ return m_shapeType; }

		/** \brief Set the attributes for the shape node
		*/
		virtual bool setAttributes( const AttributeMap& attributes );

		/** \brief Sets the radius of a sphere shape
		*/
		void setRadius( Real radius )
		{ assert( getShapeType() == Shape_Sphere ); _setSize( radius ); }

		/** \brief Sets the size of a box using a vector3
		*/
		void setBoxSize( const frx::Vector3r& size )
		{ assert( getShapeType() == Shape_Box ); _setSize( size.x, size.y, size.z ); }

		/** \brief Sets the size of a plane using a Vector2
			\remarks Setting ther size to 0,0 creates an infinite plane
		*/
		void setPlaneSize( const frx::Vector2r& size )
		{ assert( getShapeType() == Shape_Plane ); _setSize( size.x, size.y ); }

		virtual void doDraw( gfx::Renderer& renderer ) const;

	protected:
				
		virtual void updateWorldBound() {}
		virtual void updateState( RenderStateStack& states, LightStack& lights );
	
		/** \brief Shape specific implementation of setAtrtibutes
		*/		
		bool setPlaneAttributes( const AttributeMap& attributes );
		bool setBoxAttributes( const AttributeMap& attributes );
		bool setSphereAttributes( const AttributeMap& attributes );

		/** \brief Virtual function for implementation of shape to build its shape structure when the shape type
			is specified before the attributes are loaded
		*/
		virtual void setShapeImpl() = 0;

		bool _setSize( Real s1 = 0, Real s2 = 0, Real s3 = 0 )
		{
			m_s1 = s1; m_s2 = s2; m_s3 = s3;
			return setSize( s1, s2, s3 );
		}

		/** \brief Sets the size of the shape by calling the derived implementation
			\remarks Sphere - s1 = radius
			\remarks Plane - s1 = width, s2 = length. Setting to 0 creates infinite plane
			\remarks Box - s1 = x-length, s2 = y-length, s3 = z-length
		*/
		virtual bool setSize( Real s1 = 0, Real s2 = 0, Real s3 = 0 ) = 0;

		/** \brief Draw the shape
		*/
		void doDrawShape( gfx::Renderer& renderer ) const;

	private:
		Real m_s1, m_s2, m_s3; ///< Size values which vary depending on the shape type as defined in setSize()
		frx::SmartPointer< Scene > m_owner; //< The scene the actor belongs to
		Actor* m_actor; ///< The actor the body belongs to if the body is in use
		eShape m_shapeType; ///< The type of shape defined
	};

} // END: phys

#endif