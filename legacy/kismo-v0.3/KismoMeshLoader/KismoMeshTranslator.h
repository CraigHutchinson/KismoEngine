/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\KismoMeshTranslator.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef Kismo_MESHTRANSLATOR_H
#define Kismo_MESHTRANSLATOR_H

#include "Framework/Colour.h"
#include "Gfx/MeshTranslator.h"
#include "Platform/Platform.h"

#include "DirectedEdge.h"

//forward declarations
namespace frx
{
	class BoundingBox;
	class BoundingSphere;
}


/** \brief Kismo namespace contains Kismo engine specific scene and mesh translators
*/
namespace rage
{
	/** \brief Temporary mesh structure used when loading  a mesh with KismoMeshTranslator
		\remarks Should be removed later
	*/
	struct KismoTempMesh
	{			
		Vector< DirectedEdge_Full<> > edges;
		Vector< PositionStruct<> > positions;
		Vector< NormalStruct > normals;
		Vector< UVStruct > uvs;

		struct FaceSet
		{
			Count sides; ///< Nuber of sides on the faces
			String material; ///< name of material used
			Vector< Vertex_UNV<> > vertices; ///< Vertex indexes
		};
		typedef Vector< FaceSet > FaceArray;
		FaceArray faceSets;
	};
	
	/** \brief Mesh trnaslator for the Kismo engine default fiel format
	*/
	class KismoMeshTranslator : public gfx::MeshTranslator
	{
	public:

		/** \brief  \copydoc gfx::MeshTranslator::MeshTranslator()
		*/
		KismoMeshTranslator();

		/** \brief \copydoc gfx::MeshTranslator::writer()
		*/
		virtual bool writer( const gfx::Mesh* mesh, OutStream& stream );

		/** \brief \copydoc gfx::MeshTranslator::reader()
		*/
		virtual bool reader( gfx::Mesh* mesh, InStream& stream );

		virtual eFileMatch identifyFile( const String& filename, const Char* buffer, Size size ) const;

	protected:

		bool hierarchyReader( KismoTempMesh& mesh, InStream& stream ){};
		bool hierarchyMeshReader( KismoTempMesh& mesh, InStream& stream ){};
		bool hierarchyTransformReader( KismoTempMesh& mesh, InStream& stream ){};
		bool materialsReader( KismoTempMesh& mesh, InStream& stream ){};
			bool materialReader( KismoTempMesh& mesh, InStream& stream ){};
		bool meshesReader( KismoTempMesh& mesh, InStream& stream );
			bool meshReader( KismoTempMesh& mesh, InStream& stream );
				bool positionsReader( KismoTempMesh& mesh, InStream& stream );
				bool UVSetReader( KismoTempMesh& mesh, InStream& stream ){};
				bool normalsReader( KismoTempMesh& mesh, InStream& stream );
				bool faceSetReader( KismoTempMesh& mesh, InStream& stream );

		bool boundingBoxReader( InStream& stream, frx::BoundingBox& bb ){};
		bool boundingSphereReader( InStream& stream, frx::BoundingSphere& bs ){};
		bool colourReader( InStream& stream, frx::Colour4& c ){};
		bool specularColourReader( InStream& stream, frx::Colour4& c, Real& cosPower ){};

		/** \brief takes the temp mesh data and converts it for the target mesh
		*/
		Bool createMesh( gfx::Mesh* omesh, KismoTempMesh& tmesh );
	};

	/** \brief Specialisation of KismoMeshTranslator to allow loading of inline meshes in a scene file
	*/
	class KismoInlineMeshTranslator : public KismoMeshTranslator
	{
	public:

		KismoInlineMeshTranslator( InStream* inStream ) :
			m_inStream( inStream )
		{}

		/** \brief \copydoc gfx::MeshTranslator::reader()
		*/
		virtual bool reader( gfx::Mesh* mesh, InStream& stream );

	private:

		InStream* m_inStream; ///< The input stream
	};

} //END: rage


#endif