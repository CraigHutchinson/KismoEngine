/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoDagNode.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_DAGNODE_H
#define	_KISMO_DAGNODE_H

#include "KismoTypes.h"

namespace kismo
{
	namespace frx
	{		
		template < class TNode > 
		class DagNode
		{
		protected:
			
			typedef std::set< TNode* > NODE_SET;

		public:

			DagNode() {}
			virtual ~DagNode(){}

			//Virtual event functions
			virtual void onParentAdded( TNode* parent ) {} //Virtual function called when a parent is added
			virtual void onParentRemoved( TNode* parent ) {} //Virtual function called when a parent is removed
			virtual void onChildAdded( TNode* child ) {} //Virtual function called when a child is added
			virtual void onChildRemoved( TNode* child ) {} //Virtual function called when a child is removed

			void addChild( TNode* child )
			{
				if ( m_children.find( child ) == m_children.end() ) //Stop duplicate insertion attempts
				{
					child->addParent( (TNode*)this ); //Set the childs parent to this
					m_children.insert( child ); //Store the child in the set
					onChildAdded( child );
				}
			}

			void removeChild( TNode* child )
			{
				CHILD_SET::iterator find = m_children.find( (TNode*)child );

				if ( find != m_children.end() ) //Make sure the child exists
				{
					m_children.erase( find ); //Remove the child
					child->removeParent( (TNode*)this ); //Remove the parent ref from the child
					onChildRemoved( child );
				}
			}


			void addParent( TNode* parent )
			{
				if ( m_parents.find( parent ) == m_parents.end() ) //Stop duplicate insertion attempts
				{
					m_parents.insert( parent ); //Store the parent in the set
					parent->addChild( (TNode*)this ); //Set the parents parent to this
					onParentAdded( parent );
				}
			}

			void removeParent( TNode* parent )
			{
				CHILD_SET::iterator find = m_parents.find( (TNode*)parent );

				if ( find != m_parents.end() ) //Make sure the parent exists
				{
					m_parents.erase( find ); //Remove the parent
					parent->removeChild( (TNode*)this ); //Remove the parent ref from the parent
					onParentRemoved( parent );
				}
			}

			k_Count childCount() { return (k_Count)m_children.size(); }
			k_Count parentCount() { return (k_Count)m_parents.size(); }

			/** \brief Resets the child iterator */
			TNode* firstChild() { return nextChild( kc_TRUE ); }
			/** \brief iterate the child nodes
			\param begin - resets iteration from the start */
			TNode* nextChild( k_Bool begin = kc_FALSE ) 
			{ 
				if ( begin )
					m_childIter = m_children.begin();

				if ( m_childIter != m_children.end() )
				{				
					TNode* ptr = (*m_childIter);
					++m_childIter;
					return ptr;
				}
				return 0;
			}
			/** \brief Resets the parent iterator */
			TNode* firstParent() { return nextParent( kc_TRUE ); }
			/** \brief iterate the parent nodes
			\param begin - resets iteration from the start */
			TNode* nextParent( k_Bool begin = kc_FALSE ) 
			{ 
				if ( begin )
					m_parentIter = m_parents.begin();

				if ( m_parentIter != m_parents.end() )
				{				
					TNode* ptr = (*m_parentIter);
					++m_parentIter;
					return ptr;
				}
				return 0;
			}
		protected:

			NODE_SET m_children; ///< children set
			typename NODE_SET::const_iterator m_childIter;///< children set iterator

			NODE_SET m_parents; ///< parent set
			typename NODE_SET::const_iterator m_parentIter;///< parent set iterator

		};
	
	} // END: frx
} // END: kismo

#endif
