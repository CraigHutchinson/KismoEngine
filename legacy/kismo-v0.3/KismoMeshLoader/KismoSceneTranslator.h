/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\KismoSceneTranslator.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef Kismo_SCENETRANSLATOR_H
#define Kismo_SCENETRANSLATOR_H

#include "Framework/Colour.h"
#include "Gfx/SceneTranslator.h"
#include "Platform/Platform.h"

#include "DirectedEdge.h"
#include "KismoFileParser.h"

//forward declarations
//namespace frx
//{
//	class BoundingBox;
//	class BoundingSphere;
//}

namespace rage
{
	/** \brief Implementation of the Kismo file format loader for scene graphs
	*/
	class KismoSceneTranslator : public gfx::SceneTranslator
	{
	public:

		/** \brief  \copydoc gfx::SceneTranslator::SceneTranslator()
		*/
		KismoSceneTranslator();

		/** \brief \copydoc gfx::SceneTranslator::writer()
		*/
		virtual bool writer( const gfx::Scene::Ptr scene, OutStream& stream );

		/** \brief \copydoc gfx::SceneTranslator::reader()
		*/
		virtual bool reader( gfx::Scene::Ptr scene, InStream& stream );

		virtual eFileMatch identifyFile( const String& filename, const Char* buffer, Size size ) const;

	private:

		//bool hierarchyReader( KismoTempScene& mesh, InStream& stream ){};
		//bool hierarchySceneReader( KismoTempScene& mesh, InStream& stream ){};
		//bool hierarchyTransformReader( KismoTempScene& mesh, InStream& stream ){};
		//bool materialsReader( KismoTempScene& mesh, InStream& stream ){};
		//	bool materialReader( KismoTempScene& mesh, InStream& stream ){};
		//bool meshesReader( KismoTempScene& mesh, InStream& stream );
		//	bool meshReader( KismoTempScene& mesh, InStream& stream );
		//		bool positionsReader( KismoTempScene& mesh, InStream& stream );
		//		bool UVSetReader( KismoTempScene& mesh, InStream& stream ){};
		//		bool normalsReader( KismoTempScene& mesh, InStream& stream );
		//		bool faceSetReader( KismoTempScene& mesh, InStream& stream );

		//bool boundingBoxReader( InStream& stream, frx::BoundingBox& bb ){};
		//bool boundingSphereReader( InStream& stream, frx::BoundingSphere& bs ){};
		//bool colourReader( InStream& stream, frx::Colour4& c ){};
		//bool specularColourReader( InStream& stream, frx::Colour4& c, Real& cosPower ){};

		///** \brief takes the temp mesh data and converts it for the target mesh
		//*/
		//Bool createScene( gfx::Scene::Ptr omesh, KismoTempScene& tmesh );
	};



} //END: rage


#endif