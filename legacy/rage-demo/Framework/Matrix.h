/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Matrix.h
 * Status: superseded by kismo-v0.4/KismoFRX/Matrix.h (same name, content differs)
 */
#ifndef FRX_MATRIX4_H
#define	FRX_MATRIX4_H

#include "Framework/CoreMath.h"
#include "Framework/Vector.h"

namespace frx
{
	/** \brief Matrix4 class defines a 4 by 4 matrix of floats
	*/
	template< class _TReal >
	class _DllExport Matrix4
	{
	public:

		/** \brief special matrix instances
		*/		
		static const Matrix4 Zero;
		static const Matrix4 Identity;
		static const Matrix4 Right2left; //< right to left conversion for Dx/opengl conversion

		/** \brief Creates a Translation matrix
		*/
		static Matrix4 translation( _TReal x, _TReal y, _TReal z ); 
		 
		/** \brief Creates a Local axis scaling matrix
		*/
		static Matrix4 scaling( _TReal sx, _TReal sy, _TReal sz );

		/** \brief Creates a Overall scaling matrix
		*/
		static Matrix4 scaling( _TReal s ); 

		/** \brief Creates a Rotation matrix about the X-axis
		*/
		static Matrix4 rotationX( _TReal deg );

		/** \brief Creates a Rotation matrix about the Y-axis
		*/
		static Matrix4 rotationY( _TReal deg );

		/** \brief Creates a Rotation matrix about the Z-axis
		*/
		static Matrix4 rotationZ( _TReal deg );

		/** \brief Enumerator for defining the order of rotation when using euler angles
		*/
		enum eAxisOrder
		{ 
			XYZ = 0, YZX, ZXY, XZY, YXZ, ZYX 
		};

		/** \brief Creates a Rotation matrix about the 3 major axis in a specified order
		*/
		static Matrix4 rotation( _TReal x, _TReal y, _TReal z, eAxisOrder order = XYZ );

	public:

		/** \brief Linear array of data to maintain data locality and order across different platforms
		*/ 
		_TReal m[ 4 * 4 ];

		/** \brief Default constructor creates an unitialised matrix for efficiency
		*/
		Matrix4() {}
		
		/** \brief _TReal constructor
		*/
		Matrix4( const _TReal* pf );

		/** \brief Constructor with paramaters for all 16 floats
		*/
		Matrix4(	_TReal f11, _TReal f21, _TReal f31, _TReal f41,
					_TReal f12, _TReal f22, _TReal f32, _TReal f42,
					_TReal f13, _TReal f23, _TReal f33, _TReal f43,
					_TReal f14, _TReal f24, _TReal f34, _TReal f44 );

		
		/** \brief Access operator for gettin a matrix value from a row and colum index
			\remarks Not bounds checked for efficiency
		*/
		inline _TReal& operator () ( Index col, Index row )
		{ return at( col, row ); }
		
		/** \brief Access operator for gettin a matrix value from a row and colum index
			\remarks Not bounds checked for efficiency
		*/
		inline const _TReal& operator () ( Index col, Index row ) const
		{ return at( col, row ); }
			
		/** \brief Access operator for gettin a matrix value from a row and colum index
			\remarks Not bounds checked for efficiency
		*/
		inline _TReal& at( Index col, Index row )
		{ return m[ col + row*4 ]; }
		
		/** \brief Access operator for gettin a matrix value from a row and colum index
			\remarks Not bounds checked for efficiency
		*/
		inline const _TReal& at( Index col, Index row ) const
		{ return m[ col + row*4 ]; }	

		/** \brief Cast operator to a _TReal pointer
		*/
		operator _TReal*()
		{ return (_TReal*)m; }

		/** \brief Cast operator to a const _TReal pointer
		*/
		operator const _TReal*() const
		{ return (_TReal*)m;  }

		/** \brief  Overloaded *= sign
		*/
		Matrix4< _TReal >& operator *= ( const Matrix4< _TReal >& mat );

		/** \brief  Overloaded += sign
		*/
		Matrix4< _TReal >& operator += ( const Matrix4< _TReal >& mat ); 

		/** \brief  Overloaded -= sign
		*/
		Matrix4< _TReal >& operator -= ( const Matrix4< _TReal >& mat ); 

		/** \brief  Overloaded *= sign
		*/
		Matrix4< _TReal >& operator *= ( _TReal f );

		/** \brief  Overloaded /= sign
		*/
		Matrix4< _TReal >& operator /= ( _TReal f );  
		
		// Vector operators		
		Vector3< _TReal > operator * ( const Vector3< _TReal >& vec ) const;

		/** \brief Unary operator for positive... i.e. does nothing!
		*/
		Matrix4 operator +() const
		{ return *this; }

		/** \brief Unary operator for negating the matrix cells
			\remarks this IS NOT the same as invert()
		*/
		Matrix4 operator -() const;		
		
		/** \brief binary operators
		*/
		Matrix4 operator * ( const Matrix4< _TReal >& mat ) const;
		Matrix4 operator + ( const Matrix4< _TReal >& mat ) const;
		Matrix4 operator - ( const Matrix4< _TReal >& mat ) const;
		Matrix4 operator * ( _TReal f ) const;
		Matrix4 operator / ( _TReal f ) const;

		/** \brief Tranposes the matrix so the rows become columns and visa-versa
		*/
		Matrix4 transposed() const;

		/** \brief Calculats the inverse of the matrix
		*/
		Matrix4 inverse() const;

		/*_DllExport*/ friend OutStream& operator << ( OutStream& outStream, const Matrix4& m )
		{
			for ( Index i = 0; i < 4*4 -1; ++i )
				outStream << m[i] << _T('\t');

			outStream << m[4*4 -1];
			return outStream;
		}

		/*_DllExport*/ friend InStream& operator >> ( InStream& inStream, Matrix4& m )
		{
			for ( Index i = 0; i < 4*4; ++i )
				inStream >> m[i];
			return inStream;
		}


		/** this conversion uses conventions as described on page:
		*   http://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
		*   Coordinate System: right hand
		*   Positive angle: right hand
		*   Order of euler angles: heading first, then attitude, then bank
		*   matrix row column ordering:
		*   [m00 m01 m02]
		*   [m10 m11 m12]
		*   [m20 m21 m22]
		*/
		Vector3< _TReal > asEuler() const 
		{
			// Assuming the angles are in radians.
			if ((Real)at(0,1) > (1 - Real_EPSILON)) // singularity at north pole
				return Vector3< _TReal >( (_TReal)math::HalfPI, (_TReal)math::atan2( (Real)at(2,0), (Real)at(2,2) ), 0 );
			if ((Real)at(0,1) < (-1 + Real_EPSILON) ) // singularity at south pole
				return Vector3< _TReal >( (_TReal)-math::HalfPI, (_TReal)math::atan2( (Real)at(2,0), (Real)at(2,2) ), 0 );

			return Vector3< _TReal >( (_TReal)math::atan2( (Real)-at(2,1), (Real)at(1,1) ), (_TReal)math::atan2( (Real)-at(0,2) , (Real)at(0,0) ), (_TReal)math::asin( (Real)at(0,1) ) );
		}

	};

	typedef Matrix4< Real > Matrix4r;
	typedef Matrix4< Float32 > Matrix4f;
	typedef Matrix4< Float64 > Matrix4d;

} // END: frx


#endif




