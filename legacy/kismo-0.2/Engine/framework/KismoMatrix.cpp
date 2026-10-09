/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMatrix.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoMatrix.h"

namespace kismo
{
	namespace frx
	{
		const Matrix3x3 Matrix3x3::IDENTITY(	1, 0, 0,
											0, 1, 0,
											0, 0, 1 );

		const Matrix3x3 Matrix3x3::ZERO(	0, 0, 0,
										0, 0, 0,
										0, 0, 0 );



		const Matrix4x4 Matrix4x4::IDENTITY(	1, 0, 0, 0,
											0, 1, 0, 0,
											0, 0, 1, 0,
											0, 0, 0, 1 );

		const Matrix4x4 Matrix4x4::ZERO(	0, 0, 0, 0,
											0, 0, 0, 0,
											0, 0, 0, 0,
											0, 0, 0, 0 );
		/*
		CMatrix4 CMatrix4::right2left = CMatrix4(	1, 0, 0, 0,
													0, 1, 0, 0,
													0, 0, -1, 0,
													0, 0, 0, 1 );
													*/


		//###################
		// STATIC FUNCTIONS
		//###################

		//CMatrix4 CMatrix4::translation( k_Float32 x, k_Float32 y, k_Float32 z )
		//{
		//	return CMatrix4(	1, 0, 0, 0,
		//						0, 1, 0, 0,
		//						0, 0, 1, 0,
		//						x, y, z, 1 );
		//}
		//
		//CMatrix4 CMatrix4::scaling( k_Float32 overallScale )
		//{
		//	return CMatrix4(	1, 0, 0, 0,
		//						0, 1, 0, 0,
		//						0, 0, 1, 0,
		//						0, 0, 0, overallScale );
		//}
		//
		//CMatrix4 CMatrix4::scaling( k_Float32 sx, k_Float32 sy, k_Float32 sz )
		//{
		//	return CMatrix4(	sx, 0, 0, 0,
		//						0, sy, 0, 0,
		//						0, 0, sz, 0,
		//						0, 0, 0, 1 );
		//}
		//
		//CMatrix4 CMatrix4::rotationX( k_Float32 rad )
		//{
		//	k_Float32 c = cos(rad * kc_DEG2RAD);
		//	k_Float32 s = sin(rad * kc_DEG2RAD);
		//
		//	return CMatrix4(	1, 0, 0, 0,
		//						0, c, s, 0,
		//						0, -s, c, 0,
		//						0, 0, 0, 1 );
		//}
		//
		//CMatrix4 CMatrix4::rotationY( k_Float32 rad )
		//{
		//	k_Float32 c = cos(rad * kc_DEG2RAD);
		//	k_Float32 s = sin(rad * kc_DEG2RAD);
		//
		//	return CMatrix4(	c, 0, -s, 0,
		//						0, 1, 0, 0,
		//						s, 0, c, 0,
		//						0, 0, 0, 1 );
		//}
		//
		//CMatrix4 CMatrix4::rotationZ( k_Float32 rad )
		//{
		//	k_Float32 c = cos(rad * kc_DEG2RAD);
		//	k_Float32 s = sin(rad * kc_DEG2RAD);
		//
		//	return CMatrix4(	c, s, 0, 0,
		//						-s, c, 0, 0,
		//						0, 0, 1, 0,
		//						0, 0, 0, 1 );
		//}
		//
		//CMatrix4 CMatrix4::rotation( k_Float32 x, k_Float32 y, k_Float32 z, eAxisOrder order )
		//{
		//	CMatrix4 n;
		//
		//	//Loop through the 3 axis
		//	for (k_Int32 i = 0; i < 3; i++)
		//		//Perform a switch on the axis ID
		//		switch ( AxisOrderSequence[ order ][ i ] )
		//		{
		//		case eX: n *= CMatrix4::rotationX( x );  break;
		//		case eY: n *= CMatrix4::rotationY( y ); break;
		//		case eZ: n *= CMatrix4::rotationZ( z );  break;
		//		}
		//
		//	return n;
		//}
		//
		//CMatrix4 CMatrix4::inverse() const
		//{
		//	CMatrix4 out;
		//	D3DXMatrixInverse( (D3DXMATRIX*)&out, 0, (D3DXMATRIX*)this );
		//	return out;
		//}
	} //END: frx
} //END: kismo