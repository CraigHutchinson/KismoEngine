/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\OctTreeNode.h
 * Status: duplicate of kismo-v0.3/Gfx/OctTreeNode.h
 */
#ifndef GFX_OCTTREENODE_H
#define	GFX_OCTTREENODE_H

#include "Gfx/Node.h"

namespace gfx
{	
	/** \brief Oct tree scene graph node implementation
		\remarks Not implemented due to time issues
		/TODO: Implement this
	*/
	class /*_DllExport*/ OctTreeNode : public Node
	{
		DECLARE_RTTI;

	public:

		OctTreeNode( const String& name );

		virtual ~OctTreeNode();
	
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