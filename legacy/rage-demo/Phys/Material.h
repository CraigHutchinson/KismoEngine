/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Material.h
 * Status: superseded by kismo-v0.3/Phys/Material.h (same name, content differs)
 */
#ifndef PHYS_MATERIAL_H
#define	PHYS_MATERIAL_H

#include "Framework/Named.h"
#include "Framework/Rtti.h"
#include "Framework/Vector.h"
#include "Framework/SmartPointer.h"
#include "Gfx/Node.h"
#include "Phys/Shape.h"

namespace phys
{
	//Forward declaration
	class Scene;

	/** \brief Base physics material
	*/
	class _DllExport Material : public frx::SmartReferenced
	{
	public:
		
		typedef frx::SmartPointer< Material > Ptr;

	protected:

		Material( Scene* owner );
		virtual ~Material();

	public:

		/** \brief Get the physics scene the actor belongs to
		*/
		Scene* getScene() const
		{ return m_owner; }		

		/** \brief Initialise and create the scene
		*/
		virtual Bool initialise() = 0;
		
		/** \brief Destroy the scene
		*/
		virtual void destroy() = 0;

		/** \brief get the dyanmnic friction
		*/
		virtual Real getDynamicFriction() const = 0;

		/** \brief Sets the duynamic friction of the material
		*/
		virtual void setDynamicFriction( Real friction ) = 0;

		/** \brief get the static friction
		*/
		virtual Real getStaticFriction() const = 0;

		/** \brief Sets the static friction of the material
		*/
		virtual void setStaticFriction( Real friction ) = 0;

		/** \brief get the bounciness/resititution
		*/
		virtual Real getRestitution() const = 0;

		/** \brief Sets the bounciness of the material
		*/
		virtual void setRestitution( Real bounciness ) = 0;

	private:
		Scene* m_owner; //< The scene the actor belongs to
	};

} // END: phys

#endif