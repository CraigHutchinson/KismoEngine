/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Constraint.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef PHYS_PHYSICSCONSTRAINT_H
#define	PHYS_PHYSICSCONSTRAINT_H

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
	class _DllExport Constraint : public gfx::Spatial
	{		
		DECLARE_RTTI;	
	public:
		
		/** \brief Smart pointer type keeps track of and auto destroys the shape when not in use
		*/
		typedef frx::SmartPointer< Constraint > Ptr;

		/** \brief Vector style set class
		*/
		typedef frx::SmallSet< Ptr > PtrLookup;

		/** \brief Enumerator for the type of shape this shape defines
		*/
		enum eConstraint
		{
			Constraint_Invalid, ///< No shape is set and is therefore invalid!
			Constraint_Hinge, ///< A hinge constraint
			Constraint_Pin, ///< A pin constraint i.e Ball Joint
			//TODO: Allow more types
		};

	protected:

		Constraint( const String& name, const Scene::Ptr& owner );
		virtual ~Constraint();

	public:

		/** \brief Get the physics scene the actor belongs to
		*/
		Scene::Ptr getScene() const
		{ return m_owner; }	
		
		/** \brief Initialise and create the scene
			\remarks Will be called after the first update
		*/
		virtual Bool initialise() = 0;
		
		/** \brief Destroy the scene
		*/
		virtual void destroy() = 0;

		/** \brief Sets the name of the actor which should be connected to the constraint at the specified index
			\remarks A call to lookupActors() will use this name to call setActor()
		*/
		void setActorLookup( const String& name, Index index );

		/** \brief Looks up the actors of the specified names and calls setActor where possbile
		*/
		bool lookupActors();

		/** \brief set the Actor that this body belongs to
		*/
		virtual void setActor( Actor* actor, Index index );

		/** \brief Get the actor the body belongs to if any
		*/
		Actor* getActor( Index index ) const
		{ assert( index < 2 ); return m_actor[ index ]; }

		/** \brief Get the actor name lookup
			\remark The name can be null if the actor is not specified or 
			the name supplied references a non-actor entity
		*/
		const String& getActorLookup( Index index ) const
		{ assert( index < 2 ); return m_actorLookup[ index ]; }

		/** \brief Set the type of shjape that is defined
		*/
		void setConstraint( eConstraint constraintType );

		/** \brief Get the type of constraint that has been defined
		*/
		eConstraint getConstraintType() const
		{ return m_constraintType; }

		/** \brief Set the attributes for the constraint node
		*/
		virtual bool setAttributes( const AttributeMap& attributes );


		/** \brief Set a motor force within the constaint joint
			\remarks Only valid on hinge and pulley joints
			\param[in] freeSpin If true, motor will not brake when it spins faster than velTarget
		*/
		virtual bool setMotor( Real force, Real velocity, Bool freeSpin = false ) = 0;
	
		virtual bool getMotor( Real& force, Real& velocity, Bool& freeSpin ) = 0;

		/** \brief Set a string constraint to the joint
			\param[in] spring Spring Coefficient. <b>Range:</b> (-inf,inf)
			\param[in] damper Damper Coefficient. <b>Range:</b>  [0,inf)
			\param[in] targetValue Target value (angle/position) of spring where the spring force is zero specified in -PI to PI. 
		*/
		virtual void setSpring( Real spring, Real damper = 0, Real targetValue = 0 ) = 0;

		/** \brief Get the position of the global anchor between the two anchors
		*/
		virtual math::Vector3 getGlobalAnchor() const = 0;

		/** \brief Set the position of the global anchor between the two anchors
		*/
		virtual void setGlobalAnchor( const math::Vector3& anchor ) = 0;
	
		/** \brief Get the position of the global axis between the two anchors
		*/
		virtual math::Vector3 getGlobalAxis() const = 0;

		/** \brief Set the position of the global axis between the two anchors
		*/
		virtual void setGlobalAxis( const math::Vector3& axis ) = 0;

		/** \brief Get an angle in radians for the joint
			\param[in] axis Index for the joint axis to retrieve
		*/
		virtual Real getAngle( Index axis ) const = 0;
				
		/** \brief Set an angle in radians for the joint
			\param[in] axis Index for the joint axis to set
			\param[in] radians the angle in radians from the intial positions
			\returns Whether the axis is valid
			\remarks NOT IMPLEMENTED
		*/
		virtual bool setAngle( Index axis, Real radians ) = 0;

		/** \brief Get the angular velocity of a joint axis
			\param[in] axis Index for the joint axis to retrieve
			\returns Whether the axis is valid
		*/
		virtual Real getVelocity( Index axis ) const = 0;

		/** \breif Serts the angular velocity of an axis angle
			\remarks NOT IMPLEMENTED
		*/
		virtual bool setVelocity( Index axis, Real velocity ) = 0;

		/** \brief Sets the constrint angle for the joint
			\param[in] index The limit ndex where 0=min, and 1=max
		*/
		virtual bool setLimit( Index index, Real radians, Real restitution ) = 0;

		/** \brief get the limits of the joint
		*/
		virtual bool getLimit( Index index, Real& radians, Real& restitution ) const = 0;

		/** \brief remove all constraint limits
		*/
		virtual void removeAllLimits() = 0;

	protected:
		
		virtual void updateWorldData( Real tDelta, Real tApp );
		virtual void doDraw( gfx::Renderer& renderer ) const{}		
		virtual void updateWorldBound() {}
		virtual void updateState( RenderStateStack& states, LightStack& lights ) {} 
	
		/** \brief Constraint specific implementation of setAtrtibutes
		*/		
		bool setHingeAttributes( const AttributeMap& attributes );
		bool setPinAttributes( const AttributeMap& attributes );

		/** \brief Virtual function for implementation of constraint to build its constraint structure when the constraint type
			is specified before the attributes are loaded
		*/
		virtual void setConstraintImpl() = 0;
	
		/** \brief Virtual function called when an actor is specified and allows the derived class to update the required things
		*/
		virtual bool updateConstraintActors() = 0;
	

	protected:

		Bool m_firstUpdate; //< Whether this is the first update which wil store the inital position

	private:
		frx::SmartPointer< Scene > m_owner; //< The scene the actor belongs to
		String m_actorLookup[2]; //< The name of the actors if using a name lookup
		Actor* m_actor[2]; ///< The actors the constraint belongs to
		eConstraint m_constraintType; ///< The type of shape defined
	};

} // END: phys

#endif