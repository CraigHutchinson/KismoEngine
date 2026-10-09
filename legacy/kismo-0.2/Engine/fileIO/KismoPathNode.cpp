/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoPathNode.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoPathNode.h"

namespace kismo
{
	namespace fio
	{
		PathNode::PathNode( const k_String& fullPath, ke_PathNodeType nodeType ) :
			m_parentNode( 0 ), m_nodeType( nodeType )
		{
			k_String tpath = slashify( fullPath, nodePathType( nodeType ) ); //Affirm the path to certain rules

			k_String::Index_T lastSlash = tpath.findFromEnd( platform::kc_Path_Seperator ) + 1;//Get the index for last directory seperator

			tpath.right( m_name, lastSlash ); //Name
			tpath.left( m_path, lastSlash ); //Path
		}	

		PathNode::PathNode( const k_String& nodeName, PathNode::Ptr parentNode, ke_PathNodeType nodeType ) :
			m_parentNode( parentNode ), m_name( nodeName ), m_nodeType( nodeType )
		{}
	}
}
