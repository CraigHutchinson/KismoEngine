/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoGFX\SceneNode.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef RTNODE_H
#define RTNODE_H

#include "kismo/Platform.h"
#include "kismo/Types.h"
#include "kismo/Vector.h"
#include "kismo/Rtti.h"

namespace kismo { namespace gfx
{
	//forward declarations
	class SceneNodeFactory;
	class IRenderer;
	class ISceneNodeImplFactory;

	/**
		\brief Node events
	*/
	enum eEvent
	{
		/**
			\brief Function called prior to a new child node being added and returns whether a child is supported for this node type
			so that an addChild() may fail accordingly
			\remarks This allows validation of child nodes to perfromed to reject some invalid hierarchies etc
			\par e.g. RenderNode validates that only a single shadowNode is attached at any time
		*/
		Event_PreChildAdd,
		Event_OnChildAdded,
		Event_PreChildRemove,
		Event_OnChildRemoved,

		Event_PreParentAdd,
		Event_OnParentAdded,
		Event_PreParentRemove,
		Event_OnParentRemoved,

		/**
			\brief Called before the active state of a node has been changed
			\remarks Called before child node states are set recursively if specified in the setActive() call
		*/
		/*Event_OnMadeActive,
		Event_PostMadeActive, //, After active satet is set
		Event_OnMadeInactive,
		Event_PostMadeInactive, //, After in-active satet is set*/

		/**
			\brief Function called when a child of this node decides the parent (i.e this) needs to be told about changes and possibly update
			\par i.e This function will be called whenever a child node has been modified
			\remarks It is up to the derived implementation to decide whether to call signalChildUpdated() to recurse up the tree
			\par It is safe to call signalParentUpdated() in this method. The parent that signalled this onChildUpdated() will not be re-signalled.
			If you require to do so call allowUpdateEcho(child) while being aware of possible infinite recursion
		*/
		Event_OnChildUpdated,
	
		/**
			\brief Function called when a parent of this node decides the parent (i.e this) needs to be told about changesand possibly update
			\par i.e This function will be called whenever a parent node has been modified
			\remarks It is up to the derived implementation to decide whether to call signalparentUpdated() to recurse down the tree
			\par It is safe to call signalChildUpdated() in this method. The child that signalled this onParentUpdated() will not be re-signalled
			If you require to do so call allowUpdateEcho(child) while being aware of possible infinite recursion
		*/
		Event_OnParentUpdated,

		Event_AccelerationUpdated,
		Event_GeometryUpdated,
		Event_GeometryPartialUpdated, //< partial updated for positions and normals TODO: some better event types or maybe some data long with the event!?
		Event_GroupUpdated,
		Event_TransformUpdated,
		Event_ShadowerUpdated,
		Event_LightUpdated,			
		Event_AtmosphereUpdated,
		Event_SpectralProfileUpdated,
	};

	class Kismo_EXPORT SceneNode
	{
		DECLARE_BASE_RTTI //, runtime type information
		friend class IRenderer;
	public:

		/**
			\brief Helper node class wraps a node handle so it can be used like a SceneNode* as if it were direct
		*/
		class NodeHandle
		{
		public:
			typedef Vector<NodeHandle,true> Array;
		public:
			NodeHandle() : m_handle(0) {}
			NodeHandle( SceneNode* node ) : 
				m_handle( node->handle() )
#ifdef _DEBUG
				, _nodePtr(node) 
#endif
			{ DEBUG_ONLY( assert( getNode(m_handle) == _nodePtr) ); }

			NodeHandle( Handle handle ) : 
				m_handle(handle)
#ifdef _DEBUG
				, _nodePtr( getNode(handle) ) 
#endif
			{}

			SceneNode* ptr() const
			{ SceneNode* node = getNode(m_handle); DEBUG_ONLY( assert(node==_nodePtr) ); return node;  }

			SceneNode* operator->() const
			{ SceneNode* t = ptr(); assert(t); return t; }

			operator SceneNode*() const 
			{ return ptr(); }

			operator Handle() const 
			{ return m_handle; }

			Handle handle() const
			{ return m_handle; }

		private:
			Handle m_handle;
			DEBUG_ONLY( SceneNode* _nodePtr; )
		};

		typedef Vector<SceneNode,false> NodeArray;

		typedef std::set<SceneNode*> VisitedLookup;
		typedef Vector<SceneNode*> VisitedArray;

		typedef NodeHandle::Array::iterator iterator;
		typedef NodeHandle::Array::const_iterator const_iterator;

		/**
			\brief gets a pointer to a node from its handle
		*/
		static SceneNode* getNode( Handle handle );

		
		class Kismo_EXPORT Impl
		{
		public:
			friend class SceneNode;
			typedef std::map<ISceneNodeImplFactory*, Impl*> Lookup;

		public:
			virtual ~Impl()
			{
				owner()->removeImpl(m_factory);
			}

			SceneNode* owner() const
			{ return m_ownerNode; }
			
			//const SceneNode* owner() const
			//{ return m_ownerNode; }

			IRenderer* renderer() const;

			ISceneNodeImplFactory* factory() const
			{ return m_factory; }

			//bool isActive() const { return owner()->isActive(); }

			virtual bool initialise() = 0;
			virtual void destroy() = 0;

			virtual bool onEvent( eEvent type, SceneNode* node ) { return true; }// = 0;

		protected:
			Impl( SceneNode* owner, ISceneNodeImplFactory* factory );

		protected:
			ISceneNodeImplFactory* m_factory;
			SceneNode* m_ownerNode;
		};

		class Kismo_EXPORT NoImpl : public Impl
		{
		public:
			NoImpl( SceneNode* owner, ISceneNodeImplFactory* factory ) : Impl(owner,factory) {}
			virtual bool initialise() { return true; }
			virtual void destroy() { return; }
		};

		/**
			\brief EventHook allows external classes to be notified of sceneNode additions, removals. etc
			\TODO: We could have a SceneNodeEventManager which all events are triggered through which could have the hooks
			attached to, this could remove the static array!? or event hooks could be explicitly set per node
		*/
		class Kismo_EXPORT EventHook
		{
		public:

			/**
				\brief Virtual callback triggered when an event occurs on a node
				\par This allows a Gui or other systems to mirror changes in the hierarchy.
				\param[in] thisNode The node which the event has been received on
				\param[in] eventType The event type enumerator
				\param[in] eventNode The node passed to thisNode along with the event. This may be the source of the triggering event etc
				\returns Value is ignored
			*/
			virtual bool onEvent( SceneNode* thisNode, eEvent eventType, SceneNode* eventNode )
			{ return true; } 
		};

	public:

		SceneNode( SceneNodeFactory* factory = 0, bool doAddRendererImpl = true );
		virtual ~SceneNode();

		SceneNodeFactory* factory() const
		{ return m_factory; }

		Handle handle() const
		{ return m_handle; }

		/**
			\brief Returns whether the node should add renderer Impl if requested
		*/
		bool doAddRendererImpl() const
		{ return m_doAddRendererImpl; }
		
		/**
			\brief Sets whether the node should add renderer Impl if a parent has renderer impl
			\remarks This is automatically called when a (Geometry)GroupNode is added as a child
			\par If a parent exists with Impl already then the implementation will be automatically added before exit from setAddRendererImpl(true)
		*/
		bool setAddRendererImpl( bool enable );

		/**
			\brief Obtains the number of nodes this ndoe references
			\returns 0 if the node is not part fo any tree
			\warnign If a call to setChildCount() has been made this value may e incorrect
		*/
		Count countReferencedNodes() const
		{ return Count(m_children.size() + m_parents.size()); }
		
		//TODO: Add method to check that all the referenced nodes actually reference us equally

		/**
			\brief Sets a single parent for this node and removes any existing parent nodes. Also updates the parents child list accordingly
			\remarks Signals onChildAdded(this) on the new parent and onChildRemoved(this) on any old parent
		*/
		bool setParent( SceneNode* parent );

		/**
			\brief Adds a single parent for this node maintaining existing parents as required. Also updates the parents child list accordingly
			\remarks Signals onChildAdded( this ) on the new parent
		*/
		bool addParent( SceneNode* parent );
		
		bool removeParent( SceneNode* parent );

	
		/**
			\brief returns the number fo children this ndoe has
		*/
		Count getChildCount() const
		{ return m_children.size(); }

		/**
			\brief returns the number fo parents this ndoe has
		*/
		Count getParentCount() const
		{ return m_parents.size(); }

		/**
			\brief Sets the number of children this node has so setChild(Index,SceneNode*) can be used to insert children. 
			\remarks This is more efficient then calling addChild() multiple times if there are a large number of children
		*/
		void setChildCount( Count newCount );
		
		/**
			\brief Sets a single child for this node and removes any existing child node at that index. Also updates the childs parent list accordingly
			\remarks Signals onParentAdded(this) on the new child and onChildRemoved(this) on any old child
			\remarks index The index wihtin this node that is to be set
		*/
		bool setChild( Index index, SceneNode* child );

		/**
			\brief Adds a single child for this node maintaining existing childs as required. Also updates the childs parent list accordingly
			\remarks Signals onParentAdded( this ) on the new child
		*/
		bool addChild( SceneNode* child );

		bool removeChild( SceneNode* child );


		iterator childBegin() { return m_children.begin(); }
		iterator childEnd() { return m_children.end(); }
		const_iterator childBegin() const { return m_children.begin(); }
		const_iterator childEnd() const { return m_children.end(); }

		iterator parentBegin() { return m_parents.begin(); }
		iterator parentEnd() { return m_parents.end(); }
		const_iterator parentBegin() const { return m_parents.begin(); }
		const_iterator parentEnd() const { return m_parents.end(); }
		
		iterator findChild( SceneNode* child );
		const_iterator findChild( SceneNode* child ) const;

		iterator findParent( SceneNode* parent );
		const_iterator findParent( SceneNode* parent ) const;

		iterator findFirstChild( const framework::Rtti& type, bool derived = true ) { return findNextChild( iterator(), type, derived ); }
		const_iterator findFirstChild( const framework::Rtti& type, bool derived = true ) const { return findNextChild( const_iterator(), type, derived ); }

		iterator findNextChild( iterator iFirst, const framework::Rtti& type, bool derived = true );
		const_iterator findNextChild( const_iterator iFirst, const framework::Rtti& type, bool derived = true ) const;
		
		iterator findFirstParent( const framework::Rtti& type, bool derived = true ) { return findNextParent( iterator(), type, derived ); }
		const_iterator findFirstParent( const framework::Rtti& type, bool derived = true ) const { return findNextParent( const_iterator(), type, derived ); }

		iterator findNextParent( iterator iFirst, const framework::Rtti& type, bool derived = true );
		const_iterator findNextParent( const_iterator iFirst, const framework::Rtti& type, bool derived = true ) const;
		
		/**
			\brief Finds all nodes that macth the supplied type using isDerived()
			\TODO: add otpion for isDerived() or isExactly() type checking
			\param[in] nodeHandles Array which is populated with hjandles ot the child nodes that are found. Any existing nodes will be cleared from the array
		*/
		Count findChildren( NodeHandle::Array& nodeHandles, const framework::Rtti& type ) const;
		Count findParents( NodeHandle::Array& nodeHandles, const framework::Rtti& type ) const;

		template < class _T > _T* findFirstChild() const
		{
			const_iterator iFind = findFirstChild( _T::TYPE );
			return (iFind != childEnd() ? (_T*)(SceneNode*)*iFind : 0 );
		}
		template < class _T > _T* findFirstParent() const
		{
			const_iterator iFind = findFirstParent( _T::TYPE );
			return (iFind != parentEnd() ? (_T*)(SceneNode*)*iFind : 0 );
		}

		/**
			\brief Recirsively searches up the tree to find a parent of the desired type using a breadth first approach
		*/
		template < class _T > _T* findFirstParentRecursive() const
		{
			const_iterator iFind = findFirstParent( _T::TYPE );
			if ( iFind != parentEnd() )
				return (_T*)(SceneNode*)*iFind;
			for ( iFind = parentBegin(); iFind != parentEnd(); ++iFind )
			{
				_T* ret = (*iFind)->findFirstParentRecursive<_T>();
				if ( ret ) 
					return ret; 
			}
			return 0;
		}


		Impl* getImpl( IRenderer* renderer ) const;
		Impl* getImpl( ISceneNodeImplFactory* factory  ) const;

		template< class _T >
		_T* getImpl( IRenderer* renderer ) const
		{ return (_T*)getImpl( renderer ); }

		template< class _T >
		_T* getImpl( ISceneNodeImplFactory* factory ) const
		{ return (_T*)getImpl( factory ); }


		/**
			\brief ISceneNodeImplFactory* used internally
		*/
		Impl* newImpl( IRenderer* renderer );
		Impl* newImpl( ISceneNodeImplFactory* factory );

		/**
			\brief ISceneNodeImplFactory* used internally
		*/
		bool deleteImpl( IRenderer* renderer );
		bool deleteImpl( ISceneNodeImplFactory* factory );
	
		bool SceneNode::newTreeImpl( ISceneNodeImplFactory* factory, bool recursive = true );
		bool newTreeImpl( ISceneNodeImplFactory* factory, bool recursive, VisitedLookup& visitedList, VisitedArray& visitedArray );

		bool deleteTreeImpl( ISceneNodeImplFactory* factory, bool recursive = true, VisitedLookup* visitedList = 0 );


		bool addEventHook( EventHook* hook )
		{ m_eventHooks.insert(hook); return true; 	}

		bool removeEventHook( EventHook* hook )
		{ m_eventHooks.erase(m_eventHooks.find(hook) ); return true; }

	protected: ///, helper functions

		/**
			\brief ISceneNodeImplFactory* used internally
		*/
		bool removeImpl( IRenderer* renderer );
		bool removeImpl( ISceneNodeImplFactory* factory );


		/**
			\brief Sends a signal to all parent nodes that this node has been updated by calling onChildUpdated() on each in turn
			\returns true
		*/
		bool signalChildUpdated( eEvent type );

		/**
			\brief Sends a signal to all child nodes that this node has been updated by calling onParentUpdated() on each in turn
			\remarks This function will not update a child who called onChildUpdated()
			\returns true
		*/
		bool signalParentUpdated( eEvent eventType );
		bool signalParentUpdated( eEvent eventType, const framework::Rtti& type, bool derived = true );

		/**
			\brief 
		*/
		void allowUpdateEcho( SceneNode* node )
		{ node->m_inUpdateSignal = false; }

		/**
			\brief Returns whether this ndoe should load itself correctly/fully for use
		*/
		//bool isActive() const
		//{ return m_isActive; }

		//void setActive( bool active, bool recursive = true );

protected:
	
		/**
			\brief Callback that occurs when an event happends
			\remarks Always called after the render implementation receive the event
		*/
		virtual bool onEvent( eEvent type, SceneNode* node )
		{ return true; }

		/**
			\brief base onEvent() function calls the implementation specific onEvent and then calls the onevent() directly for the implementaiotn of 'this'
		*/
		bool signalEvent( eEvent type, SceneNode* node = 0 );
		
		Impl::Lookup::iterator implBegin() { return m_impls.begin(); }
		Impl::Lookup::const_iterator implBegin() const { return m_impls.begin(); }
		Impl::Lookup::iterator implEnd() { return m_impls.end(); }
		Impl::Lookup::const_iterator implEnd() const { return m_impls.end(); }


	private:
		SceneNodeFactory* m_factory;
		bool m_doAddRendererImpl; //<  Whether the node should add renderer implmentation when it is attached to a renderer TODO: this could be per renderer type!?
		Handle m_handle; //, the node handle
		NodeHandle::Array m_children; //, handles of all child nodes of this node
		NodeHandle::Array m_parents; //, handles of all parent nodes of this node

		Impl::Lookup m_impls; //< Set of implementations for the current renderer(s) using/attached-to this node


		typedef std::set<EventHook*> EventHookSet;
		EventHookSet m_eventHooks;

	private:
		static Handle ms_lastNewHandle; //, the handle given to last node that was created(initialised to 0)

		typedef std::map<Handle,SceneNode*> NodeLookup;
		static NodeLookup ms_nodeLookup; ///, lookup to find nodes by their handles
	};


} /* END: gfx*/ } /*END: Kismo*/

#endif