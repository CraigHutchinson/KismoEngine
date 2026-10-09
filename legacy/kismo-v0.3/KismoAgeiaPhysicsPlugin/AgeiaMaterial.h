/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaMaterial.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef AGEIA_PHYS_MATERIAL_H
#define	AGEIA_PHYS_MATERIAL_H

#include "Phys/Material.h"
#include "AgeiaScene.h"
#include <NxMaterialDesc.h>
#include <NxP.h>

//forqard declarations
class NxMaterial;

namespace phys
{
	//Forward declaration
	/** Base physics material
	*/
	class AgeiaMaterial : public Material
	{
	public:

		/** Constructor
			\param[in] owner Pointer to the scene that owns this material
			\param[in] manualIndex A manual index which specifies the material this class wraps
			\remarks Passing manualIndex as 0 will give a deafult material instance
		*/
		AgeiaMaterial( Scene* owner, NxMaterialIndex manualIndex =(NxMaterialIndex)-1 );
		virtual ~AgeiaMaterial();

		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene() const
		{ return ((AgeiaScene*)getScene())->getNxScene(); }

		/** gaet the index for the material withint the physX engine
		*/
		NxMaterialIndex getMaterialindex() const
		{ return m_materialIndex; }

		/** Initialise and create the scene
		*/
		virtual Bool initialise();
		
		/** Destroy the scene
		*/
		virtual void destroy();

		/** get the dyanmnic friction
		*/
		virtual Real getDynamicFriction() const;

		/** Sets the duynamic friction of the material
		*/
		virtual void setDynamicFriction( Real friction );

		/** get the static friction
		*/
		virtual Real getStaticFriction() const;

		/** Sets the static friction of the material
		*/
		virtual void setStaticFriction( Real friction );

		/** get the bounciness/resititution
		*/
		virtual Real getRestitution() const;

		/** Sets the bounciness of the material
		*/
		virtual void setRestitution( Real bounciness );

	private:
		NxMaterialDesc m_materialDescription; ///< The material description used to create the material
		NxMaterial* m_material; ///< The material instance
		NxMaterialIndex m_materialIndex; ///< The Ageia PhysX material index
	};

} // END: phys

#endif