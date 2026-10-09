/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Geometry.h
 * Status: duplicate of kismo-v0.3/Gfx/Geometry.h
 */
#ifndef GFX_GEOMETRY_H
#define	GFX_GEOMETRY_H

#include "Gfx/Spatial.h"
#include "Gfx/Buffer.h"
#include "Gfx/MaterialState.h"

namespace gfx
{	
	//forward declaration
	class MaterialState;

	/** \brief Geometry defines as scene transform entity
	*/
	class _DllExport Geometry : public Spatial
	{
		DECLARE_RTTI;
	public:

		typedef frx::SmartPointer< Geometry > Ptr;

	public:

		Geometry( const String& name );
		virtual ~Geometry();

		/** \brief Function for updating the model state
			\param[in] tDelta The delta time step of the system
			\param[in] tApp The application timer
			\remarks Should be called whenever the vertices are updated
			\remarks All it does is calls updateModelBound()
		*/
		void updateGeometryState( Real tDelta, Real tApp );
		
		/** \brief Holds the data for a vertexElement of the gemetry
			\remarks Used by renderer when rendering the geometry
		*/
		struct VertexElement
		{
			UInt8 stride;
			Buffer* buffer;
			UInt8 offset;
		};

		VertexElement position;
		VertexElement normal;
		VertexElement uv;


		/** \brief Holds data for an Index value within a buffer
			\remarks Used by renderer when rendering the geometry
		*/
		struct IndexElement
		{
			UInt8 stride;
			Buffer* buffer;
			UInt8 positionOffset;
			UInt8 normalOffset;
			UInt8 uvOffset;
			MaterialState::Ptr material; //< THe material for the indice set
		};

		typedef Vector< IndexElement > IndiceArray;
		IndiceArray indiceSets;


	protected:

		/** \brief Updates the bounding volume of the model
		*/
		virtual void updateModelBound();
		
		/** \brief Updates the world bounding volume
		*/
		virtual void updateWorldBound(); 

		/** \brief update the state and the children if required
		*/
		virtual void updateState( RenderStateStack& states, LightStack& lights );

	};

} //END: gfx

#endif