/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\KismoMeshTranslator.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoMeshTranslator.h"
#include "Framework/Log.h"
#include "Gfx/Mesh.h"
#include "Gfx/Renderer.h"

//------------------------------------------
#define AsciiFileHeader _T("Kismo Ascii Scene Object v1.00")
#define AsciiComment _T("//")
#define AsciiCommentHeader AsciiComment _T("\t")
#define ObjectHierarchyHeader _T("hierarchy")
#define MeshesHeader _T("meshes")
#define MaterialsHeader _T("materials")

#define TransformHeader _T("transform")
#define LODGroupHeader _T("lod_group")
#define MeshHeader _T("mesh")
#	define MeshUVSetHeader _T("uv_set")
#	define MeshPositionsHeader _T("positions")
#	define MeshNormalsHeader _T("normals")
#	define MeshFaceSetHeader _T("face_set")
#	define MeshBoundingBoxHeader _T("bounding_box")
#	define MeshBoundingSphereHeader _T("bounding_sphere")
#define SkinClusterHeader _T("skin_cluster")
#define JointHeader _T("joint")

#define MaterialHeader _T("material")
#	define MaterialAmbientHeader _T("ambient")
#	define MaterialDiffuseHeader _T("diffuse")
#	define MaterialSpecularHeader _T("specular")
#	define MaterialTransparencyHeader _T("transparency")

#define OpenLevelHeader _T("{")
#define CloseLevelHeader _T("}")

#define ParamCount _T("count")
#define ParamMaterial _T("material")
#define ParamSides _T("sides")
#define ParamID _T("id")
#define ParamType _T("type")

#define StartNodeParam _T("(")
#define EndNodeParam _T(")")
//------------------------------------------

namespace rage
{
	KismoMeshTranslator::KismoMeshTranslator() :
		MeshTranslator( true, false, _T("rmf") )
	{
	}

	frx::ResourceTranslator::eFileMatch KismoMeshTranslator::identifyFile( const String& filename, const Char* buffer, Size size ) const
	{ 
		String header( AsciiCommentHeader AsciiFileHeader );

		if ( header == String( buffer ).left( header.length() ) )
			return FileMatch_IsMyFileType;
		else
			return FileMatch_NotMyFileType;
	}

	bool KismoMeshTranslator::writer( const gfx::Mesh* mesh, OutStream& stream ) 
	{ 
		return false; 
	}

	bool getNextWord( String& buff, InStream& stream )
	{
		Bool cont = false;
		do
		{
			cont = false;
			stream >> buff;
			

			if ( buff == AsciiComment )
			{
				stream.ignore();
				cont = true; //Should continue!
			}

			if (	 buff == EndNodeParam // )
					|| buff == EndNodeParam OpenLevelHeader // ){
					|| buff == CloseLevelHeader // } 
				)
				return false; ///Stop processing headers now
		}
		while ( cont );

		//TODO: Update mesh loader to use io::FileParser!
		buff.trim( _T('"') );
		return !(!stream);
	}

	bool KismoMeshTranslator::reader( gfx::Mesh* omesh, InStream& stream ) 
	{
		if ( !stream  )
			return false;

		KismoTempMesh tmesh;

		String buff( 512 );
		while ( stream )
		{	
			getNextWord( buff, stream );

			if ( buff == ObjectHierarchyHeader StartNodeParam )
			{
				logComment( "Hierarchy found" );
				while ( getNextWord( buff, stream ) ){}
			}
			else			
			if ( buff == MaterialsHeader StartNodeParam )
			{
				logComment( "Materials found" );
				while ( getNextWord( buff, stream ) ){}
			}
			else			
			if ( buff == TransformHeader StartNodeParam )
			{
				logComment( "Transform found" );
				while ( getNextWord( buff, stream ) ){}
			}
			else			
			if ( buff == LODGroupHeader StartNodeParam )
			{
				logComment( "LODGroup found" );
				while ( getNextWord( buff, stream ) ){}
			}
			else			
			if ( buff == MeshesHeader StartNodeParam )
			{
				logComment( "Meshes found" );
				meshesReader( tmesh, stream );
			}
			else
			if ( buff == MaterialHeader StartNodeParam )
			{
				logComment( "Material found" );
				while ( getNextWord( buff, stream ) ){}
			}
		}

		return createMesh( omesh, tmesh );
	}

	bool KismoInlineMeshTranslator::reader( gfx::Mesh* omesh, InStream& stream ) 
	{

		KismoTempMesh tmesh;

		assert( m_inStream );
		if ( !m_inStream )
		{
			return false;
		}

		//Read the mesh
		meshReader( tmesh, *m_inStream );


		bool ret = createMesh( omesh, tmesh );

		omesh->setLoader( 0 ); //< We can't reload as the stream will become invalid soon!
		delete this; ///< Delete this!
		return ret;

	}

	Bool KismoMeshTranslator::createMesh( gfx::Mesh* omesh, KismoTempMesh& tmesh )
	{
		gfx::Buffer::PtrArray buffers;
		buffers.reserve( 3 + tmesh.faceSets.size() );

		//Add the positions buffer
		if ( tmesh.positions.size() )
		{
			buffers.push_back(
				_Renderer.createBuffer( gfx::Buffer::Purpose_Vertex, PositionStruct<>::getDefinition(), 
				tmesh.positions.size(), (Byte*)&tmesh.positions.front() ) );
		}
		
		if ( tmesh.normals.size() )
		{
			buffers.push_back( 
				_Renderer.createBuffer( gfx::Buffer::Purpose_Vertex, NormalStruct::getDefinition(), 
				tmesh.normals.size(), (Byte*)&tmesh.normals.front() ) );
		}

		//Add the uv buffer
		if ( tmesh.uvs.size() )
		{
			buffers.push_back( 
				_Renderer.createBuffer( gfx::Buffer::Purpose_Vertex, UVStruct::getDefinition(), 
				tmesh.uvs.size(), (Byte*)&tmesh.uvs.front() ) );
		}
		
		if ( tmesh.faceSets.size() )
		{
			String::Array materials;
			materials.reserve( tmesh.faceSets.size() ); //Reserve space for speed

			for ( KismoTempMesh::FaceArray::const_iterator iSet = tmesh.faceSets.begin(); iSet != tmesh.faceSets.end(); ++iSet )
			{
				materials.push_back( iSet->material );

				//Add the face index buffer
				//TODO: Multiple materials!
				buffers.push_back( _Renderer.createBuffer( gfx::Buffer::Purpose_Index, Vertex_UNV<>::getDefinition(), 
					iSet->vertices.size(), (Byte*)&(iSet->vertices.front()) ) );
			}
		
			omesh->setMaterials( materials );
		}

		assert( buffers.size() );

		omesh->setBuffers( buffers );
		return true;
	}

	bool KismoMeshTranslator::meshesReader( KismoTempMesh& mesh, InStream& stream )
	{		
		String buff( 512 );
		//Read the meshes header
		Count meshCount = 0;
		while ( getNextWord( buff, stream ) )
		{
			if ( buff == ParamCount )
			{
				getNextWord( buff, stream );
				meshCount = buff.asInt32();
				logComment( "Loading " << meshCount << " meshes" );
			}
		}

		//Read all the meshes
		Count meshesFound = 0;
		while ( getNextWord( buff, stream ) )
		{
			if ( buff == MeshHeader StartNodeParam )
			{
				++meshesFound;
				logComment( "Mesh " << meshesFound );
				if ( meshesFound == 1 )
					meshReader( mesh, stream );
			}
		}

		if ( meshesFound != meshCount )
		{
			logRecoverable( "The number of meshes found (" 
				<< meshesFound << ") and the number specified (" 
				<< meshCount << ") in the file do not match!" );
		}

		return true;
	}

		
	bool KismoMeshTranslator::meshReader( KismoTempMesh& mesh, InStream& stream )
	{
		String buff( 512 );

		//String meshId;
		////Read the mesh params
		//while ( getNextWord( buff, stream ) )
		//{
		//	if ( buff == ParamID )
		//		stream >> meshId;
		//}

		//Read the mesh contents
		while ( getNextWord( buff, stream ) 
			|| buff == EndNodeParam
			|| buff == EndNodeParam OpenLevelHeader  )
		{		
			if ( buff == MeshUVSetHeader StartNodeParam )
			{
				logComment( "Mesh UVSet found" );
				while ( getNextWord( buff, stream ) ){}
				while ( getNextWord( buff, stream ) ){}
			}
			else			
			if ( buff == MeshPositionsHeader StartNodeParam )
			{
				logComment( "Mesh Positions found" );
				positionsReader( mesh, stream );
			}
			else				
			if ( buff == MeshNormalsHeader StartNodeParam )
			{
				logComment( "Mesh Normals found" );
				normalsReader( mesh, stream );
			}
			else					
			if ( buff == MeshFaceSetHeader StartNodeParam )
			{
				logComment( "Mesh FaceSet found" );
				if ( !faceSetReader( mesh, stream ) )
				{
					assert( false );
				}
			}
			else					
			if ( buff == MeshBoundingBoxHeader StartNodeParam )
			{
				logComment( "Mesh BoundingBox found" );
				while ( getNextWord( buff, stream ) ){}
			}
			else					
			if ( buff == MeshBoundingSphereHeader StartNodeParam )
			{
				logComment( "Mesh BoundingSphere found" );
				while ( getNextWord( buff, stream ) ){}
			}
		}


		return true;
	}
	
	bool KismoMeshTranslator::positionsReader( KismoTempMesh& mesh, InStream& stream )
	{
		String buff( 512 );

		Count positionCount = 0;
		while ( getNextWord( buff, stream ) )
		{
			if ( buff == ParamCount )
			{
				getNextWord( buff, stream );
				positionCount = (Count)buff.asInt32(); //get the string as a number
				logComment( "Position count = " << positionCount );
			}
			else
			if ( buff == ParamType )
			{
				String type;
				getNextWord( type, stream );
				if ( type != _T("float3") )
				{
					logError( "Mesh positions type is invalid - '" << type );
					return false;
				}
				else
					logComment( "Position type = " << type );

			}
		}

		//Make space to store the positions
		assert( mesh.positions.empty() );
		mesh.positions.resize( positionCount );

		//Read in all the posiitons
		for ( Count i  = 0; i < positionCount; ++ i )
		{
			stream >> mesh.positions[ i ].vec;
			mesh.positions[ i ].iedge_next = 0;
		}
		
		///Should be the section end terminator after the positions
		if ( getNextWord( buff, stream ) ) 
			return false;

		return true;
	}
	
	bool KismoMeshTranslator::normalsReader( KismoTempMesh& mesh, InStream& stream )
	{
		String buff( 512 );

		Count normalCount = 0;
		while ( getNextWord( buff, stream ) )
		{
			if ( buff == ParamCount )
			{
				getNextWord( buff, stream );
				normalCount = (Count)buff.asInt32(); //get the string as a number
				logComment( "Normal count = " << normalCount );
			}
			else
			if ( buff == ParamType )
			{
				String type;
				getNextWord( type, stream );
				if ( type != _T("float3") )
				{
					logError( "Mesh normals type is invalid - '" << type );
					return false;
				}
				else
					logComment( "Normal type = " << type );

			}
		}

		//Make space to store the normals
		assert( mesh.normals.empty() );
		mesh.normals.resize( normalCount );

		//Read in all the posiitons
		for ( Count i  = 0; i < normalCount; ++ i )
		{
			stream >> mesh.normals[ i ].vec;
		}
		
		///Should be the section end terminator after the normals
		if ( getNextWord( buff, stream ) ) 
			return false;

		return true;
	}

	bool KismoMeshTranslator::faceSetReader( KismoTempMesh& mesh, InStream& stream )
	{
		String buff( 512 );

		mesh.faceSets.resize( mesh.faceSets.size() + 1 );
		KismoTempMesh::FaceSet& faceSet = mesh.faceSets.back();

		Count vertexCount = 0;
		while ( getNextWord( buff, stream ) )
		{
			if ( buff == ParamCount )
			{
				getNextWord( buff, stream );
				vertexCount = (Count)buff.asInt32(); //get the string as a number
				logComment( "FaceSet triangle count = " << vertexCount );
			}
			else
			if ( buff == ParamMaterial )
			{
				faceSet.material.reserve( 64 );
				getNextWord( faceSet.material, stream ); //get the name of the material
				logComment( "FaceSet material = " << faceSet.material );
			}
			else
			if ( buff == ParamType )
			{
				String type;
				getNextWord( type, stream );
				if ( type != _T("U_N_V") )
				{
					logError( "Mesh faceSet type is invalid - '" << type );
					return false;
				}
				else
					logComment( "FaceSet type = " << type );
			}
			else
			if ( buff == ParamSides )
			{				
				getNextWord( buff, stream );
				faceSet.sides = (Count)buff.asInt32(); //get the string as a number
				if ( faceSet.sides != 3 )
				{
					logError( "FaceSet sides are invalid = " << faceSet.sides );
					return false;
				}
				else
					logComment( "FaceSet sides = " << faceSet.sides );

			}
		}

		vertexCount *= faceSet.sides;

		//Make space to store the vertex indices
		assert( faceSet.vertices.empty() );
		faceSet.vertices.resize( vertexCount );

		//Read in all the posiitons
		for ( Count i  = 0; i < vertexCount; ++ i )
		{
			stream	>> faceSet.vertices[ i ].iuv
					>> faceSet.vertices[ i ].inormal
					>> faceSet.vertices[ i ].ipos;
			//assert( faceSet.vertices[ i ].iuv < mesh.uvs.size() );
			assert( faceSet.vertices[ i ].inormal < mesh.normals.size() );
			assert( faceSet.vertices[ i ].ipos < mesh.positions.size() );
		}
		
		///Should be the section end terminator after the positions
		if ( getNextWord( buff, stream ) ) 
			return false;

		return true;
	}
};