/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportMaterial.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CExportMaterial.h"

#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MColor.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MImage.h>

bool CExportMaterial::export( MObject& objMtrl, fstream& outFile ) 
{
	MFnDependencyNode dnMtrl( objMtrl );

	switch( objMtrl.apiType() ) 
	{
  	// if found phong shader
	case MFn::kPhong:
	{
		MFnPhongShader fn( objMtrl );

		outFile << "<material name=\"" << fn.name() << "\" >\n";

		//get the ambient colour
		MColor ambient = fn.ambientColor();		

		//Output the ambient texture filename
		MString ambientFile;
		if ( getTextureFile( fn, "ambientColor", ambientFile ) )
			outFile << "\t<image src=\"" << ambientFile << "\" />\n";
		
		//Output the ambient colour
		outFile << "\t<ambient a=\"" << ambient.a << "\" r=\"" << ambient.r << "\" g=\"" << ambient.g << "\" b=\"" << ambient.b << "\" />\n";
		
		//get the diffuse colour
		MColor diffuse = fn.color();

		//-------------------- DIFFUSE --------------------

		//Output the diffuse texture filename
		MString diffuseFile;
		if ( getTextureFile( fn, "color", diffuseFile ) )
		{
			//Output the image file information
			outFile << "\t<image name=\"diffuse\" src=\"" << diffuseFile << "\" />\n";
			
			//set the diffuse colour to the diffuse coefficient
			diffuse.a = diffuse.r = diffuse.g = diffuse.b = fn.diffuseCoeff();
		}
		else
		{
			//Scale the colour by the diffuse coefficient which shoudl give the actual diffuse
			diffuse *= fn.diffuseCoeff();
		}
		
		//Output the diffuse colour
		outFile << "\t<diffuse a=\"" << diffuse.a << "\" r=\"" << diffuse.r << "\" g=\"" << diffuse.g << "\" b=\"" << diffuse.b << "\" />\n";

		//-------------------------------------------------

		//-------------------- SPECULAR --------------------

		//get the diffuse colour
		MColor specular = fn.specularColor();

		//Output the specular texture filename
		MString specularFile;
		if ( getTextureFile( fn, "specularColor", diffuseFile ) )
			outFile << "\t<image name=\"specular\" src=\"" << specularFile << "\" />\n";
		
		//Output the specular colour & shininess
		outFile << "\t<specular a=\"" << specular.a << "\" r=\"" << specular.r << "\" g=\"" << specular.g << "\" b=\"" << specular.b << "\" shininess=\"" << fn.cosPower() << "\" />\n";

		//"incandescence", "transparency"

		//-------------------------------------------------

		outFile << "</material>\n";
		break;
	}
	// if found lambert shader
	case MFn::kLambert:
	{
		MFnLambertShader fn( objMtrl );

		outFile << "<material name=\"" << fn.name() << "\" >\n";

		//get the ambient colour
		MColor ambient = fn.ambientColor();		

		//Output the ambient texture filename
		MString ambientFile;
		if ( getTextureFile( fn, "ambientColor", ambientFile ) )
			outFile << "\t<ambient src=\"" << ambientFile << "\" />\n";
		else
			//Output the ambient colour
			outFile << "\t<ambient a=\"" << ambient.a << "\" r=\"" << ambient.r << "\" g=\"" << ambient.g << "\" b=\"" << ambient.b << "\" />\n";
		
		//get the diffuse colour
		MColor diffuse = fn.color();

		//Scale the colour by the diffuse coefficient which shoudl give the actual diffuse
		diffuse *= fn.diffuseCoeff();

		//Output the diffuse texture filename
		MString diffuseFile;
		if ( getTextureFile( fn, "color", diffuseFile ) )
			outFile << "\t<image src=\"" << diffuseFile << "\" coeff=\"" << fn.diffuseCoeff() << "\" />\n";
		else
			//Output the diffuse colour
			outFile << "\t<diffuse a=\"" << diffuse.a << "\" r=\"" << diffuse.r << "\" g=\"" << diffuse.g << "\" b=\"" << diffuse.b << "\" />\n";
		
		//"incandescence" , "transparency"

		outFile << "</material>\n";
		break;
	}
	default:
		break;
	}

	outFile << "\n";

	cerr<< "material " << dnMtrl.name() << " exported\n";

	return true;
}

bool CExportMaterial::getTextureFile( MFnDependencyNode& fn, const MString& name, MString& outFilename )
{
	MPlug plug = fn.findPlug( name );

	MString texname;

	//Get the plugs connected to the attribute
	MPlugArray plugs;
	plug.connectedTo( plugs, true, false );

	//Loop through all the plugs
	for( int i = 0; i != plugs.length(); ++i )
	{
  		//Check if the node is a file texture
		if( plugs[i].node().apiType() == MFn::kFileTexture )
		{
			// attach a dependency node to the file node
			MFnDependencyNode fn( plugs[i].node());

			MPlug ftn = fn.findPlug("ftn");

			// get the filename from the attribute
			ftn.getValue( outFilename );

			cerr << "texture file = " << outFilename << "\n";
			
			return true;
		}
	}

	return false;
}
