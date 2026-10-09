/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Body.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef PHYS_BODY_H
#define	PHYS_BODY_H

#include "Gfx/Spatial.h"
#include "Phys/Scene.h"

namespace phys
{
	//Forward declaration
	class Scene;
	class Actor;

	/** \brief Physics Dynamic body implementation
	\remarks 
		Base physics interface which can be specialised in a plugin to use a specific engine
	*/
	class _DllExport Body : public gfx::Spatial
	{		
		DECLARE_RTTI;	
	protected:

		Body( const String& name, const Scene::Ptr& owner );
		virtual ~Body();

	public:

		/** \brief Get the physics scene the actor belongs to
		*/
		Scene::Ptr getScene() const
		{ return m_owner; }	

		/** \brief set the Actor that this body belongs to
		*/
		virtual void setActor( Actor* actor );

		/** \brief Get the actor the body belongs to if any
		*/
		Actor* getActor() const
		{ return m_actor; }

		/** \brief set the mass of the body
		*/
		virtual void setMass( Real mass ) = 0;

		/** \brief get the mass of the body
		*/
		virtual Real getMass() const = 0;

		/** \brief set the centre of mass of the body
		*/
		virtual void setCentreOfMass( const math::Vector3& mass ) = 0;

		/** \brief get the centre of mass of the body
		*/
		virtual math::Vector3 getCentreOfMass() const = 0;

		/** \brief Set the attributes for the spatial node
		*/
		virtual bool setAttributes( const AttributeMap& attributes );
		
		virtual void setAngularDamping( Real damp ) = 0;

		virtual Real getAngularDamping() const = 0;

		virtual void setLinearDamping( Real damp ) = 0;

		virtual Real getLinearDamping() const = 0;

		virtual void setInertiaTensor( const math::Vector3& it ) = 0;

		virtual math::Vector3 getInertiaTensor() const = 0;

	protected:
		
		virtual void doDraw( gfx::Renderer& renderer ) const{}		
		virtual void updateWorldBound() {}
		virtual void updateState( RenderStateStack& states, LightStack& lights ) {} 

	private:
		frx::SmartPointer< Scene > m_owner; //< The scene the actor belongs to
		Actor* m_actor; ///< The actor the body belongs to if the body is in use
	};

} // END: phys

#endif