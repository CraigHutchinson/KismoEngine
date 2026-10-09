/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\RAGESceneTranslator.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef RAGE_SCENETRANSLATOR_H
#define RAGE_SCENETRANSLATOR_H

#include "Framework/Colour.h"
#include "Gfx/SceneTranslator.h"
#include "Platform/Platform.h"

#include "DirectedEdge.h"
#include "RAGEFileParser.h"

//forward declarations
//namespace frx
//{
//	class BoundingBox;
//	class BoundingSphere;
//}

namespace rage
{
	/** \brief Implementation of the RAGE file format loader for scene graphs
	*/
	class RAGESceneTranslator : public gfx::SceneTranslator
	{
	public:

		/** \brief  \copydoc gfx::SceneTranslator::SceneTranslator()
		*/
		RAGESceneTranslator();

		/** \brief \copydoc gfx::SceneTranslator::writer()
		*/
		virtual bool writer( const gfx::Scene::Ptr scene, OutStream& stream );

		/** \brief \copydoc gfx::SceneTranslator::reader()
		*/
		virtual bool reader( gfx::Scene::Ptr scene, InStream& stream );

		virtual eFileMatch identifyFile( const String& filename, const Char* buffer, Size size ) const;

	private:

		//bool hierarchyReader( RAGETempScene& mesh, InStream& stream ){};
		//bool hierarchySceneReader( RAGETempScene& mesh, InStream& stream ){};
		//bool hierarchyTransformReader( RAGETempScene& mesh, InStream& stream ){};
		//bool materialsReader( RAGETempScene& mesh, InStream& stream ){};
		//	bool materialReader( RAGETempScene& mesh, InStream& stream ){};
		//bool meshesReader( RAGETempScene& mesh, InStream& stream );
		//	bool meshReader( RAGETempScene& mesh, InStream& stream );
		//		bool positionsReader( RAGETempScene& mesh, InStream& stream );
		//		bool UVSetReader( RAGETempScene& mesh, InStream& stream ){};
		//		bool normalsReader( RAGETempScene& mesh, InStream& stream );
		//		bool faceSetReader( RAGETempScene& mesh, InStream& stream );

		//bool boundingBoxReader( InStream& stream, frx::BoundingBox& bb ){};
		//bool boundingSphereReader( InStream& stream, frx::BoundingSphere& bs ){};
		//bool colourReader( InStream& stream, frx::Colour4& c ){};
		//bool specularColourReader( InStream& stream, frx::Colour4& c, Real& cosPower ){};

		///** \brief takes the temp mesh data and converts it for the target mesh
		//*/
		//Bool createScene( gfx::Scene::Ptr omesh, RAGETempScene& tmesh );
	};



} //END: rage


#endif