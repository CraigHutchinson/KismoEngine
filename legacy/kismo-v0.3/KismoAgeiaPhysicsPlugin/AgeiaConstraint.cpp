/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaConstraint.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaConstraint.h"
#include "AgeiaScene.h"
#include "AgeiaActor.h"
#include <NxRevoluteJoint.h>
#include <NxSphericalJoint.h>
#include <NxUserContactReport.h>

namespace phys
{
	AgeiaConstraint::AgeiaConstraint( const String& name, const Scene::Ptr& owner ) :
		Constraint( name, owner ),
		m_constraintDescription( 0 ),
		m_constraint( 0 )
	{
	}

	AgeiaConstraint::~AgeiaConstraint()
	{
		destroy();
		frx::safeDelete( m_constraintDescription );
	}

	void AgeiaConstraint::setConstraintImpl()
	{
		if (  m_constraintDescription )
			frx::safeDelete( m_constraintDescription );

		switch( getConstraintType() )
		{
		case Constraint_Hinge:
			{				
				m_constraintDescription = new NxRevoluteJointDesc;
				break;
			}
		case Constraint_Pin: ///< Ball joint
			{
				m_constraintDescription = new NxSphericalJointDesc;
				break;
			}
		default:
			{
				assert( false );
				break;
			}
		}
	}

	bool AgeiaConstraint::updateConstraintActors()
	{
		assert( m_constraintDescription );

		Count count = 0;
		for ( int i =0; i < 2; ++i )
		{
			AgeiaActor* actor = (AgeiaActor*)getActor( i );

			if ( actor && actor->getBody() != 0 )
			{
				//assert( actor->getNxActor() );
				if ( !actor->getNxActor() )
					return false;

				++count;
				m_constraintDescription->actor[ i ] = actor->getNxActor();
			}
			else
			{
				m_constraintDescription->actor[ i ] = 0;
				//This is valid if the joint is anchored on the world
			}
		}
		return count != 0;
	}
	
	NxScene* AgeiaConstraint::getNxScene() const
	{ return ((AgeiaScene*)getScene())->getNxScene(); }

	Bool AgeiaConstraint::initialise()
	{
		if (  m_constraint )
		{
			assert( false );
			getNxScene()->releaseJoint( *m_constraint );
		}

		assert( m_constraintDescription );
		if ( !m_constraintDescription )
			return false;

		m_constraintDescription->setGlobalAnchor( toNxVec3( worldTransform().getTranslate() ) );
		
		//if ( worldTransform().isRSMatrix() )
		//	m_constraintDescription->globalPose.M = toNxMat33( worldTransform().getRotate() );

		if ( worldTransform().isRSMatrix() )
			m_constraintDescription->setGlobalAxis( 
				toNxVec3( worldTransform().getRotate() * frx::Vector3r::Unit_Z ) );

		switch( getConstraintType() )
		{
		case Constraint_Hinge: //NxRevoluteJoint
			{		
				NxRevoluteJointDesc* constraint = (NxRevoluteJointDesc*)m_constraintDescription;
				//constraint->projectionMode = NX_JPM_POINT_MINDIST;
				//constraint->projectionDistance = 0.01;
				//constraint->projectionAngle = 0.01;
				break;
			}
		case Constraint_Pin: ///< Ball joint NxSphericalJoint
			{

				break;
			}
		default:
			{
				assert( false );
				break;
			}
		}

		m_constraint = getNxScene()->createJoint( *m_constraintDescription );

		if ( getActor(0) && getActor(1) )
			getNxScene()->setActorPairFlags(
				*((AgeiaActor*)getActor(0))->getNxActor(), 
				*((AgeiaActor*)getActor(1))->getNxActor(), NX_IGNORE_PAIR);


		return m_constraint != 0;

	}
	
	void AgeiaConstraint::destroy()
	{
		if (  m_constraint )
		{
			getNxScene()->releaseJoint( *m_constraint );
		}

	}

	
	bool AgeiaConstraint::setMotor( Real force, Real velocity, bool freeSpin )
	{
		if ( !m_constraint )
			return false;

		switch( getConstraintType() )
		{
		case Constraint_Hinge: //NxRevoluteJoint
			{		
				NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
				constraint->setMotor( NxMotorDesc( velocity, force, freeSpin ) );
				return true;
			}		
		}
		return false;
	}

	bool AgeiaConstraint::getMotor( Real& force, Real& velocity, Bool& freeSpin )
	{
		if ( m_constraint )
		{
			switch( getConstraintType() )
			{
			case Constraint_Hinge: //NxRevoluteJoint
				{		
					NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
					NxMotorDesc motor;
					if ( constraint->getMotor( motor ) )
					{
						velocity = motor.velTarget;
						force = motor.maxForce;
						freeSpin = motor.freeSpin == NX_TRUE;
						return true;
					}
				}		
			}
		}
		//Set deafults (no motor)
		velocity = 0;
		force = 0;
		freeSpin = true;
		return false;
	}

	void AgeiaConstraint::setSpring( Real spring, Real damper, Real targetValue )
	{
		if ( !m_constraint )
			return;

		switch( getConstraintType() )
		{
		case Constraint_Hinge: //NxRevoluteJoint
			{		
				NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
				constraint->setSpring( NxSpringDesc( spring, damper, targetValue ) );
				break;
			}		
		}
	}

	frx::Vector3r AgeiaConstraint::getGlobalAnchor() const
	{
		assert( m_constraint );
		return toVec3r( m_constraint->getGlobalAnchor() );
	}

	void AgeiaConstraint::setGlobalAnchor( const frx::Vector3r& anchor )
	{
		assert( m_constraint );
		m_constraint->setGlobalAnchor( toNxVec3( anchor ) );
	}

	frx::Vector3r AgeiaConstraint::getGlobalAxis() const
	{
		assert( m_constraint );
		return toVec3r( m_constraint->getGlobalAxis() );
	}

	void AgeiaConstraint::setGlobalAxis( const frx::Vector3r& axis )
	{
		assert( m_constraint );
		m_constraint->setGlobalAxis( toNxVec3( axis ) );
	}

	Real AgeiaConstraint::getAngle( Index axis ) const
	{
		if ( m_constraint )
		{
			switch( getConstraintType() )
			{
			case Constraint_Hinge: //NxRevoluteJoint
				{		
					NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
					return (Real)constraint->getAngle();
				}		
			}
		}
		return 0;
	}

	Real AgeiaConstraint::getVelocity( Index axis ) const
	{
		if ( m_constraint )
		{
			switch( getConstraintType() )
			{
			case Constraint_Hinge: //NxRevoluteJoint
				{		
					NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
					return (Real)constraint->getVelocity();
				}		
			}
		}
		return 0;
	}

	void AgeiaConstraint::removeAllLimits()
	{
		if ( m_constraint )
		{
			m_constraint->purgeLimitPlanes();
		}
	}

	bool AgeiaConstraint::setLimit( Index index, Real radians, Real restitution )
	{
		if ( m_constraint )
		{
			switch( getConstraintType() )
			{
			case Constraint_Hinge: //NxRevoluteJoint
				{		
					NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
					NxJointLimitPairDesc constraintPair;

					//Get existing values incase
					constraint->getLimits( constraintPair );

					if ( index == 0 )
					{
						constraintPair.low.value = radians;
						constraintPair.low.restitution = restitution;
					}
					else
					if ( index == 1 )
					{
						constraintPair.high.value = radians;
						constraintPair.high.restitution = restitution;
					}
					else
					{
						assert( false );
						return false;
					}

					constraint->setLimits( constraintPair );
					return true;
				}		
			}
		}
		return false;
	}

	bool AgeiaConstraint::getLimit( Index index, Real& radians, Real& restitution ) const
	{
		if ( m_constraint )
		{
			switch( getConstraintType() )
			{
			case Constraint_Hinge: //NxRevoluteJoint
				{		
					NxRevoluteJoint* constraint = (NxRevoluteJoint*)m_constraint;
					NxJointLimitPairDesc constraintPair;
					if ( !constraint->getLimits( constraintPair ) )
						return false;

					if ( index == 0 )
					{
						radians = constraintPair.low.value;
						restitution = constraintPair.low.restitution;
						return true;
					}
					else
					if ( index == 1 )
					{
						radians = constraintPair.high.value;
						restitution = constraintPair.high.restitution;
						return true;
					}
					else
					{
						assert( false );
					}
				}		
			}
		}
		return false;
	}


} //END: phys