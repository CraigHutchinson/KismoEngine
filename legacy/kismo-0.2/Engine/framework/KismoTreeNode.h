/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoTreeNode.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_TREENODE_H
#define	_KISMO_TREENODE_H

#include "KismoTypes.h"

namespace kismo
{
	namespace frx
	{		
		template < class TNode > 
		class TreeNode
		{
		public:

			typedef std::set< TNode* > CHILD_SET;

		public:

			TreeNode() : m_parent( 0 ) {}
			virtual ~TreeNode(){}

			//Virtual event functions
			virtual void beforeParentChanged() {} //Virtual function called before the parent changes
			virtual void onParentChanged() {} //Virtual function called when the parent changes
			virtual void beforeChildAdded( TNode* child ) {} //
			virtual void onChildAdded( TNode* child ) {} //Virtual function called when a child is added

			void addChild( TNode* child )
			{
				if ( m_children.find( child ) == m_children.end() ) //Stop duplicate insertion attempts
				{
					beforeChildAdded( child );
					child->setParent( (TNode*)this ); //Set the childs parent to this
					m_children.insert( child ); //Store the child in the set
					onChildAdded( child );
				}
			}

			virtual void onChildRemoved( TNode* child ) {} //Virtual function called when a child is removed
			void removeChild( TNode* child )
			{
				CHILD_SET::iterator find = m_children.find( (TNode*)child );

				if ( find != m_children.end() ) //Make sure the child exists
				{
					(*find)->setParent( 0 ); //Remove the parent ref from the child
					m_children.erase( (*find) ); //Remove the child
					onChildRemoved( child );
				}
			}

			k_Count childCount() { return (k_Count)m_children.size(); }

			inline TNode* getParent() { return m_parent; } //Get a pointer to the parent
			void setParent( TNode* parent )
			{
				if ( parent == m_parent ) //Check if the parent is already set
					return;

				beforeParentChanged(); //Allow the parent change event to be detected before it occurs

				if ( isValidPointer( m_parent ) )
				{
					TNode* oldParent = m_parent;
					m_parent = parent; //Set the parent to prevent an infinite loop
					oldParent->removeChild( (TNode*)this ); //remove reference in current parent
				}
				else
					m_parent = parent;			

				if ( isValidPointer( m_parent ) )
				{
					m_parent->addChild( (TNode*)this ); //Add reference to the new parent
				}

				onParentChanged();
			}

			//Iterates child nodes
			TNode* firstChild() 
			{ 
				return nextChild( kc_TRUE );
			}

			// begin - resets iteration from the start
			TNode* nextChild( k_Bool begin = kc_FALSE ) 
			{ 
				if ( begin )
					m_childIter = m_children.begin();

				//assert( m_childIter != m_children.end() );//Make sure a continued iteration doesn't occur after the end of the child list!

				if ( m_childIter != m_children.end() )
				{				
					TNode* ptr = (*m_childIter);
					++m_childIter;
					return ptr;
				}
				else
					return 0;
			}

		protected:

			TNode* m_parent; //parent node
			CHILD_SET m_children; //children set
			typename CHILD_SET::const_iterator m_childIter;

		};
	
	} // END: frx
} // END: kismo

#endif
