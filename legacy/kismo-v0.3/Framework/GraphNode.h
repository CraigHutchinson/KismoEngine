/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\GraphNode.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/GraphNode.h (same name, content differs)
 */
#ifndef FRX_GRAPHNODE_H
#define	FRX_GRAPHNODE_H

#include "Types.h"
#include "SmallSet.h"

namespace frx
{		
	/** \brief Grpha node defines a node of a graph which may have multiple children
		and/or multiple parent nodes
	*/
	template < class TNode, class TSet = SmallSet< TNode* > , class TRemoveSet = Set< TNode* > > class GraphNode
	{
	public:	

		typedef TSet NodeSet; ///< Typedef the node set
		typedef TRemoveSet NodeRemoveSet; ///< Type of node set to use for the remove list when destroying the graph

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
			if ( removed.find( (TNode*)this ) == removed.end() )
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
		void addChild( TNode* child )
		{
			//if ( m_children.find( child ) == m_children.end() ) //Stop duplicate insertion attempts
			{
				//child->addParent( (TNode*)this ); //Set the childs parent to this
				child->m_parents.insert( (TNode*)this ); //Store the parent in the set
				m_children.insert( child ); //Store the child in the set
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
				child->m_parents.erase( child->m_parents.find( (TNode*)this ) );
				//child->removeParent( (TNode*)this ); //Remove the parent ref from the child
			}
		}

		/** \brief Removes all child nodes
		*/
		void removeAllChildred()
		{			
			for (iterator iChild = childBegin(); iChild != childEnd(); ++iChild/* = childBegin()*/ )
			{
				(*iChild)->m_parents.erase( (*iChild)->m_parents.find( (TNode*)this ) );
				//(*iChild)->removeParent( (TNode*)this ); //Remove the parent ref from the child
			}
			m_children.clear();
		}

		/** \brief Adds a parent node
		*/
		void addParent( TNode* parent )
		{
			//if ( m_parents.find( parent ) == m_parents.end() ) //Stop duplicate insertion attempts
			{
				m_parents.insert( parent ); //Store the parent in the set
				parent->m_children.insert( parent ); //Store the parent in the set				
				//parent->addChild( (TNode*)this ); //Set the parents parent to this
			}
		}

		/** \brief Removes a parent node
		*/
		void removeParent( TNode* parent )
		{
			iterator find = m_parents.find( (TNode*)parent );

			if ( find != m_parents.end() ) //Make sure the parent exists
			{
				m_parents.erase( find ); //Remove the parent
				parent->m_children.erase( parent->m_children.find( (TNode*)this ) );
				//parent->removeChild( (TNode*)this ); //Remove the parent ref from the parent
			}
		}

		/** \brief Removes all child nodes
		*/
		void removeAllParents()
		{			
			for (iterator iParent = parentBegin(); iParent != parentEnd(); ++iParent/* = parentBegin()*/ )
			{
				(*iParent)->m_children.erase( (*iParent)->m_children.find( (TNode*)this ) );
				//(*iParent)->removeChild( (TNode*)this ); //Remove the child ref from the parent
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
			removed.insert( (TNode*)this );	
			
			for ( iterator iChild = m_children.begin(); iChild != m_children.end(); iChild = m_children.begin() )
				if ( removed.find( (TNode*)(*iChild) ) == removed.end() )
					(*iChild)->destoryGraphImpl( removed );
				else
					m_children.erase( iChild );
			
			for ( iterator iParent = m_parents.begin(); iParent != m_parents.end(); iParent = m_parents.begin() )
				if ( removed.find( (TNode*)(*iParent) ) == removed.end() )
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
