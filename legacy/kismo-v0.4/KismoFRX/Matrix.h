/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Matrix.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMOFRX_MATRIX_H
#define	KISMOFRX_MATRIX_H

#include "KismoFRX/Math.h"
#include "KismoFRX/VectorN.h"

namespace kismo
{
	/** \brief Matrix4 class defines a 4 by 4 matrix of floats
	*/
	//template< class _TReal >
	//class KISMOFRX_EXPORT Matrix4
	//{
	//public:

	//	/** \brief special matrix instances
	//	*/		
	//	static const Matrix4 Zero;
	//	static const Matrix4 Identity;
	//	static const Matrix4 Right2left; //< right to left conversion for Dx/opengl conversion

	//	/** \brief Creates a Translation matrix
	//	*/
	//	static Matrix4 translation( _TReal x, _TReal y, _TReal z ); 
	//	 
	//	/** \brief Creates a Local axis scaling matrix
	//	*/
	//	static Matrix4 scaling( _TReal sx, _TReal sy, _TReal sz );

	//	/** \brief Creates a Overall scaling matrix
	//	*/
	//	static Matrix4 scaling( _TReal s ); 

	//	/** \brief Creates a Rotation matrix about the X-axis
	//	*/
	//	static Matrix4 rotationX( _TReal deg );

	//	/** \brief Creates a Rotation matrix about the Y-axis
	//	*/
	//	static Matrix4 rotationY( _TReal deg );

	//	/** \brief Creates a Rotation matrix about the Z-axis
	//	*/
	//	static Matrix4 rotationZ( _TReal deg );

	//	/** \brief Enumerator for defining the order of rotation when using euler angles
	//	*/
	//	enum eAxisOrder
	//	{ 
	//		XYZ = 0, YZX, ZXY, XZY, YXZ, ZYX 
	//	};

	//	/** \brief Creates a Rotation matrix about the 3 major axis in a specified order
	//	*/
	//	static Matrix4 rotation( _TReal x, _TReal y, _TReal z, eAxisOrder order = XYZ );

	//public:

	//	/** \brief Linear array of data to maintain data locality and order across different platforms
	//	*/ 
	//	_TReal m[ 4 * 4 ];

	//	/** \brief Default constructor creates an unitialised matrix for efficiency
	//	*/
	//	Matrix4() {}
	//	
	//	/** \brief _TReal constructor
	//	*/
	//	Matrix4( const _TReal* pf );

	//	/** \brief Constructor with paramaters for all 16 floats
	//	*/
	//	Matrix4(	_TReal f11, _TReal f21, _TReal f31, _TReal f41,
	//				_TReal f12, _TReal f22, _TReal f32, _TReal f42,
	//				_TReal f13, _TReal f23, _TReal f33, _TReal f43,
	//				_TReal f14, _TReal f24, _TReal f34, _TReal f44 );

	//	
	//	/** \brief Access operator for gettin a matrix value from a row and colum index
	//		\remarks Not bounds checked for efficiency
	//	*/
	//	inline _TReal& operator () ( Index col, Index row )
	//	{ return at( col, row ); }
	//	
	//	/** \brief Access operator for gettin a matrix value from a row and colum index
	//		\remarks Not bounds checked for efficiency
	//	*/
	//	inline const _TReal& operator () ( Index col, Index row ) const
	//	{ return at( col, row ); }
	//		
	//	/** \brief Access operator for gettin a matrix value from a row and colum index
	//		\remarks Not bounds checked for efficiency
	//	*/
	//	inline _TReal& at( Index col, Index row )
	//	{ return m[ col + row*4 ]; }
	//	
	//	/** \brief Access operator for gettin a matrix value from a row and colum index
	//		\remarks Not bounds checked for efficiency
	//	*/
	//	inline const _TReal& at( Index col, Index row ) const
	//	{ return m[ col + row*4 ]; }	

	//	/** \brief Cast operator to a _TReal pointer
	//	*/
	//	operator _TReal*()
	//	{ return (_TReal*)m; }

	//	/** \brief Cast operator to a const _TReal pointer
	//	*/
	//	operator const _TReal*() const
	//	{ return (_TReal*)m;  }

	//	/** \brief  Overloaded *= sign
	//	*/
	//	Matrix4< _TReal >& operator *= ( const Matrix4< _TReal >& mat );

	//	/** \brief  Overloaded += sign
	//	*/
	//	Matrix4< _TReal >& operator += ( const Matrix4< _TReal >& mat ); 

	//	/** \brief  Overloaded -= sign
	//	*/
	//	Matrix4< _TReal >& operator -= ( const Matrix4< _TReal >& mat ); 

	//	/** \brief  Overloaded *= sign
	//	*/
	//	Matrix4< _TReal >& operator *= ( _TReal f );

	//	/** \brief  Overloaded /= sign
	//	*/
	//	Matrix4< _TReal >& operator /= ( _TReal f );  
	//	
	//	// Vector operators		
	//	Vector3< _TReal > operator * ( const Vector3< _TReal >& vec ) const;

	//	/** \brief Unary operator for positive... i.e. does nothing!
	//	*/
	//	Matrix4 operator +() const
	//	{ return *this; }

	//	/** \brief Unary operator for negating the matrix cells
	//		\remarks this IS NOT the same as invert()
	//	*/
	//	Matrix4 operator -() const;		
	//	
	//	/** \brief binary operators
	//	*/
	//	Matrix4 operator * ( const Matrix4< _TReal >& mat ) const;
	//	Matrix4 operator + ( const Matrix4< _TReal >& mat ) const;
	//	Matrix4 operator - ( const Matrix4< _TReal >& mat ) const;
	//	Matrix4 operator * ( _TReal f ) const;
	//	Matrix4 operator / ( _TReal f ) const;

	//	/** \brief Tranposes the matrix so the rows become columns and visa-versa
	//	*/
	//	Matrix4 transposed() const;

	//	/** \brief Calculats the inverse of the matrix
	//	*/
	//	Matrix4 inverse() const;

	//	KISMOFRX_EXPORT friend OutStream& operator << ( OutStream& outStream, const Matrix4& m )
	//	{
	//		for ( Index i = 0; i < 4*4 -1; ++i )
	//			outStream << m[i] << _T('\t');

	//		outStream << m[4*4 -1];
	//		return outStream;
	//	}

	//	KISMOFRX_EXPORT friend InStream& operator >> ( InStream& inStream, Matrix4& m )
	//	{
	//		for ( Index i = 0; i < 4*4; ++i )
	//			inStream >> m[i];
	//		return inStream;
	//	}


	//	/** this conversion uses conventions as described on page:
	//	*   http://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
	//	*   Coordinate System: right hand
	//	*   Positive angle: right hand
	//	*   Order of euler angles: heading first, then attitude, then bank
	//	*   matrix row column ordering:
	//	*   [m00 m01 m02]
	//	*   [m10 m11 m12]
	//	*   [m20 m21 m22]
	//	*/
	//	Vector3< _TReal > asEuler() const 
	//	{
	//		// Assuming the angles are in radians.
	//		if ((Real)at(0,1) > (1 - Real_Epsilon)) // singularity at north pole
	//			return Vector3< _TReal >( (_TReal)HalfPI, (_TReal)atan2( (Real)at(2,0), (Real)at(2,2) ), 0 );
	//		if ((Real)at(0,1) < (-1 + Real_Epsilon) ) // singularity at south pole
	//			return Vector3< _TReal >( (_TReal)-HalfPI, (_TReal)atan2( (Real)at(2,0), (Real)at(2,2) ), 0 );

	//		return Vector3< _TReal >( (_TReal)atan2( (Real)-at(2,1), (Real)at(1,1) ), (_TReal)atan2( (Real)-at(0,2) , (Real)at(0,0) ), (_TReal)asin( (Real)at(0,1) ) );
	//	}

	//};


	template < class _Real, Count _Rows, Count _Cols >
	class MatrixNData
	{
	public:
		VectorN< _Real, _Cols > v[ _Rows ]; //Vector storage 
	};

	template < class _Real, Count _Rows, Count _Cols >
	class MatrixN : public MatrixNData<_Real,_Rows,_Cols>
	{
	public:

		typedef VectorN< _Real, _Cols > _VectorN;
		typedef VectorT< _Real, _Cols > _VectorT;

		static MatrixN makeIdentity()
		{
			MatrixN mat;
			for ( Index i = 0; i < _Rows; ++i )
			{
				mat.v[i] = (_Real)0.0;
				if ( i < _Cols ) //< There may be less columns than rows so we make sure that we don't access past the end of the row
					mat.v[i][i] = 1; //< Set the identity matrix to one on the diagonal
			}
			return mat;
		}

		static MatrixN makeUniform( _Real f )
		{
			MatrixN mat;
			for ( Index i = 0; i < _Rows; ++i )
				mat.v[i] = f;
			return mat;
		}

	public:

		MatrixN(){}

		template<Count _Rows2, Count _Cols2>
		MatrixN( const MatrixN<_Real,_Rows2,_Cols2>& rhs ) { *this = rhs; }

		_Real* c_ptr() { return ((_Real*)v); }
		const _Real* c_ptr() const { return ((_Real*)v); }

		_Real& operator [] ( Index i ) { return at(i); }
		_Real& at( Index i ) { assert( i < _Cols*_Rows ); return c_ptr()[ i ]; }

		const _Real& operator [] ( Index i ) const { return at(i); }
		const _Real& at( Index i ) const { assert( x < _Cols*_Rows ); return ((_Real*)v)[ i ]; }

		_Real& operator () ( Index x, Index y ) { return at(x,y); }
		_Real& at( Index col, Index row ) { assert( col < _Cols); assert( row < _Rows); return v[row][col]; }
		
		const _Real& operator () ( Index x, Index y ) const { return at(x,y); }
		const _Real& at( Index col, Index row ) const { assert( col < _Cols); assert( row < _Rows); return v[row][col]; }
		//
		////TODO: Implement for non square etc!
		//MatrixN operator * ( const MatrixN& rhs ) const
		//{
		//	MatrixN tMat;	//new temporary matrix	
		//	for ( Index row = 0; row < _Rows; ++row ) 
		//		for ( Index col = 0; col < _Cols; ++col )
		//			tMat.v[col][row] =	v[ 0 ][ row ] * rhs.v[ col ][ 0 ] +
		//							v[ 1 ][ row ] * rhs.v[ col ][ 1 ] +
		//							v[ 2 ][ row ] * rhs.v[ col ][ 2 ] +
		//							v[ 3 ][ row ] * rhs.v[ col ][ 3 ];
		//	return tMat;
		//}
		
		VectorN< _Real, _Cols > operator * ( const VectorN< _Real, _Cols >& rhs ) const
		{
			//Calculate the homogeneous element as if = 1
			//_TReal h = vec.x * _41 + vec.y * _42 + vec.z * _43 + _44;
			
			//If zero represent the FLOAT as the smallest possible value
			//if ( h == 0.0f )
			//	h = (_TReal)Real_MIN;

			VectorN< _Real, _Rows > ret;
			for ( Index col = 0; col < _Cols; ++col )
			{
				ret[col] = 0;
				for ( Index row = 0; row < _Rows; ++row ) 
					ret[col] += (row <_Cols? rhs[row] : float(row==col)) * v[ row ][ col ];
			}
			
			if ( _Rows > _Cols ) //< Apply the homogenous scale - this won't work if the vector has more than 1 extra row etc
				return VectorN< _Real, _Cols >(ret) / ret[_Rows];
			else
				return ret;
		}
		
		friend VectorN< _Real, _Cols > operator * ( const VectorN< _Real, _Cols >& lhs, const MatrixN& rhs )
		{ return rhs * lhs; }

	protected:
		
		/**
			\brief Conversion from any 3-column matrix
			\remarks Currently if the matrix being copied from has fewer rows than this then the remaining rows will be left as is
		*/
		template<Count _Rows2, Count _Cols2>
		void operator = ( const MatrixN<_Real,_Rows2,_Cols2>& rhs )
		{
			Count minRows = ( _Rows < _Rows2 ? _Rows : _Rows2 );
			Count minCols = ( _Cols < _Cols2 ? _Cols : _Cols2 );

			//Copy all the data that exists within both matrices first which is minRows,minCols in size
			for ( Index row = 0; row < minRows; ++row )
				for ( Index col = 0; col < minCols; ++col )
					v[row][col] = rhs.v[row][col];

			//Now fill in the remaining as identity for columns and rows if there are more columns or rows in the target matrix
			// - Fors rows upto minRows we only fill in the elements we missed, after that we do the whole row
			//TODO: we could do this with a branch for assigning from rhs instead.. todo, test speed!
			for ( Index row = 0; row < _Rows; ++row )
				for ( Index col = (row<minRows? minCols : 0 ); col < _Cols; ++col )
					v[row][col] = ( row==col ? (_Real)1.0: (_Real)0.0 );
		}


	};

	template < class _Real, Count _Rows, Count _Cols >
	class MatrixT : public MatrixN<_Real,_Rows,_Cols> 
	{
	public:
		MatrixT() {}
		MatrixT( const MatrixN<_Real,_Rows,_Cols>& rhs ) : MatrixN<_Real,_Rows,_Cols>(rhs) {}
	};

	template < class _Real >
	class MatrixT<_Real,3,3> : public MatrixN<_Real,3,3> 
	{
	public:

		static MatrixT makeIdentity() 
		{ return MatrixN::makeIdentity(); }

		static MatrixN rotationX( _Real rad )
		{
			_Real c = (_Real)cos(rad), s = (_Real)sin(rad);
			MatrixN t;
			t.v[0] = MatrixN::_VectorT( 1, 0, 0 /*,0*/ );
			t.v[1] = MatrixN::_VectorT( 0, c, s /*,0*/ );
			t.v[2] = MatrixN::_VectorT( 0, -s, c /*,0*/ );
			return t;
		}
		
		static MatrixN rotationY( _Real rad )
		{
			_Real c = (_Real)cos(rad), s = (_Real)sin(rad);
			MatrixN t;
			t.v[0] = MatrixN::_VectorT(	c, 0, -s /*,0*/ );
			t.v[1] = MatrixN::_VectorT( 0, 1, 0 /*,0*/ );
			t.v[2] = MatrixN::_VectorT( s, 0, c /*,0*/ );
			return t;
		}
		
		static MatrixN rotationZ( _Real rad )
		{
			_Real c = (_Real)cos(rad), s = (_Real)sin(rad);
			MatrixN t;
			t.v[0] = MatrixN::_VectorT(	c, s, 0 /*,0*/ );
			t.v[1] = MatrixN::_VectorT( -s, c, 0 /*,0*/ );
			t.v[2] = MatrixN::_VectorT( 0, 0, 1 /*,0*/ );
			return t;
		}
		
		//static MatrixN rotation( _Real x, _Real y, _Real z, eAxisOrder order )
		//{
		//	MatrixN n;
		//
		//	//Loop through the 3 axis
		//	for (INT32 i = 0; i < 3; i++)				
		//		switch ( AxisOrderSequence[ order ][ i ] )//Perform a switch on the axis ID
		//		{
		//		case eX: n *= CMatrix4::rotationX( x );  break;
		//		case eY: n *= CMatrix4::rotationY( y ); break;
		//		case eZ: n *= CMatrix4::rotationZ( z );  break;
		//		}
		//
		//	return n;
		//}

	public:
		MatrixT() {}
		MatrixT( const MatrixN<_Real,3,3>& rhs ) : MatrixN<_Real,3,3>(rhs) {}


		MatrixT operator * ( const MatrixT& rhs ) const
		{
			const Count _Rows = 3;
			const Count _Cols = 3;

			MatrixT tMat;	//new temporary matrix	
			for ( Index row = 0; row < _Rows; ++row ) 
				for ( Index col = 0; col < _Cols; ++col )
					tMat.v[col][row] =	v[ 0 ][ row ] * rhs.v[ col ][ 0 ] +
									v[ 1 ][ row ] * rhs.v[ col ][ 1 ] +
									v[ 2 ][ row ] * rhs.v[ col ][ 2 ];
			return tMat;
		}
		MatrixN& operator *= ( const MatrixN& rhs )
		{ *this = *this * rhs; return *this; }
	};

	template < class _Real >
	class MatrixT<_Real,4,3> : public MatrixN<_Real,4,3> 
	{
	public:

		static MatrixT makeIdentity() 
		{ return MatrixN::makeIdentity(); }

	public:

		MatrixT() {}
		MatrixT( const MatrixN<_Real,4,3>& rhs ) : MatrixN<_Real,4,3>(rhs) {}
		MatrixT( const MatrixN<_Real,4,4>& rhs ) { *this = rhs; }

		/**
			\brief Cast 4x3 to Nx3 i.e  to access just the rotatio part fo the matrix
			\remarks Asserts if N is greater than _Rows
		*/
		template<Count _Rows2> 
		operator const MatrixT<_Real,_Rows2,3>& () const
		{ assert(_Rows2<4); return *(MatrixT<_Real,_Rows2,3>*)this; }

		/**
			\brief Cast 4x3 to Nx3 i.e  to access just the rotatio part fo the matrix
			\remarks Asserts if N is greater than _Rows
		*/
		template<Count _Rows2> 
		operator MatrixT<_Real,_Rows2,3>& ()
		{ assert(_Rows2<4); return *(MatrixT<_Real,_Rows2,3>*)this; }

		//operator const MatrixN<_Real,4,3>& () const { return *this; }
		//operator MatrixN<_Real,4,3>& () { return *this; }

		void operator = ( const MatrixN<_Real,4,4>& rhs ) { *(MatrixN*)this = rhs; }

		//template<Count _Rows2>
		//void operator = ( const MatrixT<_Real,_Rows2,3>& rhs )
		//{
		//	const Index rows =(_Rows2>4 ? 4 : _Rows2); //, maximum rows = _Rows
		//	Index i =0;
		//	for ( ; i < rows; ++i )
		//	{
		//		v[i] = rhs.v[i];
		//		v[i][3] = ( i==3 ? (_Real)1.0: (_Real)0.0 ); //< Fill the alst column correctly with a unit scale	
		//	}

		//	//If the number of rows doesn't match we make the last rows identity
		//	for ( ; i < 4; ++i )
		//	{
		//		v[i] = 0;
		//		v[i][i] = (_Real)1; //< Fill the alst column correctly with a unit scale	
		//	}
		//}
	};

	template < class _Real >
	class MatrixT<_Real,4,4> : public MatrixN<_Real,4,4> 
	{
	public:

		static MatrixT makeIdentity() 
		{ return MatrixN::makeIdentity(); }

	public:
		/**
			\brief Construct a Perspective matrix from frustum angles and near/far distances
			\remarks Equivilent to glFrustum()
		*/
		static MatrixT frustum( _Real left, _Real right, _Real bottom, _Real top, _Real zNear, _Real zFar )
		{
			_Real a = (right+left)/(right-left);
			_Real b = (top+bottom)/(top-bottom);
			_Real c = -((zFar+zNear)/(zFar-zNear));
			_Real d = -((2*zFar*zNear)/(zFar-zNear));
			
			MatrixT mat;
			mat.v[0] = _VectorT( _Real((2*zNear)/(right-left)), (_Real)0,						(_Real)a,	(_Real)0 );
			mat.v[1] = _VectorT( (_Real)0,						_Real((2*zNear)/(top-bottom)),	(_Real)b,	(_Real)0 );
			mat.v[2] = _VectorT( (_Real)0,						(_Real)0,						(_Real)c,	(_Real)d );
			mat.v[3] = _VectorT( (_Real)0,						(_Real)0,						(_Real)-1,	(_Real)0 );
			return mat;
		}

	public:

		MatrixT() {}
		MatrixT( const MatrixN<_Real,4,4>& rhs ) : MatrixN<_Real,4,4>(rhs) {}
		MatrixT( const MatrixN<_Real,4,3>& rhs ) { *this = rhs; }
		MatrixT( const MatrixN<_Real,3,3>& rhs ) { *this = rhs; }

		void operator = ( const MatrixN<_Real,4,3>& rhs ) { *(MatrixN*)this = rhs; }
		void operator = ( const MatrixN<_Real,3,3>& rhs ) { *(MatrixN*)this = rhs; }

		//operator const MatrixN<_Real,4,4>& () const { return *this; }
		//operator MatrixN<_Real,4,4>& () { return *this; }

		
		/**
			\brief Conversion from any 3-column matrix
			\remarks Currently if the matrix being copied from has fewer rows than this then the remaining rows will be left as is
		*/
		//template<Count _Rows2>
		//void operator = ( const MatrixT<_Real,_Rows2,3>& rhs )
		//{
		//	const Index rows =(_Rows2>4 ? 4 : _Rows2); //, maximum rows = _Rows
		//	Index i =0;
		//	for ( ; i < rows; ++i )
		//	{
		//		v[i] = rhs.v[i];
		//		v[i][3] = ( i==3 ? (_Real)1.0: (_Real)0.0 ); //< Fill the alst column correctly with a unit scale	
		//	}

		//	//If the number of rows doesn't match we make the last rows identity
		//	for ( ; i < 4; ++i )
		//	{
		//		v[i] = 0;
		//		v[i][i] = (_Real)1; //< Fill the alst column correctly with a unit scale	
		//	}
		//}

		
		//TODO: Implement for non square etc!
		MatrixT operator * ( const MatrixT& rhs ) const
		{
			const Count _Rows = 4;
			const Count _Cols = 4;

			MatrixT tMat;	//new temporary matrix	
			for ( Index row = 0; row < _Rows; ++row ) 
				for ( Index col = 0; col < _Cols; ++col )
					tMat.v[col][row] =	v[ 0 ][ row ] * rhs.v[ col ][ 0 ] +
									v[ 1 ][ row ] * rhs.v[ col ][ 1 ] +
									v[ 2 ][ row ] * rhs.v[ col ][ 2 ] +
									v[ 3 ][ row ] * rhs.v[ col ][ 3 ];
			return tMat;
		}
		MatrixN& operator *= ( const MatrixN& rhs )
		{ *this = *this * rhs; return *this; }

	};

	typedef MatrixT< Real, 2, 2 > Matrix22;
	typedef MatrixT< Float32, 2, 2 > Matrix22f;
	typedef MatrixT< Float64, 2, 2 > Matrix22d;

	typedef MatrixT< Real, 3, 3 > Matrix33;
	typedef MatrixT< Float32, 3, 3 > Matrix33f;
	typedef MatrixT< Float64, 3, 3 > Matrix33d;

	typedef MatrixT< Real, 4, 3 > Matrix43;
	typedef MatrixT< Float32, 4, 3 > Matrix43f;
	typedef MatrixT< Float64, 4, 3 > Matrix43d;

	typedef MatrixT< Real, 4, 4 > Matrix44;
	typedef MatrixT< Float32, 4, 4 > Matrix44f;
	typedef MatrixT< Float64, 4, 4 > Matrix44d;

} // END: math


#endif




