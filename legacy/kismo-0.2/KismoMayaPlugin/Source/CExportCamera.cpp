/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportCamera.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CExportCamera.h"

#include <maya/MVector.h>
#include "CExportTransform.h"

const FLOAT DEG2RAD = (FLOAT)M_PI / 180.0f;
const FLOAT RAD2DEG = 1.0f / DEG2RAD;

bool CExportCamera::export( MFnCamera& fnCamera, fstream& outFile ) 
{

	//Get the cameras parent node
	MFnDependencyNode parent( fnCamera.parent(0) );
	
    //start tag for the camera with name
	outFile << "<camera name=\"" << parent.name() << "\" >\n";

	//the far clipping plane
	outFile << "\t<farClippingPlane>" << fnCamera.farClippingPlane() << "</farClippingPlane>\n";
	
	//the near clipping plane
	outFile << "\t<nearClippingPlane>" << fnCamera.nearClippingPlane() << "</nearClippingPlane>\n";
	
	//the direction at which the camera faces e.g. 0,0,-1
	MVector dir = fnCamera.viewDirection();	
	outFile << "\t<direction>" << dir.x << " " << dir.y << " " << dir.z << "</direction>\n";
	
	//the up vector for the camera e.g. 0, 1, 0
	MVector up = fnCamera.upDirection();
	outFile << "\t<up>" << up.x << " " << up.y << " " << up.z << "</up>\n";

	//
    MPoint interest = fnCamera.centerOfInterestPoint();
	interest.cartesianize();
	outFile << "\t<interest>" << interest.x << " " << interest.y << " " << interest.z << "</interest>\n";

	outFile << "\t<fieldOfView>" << fnCamera.horizontalFieldOfView() * RAD2DEG  << "</fieldOfView>\n";

	//Out put the transform information
	CExportTransform trans;
	trans.export( MFnTransform( fnCamera.parent(0) ), outFile );

	outFile << "</camera>\n\n";

	cerr<< "camera exported\n";

	return true;
}