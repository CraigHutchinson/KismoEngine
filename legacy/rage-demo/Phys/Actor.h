/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Actor.h
 * Status: superseded by kismo-v0.3/Phys/Actor.h (same name, content differs)
 */
#ifndef PHYS_ACTOR_H
#define	PHYS_ACTOR_H

#include "Framework/Named.h"
#include "Framework/Rtti.h"
#include "Framework/Vector.h"
#include "Gfx/Node.h"
#include "Phys/Shape.h"
#include "Phys/Material.h"
#include "Gfx/RenderModeState.h"

namespace phys
{
	//Forward declaration
	class Scene;
	class Body;
	class Shape;

	/** \brief Physics base class
	\remarks 
		Base physics interface which can be specialised in a plugin to use a specific engine
	*/
	class _DllExport Actor : public gfx::Node
	{		
		DECLARE_RTTI;

	public:

		typedef Shape::PtrLookup::const_iterator const_shape_iterator;

	protected:
		Actor( const String& name, const Scene::Ptr& owner );

		virtual ~Actor();

	public:

		/** \brief Get the physics scene the actor belongs to
		*/
		Scene::Ptr getScene() const
		{ return m_owner; }		

		/** \brief Initialise and create the scene
		*/
		virtual Bool initialise() = 0;
		
		/** \brief Destroy the scene
		*/
		virtual void destroy() = 0;

		/** \brief Apply a force to the actor in world coordinates
		*/
		virtual void addForceAtPos( const frx::Vector3r& worldForce, const frx::Vector3r& worldPos ) = 0;

		/** \brief Apply a force to the actor whose direction is in world coordinates but the position is local
		*/
		virtual void addForceAtLocalPos( const frx::Vector3r& worldForce, const frx::Vector3r& localPos ) = 0;

		/** \brief Apply a force to the actor whose direction is in local coordinates but positio is in the world space
		*/
		virtual void addLocalForceAtPos( const frx::Vector3r& localForce, const frx::Vector3r& worldPos ) = 0;

		/** \brief Apply a force to the actor in local coordinates at a local position
		*/
		virtual void addLocalForceAtLocalPos( const frx::Vector3r& localForce, const frx::Vector3r& localPos ) = 0;

		/** \brief Apply a force to the actor in world coordinates through the center of the actor
		*/
		virtual void addForce( const frx::Vector3r& worldForce ) = 0;

		/** \brief Apply a force to the actor in local coordinates through the center of the actor
		*/
		virtual void addLocalForce( const frx::Vector3r& localForce ) = 0;

		/** \brief Apply a force to the actor in world coordinates
		*/
		virtual void addTorque( const frx::Vector3r& worldTorque ) = 0;

		/** \brief Apply a force to the actor in local coordinates
		*/
		virtual void addLocalTorque( const frx::Vector3r& localTorque ) = 0;

		/** \brief Uses the current world position to set the pose of the dynamic actor
			//TODO: remove this?
		*/
		virtual void setCurrentPose() = 0;

		
		//virtual void setCollisionLayerFriction( Int32 layer ) = 0;

		/** \brief set the body that belongs to this actor
		*/
		virtual void setBody( Body* body );
		/** \brief Get the body that belongs to this actor
			\remarks If there is no body the actor is passive/non-dynamic
		*/
		Body* getBody() const
		{ return m_body; }

		/** \brief Add a new physics shape to the actor
			\remarks Any number of shapes can be applied to an actor
		*/
		virtual void addShape( Shape* shape );

		/** \brief Checks whether a specified shape already exists within the actor
		*/
		Bool hasShape( Shape* shape );
		
		/** \brief Checks the actors children for body deifnition to use with this actor
			\return True if a body is found and is therefore active and false if no body is found
		*/
		bool getChildBody();

		/** \brief Checks the actors children for shape deifnition to use with this actor
			\param[in] node Pointer to the node that will be searched for shapes. By default 0 wil point to this actor
			\return Whether the actor has shapes when the function finishes searching
		*/
		bool getChildShapes(  gfx::Spatial* node = 0 );

		/** \brief Get the number of shapes the actor has
		*/
		Count getShapeCount() const
		{ return m_shapes.size(); }

		/** \brief Shape iterator
		*/
		const_shape_iterator shapeBegin()
		{ return m_shapes.begin(); }

		/** \brief Shape iterator
		*/
		const_shape_iterator shapeEnd()
		{ return m_shapes.end(); }

		/** \brief Set the attributes for the spatial node
		*/
		virtual bool setAttributes( const AttributeMap& attributes );

		virtual frx::Vector3r getAngularVelocity() const = 0;

		/** \brief Set the angular velocity of the actor
		*/
		virtual void setAngularVelocity( const frx::Vector3r& angVel ) = 0;
		
		/** \brief get the linear velocity of the actor
		*/
		virtual frx::Vector3r getLinearVelocity() const = 0;

		/** \brief Set the linear velocity of the actor
		*/
		virtual void setLinearVelocity( const frx::Vector3r& linVel ) = 0;

		/** \brief Set the iterator count.
			\remarks Higher values increase accuracy but have a performance hit
			\param[in] count Value between 1 and 255
		*/
		virtual void setIterationCount( Count count ) = 0;

		/** \brief Get the material for the actor
		*/
		Material* getMaterial() const
		{ return m_material; }

		/** \brief Set the material the actor uses
			\remarks Uses reference counter for auto destruction
		*/
		void setMaterial( Material* material )
		{ m_material = material; }

		/** \brief Clears the contacts buffer
		*/
		void clearContacts();
		
		/** \brief Add a contact identifier which will display in the render if enabled
			\param[in] position The world position of the contact
			\param[in] normal The normal for the contact
		*/
		void addContact( const frx::Vector3r& position, const frx::Vector3r& normal );

		/** \brief gets whether the actor is awake
			\remarks A sleep state is entered to reduce dynamics computation
		*/
		Bool isAwake() const
		{ return m_isAwake; }

		/** \brief Sets the wake/sleep flag
			\remarks internal use only
			*/
		void _setIsAwake( Bool awake )
		{ m_isAwake = awake; }

	protected:		

		virtual void updateWorldData( Real tDelta, Real tApp );
		virtual void doDraw( gfx::Renderer& renderer ) const;		
		//virtual void updateWorldBound() {}
		//virtual void updateState( RenderStateStack& states, LightStack& lights ) {} 


	protected:

		Bool m_firstUpdate; //< Whether this is the first update which wil store the inital position

	private:
		frx::SmartPointer< Scene > m_owner; //< The scene the actor belongs to
		Body* m_body; //< The dynamics body if there is one
		Shape::PtrLookup m_shapes; ///< The shapes attached to the actor
		Material::Ptr m_material; ////< For now materials are set per actor TODO: Set per shape
		
		Bool m_isAwake; //< Stores whether the actor is awake or asleep

		gfx::WireframeState* m_wireframeState; ///< render state sued for drawing child shapes

		/** \brief Contact information used for drawing a contact
		*/
		struct Contact
		{
			frx::Vector3r position; ///< The position of the contact
			frx::Vector3r normal; ///< The normal of the contact
		};
		typedef Vector< Contact > ContactArray;
		mutable ContactArray m_contacts; ///< The contacts that have occured during the update
	};

} // END: phys

#endif