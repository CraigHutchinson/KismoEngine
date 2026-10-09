/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\MaterialState.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_MATERIALSTATE_H
#define	GFX_MATERIALSTATE_H

#include "Gfx/RenderState.h"
#include "Framework/Colour.h"

namespace gfx
{	
	/** \brief material render state
	*/
	class _DllExport MaterialState : public RenderState
	{
		DECLARE_RTTI;
	public:

		typedef frx::Colour4 Colour; ///< Typedef for convenience
		
		/** \brief Create a cast smart pointer to cast between ttypes
		*/
		typedef frx::SmartPointer< MaterialState > Ptr;

		/** \brief Array of material piointers
		*/
		typedef Vector< MaterialState::Ptr > PtrArray;

		/** \brief Map of material pointers
		*/
		typedef Map< String, MaterialState::Ptr > PtrLookup;
	
	public:

		static Ptr create( const String& name );

	protected:

		MaterialState( const String& name );

		virtual ~MaterialState();

	public:

		//Colour emissive; ///< The emmisive colour is the light emited from the material. e.g. light bulb
		Colour ambient; ///< The amount of ambient light that the material reflects
		Colour diffuse; ///< The amount of diffuse light the material reflects
		Colour specular; ///< The specular colour the material reflects
		Real specularPower; ///< The cosine coefficient for the specular highlight

		/** \brief Set the attributes for the material
		*/
		bool setAttributes( const AttributeMap& attributes )
		{
			AttributeMap::const_iterator iFind;
					
			if ( (iFind = attributes.find( _T("ambient") )) != attributes.end() )
				ambient = iFind->second;
			
			if ( (iFind = attributes.find( _T("diffuse") )) != attributes.end() )
				diffuse = iFind->second;
			
			if ( (iFind = attributes.find( _T("specular") )) != attributes.end() )
				specular = iFind->second;
			
			if ( (iFind = attributes.find( _T("cos_power") )) != attributes.end() )
				specularPower = iFind->second.asReal();

			return true;


		}

	};

} //END: gfx

#endif