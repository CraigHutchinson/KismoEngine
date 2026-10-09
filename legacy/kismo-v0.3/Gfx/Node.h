/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Node.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_NODE_H
#define	GFX_NODE_H

#include "Gfx/Spatial.h"

namespace gfx
{	
	/** \brief Node defines as scene object which can have child spatial objects
	ref. Game engine architecture p153
	*/
	class _DllExport Node : public Spatial
	{
		DECLARE_RTTI;

	public:

		Node( const String& name );

		virtual ~Node();

	protected:
				
		/** \brief Propagates the update of world transforms down the tree
		*/
		virtual void updateWorldData( Real tDelta, Real tApp ); 

		/** \brief Updates the world bounding volume
		*/
		virtual void updateWorldBound(); 

		/** \brief update the state and the children if required
		*/
		virtual void updateState( RenderStateStack& states, LightStack& lights );
		
		/** \brief Performs the actual drawing of this node
		*/
		virtual void doDraw( Renderer& renderer ) const;

	};

} //END: gfx

#endif