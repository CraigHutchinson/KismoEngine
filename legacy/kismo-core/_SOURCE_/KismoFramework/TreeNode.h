/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\TreeNode.h
 * Status: current (newest copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_TREENODE_H
#define KISMOFRAMEWORK_TREENODE_H

#include "Types.h"
#include "SmallSet.h"

namespace kismo
{		
	/** \brief Graph node defines a node of a graph which may have multiple children
		but only 1 parent
		\todo This should derive/specialise GraphNode
	*/
	template < class TNode > 
	class /*KISMOFRAMEWORK_EXPORT*/ TreeNode
	{
	public:	

		typedef SmallSet< TNode* > NodeSet; ///< Typedef the node set as a vector		

		typedef typename NodeSet::const_iterator ConstIterator; ///< Typedef the node set Iterator
		typedef typename NodeSet::const_reverse_iterator ConstReverseIterator; ///< Typedef the node set Iterator
		typedef typename NodeSet::iterator Iterator; ///< Typedef the node set Iterator
		typedef typename NodeSet::reverse_iterator ReverseIterator; ///< Typedef the node set Iterator

	public:
		
		/** \brief Constructor 
		*/
		TreeNode() :
			m_parent( 0 )
		{}

		/** \brief Virtual destructor 
			\warning Children are not automatically destroid and if they are nto deleted a memory leak may occur
		*/
		virtual ~TreeNode()
		{
			removeAllChildred(); ///Remove any children linking to this node
			setParent( 0 ); //Remove the parents link to this node
		}

		/** \brief Adds a new child graph node
			\remarks Will automatically ignore duplicate entries
		*/
		bool addChild( TNode* child )
		{
			Assert( child );
			if (child->getParent() == this )
				return true;

			assert( m_children.find( child ) != m_children.end() ); //Stop duplicate insertion attempts

			//TODO: this 'insert()' ensures the node doesn't exist when we know it doesn't. Potentially need to
			//sort some clean way of changing this behaviour
			m_children.insert( (TNode*)child ); //Store the child in the set
			child->setParent( (TNode*)this ); //Set the childs parent to this
			return true;
		}

		/** \brief Removes a child node
		*/
		void removeChild( TNode* child )
		{
			Assert( child );
			Iterator find = findChild( child );

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
			for (Iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
				(*iChild)->m_parent = 0; //Remove the parent ref from the child

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

		/** \brief Returns the Iterator for the first child used for iterating the nodes
		*/
		Iterator childBegin()
		{ return m_children.begin(); }

		ConstIterator childBegin() const
		{ return m_children.begin(); }

		ReverseIterator childRBegin()
		{ return m_children.rbegin(); }

		ConstReverseIterator childRBegin() const
		{ return m_children.rbegin(); }

		/** \brief Returns the Iterator for the first child used for iterating the nodes
		*/
		Iterator childEnd()
		{ return m_children.end();  }

		ConstIterator childEnd() const
		{ return m_children.end(); }

		ReverseIterator childREnd()
		{ return m_children.rend(); }

		ConstReverseIterator childREnd() const
		{ return m_children.rend(); }

		Iterator findChild( TNode* child )
		{ Assert(child); return m_children.find( child ); }

		ConstIterator findChild( TNode* child ) const
		{ Assert(child); return m_children.find( child ); }

		/*
		Iterator findParent( TNode* parent );
		ConstIterator findParent( TNode* parent ) const;*/
	protected:
		NodeSet m_children; ///< Children set
		TNode* m_parent; ///< Parent node. NOTE: NOt reference counted so if a parent is destroid all its children should be
	};

} // END: frx

#endif
