/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoPathNode.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_PATHNODE_H
#define _KISMO_PATHNODE_H

#include "fileIO\KismoDataStream.h"
#include "fileIO\KismoFilePlatform.h"
#include "framework/KismoSmartPtr.h"

namespace kismo
{
	namespace fio
	{

		class _KismoExport PathNode : public frx::IRefCountImpl< PathNode >
		{
		public:			
			typedef frx::SmartPtr< PathNode > Ptr;
			typedef k_Vector< PathNode::Ptr > List;
		public:

			enum ke_PathNodeType
			{
				kc_PathNodeType_Directory,
				kc_PathNodeType_File,
				kc_PathNodeType_Archive,
			};

			inline platform::ke_PathType nodePathType( ke_PathNodeType nodeType )
			{
				switch( nodeType )
				{
					case kc_PathNodeType_File:
						return platform::kc_PathType_File;

					case kc_PathNodeType_Directory:
					case kc_PathNodeType_Archive:
						return platform::kc_PathType_Directory;

					default:
						return platform::kc_PathType_Unknown;
				}
			}

		protected: //Protected constructors to force construction by a derived class
			
			PathNode( const k_String& fullPath, ke_PathNodeType nodeType );

			PathNode( const k_String& nodeName, PathNode::Ptr parentNode, ke_PathNodeType nodeType );
	

		public:	
			virtual ~PathNode(){}

			//Get the full name of the node
			const k_String& name() const { return m_name; }

			//get the full path of this node
			const k_String fullPath() const { return path() + m_name; }

			//Get the path of the parent node
			const k_String path() const 
			{ 
				if ( isValidPointer( m_parentNode ) )
					return m_parentNode->fullPath();
				else
					return m_path;
			}

			//get a pointer to the parent node
			PathNode::Ptr parent() { return m_parentNode; }

			//Check the path nodes type
			ke_PathNodeType nodeType() { return m_nodeType; }
			k_Bool isDirectory() { return m_nodeType == kc_PathNodeType_Directory || m_nodeType == kc_PathNodeType_Archive; }
			k_Bool isFile() { return m_nodeType == kc_PathNodeType_File; }
			k_Bool isArchive() { return m_nodeType == kc_PathNodeType_Archive; }

		protected:
			k_String m_name;

			ke_PathNodeType m_nodeType; //Directory, archive, file etc..
			
			PathNode::Ptr m_parentNode; //Pointer to the parent if it exists already
			k_String m_path; //Parent path if the parent does not exist in the file manager
		};


	} //END: fileio

} //END: kismo

#endif