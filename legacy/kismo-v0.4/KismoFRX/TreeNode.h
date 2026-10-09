/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\TreeNode.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/TreeNode.h (same name, content differs)
 */
#ifndef KISMOFRX_TREENODE_H
#define KISMOFRX_TREENODE_H

#include "Types.h"
#include "SmallSet.h"

namespace kismo
{		
	/** \brief Graph node defines a node of a graph which may have multiple children
		but only 1 parent
	*/
	template < class TNode, class TValue = kismo::SmartPointer< TNode >  > 
	class KISMOFRX_EXPORT TreeNode
	{
	public:	

		typedef SmallSet< TValue > NodeSet; ///< Typedef the node set

		typedef typename NodeSet::const_iterator const_iterator; ///< Typedef the node set iterator
		typedef typename NodeSet::const_reverse_iterator ConstReverseIterator; ///< Typedef the node set iterator
		typedef typename NodeSet::iterator iterator; ///< Typedef the node set iterator
		typedef typename NodeSet::reverse_iterator ReverseIterator; ///< Typedef the node set iterator

	public:
		
		/** \brief Constructor 
		*/
		TreeNode() :
			m_parent( 0 )
		{}

		/** \brief Virtual destructor 
		\remark
			WARNING: Children may be left hanging without a parent node
		*/
		virtual ~TreeNode()
		{
			removeAllChildred(); ///Remove any children linking to this node
			setParent( 0 ); //Remove the parents link to this node
		}

		/** \brief Adds a new child graph node
			\remarks Will automatically ignore duplicate entries
		*/
		void addChild( TNode* child )
		{
			if ( m_children.find( child ) == m_children.end() ) //Stop duplicate insertion attempts
			{
				m_children.insert( (TValue)child ); //Store the child in the set
				if (child->getParent() != this )
					child->setParent( (TNode*)this ); //Set the childs parent to this
			}
		}

		/** \brief Removes a child node
		*/
		void removeChild( TNode* child )
		{
			iterator find = m_children.find( child );

			if ( find != m_children.end() ) //Make sure the child exists
			{
				m_children.erase( find ); //Remove the child
				if ( child->getParent() == this )
					child->setParent( 0 ); //Remove the parent ref from the child
			}
		}

		/** \brief Removes all child nodes
		*/
		void removeAllChildred()
		{			
			for (iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
			{
				(*iChild)->m_parent = 0; //Remove the parent ref from the child
				(*iChild) = 0;
			}
			m_children.clear();
		}

		/** \brief Sets the parent node
		\remarks
			Passing in 0 will remove the parent
		*/
		void setParent( TNode* parent )
		{
			if ( m_parent == parent ) //No need
				return; //No need

			if ( m_parent )
			{
				TNode* oldParent = m_parent;
				m_parent = 0;
				oldParent->removeChild( (TNode*)this );
			}

			m_parent = parent;

			if ( m_parent ) //If the new parent is valid
			{
				m_parent->addChild( (TNode*)this ); //Set the parents child to this
			}
		}

		/** \brief Get a pointer to the parent
		*/
		inline TNode* getParent() const 
		{ return m_parent; }

		/** \brief Gets the number of child nodes
		*/
		Count childCount() const 
		{ return (Count)m_children.size(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		iterator childBegin()
		{ return m_children.begin(); }
		const_iterator childBegin() const
		{ return m_children.begin(); }
		ReverseIterator childRBegin()
		{ return m_children.rbegin(); }
		ConstReverseIterator childRBegin() const
		{ return m_children.rbegin(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		iterator childEnd()
		{ return m_children.end(); }
		const_iterator childEnd() const
		{ return m_children.end(); }
		ReverseIterator childREnd()
		{ return m_children.rend(); }
		ConstReverseIterator childREnd() const
		{ return m_children.rend(); }

	protected:
		NodeSet m_children; ///< Children set
		TNode* m_parent; ///< Parent node. NOTE: NOt reference counted so if a parent is destroid all its children should be
	};

} // END: frx

#endif
