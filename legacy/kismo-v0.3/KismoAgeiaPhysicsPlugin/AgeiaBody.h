/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaBody.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYS_AGEIA_BODY_H
#define	PHYS_AGEIA_BODY_H

#include "phys/Body.h"

//forward delcaration
class NxBodyDesc;
class NxScene;

namespace phys
{
	/** Ageia Actor
	*/
	class AgeiaBody : public Body
	{		
	public:

		AgeiaBody( const String& name, const Scene::Ptr& owner );
		virtual ~AgeiaBody();

		/** Get the Ageia PhysX body description
		*/
		const NxBodyDesc* getNxBody() const
		{ return m_bodyDescription; }
			
		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene() const;

		/** set the mass of the body
		*/
		virtual void setMass( Real mass );

		/** get the mass of the body
		*/
		virtual Real getMass() const;

		/** set the centre of mass of the body
		*/
		virtual void setCentreOfMass( const frx::Vector3r& mass );

		/** get the centre of mass of the body
		*/
		virtual frx::Vector3r getCentreOfMass() const;
		
		virtual void setAngularDamping( Real damp );

		virtual Real getAngularDamping() const;

		virtual void setLinearDamping( Real damp );

		virtual Real getLinearDamping() const;

		virtual void setInertiaTensor( const frx::Vector3r& it );

		virtual frx::Vector3r getInertiaTensor() const;

	private:
		
		NxBodyDesc* m_bodyDescription; //< The physics body description
	};

} // END: phys

#endif