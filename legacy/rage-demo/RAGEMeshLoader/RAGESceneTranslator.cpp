/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\RAGESceneTranslator.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "RAGESceneTranslator.h"
#include "RAGEMeshTranslator.h"
#include "Framework/Log.h"
#include "Gfx/Scene.h"
#include "Gfx/Renderer.h"
#include "Gfx/MeshManager.h"
#include "Gfx/MaterialState.h"
#include "Phys/Actor.h"
#include "Phys/Scene.h"
#include "Phys/MeshShape.h"

//------------------------------------------
#define AsciiFileHeader _T("RAGE Ascii Scene Object v1.00")
#define AsciiComment _T("//")
#define AsciiCommentHeader AsciiComment _T("\t")

#define SceneHeader _T("scene")
#define MeshesHeader _T("meshes")
#define MaterialsHeader _T("materials")

#define TransformHeader _T("node")
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
#define ParamFilename _T("file")
#define ParamColour _T("colour")
#define ParamSpecularPower _T("cos_power")

#define StartNodeParam _T("(")
#define EndNodeParam _T(")")
//------------------------------------------

typedef Map< String,  io::FileParser::Node > NodeMap;

namespace rage
{
	bool parseSceneNode( io::FileParser::Node fileNode, gfx::Scene::Ptr rootScene, gfx::Spatial::Ptr sceneNode, NodeMap& meshes, gfx::MaterialState::PtrLookup& materials );

	RAGESceneTranslator::RAGESceneTranslator() :
		SceneTranslator( true, false, _T("rsf") )
	{
	}

	frx::ResourceTranslator::eFileMatch RAGESceneTranslator::identifyFile( const String& filename, const Char* buffer, Size size ) const
	{ 
		String header( AsciiCommentHeader AsciiFileHeader );

		if ( header == String( buffer ).left( header.length() ) )
			return FileMatch_IsMyFileType;
		else
			return FileMatch_NotMyFileType;
	}

	bool RAGESceneTranslator::writer( const gfx::Scene::Ptr scene, OutStream& stream ) 
	{ 
		return false; 
	}

	gfx::MaterialState::Ptr loadMaterialState( const String& id, io::FileParser::Node materialElement )
	{
		if ( materialElement.isNull() )
		{
			assert( false );
			return 0;
		}

		gfx::MaterialState::Ptr newMaterial = gfx::MaterialState::create( id );
		assert( newMaterial );

		for ( io::FileParser::Node child = materialElement.firstChild(); child.isValid(); child = child.next() )
		{
			if ( child == MaterialAmbientHeader )
			{
				newMaterial->ambient = child.firstAttribute(ParamColour).getValue();
			}
			else
			if ( child == MaterialDiffuseHeader )
			{
				newMaterial->diffuse  = child.firstAttribute(ParamColour).getValue();
			}
			else
			if ( child == MaterialSpecularHeader )
			{
				newMaterial->specular = child.firstAttribute(ParamColour).getValue();
				newMaterial->specularPower = String( child.firstAttribute(ParamSpecularPower).getValue() ).asReal();
			}
			else
			if ( child == MaterialTransparencyHeader )
			{
				gfx::MaterialState::Colour trans = child.firstAttribute(ParamColour).getValue();
				Real t = 1 - (trans.r + trans.g + trans.b) * ((Real)1.0/3);
				newMaterial->diffuse.a = t;
			}
		}

		return newMaterial;
	}

	bool RAGESceneTranslator::reader( gfx::Scene::Ptr scene, InStream& stream ) 
	{
		if ( !stream  )
			return false;

		RAGEFileParser parser( stream );
		if ( !parser.open() ) 
		{
			logError( "RAGESceneTranslator::reader() Failed to parse scene map file." );
			return false;
		}    

		// STEP 1: Parse the meshes in the scene building a list for when we load the heirarchy
		NodeMap meshes;
		io::FileParser::Node meshesElement = parser.getRootNodeByName( MeshesHeader );		
		if ( meshesElement.isValid() )
		{
			//get the number of meshes that should eb in the file
			Count count = (Count)String( meshesElement.firstAttribute( ParamCount ).getValue() ).asInt32();

			Count index = 0;
			for ( io::FileParser::Node meshElement = meshesElement.firstChild(); meshElement.isValid(); meshElement = meshElement.next() )
			{
				assert( meshElement.getName() == String(MeshHeader) );
				++index;
				io::FileParser::Node meshID = meshElement.firstAttribute( ParamID );
				if ( meshID.isValid() )
				{
					assert( meshes.find( meshID.getValue() ) == meshes.end() );
					meshes[ meshID.getValue() ] = meshElement;
				}
				else
				{
					assert( false );
					logError( "No ID was found for a mesh wiht index " << index );
				}
			}

			if ( count != index )
			{
				logRecoverable( "Scene file translator found " << index << " meshes but there should be " << count );
			}
			else
			{
				logComment( "Scene file translator found " << count << " meshes successfully" );
			}
		}

		// STEP 2: Parse the material in the scene building a list for when we load the meshes
		gfx::MaterialState::PtrLookup materials;
		io::FileParser::Node materialsElement = parser.getRootNodeByName( MaterialsHeader );		
		if ( materialsElement.isValid() )
		{
			//get the number of materiales that should eb in the file
			Count count = (Count)String( materialsElement.firstAttribute( ParamCount ).getValue() ).asInt32();

			Count index = 0;
			for ( io::FileParser::Node materialElement = materialsElement.firstChild(); materialElement.isValid(); materialElement = materialElement.next() )
			{
				assert( materialElement == MaterialHeader );
				++index;
				io::FileParser::Node materialID = materialElement.firstAttribute( ParamID );
				if ( materialID.isValid() )
				{
					assert( materials.find( materialID.getValue() ) == materials.end() );
					materials[ materialID.getValue() ] = loadMaterialState( materialID.getValue(), materialElement );;
					assert( materials[ materialID.getValue() ] );
				}
				else
				{
					assert( false );
					logError( "No ID was found for a material wiht index " << index );
				}
			}

			if ( count != index )
			{
				logRecoverable( "Scene file translator found " << index << " materials but there should be " << count );
			}
			else
			{
				logComment( "Scene file translator found " << count << " materials successfully" );
			}
		}

		bool success = true;

		// STEP 3: Parse the scene heirarchy
		io::FileParser::Node sceneElement = parser.getRootNodeByName( SceneHeader );		
		if ( sceneElement.isValid() )
		{
			if ( !parseSceneNode( sceneElement, scene, 0, meshes, materials ) )
			{
				logError( "Scene parse failed!" );
				success = false;	
			}
		}
		else
		{
			success = false;
			logError( _T("No ") SceneHeader	_T(" node found in the scene file!") );		
		}

		return success;
	}

	bool parseSceneNode( io::FileParser::Node fileNode, gfx::Scene::Ptr rootScene, gfx::Spatial::Ptr sceneNode, NodeMap& meshes, gfx::MaterialState::PtrLookup& materials )
	{
		if ( fileNode.isNull() )
		{
			assert( false );
			return false;
		}


		if ( !sceneNode )
		{
			sceneNode = rootScene;
		}
		else
		{
			String id = fileNode.firstAttribute( ParamID ).getValue();
			
			AttributeMap attributes;
			//Pack all the attributes for the node
			for ( io::FileParser::Node attrib = fileNode.firstAttribute(); attrib.isValid(); attrib = attrib.next() )
			{
				attributes[ attrib.getName() ] = attrib.getValue();		
			}

			sceneNode = rootScene->createChild( id, fileNode.getName(), attributes, sceneNode );
			
			if ( sceneNode.isValid() )
			{
				//If the node is a mesh node we need to load the mesh
				if ( sceneNode->isDerived( gfx::MeshNode::TYPE ) ||
					sceneNode->isDerived( phys::MeshShape::TYPE ) )
				{
					gfx::Mesh::Ptr mesh;

					String file = fileNode.firstAttribute( ParamFilename ).getValue();
					if ( file.length() )
					{
						//Create a mesh node loading the mesh from file
						mesh = _MeshManager.create( file );										
					}
					else
					{
						NodeMap::iterator iMesh = meshes.find( id );
						if ( iMesh != meshes.end() )
						{
							if( iMesh->second.isValid() )
							{
								gfx::MeshTranslator* loader = new RAGEInlineMeshTranslator( iMesh->second.getValueStream() );
								mesh = _MeshManager.create( id, loader );
								iMesh->second = io::FileParser::Node(); ///< Don't load the mesh again!
							}
							else
							{
								//Create the mesh using the already existing inline one
								mesh = _MeshManager.create( id );
							}
						}
					}

					if ( mesh.isValid() ) //If we have a mesh we need to get the materials for it
					{
						if ( sceneNode->isDerived( gfx::MeshNode::TYPE ) )
						{				
							//Set the mesh to the meshNode
							gfx::MeshNode::Ptr meshNode( (gfx::MeshNode*)sceneNode );
							//Set the materials used by the mesh
							gfx::MaterialState::PtrArray meshMaterials;
							const String::Array& materialName = mesh->getMaterials();
							for ( String::Array::const_iterator iName = materialName.begin(); iName != materialName.end(); ++iName )
							{
								gfx::MaterialState::PtrLookup::iterator iMaterial = materials.find( *iName );
								if ( iMaterial != materials.end() )
								{
									meshMaterials.push_back( iMaterial->second );																
								}
								else
								{
									meshMaterials.push_back( 0 );
									assert( false );
								}							
							}
							meshNode->setMaterials( meshMaterials );

							//Set and load the mesh onto the mshnode
							if ( meshNode->setMesh( mesh ) )
								sceneNode = meshNode;
							else
							{
								logError( "Failed to load a mesh node!" );
								sceneNode = 0;
							}		
						}
						else
						if ( sceneNode->isDerived( phys::MeshShape::TYPE ) )
						{
							//Set the mesh to the physics mesh node
							phys::MeshShape* meshShape = (phys::MeshShape*)sceneNode;
							if ( meshShape->setMesh( mesh ) )
								sceneNode = meshShape;
							else
							{
								logError( "Failed to load a physics mesh node!" );
								sceneNode;
								sceneNode->getParent()->removeChild( sceneNode );
								sceneNode = 0;
							}		
						}
					}			
				}
				else
				if ( sceneNode->isDerived( phys::Scene::TYPE ) )
				{
					//The scene must be initialised befiore children are created
					if ( !((phys::Scene*)sceneNode)->initialise() )
					{
						logError( "Failed to intialise physics scene" );
						return false;
					}
				}

			}
		}

		if ( sceneNode )
		{
			for ( io::FileParser::Node fileChild = fileNode.firstChild(); fileChild.isValid(); fileChild = fileChild.next() )
			{
				if ( !parseSceneNode( fileChild, rootScene, sceneNode, meshes, materials ) )
				{
					//TODO: error output
					return false;
				}
			}
		}

		return true;
	}
	
};