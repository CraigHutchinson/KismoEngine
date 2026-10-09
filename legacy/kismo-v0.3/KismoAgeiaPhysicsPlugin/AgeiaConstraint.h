/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaConstraint.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYS_AGEIA_PHYSICSCONSTRAINT_H
#define	PHYS_AGEIA_PHYSICSCONSTRAINT_H

#include "Phys/Constraint.h"

//forward declarations
class NxJoint;
class NxJointDesc;
class NxScene;

namespace phys
{
	/** Ageia physics constraint
	*/
	class AgeiaConstraint : public Constraint
	{
	public:

		AgeiaConstraint( const String& name, const Scene::Ptr& owner );
		virtual ~AgeiaConstraint();

		/** Initialise and create the scene
		*/
		virtual Bool initialise();
		
		/** Destroy the scene
		*/
		virtual void destroy();
			
		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene() const;

		virtual bool setMotor( Real force, Real velocity, Bool freeSpin = false );
	
		virtual bool getMotor( Real& force, Real& velocity, Bool& freeSpin );

		virtual void setSpring( Real spring, Real damper = 0, Real targetValue = 0 );
		
		/** \brief Get the position of the global anchor between the two anchors
		*/
		virtual frx::Vector3r getGlobalAnchor() const;

		/** \brief Set the position of the global anchor between the two anchors
		*/
		virtual void setGlobalAnchor( const frx::Vector3r& anchor );
	
		/** \brief Get the position of the global axis between the two anchors
		*/
		virtual frx::Vector3r getGlobalAxis() const;

		/** \brief Set the position of the global axis between the two anchors
		*/
		virtual void setGlobalAxis( const frx::Vector3r& axis );

		/** \brief Get an angle in radians for the joint
			\param[in] axis Index for the joint axis to retrieve
		*/
		virtual Real getAngle( Index axis ) const;
				
		/** \brief Set an angle in radians for the joint
			\param[in] axis Index for the joint axis to set
			\param[in] radians the angle in radians from the intial positions
			\returns Whether the axis is valid
		*/
		virtual bool setAngle( Index axis, Real radians )
		{ assert( false ); return false; }

		/** \brief Get the angular velocity of a joint axis
			\param[in] axis Index for the joint axis to retrieve
			\returns Whether the axis is valid
		*/
		virtual Real getVelocity( Index axis ) const;

		/** \breif Serts the angular velocity of an axis angle
		*/
		virtual bool setVelocity( Index axis, Real velocity )
		{ assert( false ); return false; }

		virtual bool setLimit( Index index, Real radians, Real restitution ) ;

		/** \brief get the limits of the joint
		*/
		virtual bool getLimit( Index index, Real& radians, Real& restitution ) const;

		virtual void removeAllLimits();
	protected:
		
		/** Virtual function for implementation of constraint to build its constraint 
			structure when the constraint type is specified before the attributes are loaded
		*/
		virtual void setConstraintImpl();

		/** Virtual function called when an actor is specified and allows the derived class to 
			update the required things
		*/
		virtual bool updateConstraintActors();

	private:
		NxJointDesc* m_constraintDescription; ///< The constraint description used to create the joint
		NxJoint* m_constraint; ///< The constraint joint e.g. revolute/hinge, 
	};

} // END: phys

#endif