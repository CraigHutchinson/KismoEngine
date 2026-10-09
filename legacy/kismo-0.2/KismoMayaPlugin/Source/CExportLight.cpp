/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportLight.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CExportLight.h"

#include <maya/MFnPointLight.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MColor.h>

#include "CExportTransform.h"

bool CExportLight::export( MFnLight& fnLight, fstream& outFile ) 
{
	MStatus status;
	MObject lightObject = fnLight.object();

	MString type;
	switch(lightObject.apiType())
	{
	case MFn::kPointLight:		type = "point"; break;
	case MFn::kAmbientLight:	type = "ambient"; break;
	case MFn::kSpotLight:		type = "spot"; break;
	case MFn::kDirectionalLight:type = "direction"; break;
	default:
		status.perror( "Invalid light type for export " + MString(lightObject.apiTypeStr()) );
		return false;
	}

	outFile << "<light name=\"" << MFnDependencyNode( fnLight.parent(0) ).name() 
		<< "\" type=\"" << type << "\" >\n";

	MColor col = fnLight.color();
	float i = fnLight.intensity();

	outFile << "\t<diffuse>" << col.r * i << " " << col.g * i << " " << col.b * i << " " << col.a * i << "</diffuse>\n";
	
	//Output the specular as the same as the actual light colour
	if ( fnLight.lightSpecular() )
		outFile << "\t<specular>" << col.r * i << " " << col.g * i << " " << col.b * i << " " << col.a * i << "</specular>\n";


	switch(lightObject.apiType())
	{
		// output pointlight only attributes
	case MFn::kPointLight:
		{
			MFnPointLight fnPoint(lightObject);
		}
		break;
		// output ambientlight only attributes
	case MFn::kAmbientLight:
		{
			MFnAmbientLight fnAmbient(lightObject);
		}
		break;
		// output spotlight only attributes
	case MFn::kSpotLight:
		{
			MFnSpotLight fnSpot(lightObject);
		}
		break;
		// output directional light only attributes
	case MFn::kDirectionalLight:
		{
			MFnDirectionalLight fnDir(lightObject);
		}
		break;
	default:
		break;

	}

	//Out put the transform information
	CExportTransform trans;
	trans.export( MFnTransform( fnLight.parent(0) ), outFile );

	//end the light tag	
	outFile << "</light>\n\n";

	cerr<< "light exported\n";

	return true;
}