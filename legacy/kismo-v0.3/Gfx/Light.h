/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Light.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_LIGHTSPATIAL_H
#define	GFX_LIGHTSPATIAL_H

#include "Framework/SmartPointer.h"
#include "Framework/Colour.h"
#include "Gfx/Spatial.h"

namespace gfx
{	
	/** \brief A light for a scene graph
	ref. Game engine architecture p225
	*/
	class _DllExport Light : public Spatial
	{
		DECLARE_RTTI;
	public:

		typedef frx::SmartPointer< Light > Ptr;
		typedef frx::Colour4 Colour; ///< Typedef for convenience

		/** \brief Enumerator for the type of light
		*/
		enum eLightType
		{
			LightType_Ambient, ///< Ambient light applies
			LightType_Directional, ///< A directional light. used for the sun etc
			LightType_Point, ///< An omni-directional light poitn emits light in all directions. used for a light bulb
			LightType_Spot, //< A spot light. Like a desk lamp
		};

	public:

		Light( const String& name, eLightType type = LightType_Ambient );
		virtual ~Light();

		eLightType type; ///< The type of light
		Bool on; ///< Sets whether the light is on or off
		Colour ambient; ///< The colour and magnitude of the ights ambient component
		Colour diffuse; ///< The diffuse (actual light) colour and magnitude of the light
		Colour specular; ///< The specular colour and magnitude the light emits

		/** \brief Performs the actual drawing of this node
		*/
		virtual void doDraw( Renderer& renderer ) const;

	protected:

		/** \brief Updates the world bounding volume
		*/
		virtual void updateWorldBound(); 
		
		/** \brief update the state and the children if required
		*/
		virtual void updateState( RenderStateStack& states, LightStack& lights );
		

	};

} //END: gfx

#endif