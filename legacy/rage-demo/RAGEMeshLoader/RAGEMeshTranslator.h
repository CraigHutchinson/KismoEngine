/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\RAGEMeshTranslator.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef RAGE_MESHTRANSLATOR_H
#define RAGE_MESHTRANSLATOR_H

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


/** \brief RAGE namespace contains RAGE engine specific scene and mesh translators
*/
namespace rage
{
	/** \brief Temporary mesh structure used when loading  a mesh with RAGEMeshTranslator
		\remarks Should be removed later
	*/
	struct RAGETempMesh
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
	
	/** \brief Mesh trnaslator for the RAGE engine default fiel format
	*/
	class RAGEMeshTranslator : public gfx::MeshTranslator
	{
	public:

		/** \brief  \copydoc gfx::MeshTranslator::MeshTranslator()
		*/
		RAGEMeshTranslator();

		/** \brief \copydoc gfx::MeshTranslator::writer()
		*/
		virtual bool writer( const gfx::Mesh* mesh, OutStream& stream );

		/** \brief \copydoc gfx::MeshTranslator::reader()
		*/
		virtual bool reader( gfx::Mesh* mesh, InStream& stream );

		virtual eFileMatch identifyFile( const String& filename, const Char* buffer, Size size ) const;

	protected:

		bool hierarchyReader( RAGETempMesh& mesh, InStream& stream ){};
		bool hierarchyMeshReader( RAGETempMesh& mesh, InStream& stream ){};
		bool hierarchyTransformReader( RAGETempMesh& mesh, InStream& stream ){};
		bool materialsReader( RAGETempMesh& mesh, InStream& stream ){};
			bool materialReader( RAGETempMesh& mesh, InStream& stream ){};
		bool meshesReader( RAGETempMesh& mesh, InStream& stream );
			bool meshReader( RAGETempMesh& mesh, InStream& stream );
				bool positionsReader( RAGETempMesh& mesh, InStream& stream );
				bool UVSetReader( RAGETempMesh& mesh, InStream& stream ){};
				bool normalsReader( RAGETempMesh& mesh, InStream& stream );
				bool faceSetReader( RAGETempMesh& mesh, InStream& stream );

		bool boundingBoxReader( InStream& stream, frx::BoundingBox& bb ){};
		bool boundingSphereReader( InStream& stream, frx::BoundingSphere& bs ){};
		bool colourReader( InStream& stream, frx::Colour4& c ){};
		bool specularColourReader( InStream& stream, frx::Colour4& c, Real& cosPower ){};

		/** \brief takes the temp mesh data and converts it for the target mesh
		*/
		Bool createMesh( gfx::Mesh* omesh, RAGETempMesh& tmesh );
	};

	/** \brief Specialisation of RAGEMeshTranslator to allow loading of inline meshes in a scene file
	*/
	class RAGEInlineMeshTranslator : public RAGEMeshTranslator
	{
	public:

		RAGEInlineMeshTranslator( InStream* inStream ) :
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