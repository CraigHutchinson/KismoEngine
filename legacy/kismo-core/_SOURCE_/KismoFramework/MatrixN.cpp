/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\MatrixN.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/


#include "KismoFramework/MatrixN.h"
#include "KismoFramework/VectorN.h"


#define _11 at(0,0)
#define _21 at(1,0) 
#define _31 at(2,0) 
#define _41 at(3,0) 
#define _12 at(0,1) 
#define _22 at(1,1) 
#define _32 at(2,1) 
#define _42 at(3,1) 
#define _13 at(0,2) 
#define _23 at(1,2) 
#define _33 at(2,2) 
#define _43 at(3,2) 
#define _14 at(0,3) 
#define _24 at(1,3)
#define _34 at(2,3) 
#define _44 at(3,3)

namespace kismo
{
	const MatrixT<Float32,1,2> MatrixT<Float32,1,2>::Identity( makeIdentity() );
	const MatrixT<Float32,2,1> MatrixT<Float32,2,1>::Identity( makeIdentity() );
	const MatrixT<Float32,2,2> MatrixT<Float32,2,2>::Identity( makeIdentity() );
	const MatrixT<Float32,2,3> MatrixT<Float32,2,3>::Identity( makeIdentity() );
	const MatrixT<Float32,3,2> MatrixT<Float32,3,2>::Identity( makeIdentity() );
	const MatrixT<Float32,3,3> MatrixT<Float32,3,3>::Identity( makeIdentity() );
	const MatrixT<Float32,3,4> MatrixT<Float32,3,4>::Identity( makeIdentity() );
	const MatrixT<Float32,4,3> MatrixT<Float32,4,3>::Identity( makeIdentity() );
	const MatrixT<Float32,4,4> MatrixT<Float32,4,4>::Identity( makeIdentity() );

	const MatrixT<Float64,1,2> MatrixT<Float64,1,2>::Identity( makeIdentity() );
	const MatrixT<Float64,2,1> MatrixT<Float64,2,1>::Identity( makeIdentity() );
	const MatrixT<Float64,2,2> MatrixT<Float64,2,2>::Identity( makeIdentity() );
	const MatrixT<Float64,3,2> MatrixT<Float64,3,2>::Identity( makeIdentity() );
	const MatrixT<Float64,2,3> MatrixT<Float64,2,3>::Identity( makeIdentity() );
	const MatrixT<Float64,3,3> MatrixT<Float64,3,3>::Identity( makeIdentity() );
	const MatrixT<Float64,3,4> MatrixT<Float64,3,4>::Identity( makeIdentity() );
	const MatrixT<Float64,4,3> MatrixT<Float64,4,3>::Identity( makeIdentity() );
	const MatrixT<Float64,4,4> MatrixT<Float64,4,4>::Identity( makeIdentity() );

	//template < class _Real > const MatrixT<_Real,4,4> MatrixT<_Real,4,4>::Identity( makeIdentity() );


	//template< class _Real >
	//const Matrix4< _Real > Matrix4< _Real >::Zero = Matrix4(		
	//	0, 0, 0, 0,
	//	0, 0, 0, 0,
	//	0, 0, 0, 0,
	//	0, 0, 0, 0 );

	//template< class _Real >
	//const Matrix4< _Real > Matrix4< _Real >::Identity = Matrix4(	
	//	1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, 1, 0,
	//	0, 0, 0, 1 );

	//template< class _Real >
	//const Matrix4< _Real > Matrix4< _Real >::Right2left = Matrix4(	
	//	1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, -1, 0,
	//	0, 0, 0, 1 );


	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::translation( _Real x, _Real y, _Real z )
	//{
	//	return Matrix4(		1, 0, 0, 0,
	//						0, 1, 0, 0,
	//						0, 0, 1, 0,
	//						x, y, z, 1 );
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::scaling( _Real overallScale )
	//{
	//	return Matrix4(		1, 0, 0, 0,
	//						0, 1, 0, 0,
	//						0, 0, 1, 0,
	//						0, 0, 0, overallScale );
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::scaling( _Real sx, _Real sy, _Real sz )
	//{
	//	return Matrix4(		sx, 0, 0, 0,
	//						0, sy, 0, 0,
	//						0, 0, sz, 0,
	//						0, 0, 0, 1 );
	//}

	////This function is based from code used in previous work
	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::rotationX( _Real deg )
	//{
	//	_Real c = cos( deg * DegToRad);
	//	_Real s = sin( deg * DegToRad);

	//	return Matrix4(	1, 0, 0, 0,
	//					0, c, s, 0,
	//					0, -s, c, 0,
	//					0, 0, 0, 1 );
	//}

	////This function is based from code used in previous work
	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::rotationY( _Real deg )
	//{
	//	_Real c = cos( deg * DegToRad);
	//	_Real s = sin( deg * DegToRad);

	//	return Matrix4(	c, 0, -s, 0,
	//					0, 1, 0, 0,
	//					s, 0, c, 0,
	//					0, 0, 0, 1 );
	//}

	////This function is based from code used in previous work
	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::rotationZ( _Real deg )
	//{
	//	_Real c = cos( deg * DegToRad);
	//	_Real s = sin( deg * DegToRad);

	//	return Matrix4(	c, s, 0, 0,
	//					-s, c, 0, 0,
	//					0, 0, 1, 0,
	//					0, 0, 0, 1 );
	//}
	//

	////This function is based from code used in previous work
	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::rotation( _Real x, _Real y, _Real z, eAxisOrder order )
	//{
	//	//enum for the axis'
	//	enum eAxis{ X, Y, Z };

	//	//Array holds the index to the x/y/z for the axis order
	//	const eAxis AxisOrderSequence[6][3] =
	//	{
	//		{ X, Y, Z }, { Y, Z, X }, { Z, X, Y }, { X, Z, Y }, { Y, X, Z }, { Z, Y, X }
	//	};

	//	Matrix4 n = Identity;

	//	//Loop through the 3 axis
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		//Perform a switch on the axis ID
	//		switch ( AxisOrderSequence[ order ][ i ] )
	//		{
	//		case X: n *= rotationX( x );  break;
	//		case Y: n *= rotationY( y ); break;
	//		case Z: n *= rotationZ( z );  break;
	//		}
	//	}

	//	return n;
	//}

	//
	//template< class _Real >
	//VectorN<_Real,3 > Matrix4< _Real >::operator * ( const VectorN<_Real,3 >& vec ) const
	//{
	//	//Calculate the homogeneous element as if = 1
	//	//_Real h = vec.x * _41 + vec.y * _42 + vec.z * _43 + _44;
	//	
	//	//If zero represent the FLOAT as the smallest possible value
	//	//if ( h == 0.0f )
	//	//	h = (_Real)Real_MIN;

	//	return VectorN<_Real,3 >( 
	//		( (vec.x * _11) + (vec.y * _12) + (vec.z * _13) + _14 )/* / h*/ , 
	//		( (vec.x * _21) + (vec.y * _22) + (vec.z * _23) + _24 )/* / h*/ , 
	//		( (vec.x * _31) + (vec.y * _32) + (vec.z * _33) + _34 )/* / h*/ 
	//		 );

	//}


	//template< class _Real >
	//Matrix4< _Real >::Matrix4(	_Real f11, _Real f21, _Real f31, _Real f41,
	//					_Real f12, _Real f22, _Real f32, _Real f42,
	//					_Real f13, _Real f23, _Real f33, _Real f43,
	//					_Real f14, _Real f24, _Real f34, _Real f44 )
	//{
	//	_11 = f11; _21 = f21; _31 = f31; _41 = f41;
	//	_12 = f12; _22 = f22; _32 = f32; _42 = f42;
	//	_13 = f13; _23 = f23; _33 = f33; _43 = f43;
	//	_14 = f14; _24 = f24; _34 = f34; _44 = f44;
	//}

	//template< class _Real >
	//Matrix4< _Real >::Matrix4( const _Real* pf )
	//{
	//	Assert( pf != 0 );
	//	memcpy( &_11, pf, sizeof( Matrix4 ) ); //copy the matrix
	//}


	//template< class _Real >
	//Matrix4< _Real >& Matrix4< _Real >::operator *= ( const Matrix4< _Real >& mat )
	//{
	//	Matrix4< _Real > tMat; /// Temporary matrix

	//	for ( int row = 0; row < 4; ++row ) for ( int col = 0; col < 4; ++col )
	//		tMat( col, row ) =	at( 0, row ) * mat( col, 0 ) +
	//							at( 1, row ) * mat( col, 1 ) +
	//							at( 2, row ) * mat( col, 2 ) +
	//							at( 3, row ) * mat( col, 3 );
	//	*this = tMat;
	//	return *this;	
	//}

	//template< class _Real >
	//Matrix4< _Real >& Matrix4< _Real >::operator += ( const Matrix4< _Real >& mat )
	//{
	//	_11 += mat._11; _21 += mat._21; _31 += mat._31; _41 += mat._41;
	//	_12 += mat._12; _22 += mat._22; _32 += mat._32; _42 += mat._42;
	//	_13 += mat._13; _23 += mat._23; _33 += mat._33; _43 += mat._43;
	//	_14 += mat._14; _24 += mat._24; _34 += mat._34; _44 += mat._44;
	//	return *this;
	//}

	//template< class _Real >
	//Matrix4< _Real >& Matrix4< _Real >::operator -= ( const Matrix4& mat )
	//{
	//	_11 -= mat._11; _21 -= mat._21; _31 -= mat._31; _41 -= mat._41;
	//	_12 -= mat._12; _22 -= mat._22; _32 -= mat._32; _42 -= mat._42;
	//	_13 -= mat._13; _23 -= mat._23; _33 -= mat._33; _43 -= mat._43;
	//	_14 -= mat._14; _24 -= mat._24; _34 -= mat._34; _44 -= mat._44;
	//	return *this;
	//}

	//template< class _Real >
	//Matrix4< _Real >& Matrix4< _Real >::operator *= ( _Real f )
	//{
	//	_11 *= f; _21 *= f; _31 *= f; _41 *= f;
	//	_12 *= f; _22 *= f; _32 *= f; _42 *= f;
	//	_13 *= f; _23 *= f; _33 *= f; _43 *= f;
	//	_14 *= f; _24 *= f; _34 *= f; _44 *= f;
	//	return *this;
	//}

	//template< class _Real >
	//Matrix4< _Real >& Matrix4< _Real >::operator /= ( _Real f )
	//{ 
	//	_Real fInv = 1.0f / f;
	//	_11 *= fInv; _21 *= fInv; _31 *= fInv; _41 *= fInv;
	//	_12 *= fInv; _22 *= fInv; _32 *= fInv; _42 *= fInv;
	//	_13 *= fInv; _23 *= fInv; _33 *= fInv; _43 *= fInv;
	//	_14 *= fInv; _24 *= fInv; _34 *= fInv; _44 *= fInv;
	//	return *this;
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::operator -() const
	//{
	//	return Matrix4(		-_11, -_12, -_13, -_14,
	//						-_21, -_22, -_23, -_24,
	//						-_31, -_32, -_33, -_34,
	//						-_41, -_42, -_43, -_44
	//						);
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::operator * ( const Matrix4< _Real >& mat ) const
	//{
	//	Matrix4< _Real > tMat;	// new temporary matrix

	//	for ( int row = 0; row < 4; ++row ) for ( int col = 0; col < 4; ++col )
	//		tMat(col,row) =	at( 0, row ) * mat( col, 0 ) +
	//						at( 1, row ) * mat( col, 1 ) +
	//						at( 2, row ) * mat( col, 2 ) +
	//						at( 3, row ) * mat( col, 3 );
	//	return tMat;
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::operator + ( const Matrix4< _Real >& mat ) const
	//{
	//	return Matrix4(	_11 + mat._11, mat._21 + mat._21, mat._31 + mat._31, _41 + mat._41,
	//					_12 + mat._12, mat._22 + mat._22, mat._32 + mat._32, _42 + mat._42,
	//					_13 + mat._13, mat._23 + mat._23, mat._33 + mat._33, _43 + mat._43,
	//					_14 + mat._14, mat._24 + mat._24, mat._34 + mat._34, _44 + mat._44 );
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::operator - ( const Matrix4< _Real >& mat ) const
	//{
	//	return Matrix4(	_11 - mat._11, mat._21 - mat._21, mat._31 - mat._31, _41 - mat._41,
	//					_12 - mat._12, mat._22 - mat._22, mat._32 - mat._32, _42 - mat._42,
	//					_13 - mat._13, mat._23 - mat._23, mat._33 - mat._33, _43 - mat._43,
	//					_14 - mat._14, mat._24 - mat._24, mat._34 - mat._34, _44 - mat._44 );
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::operator * ( _Real f ) const
	//{
	//	return Matrix4(	_11 * f, _21 * f, _31 * f, _41 * f,
	//					_12 * f, _22 * f, _32 * f, _42 * f,
	//					_13 * f, _23 * f, _33 * f, _43 * f,
	//					_14 * f, _24 * f, _34 * f, _44 * f );
	//}

	//template< class _Real >	
	//Matrix4< _Real > Matrix4< _Real >::operator / ( _Real f ) const
	//{ 
	//	_Real fInv = 1.0f / f;
	//	return Matrix4(	_11 * fInv, _21 * fInv, _31 * fInv, _41 * fInv,
	//					_12 * fInv, _22 * fInv, _32 * fInv, _42 * fInv,
	//					_13 * fInv, _23 * fInv, _33 * fInv, _43 * fInv,
	//					_14 * fInv, _24 * fInv, _34 * fInv, _44 * fInv );
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::transposed() const
	//{
	//	return Matrix4(	_11, _12, _13, _14,
	//						_21, _22, _23, _24,
	//						_31, _32, _33, _34,
	//						_41, _42, _43, _44 );
	//}

	////Invert function from Intel :
	////ftp://download.intel.com/design/PentiumIII/sml/24504301.pdf
	////5.2 C Code with Cramer's rule
	//template < class _Real >
	//void invert4x4( const _Real *mat, _Real *dst ) 
	//{
	//	_Real tmp[12]; /* temp array for pairs */
	//	_Real src[16]; /* array of transpose source matrix */
	//	_Real det; /* determinant */

	//	/* transpose matrix */
	//	for (int i = 0; i < 4; i++) {
	//		src[i] = mat[i*4];
	//		src[i + 4] = mat[i*4 + 1];
	//		src[i + 8] = mat[i*4 + 2];
	//		src[i + 12] = mat[i*4 + 3];
	//	}
	//	/* calculate pairs for first 8 elements (cofactors) */
	//	tmp[0] = src[10] * src[15];
	//	tmp[1] = src[11] * src[14];
	//	tmp[2] = src[9] * src[15];
	//	tmp[3] = src[11] * src[13];
	//	tmp[4] = src[9] * src[14];
	//	tmp[5] = src[10] * src[13];
	//	tmp[6] = src[8] * src[15];
	//	tmp[7] = src[11] * src[12];
	//	tmp[8] = src[8] * src[14];
	//	tmp[9] = src[10] * src[12];
	//	tmp[10] = src[8] * src[13];
	//	tmp[11] = src[9] * src[12];

	//	/* calculate first 8 elements (cofactors) */
	//	dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	//	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	//	dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	//	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	//	dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	//	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	//	dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	//	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	//	dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	//	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	//	dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	//	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	//	dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	//	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	//	dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	//	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	//	/* calculate pairs for second 8 elements (cofactors) */
	//	tmp[0] = src[2]*src[7];
	//	tmp[1] = src[3]*src[6];
	//	tmp[2] = src[1]*src[7];
	//	tmp[3] = src[3]*src[5];
	//	tmp[4] = src[1]*src[6];
	//	tmp[5] = src[2]*src[5];
	//	tmp[6] = src[0]*src[7];
	//	tmp[7] = src[3]*src[4];
	//	tmp[8] = src[0]*src[6];
	//	tmp[9] = src[2]*src[4];
	//	tmp[10] = src[0]*src[5];
	//	tmp[11] = src[1]*src[4];

	//	/* calculate second 8 elements (cofactors) */
	//	dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	//	dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	//	dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	//	dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	//	dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	//	dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	//	dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	//	dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	//	dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	//	dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	//	dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	//	dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	//	dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	//	dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	//	dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	//	dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	//	/* calculate determinant */
	//	det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
	//	/* calculate matrix inverse */
	//	det = 1/det;
	//	for (int j = 0; j < 16; j++)
	//		dst[j] *= det;
	//}

	//template< class _Real >
	//Matrix4< _Real > Matrix4< _Real >::inverse() const
	//{
	//	Matrix4< _Real > t;
	//	invert4x4( m, t.m);    
	//	return t;
	//}

	////Instanciate the matrix class implementations
	//template class Matrix4< Float32 >;
	//template class Matrix4< Float64 >;

	template class MatrixN< Float32, 2, 2 >;
	template class MatrixN< Float64, 2, 2 >;

	//template class MatrixN< Real, 3, 3 >;
	template class MatrixN< Float32, 3, 3 >;
	template class MatrixN< Float64, 3, 3 >;

	//template class MatrixN< Real, 4, 3 >;
	template class MatrixN< Float32, 4, 3 >;
	template class MatrixN< Float64, 4, 3 >;

	//template class MatrixN< Real, 4, 4 >;
	template class MatrixN< Float32, 4, 4 >;
	template class MatrixN< Float64, 4, 4 >;

	//template class MatrixT< Real, 2, 2 >;
	template class MatrixT< Float32, 2, 2 >;
	template class MatrixT< Float64, 2, 2 >;

	//template class MatrixT< Real, 3, 3 >;
	template class MatrixT< Float32, 3, 3 >;
	template class MatrixT< Float64, 3, 3 >;

	//template class MatrixT< Real, 4, 3 >;
	template class MatrixT< Float32, 4, 3 >;
	template class MatrixT< Float64, 4, 3 >;

	//template class MatrixT< Real, 4, 4 >;
	template class MatrixT< Float32, 4, 4 >;
	template class MatrixT< Float64, 4, 4 >;

} //END: kismo