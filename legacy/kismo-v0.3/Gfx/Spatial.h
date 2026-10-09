/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Spatial.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_SPATIAL_H
#define	GFX_SPATIAL_H

#include "Platform/Platform.h"
#include "Framework/Rtti.h"
#include "Framework/TreeNode.h"
#include "Framework/Object.h"
#include "Framework/SmallMap.h"
#include "Gfx/Transform.h"
#include "Gfx/RenderState.h"

namespace gfx
{	
	//Forward declarations
	class RenderState;
	class Light;
	class Renderer;
	class Spatial;
	class Scene;

	/** \brief Spatial defines as scene transform entity
	ref. Game engine architecture p153
	*/
	class _DllExport Spatial : public frx::Object, public frx::TreeNode< Spatial >
	{
		DECLARE_RTTI;
		friend class Scene;
	public:		

		typedef frx::SmartPointer< Spatial > Ptr;

		/** \brief typedef used to pass a map of render states down the tree 
			//TODO: Stack is implemented as a list..... could do with a SmallStack!?!?
		*/
		typedef frx::SmallMap< const frx::Rtti*, Stack< RenderState* > > RenderStateStack;

		/** \brief typedef used to pass a map of Lights down the tree
		*/
		typedef Vector< Light* > LightStack;

	protected:

		Spatial( const String& name );

		virtual ~Spatial();

	public:


		/** \brief \copydoc frx::TreeNode::getParent()
		*/
		inline Spatial* getParent() const
		{ return frx::TreeNode< Spatial >::getParent(); }

		/** \brief \copydoc frx::TreeNode::setParent()
		*/
		inline void setParent( Spatial* parent )
		{ frx::TreeNode< Spatial >::setParent( parent ); }

		/** \brief Get the local transform of the scene node
		*/
		TransformR& localTransform()
		{ return m_localTransform; }

		/** \brief Get the local const transform of the scene node
		*/
		const TransformR& localTransform() const
		{ return m_localTransform; }

		/** \brief Get the world transform of the scene node
		*/
		TransformR& worldTransform()
		{ return m_worldTransform; }

		/** \brief Get the world const transform of the scene node
		*/
		const TransformR& worldTransform() const
		{ return m_worldTransform; }

		/** \brief Function for updating the geometric state
			\param[in] tDelta The delta time step of the system
			\param[in] tApp The application timer
			\param[in] initiator Specifies whether this is the root of an update for efficiency
		*/
		void updateGeometryState( Real tDelta, Real tApp, Bool initiator = true );

		/** \brief Function for updating the bounding state
		*/
		void updateBoundState();

		/** \brief Sets the render state at the scene node
		*/
		void setRenderState( RenderState* state );

		/** \brief Gets the render state of a specified type
		*/
		RenderState* getRenderState( const frx::Rtti& type ) const;

		/** \brief Removes a render state form this node
		*/
		void removeRenderState( const frx::Rtti& type );

		/** \brief Removes all render states
		*/
		void removeAllRenderStates();

		/** \brief Updates the render state of the node
			\remarks This is the root implementation that should be used!
		*/
		void updateRenderState();
		
		/** \brief Updates the render state of the node
			\remarks This is called via updateRenderState() and should not be called directly unkess required
		*/
		virtual void updateRenderState( RenderStateStack& states, LightStack& lights );


		/** \brief Adds a light to this scene graph node
			\remarks The light will apply to all child nodes
		*/
		void addLight( Light* light );

		/** \brief Get the number of lights attached to the node
		*/
		Count getLightCount() const
		{ return (Count)m_lights.size(); }

		/** \brief gets a light from its index
		*/
		Light* getLight( Index index ) const;

		/** \brief Remove a light from the node
		*/
		void removeLight( Light* light );

		/** \brief Remove all lights from the node
		*/
		void removeAllLights();

		/** \brief Called to draw the node bt a renderer or parent node
		*/
		void draw( Renderer& renderer );

		/** \brief Performs the actual drawing of this node
		*/
		virtual void doDraw( Renderer& renderer ) const = 0;

		/** \brief Set the attributes for the spatial node
		*/
		virtual bool setAttributes( const AttributeMap& attributes );

		/** \brief set whether the world transform is current and should not be updated automatically
			\remarks used by physics or other systems where the poisition is not part of the scene heirarchy
		*/
		void setWorldIsCurrent( bool current )
		{ m_worldIsCurrent = current; }

		/** \brief Helper functionf or finding a child node of a specified type
		*/
		gfx::Spatial* getChildOfType( const frx::Rtti& type );

	protected:

		/** \brief Propagates the update of world transforms down the tree
			\remarks Virtual allowing for customizeable implementations
		*/
		virtual void updateWorldData( Real tDelta, Real tApp ); 

		/** \brief Updates the bounding volume
		*/
		virtual void updateWorldBound() = 0; 

		/** \brief propagates the bounding volume up the tree to the root
		*/
		void propagateBoundToRoot(); 

		/** \brief Returns whether the world transform is upto date
		*/
		Bool isWorldTransformCurrent() const
		{ return m_worldIsCurrent; }

		/** \brief Returns whether the world bound of the node is upto date
		*/
		Bool isWorldBoundCurrent() const
		{ return m_worldBoundIsCurrent; }

		/** \brief Propagtes the state of parent nodes to get the state above this node.
			\remarks Does not push the state of this node onto the stacks
			\remarks Used when only a branch of the scene graph needs an update in render state
		*/
		void propagateRenderStateFromRoot( RenderStateStack& states, LightStack& lights, bool initiator = false );

		/** \brief Pushes states onto the local state
		*/
		void pushState( RenderStateStack& states, LightStack& lights );

		/** \brief pops the local states form teh stacks
		*/
		void popState( RenderStateStack& states, LightStack& lights );

		/** \brief update the state and the children if required
		*/
		virtual void updateState( RenderStateStack& states, LightStack& lights ) = 0;
		

	protected:

		typedef frx::SmallMap< const frx::Rtti*, RenderState::Ptr > RenderStateMap;
		RenderStateMap m_renderStates; ///< Map of render states indexed by the type

		typedef Vector< frx::SmartPointer< Light > > LightArray;
		LightArray m_lights; ///< Array of lights attached to this node

		TransformR m_localTransform; ///< The local transform
		TransformR m_worldTransform; ///< The world transform
		Bool m_worldIsCurrent; ///< Whether the world transform has been set externally and does nto need computing
		Bool m_worldBoundIsCurrent; ///< Whether the world bounding volume is upto date
	};

} //END: gfx

#endif