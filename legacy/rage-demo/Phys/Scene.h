/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Scene.h
 * Status: superseded by kismo-v0.3/Gfx/Scene.h (same name, content differs)
 */
#ifndef PHYS_SCENE_H
#define	PHYS_SCENE_H

#include "Framework/Named.h"
#include "Framework/Vector.h"
#include "Framework/Rtti.h"
#include "Gfx/Spatial.h"
#include "Gfx/Mesh.h"

namespace phys
{
	//Forward declaration
	class Physics;
	class Actor;
	class Body;
	class Shape;
	class MeshShape;
	class Constraint;
	class Material;

	/** \brief 
	*/
	class _DllExport Scene : public gfx::Spatial
	{		
		DECLARE_RTTI;	
	public:
		
		typedef frx::SmartPointer< Scene > Ptr;
		
		typedef Map< String, Actor* > ActorMap;
		typedef Map< String, Constraint* > ConstraintMap;

		typedef ActorMap::const_iterator ActorIterator;
		typedef ConstraintMap::const_iterator ConstraintIterator;

	protected:

		/** \brief Create a new scene from the specified Physics manager
			\remarks Protected so can only be called by derived implementation
		*/
		Scene( const String& name, Physics* owner );

		virtual ~Scene();

	public:		

		/** \brief Set the attributes for the spatial node
		*/
		virtual bool setAttributes( const AttributeMap& attributes ) 
		{ 			
			AttributeMap::const_iterator iFind;
					
			if ( (iFind = attributes.find( _T("gravity") )) != attributes.end() )
				setGravity( frx::Vector3r( iFind->second ) );

			return Spatial::setAttributes( attributes ); 
		}

		/** \brief Create a new dynamic physics body
		*/
		Body* createBody( const String& name );

		/** \brief Create a new static physics actor
		*/
		Actor* createActor( const String& name );

		/** \brief Create a new physics shape
		*/
		Shape* createShape( const String& name );

		/** \brief Create a new physics shape
		*/
		MeshShape* createMeshShape( const String& name );

		/** \brief Create a new physics Constraint
		*/
		Constraint* createConstraint( const String& name );

		/** \brief Release a body 
		*/		
		void releaseBody( Body* body );

		/** \brief Release a Actor
		*/		
		void releaseActor( Actor* actor );

		/** \brief Release a body shape
		*/		
		void releaseShape( Shape* shape );

		/** \brief Initialise and create the scene
			\remarks Base implementation must be called for contact reports
		*/
		virtual Bool initialise();

		/** \brief Destroy the scene
		*/
		virtual void destroy() = 0;

		/** \brief Advance the simulation by a delta time step
		*/
		virtual void updateSimulation( Real tDelta, Real tApp ) = 0;

		/** \brief Set the value of gravity constant in metres per second
		*/
		virtual void setGravity( const frx::Vector3r& gravity ) = 0;

		/** \brief Get the gravitational constant in metres per second
		*/
		virtual frx::Vector3r getGravity() const = 0;

		/** \brief Get the physics system the scene belongs to
		*/
		Physics* getPhysics() 
		{ return m_owner; }


		/** \brief Get the default material for the scene
		*/
		Material* getDefaultMaterial() const
		{ return m_defaultMaterial; } 

		/** \brief Gets a reference to the mesh the scene is using to mark contacts
		*/
		const gfx::MeshNode::Ptr& getContactMarker() const
		{ return m_contactMarker; }

		/** \brief Actor iterators
		*/
		ActorIterator actorBegin()
		{ return m_actors.begin(); }

		/** \brief Actor iterators
		*/
		ActorIterator actorEnd()
		{ return m_actors.end(); }

		/** \brief Constraint iterators
		*/
		ConstraintIterator constraintBegin()
		{ return m_constraints.begin(); }

		/** \brief Constraint iterators
		*/
		ConstraintIterator constraintEnd()
		{ return m_constraints.end(); }

	protected:

		/** \brief Create a new dynamic physics body Implementation
		*/
		virtual Body* createBodyImpl( const String& name ) = 0;

		/** \brief Create a new static physics actor Implementation
		*/
		virtual Actor* createActorImpl( const String& name ) = 0;

		/** \brief Create a new physics shape Implementation
		*/
		virtual Shape* createShapeImpl( const String& name ) = 0;

		/** \brief Create a new physics smesh hape Implementation
		*/
		virtual MeshShape* createMeshShapeImpl( const String& name ) = 0;

		/** \brief Create a new physics constraint Implementation
		*/
		virtual Constraint* createConstraintImpl( const String& name ) = 0;

		/** \brief Release/destroy the instances...
		*/		
		virtual void releaseBodyImpl( Body* body ) = 0;
		virtual void releaseActorImpl( Actor* actor ) = 0;
		virtual void releaseShapeImpl( Shape* shape ) = 0;

		/** \brief Should be called after the scene is initialised so that all the actors are initialised
		*/
		//bool initialiseActors()
				
		/** \brief Draw function used to draw the collision contacts for the scene
		*/
		virtual void doDraw( gfx::Renderer& renderer ) const;	
		virtual void updateWorldBound() {}
		virtual void updateState( RenderStateStack& states, LightStack& lights ) {} 

	protected:

		Material* m_defaultMaterial; ////< the default material
		
	private:

		Physics* m_owner; ///< The physics system that owns the scene
		
		ActorMap m_actors; ///< Map of all the actors in the scene
		ConstraintMap m_constraints; ///< Map of all the constraints in the scene

		gfx::MeshNode::Ptr m_contactMarker; ///< Pointer to a mesh used to draw contacts
	};

} // END: phys

#endif