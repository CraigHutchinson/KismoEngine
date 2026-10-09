/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\GraphNode.h
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

#ifndef KISMOFRAMEWORK_GRAPHNODE_H
#define KISMOFRAMEWORK_GRAPHNODE_H

#include "Types.h"
#include "SmallSet.h"

namespace kismo
{		
	/** Graph node defines a node of a graph which may have multiple children
		and/or multiple parent nodes
	*/
	template < class _NodeT, class _ContainerT = SmallSet< _NodeT* > > 
	class GraphNode
	{
	public:	

		typedef _ContainerT NodeSet; ///< Typedef the node set
		typedef Set< _NodeT* > NodeRemoveSet; ///< Type of node set to use for the remove list when destroying the graph

		typedef typename NodeSet::const_iterator const_iterator; ///< Typedef the node set iterator
		typedef typename NodeSet::iterator iterator; ///< Typedef the node set iterator

	public:
		
		/** \brief Constructor 
		*/
		GraphNode() 
		{}

		/** \brief Virtual destructor 
		*/
		virtual ~GraphNode()
		{
			removeAllChildred(); ///Remove any children linking to this node
			removeAllParents(); ///Remove any children linking to this node
		}

		/** \brief Destroy the graph freeing any memory it may be using
			\param[in] removed A set of removed nodes usefull if the graph is being deleted from multiple known
			nodes
		*/
		void destroyGraph( NodeRemoveSet& removed )
		{ 
			if ( removed.find( (_NodeT*)this ) == removed.end() )
				destoryGraphImpl( removed );
		}

		/** \brief Destroy the graph freeing any memory it may be using
			\remarks Will delete only connected nodes. If any are un-referenced a memory leak can occur
		*/
		void destroyGraph()
		{ NodeRemoveSet removed; destoryGraphImpl( removed ); }

		/** \brief Adds a new child graph node
			\remarks Will automatically ignore duplicate entries
		*/
		void addChild( _NodeT* child )
		{
			//if ( m_children.find( child ) == m_children.end() ) //Stop duplicate insertion attempts
			{
				//child->addParent( (_NodeT*)this ); //Set the childs parent to this
				child->m_parents.insert( (_NodeT*)this ); //Store the parent in the set
				m_children.insert( child ); //Store the child in the set
			}
		}

		/** \brief Removes a child node
		*/
		void removeChild( _NodeT* child )
		{
			iterator find = m_children.find( child );

			if ( find != m_children.end() ) //Make sure the child exists
			{
				m_children.erase( find ); //Remove the child
				child->m_parents.erase( child->m_parents.find( (_NodeT*)this ) );
				//child->removeParent( (_NodeT*)this ); //Remove the parent ref from the child
			}
		}

		/** \brief Removes all child nodes
		*/
		void removeAllChildred()
		{			
			for (iterator iChild = childBegin(); iChild != childEnd(); ++iChild/* = childBegin()*/ )
			{
				(*iChild)->m_parents.erase( (*iChild)->m_parents.find( (_NodeT*)this ) );
				//(*iChild)->removeParent( (_NodeT*)this ); //Remove the parent ref from the child
			}
			m_children.clear();
		}

		/** \brief Adds a parent node
		*/
		void addParent( _NodeT* parent )
		{
			//if ( m_parents.find( parent ) == m_parents.end() ) //Stop duplicate insertion attempts
			{
				m_parents.insert( parent ); //Store the parent in the set
				parent->m_children.insert( parent ); //Store the parent in the set				
				//parent->addChild( (_NodeT*)this ); //Set the parents parent to this
			}
		}

		/** \brief Removes a parent node
		*/
		void removeParent( _NodeT* parent )
		{
			iterator find = m_parents.find( (_NodeT*)parent );

			if ( find != m_parents.end() ) //Make sure the parent exists
			{
				m_parents.erase( find ); //Remove the parent
				parent->m_children.erase( parent->m_children.find( (_NodeT*)this ) );
				//parent->removeChild( (_NodeT*)this ); //Remove the parent ref from the parent
			}
		}

		/** \brief Removes all child nodes
		*/
		void removeAllParents()
		{			
			for (iterator iParent = parentBegin(); iParent != parentEnd(); ++iParent/* = parentBegin()*/ )
			{
				(*iParent)->m_children.erase( (*iParent)->m_children.find( (_NodeT*)this ) );
				//(*iParent)->removeChild( (_NodeT*)this ); //Remove the child ref from the parent
			}
			m_parents.clear();
		}


		/** \brief Gets the number of child nodes
		*/
		Count childCount() const 
		{ return (Count)m_children.size(); }

		/** \brief Gets the number of parent nodes
		*/
		Count parentCount() const
		{ return (Count)m_parents.size(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		iterator childBegin()
		{ return m_children.begin(); }
		const_iterator childBegin() const
		{ return m_children.begin(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		iterator childEnd()
		{ return m_children.end(); }
		const_iterator childEnd() const
		{ return m_children.end(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		iterator parentBegin()
		{ return m_parents.begin(); }
		const_iterator parentBegin() const
		{ return m_parents.begin(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		iterator parentEnd()
		{ return m_parents.end(); }
		const_iterator parentEnd() const
		{ return m_parents.end(); }

	protected:
	
		/** \brief Destroy this node and all its parents/children while keeping a record or removed nodes
			to prevent memory access violations
		*/
		void destoryGraphImpl( NodeRemoveSet& removed )
		{
			removed.insert( (_NodeT*)this );	
			
			for ( iterator iChild = m_children.begin(); iChild != m_children.end(); iChild = m_children.begin() )
				if ( removed.find( (_NodeT*)(*iChild) ) == removed.end() )
					(*iChild)->destoryGraphImpl( removed );
				else
					m_children.erase( iChild );
			
			for ( iterator iParent = m_parents.begin(); iParent != m_parents.end(); iParent = m_parents.begin() )
				if ( removed.find( (_NodeT*)(*iParent) ) == removed.end() )
					(*iParent)->destoryGraphImpl( removed );
				else
					m_parents.erase( iParent );
			
			delete this;
		}

	protected:
		NodeSet m_children; ///< Children set
		NodeSet m_parents; ///< Parent set
	};

} // END: frx

#endif
