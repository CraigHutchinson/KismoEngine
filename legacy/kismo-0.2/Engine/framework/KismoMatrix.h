/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMatrix.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_MATRIX_H
#define	_KISMO_MATRIX_H

#include "KismoMath.h"
#include <assert.h>

namespace kismo
{
	namespace frx
	{
		// 3 by 3 scaling/rotation matrix
		class Matrix3x3
		{
		public:

			//Special matrices
			static const Matrix3x3 IDENTITY;
			static const Matrix3x3 ZERO;

		public:

			union {; 
				// _[ROW][COL]
				struct {
					k_Real _00, _01, _02;
					k_Real _10, _11, _12;
					k_Real _20, _21, _22;
				};
				k_Real m[3][3];
			};

			//Default constructor - NOTICE: does not create the matrix for efficiency
			Matrix3x3(){};
			
			//Constructor takes a 3 by 3 array of floats
			Matrix3x3( const k_Real arr[3][3] ) { memcpy( m, arr, 9 * sizeof( k_Real ) ); }
			
			//Copy constructor
			Matrix3x3( const Matrix3x3& mat ) { memcpy( m, mat.m, 9 * sizeof( k_Real ) ); }

			//Constructor with paramaters for all 9 floats
			Matrix3x3(	k_Real f00, k_Real f01, k_Real f02,
						k_Real f10, k_Real f11, k_Real f12,
						k_Real f20, k_Real f21, k_Real f22 )
			{
				_00 = f00; _01 = f01; _02 = f02;
				_10 = f10; _11 = f11; _12 = f12;
				_20 = f20; _21 = f21; _22 = f22;
			}

			//Stream operators
			inline friend k_OStream& operator << ( k_OStream& outStream, const Matrix3x3& m )
			{
				outStream << "Matrix3x3[ " << 
					m._00 << " " << m._01 << " " << m._02 << "  " << 
					m._10 << " " << m._11 << " " << m._12 << "  " << 
					m._20 << " " << m._21 << " " << m._22 << " ]";
				return outStream;
			}
			inline friend k_IStream& operator >> ( k_IStream& inStream, Matrix3x3& m )
			{
				inStream >> (k_Real)m._00 >> (k_Real)m._01 >> (k_Real)m._02 >>
							(k_Real)m._10 >> (k_Real)m._11 >> (k_Real)m._12 >>
							(k_Real)m._20 >> (k_Real)m._21 >> (k_Real)m._22;
				return inStream; //Allow cin >> x >> y >> z
			}

		};


		class Matrix4x4
		{
		public:
			
			//Special matrices
			static const Matrix4x4 IDENTITY;
			static const Matrix4x4 ZERO;

		public:

			union {; 
				// _[ROW][COL]
				struct {
					k_Real _00, _01, _02, _03;
					k_Real _10, _11, _12, _13;
					k_Real _20, _21, _22, _23;
					k_Real _30, _31, _32, _33;
				};
				k_Real m[4][4];
			};

			//Default constructor - NOTICE: does not create the matrix for efficiency
			Matrix4x4(){};
			
			//Constructor takes a 4 by 4 array of floats
			Matrix4x4( const k_Real arr[4][4] ) { memcpy( m, arr, 16 * sizeof( k_Real ) ); }
			
			//Copy constructor
			Matrix4x4( const Matrix4x4& mat ) { memcpy( m, mat.m, 16 * sizeof( k_Real ) ); }

			//Constructor with paramaters for all 16 floats
			Matrix4x4(	k_Real f00, k_Real f01, k_Real f02, k_Real f03,
						k_Real f10, k_Real f11, k_Real f12, k_Real f13,
						k_Real f20, k_Real f21, k_Real f22, k_Real f23,
						k_Real f30, k_Real f31, k_Real f32, k_Real f33 )
			{
				_00 = f00; _01 = f01; _02 = f02; _03 = f03;
				_10 = f10; _11 = f11; _12 = f12; _13 = f13;
				_20 = f20; _21 = f21; _22 = f22; _23 = f23;
				_30 = f30; _31 = f31; _32 = f32; _33 = f33;
			}

			//Stream operators
			inline friend k_OStream& operator << ( k_OStream& outStream, const Matrix4x4& m )
			{
				outStream << "Matrix4x4[ " << 
					m._00 << " " << m._01 << " " << m._02 << " " << m._03 << "  " <<
					m._10 << " " << m._11 << " " << m._12 << " " << m._13 << "  " <<
					m._20 << " " << m._21 << " " << m._22 << " " << m._23 << "  " <<
					m._30 << " " << m._31 << " " << m._32 << " " << m._33 << " ]";
				return outStream;
			}
			inline friend k_IStream& operator >> ( k_IStream& inStream, Matrix4x4& m )
			{
				inStream >> (k_Real)m._00 >> (k_Real)m._01 >> (k_Real)m._02 >> (k_Real)m._03 >>
							(k_Real)m._10 >> (k_Real)m._11 >> (k_Real)m._12 >> (k_Real)m._13 >>
							(k_Real)m._20 >> (k_Real)m._21 >> (k_Real)m._22 >> (k_Real)m._23 >>
							(k_Real)m._30 >> (k_Real)m._31 >> (k_Real)m._32 >> (k_Real)m._33;
				return inStream; //Allow cin >> x >> y >> z
			}

		};

		/*

		//Matrix class defines a 4 by 4 matrix of floats
		class Matrix4x4
		{
			public:
				union {; 
					struct {
						k_Real _00, _21, _31, _41;
						k_Real _10, _22, _32, _42;
						k_Real _13, _23, _33, _43;
						k_Real _14, _24, _34, _44;
					};
					k_Real m[4][4];
				};
				
				//Default constructor - does not create the matrix for efficiency
				Matrix4x4(){};
				
				//k_Real constructor
				Matrix4x4( const k_Real* pf );

				//Constructor with paramaters for all 16 floats
				Matrix4x4(	k_Real f00, k_Real f21, k_Real f31, k_Real f41,
							k_Real f10, k_Real f22, k_Real f32, k_Real f42,
							k_Real f13, k_Real f23, k_Real f33, k_Real f43,
							k_Real f14, k_Real f24, k_Real f34, k_Real f44 );

				
				//Access operators
				k_Real& operator () ( k_UInt32 col, k_UInt32 row );
				k_Real operator () ( k_UInt32 col, k_UInt32 row ) const;
				
				//Cast operators
				operator k_Real*() { return &_00; }	      
				operator const k_Real*() const { return &_00; }

				//Assignment operators
				Matrix4x4& operator *= ( const Matrix4x4& mat ); // Overloaded *= sign
				Matrix4x4& operator += ( const Matrix4x4& mat ); // Overloaded += sign
				Matrix4x4& operator -= ( const Matrix4x4& mat ); // Overloaded -= sign
				Matrix4x4& operator *= ( k_Real f ); // Overloaded *= sign
				Matrix4x4& operator /= ( k_Real f );  // Overloaded /= sign

				//Unary operators
				Matrix4x4 operator +() const;
				Matrix4x4 operator -() const;		
				
				//binary operators
				Matrix4x4 operator * ( const Matrix4x4& mat ) const;
				Matrix4x4 operator + ( const Matrix4x4& mat ) const;
				Matrix4x4 operator - ( const Matrix4x4& mat ) const;
				Matrix4x4 operator * ( k_Real f ) const;
				Matrix4x4 operator / ( k_Real f ) const;

				//modifyers
				Matrix4x4 transposed() const;
				Matrix4x4 inverse() const;

				//#################
				// STATIC
				//#################
				static Matrix4x4 IDENTITY;
				//static Matrix4x4 RIGHT2LEFT;

				static Matrix4x4 translation( k_Real x, k_Real y, k_Real z ); //Translation matrix
				
				static Matrix4x4 scaling( k_Real sx, k_Real sy, k_Real sz ); //Local axis scaling matrix
				static Matrix4x4 scaling( k_Real s ); //Overall scaling matrix

				//Rotation matrix
				static Matrix4x4 rotationX( k_Real rad );
				static Matrix4x4 rotationY( k_Real rad );
				static Matrix4x4 rotationZ( k_Real rad );
				//static Matrix4x4 rotation( k_Real x, k_Real y, k_Real z, ke_axisOrder order = kc_XYZ );
		};

		//Constructor takes a pointer to a k_Real array
		inline Matrix4x4::Matrix4x4( const k_Real* pf )
		{
			assert( pf != 0 );

			memcpy( &_00, pf, sizeof( Matrix4x4 ) ); //copy the matrix
		}


		//Constructor takes 16 individual k_Real values
		inline Matrix4x4::Matrix4x4(	k_Real f00, k_Real f21, k_Real f31, k_Real f41,
							k_Real f10, k_Real f22, k_Real f32, k_Real f42,
							k_Real f13, k_Real f23, k_Real f33, k_Real f43,
							k_Real f14, k_Real f24, k_Real f34, k_Real f44 )
		{
			_00 = f00; _21 = f21; _31 = f31; _41 = f41;
			_10 = f10; _22 = f22; _32 = f32; _42 = f42;
			_13 = f13; _23 = f23; _33 = f33; _43 = f43;
			_14 = f14; _24 = f24; _34 = f34; _44 = f44;
		}*/

		////Gets the value of a matrix cell
		//inline k_Real& Matrix4x4::operator () ( k_UInt32 col, k_UInt32 row )
		//{
		//	return m[ col ][ row ];
		//}
		//
		//inline k_Real Matrix4x4::operator () ( k_UInt32 col, k_UInt32 row ) const
		//{
		//	return m[ col ][ row ];
		//}
		//
		//// Overloaded *= sign
		//inline Matrix4x4& Matrix4x4::operator *= ( const Matrix4x4& mat )
		//{
		//	Matrix4x4 tMat; //Temporary matrix
		//
		//	for ( k_Int32 row = 0; row < 4; ++row ) for ( k_Int32 col = 0; col < 4; ++col )
		//		tMat.m[ col ][row] =	m[ 0 ][ row ] * mat.m[ col ][ 0 ] +
		//								m[ 1 ][ row ] * mat.m[ col ][ 1 ] +
		//								m[ 2 ][ row ] * mat.m[ col ][ 2 ] +
		//								m[ 3 ][ row ] * mat.m[ col ][ 3 ];
		//	
		//	//D3DXMatrixMultiply( (D3DXMATRIX*)this, (D3DXMATRIX*)this, (D3DXMATRIX*)&mat);
		//
		//    *this = tMat;
		//
		//	return *this;	
		//}
		//
		//// Overloaded += sign   
		//inline Matrix4x4& Matrix4x4::operator += ( const Matrix4x4& mat )
		//{
		//	_00 += mat._00; _21 += mat._21; _31 += mat._31; _41 += mat._41;
		//    _10 += mat._10; _22 += mat._22; _32 += mat._32; _42 += mat._42;
		//    _13 += mat._13; _23 += mat._23; _33 += mat._33; _43 += mat._43;
		//    _14 += mat._14; _24 += mat._24; _34 += mat._34; _44 += mat._44;
		//	return *this;
		//}
		//
		//// Overloaded += sign 
		//inline Matrix4x4& Matrix4x4::operator -= ( const Matrix4x4& mat )
		//{
		//	_00 -= mat._00; _21 -= mat._21; _31 -= mat._31; _41 -= mat._41;
		//    _10 -= mat._10; _22 -= mat._22; _32 -= mat._32; _42 -= mat._42;
		//    _13 -= mat._13; _23 -= mat._23; _33 -= mat._33; _43 -= mat._43;
		//    _14 -= mat._14; _24 -= mat._24; _34 -= mat._34; _44 -= mat._44;
		//	return *this;
		//}
		//
		//// Overloaded += sign
		//inline Matrix4x4& Matrix4x4::operator *= ( k_Real f )
		//{
		//    _00 *= f; _21 *= f; _31 *= f; _41 *= f;
		//    _10 *= f; _22 *= f; _32 *= f; _42 *= f;
		//    _13 *= f; _23 *= f; _33 *= f; _43 *= f;
		//    _14 *= f; _24 *= f; _34 *= f; _44 *= f;
		//    return *this;
		//}
		//
		//// Overloaded += sign 
		//inline Matrix4x4& Matrix4x4::operator /= ( k_Real f )
		//{ 
		//	k_Real fInv = 1.0f / f;
		//    _00 *= fInv; _21 *= fInv; _31 *= fInv; _41 *= fInv;
		//    _10 *= fInv; _22 *= fInv; _32 *= fInv; _42 *= fInv;
		//    _13 *= fInv; _23 *= fInv; _33 *= fInv; _43 *= fInv;
		//    _14 *= fInv; _24 *= fInv; _34 *= fInv; _44 *= fInv;
		//    return *this;
		//}
		//
		//inline Matrix4x4 Matrix4x4::operator +() const
		//{
		//	return *this;
		//}
		//
		//inline Matrix4x4 Matrix4x4::operator -() const
		//{
		//	return Matrix4x4(	-_00, -_10, -_13, -_14,
		//						-_21, -_22, -_23, -_24,
		//						-_31, -_32, -_33, -_34,
		//						-_41, -_42, -_43, -_44
		//						);
		//}
		//
		//inline Matrix4x4 Matrix4x4::operator * ( const Matrix4x4& mat ) const
		//{
		//	Matrix4x4 tMat;	//new temporary matrix
		//
		//	for ( k_Int32 row = 0; row < 4; ++row ) for ( k_Int32 col = 0; col < 4; ++col )
		//		tMat.m[col][row] =	m[ 0 ][ row ] * mat.m[ col ][ 0 ] +
		//							m[ 1 ][ row ] * mat.m[ col ][ 1 ] +
		//							m[ 2 ][ row ] * mat.m[ col ][ 2 ] +
		//							m[ 3 ][ row ] * mat.m[ col ][ 3 ];
		//
		//    //D3DXMatrixMultiply( (D3DXMATRIX*)&tMat, (D3DXMATRIX*) this, (D3DXMATRIX*)&mat );
		//    return tMat;
		//}
		//
		//
		//inline Matrix4x4 Matrix4x4::operator + ( const Matrix4x4& mat ) const
		//{
		//	return Matrix4x4(	_00 + mat._00, mat._21 + mat._21, mat._31 + mat._31, _41 + mat._41,
		//						_10 + mat._10, mat._22 + mat._22, mat._32 + mat._32, _42 + mat._42,
		//						_13 + mat._13, mat._23 + mat._23, mat._33 + mat._33, _43 + mat._43,
		//						_14 + mat._14, mat._24 + mat._24, mat._34 + mat._34, _44 + mat._44 );
		//}
		//
		//inline Matrix4x4 Matrix4x4::operator - ( const Matrix4x4& mat ) const
		//{
		//	return Matrix4x4(	_00 - mat._00, mat._21 - mat._21, mat._31 - mat._31, _41 - mat._41,
		//						_10 - mat._10, mat._22 - mat._22, mat._32 - mat._32, _42 - mat._42,
		//						_13 - mat._13, mat._23 - mat._23, mat._33 - mat._33, _43 - mat._43,
		//						_14 - mat._14, mat._24 - mat._24, mat._34 - mat._34, _44 - mat._44 );
		//}
		//
		//
		//// Overloaded += sign
		//inline Matrix4x4 Matrix4x4::operator * ( k_Real f ) const
		//{
		//	return Matrix4x4(	_00 * f, _21 * f, _31 * f, _41 * f,
		//						_10 * f, _22 * f, _32 * f, _42 * f,
		//						_13 * f, _23 * f, _33 * f, _43 * f,
		//						_14 * f, _24 * f, _34 * f, _44 * f );
		//}
		//
		//// Overloaded += sign 
		//inline Matrix4x4 Matrix4x4::operator / ( k_Real f ) const
		//{ 
		//	k_Real fInv = 1.0f / f;
		//	return Matrix4x4(	_00 * fInv, _21 * fInv, _31 * fInv, _41 * fInv,
		//						_10 * fInv, _22 * fInv, _32 * fInv, _42 * fInv,
		//						_13 * fInv, _23 * fInv, _33 * fInv, _43 * fInv,
		//						_14 * fInv, _24 * fInv, _34 * fInv, _44 * fInv );
		//}
		//
		//inline Matrix4x4 Matrix4x4::transposed() const
		//{
		//	return Matrix4x4(	_00, _10, _13, _14,
		//						_21, _22, _23, _24,
		//						_31, _32, _33, _34,
		//						_41, _42, _43, _44 );
		//}
	} //END: frx

} //END: kismo

#endif




