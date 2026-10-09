/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_MayaPlugin\SceneNode.h
 * Status: superseded by kismo-v0.4/KismoGFX/SceneNode.h (same name, content differs)
 */
#ifndef _SCENENODE_H
#define _SCENENODE_H

#include <maya/MString.h>

class SceneNode
{
public:
	SceneNode( const MString& name, SceneNode* parent = 0 ) : 
		m_name( name ), m_parent( parent )
	{}

	void setparent( SceneNode* parent = 0 ) { m_parent = parent; }

protected:

	MString m_name; //Name of the node
	SceneNode* m_parent;

};

class TransformNode : public SceneNode
{
public:
	TransformNode( const MString& name, SceneNode* parent = 0 ) : 
		SceneNode( name, parent )
	{}

protected:

};

#endif