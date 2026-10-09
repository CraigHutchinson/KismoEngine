/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Vector.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Vector.h (same name, content differs)
 */
#ifndef VECTOR_H
#define VECTOR_H

#include "Platform/Platform.h"
#include "Framework/Types.h"
#include "Framework/CoreMath.h"

namespace frx
{
	//Forward delcarations
	template< class _TReal > class Matrix4;

	/** \brief 2 dimensional vector class
	*/
	template< class _TReal >
	class _DllExport Vector2
	{
	public:

		// special vectors
		static const Vector2 Zero;
		static const Vector2 Unit_X;
		static const Vector2 Unit_Y;
		static const Vector2 Unit_SCALE;
		static const Vector2 InvUnit_X;
		static const Vector2 InvUnit_Y;
		static const Vector2 InvUnit_SCALE;

	public:    
		
		_TReal x, y; // Vectors x, y, and z coordinates

		//Constructor - WARNING: does not initialise the vector for efficiency
		Vector2(){}

		//Constructor takes 3 floats defining the 3 axis scalar
		Vector2( _TReal f ) : x(f), y(f) {}
		Vector2( _TReal nx, _TReal ny ) : x(nx), y(ny) {}
		Vector2( const _TReal n[2] ) : x( n[0]) , y( n[1] ) {}
		
		//Copy constructor
		Vector2( const Vector2& v ) : x(v.x) , y(v.y) {}

		//Array accessors
		_TReal operator [] ( const Index i ) const 
		{ 
			assert( i < 2 ); 
			return *(&x + i); 
		}
		_TReal& operator [] ( const Index i ) 
		{ 
			assert( i < 2 ); 
			return *(&x + i);
		}

		//cast operators
		operator const _TReal*() const { return &x; }
		operator _TReal*() { return &x; }

		//assignment operators
		Vector2& operator =( const Vector2& v ) { x = v.x; y = v.y; return (*this); }
		Vector2& operator =( const _TReal f ) { x = y = f; return (*this); }

		//Unary operators
		Vector2 operator +( ) const { return Vector2( +x, +y ); }
		Vector2 operator -( ) const { return Vector2( -x, -y ); }

		//binary operators
		Vector2 operator -( const Vector2& v ) const { return Vector2( x - v.x, y - v.y ); }
		Vector2 operator +( const Vector2& v ) const { return Vector2( x + v.x, y + v.y ); }
		Vector2 operator *(const Vector2& v) const { return Vector2(x * v.x, y * v.y ); }
		Vector2 operator /(const Vector2& v) const { return Vector2(x / v.x, y / v.y ); }

		Vector2 operator +( _TReal f ) const { return Vector2(x + f, y + f ); } 
		Vector2 operator -( _TReal f ) const { return Vector2(x - f, y - f ); } 
		Vector2 operator *( _TReal f ) const { return Vector2(x * f, y * f ); }
		Vector2 operator /( _TReal f ) const { return Vector2(x / f, y / f ); }


		Vector2& operator +=(const Vector2& v) { x += v.x; y += v.y; return (*this); }
		Vector2& operator -=(const Vector2& v) { x -= v.x; y -= v.y; return (*this); }
		Vector2& operator *=(const Vector2& v) { x *= v.x; y *= v.y; return (*this); }
		Vector2& operator /=(const Vector2& v) { x /= v.x; y /= v.y; return (*this); }

		Vector2& operator +=( _TReal f ) { x += f; y += f; return (*this); }
		Vector2& operator -=( _TReal f ) { x -= f; y -= f; return (*this); }
		Vector2& operator *=( _TReal f ) { x *= f; y *= f; return (*this); }
		Vector2& operator /=( _TReal f ) { x /= f; y /= f; return (*this); }

		// Boolean operators.
		bool operator ==( const Vector2& v ) const { return (x == v.x) && (y == v.y); }
		bool operator ==( const _TReal f ) const { return (x == f) && (y == f); }
		bool operator !=( const Vector2& v ) const { return (x != v.x) || (y!= v.y); } 
		bool operator !=( const _TReal f ) const { return (x != f) || (y!= f); } 

		//lengths		
		Real length() const { return math::length( (Real)x, (Real)y ); } //Uses a more accurate+slower Sqrt
		Real fastLength() const { return math::fastLength( (Real)x, (Real)y ); } //Uses a FastSqrt
		Real lengthSquared() const { return math::lengthSquared( (Real)x, (Real)y ); } //Saves a Sqrt
		Real estimatedLength() const { return math::estimatedLength( (Real)x, (Real)y ); }

		//Normalise this vector by making its length 1 unit length
		Vector2 normalised() const
		{
			Real len = length();
			if(len == 0.0f) { return (*this); } //Return the zero vector 
			return Vector2( (_TReal)(x / len), (_TReal)(y / len) );
		}

		Vector2& normalise()
		{
			Real len = length();
			if (len != 0.0f) 
			{ 
				x = (_TReal)(x / len); //TODO: Can you do a type conversion in a '/=' ?
				y = (_TReal)(y / len); 
			}
			return (*this);
		}	

		//Dot products
		_TReal dot( const Vector2& v) const { return (v.x * x) + (v.y * y); }

		//Rotate the vector by an angle(Radians) about an axis 
		void rotateZ( Real r )
		{
			Real c = math::cos(r); Real s = math::sin(r);
			_TReal nx = (_TReal)((x * c) - (y * s));
			y = (_TReal)((y * c) + (x * s));
			x = nx;
		}

		/*_DllExport*/ friend OutStream& operator << ( OutStream& outStream, const Vector2& v )
		{
			outStream << v.x << _T('\t') << v.y;
			return outStream;
		}
		/*_DllExport*/ friend InStream& operator >> ( InStream& inStream, Vector2& v )
		{
			inStream >> v.x >> v.y;
			return inStream;
		}
	};

	/** \brief 3 dimensional vector class
	*/
	template< class _TReal >
	class _DllExport Vector3
	{
	public:

		typedef Vector< Vector3 > Array;
		
		// special vectors
		static const Vector3 Zero;
		static const Vector3 Unit_X;
		static const Vector3 Unit_Y;
		static const Vector3 Unit_Z;
		static const Vector3 Unit_SCALE;
		static const Vector3 InvUnit_X;
		static const Vector3 InvUnit_Y;
		static const Vector3 InvUnit_Z;
		static const Vector3 InvUnit_SCALE;

	public:    
		_TReal x, y, z; // Vectors x, y, and z coordinates

		/** \brief Constructor
		\remarks WARNING: does not initialise the vector for efficiency 
		*/
		Vector3() {}

		/** \brief Constructor takes 1 float
		\param[in] f Float value assigned to all the axis values 
		*/
		Vector3( _TReal f ) : 
			x(f), y(f), z(f) {}

		/** \brief Constructor takes 3 floats defining the 3 axis scalar
		\param[in] x X axis coordinate
		\param[in] y Y axis coordinate
		\param[in] z Z axis coordinate 
		*/
		Vector3( _TReal nx, _TReal ny, _TReal nz ) : 
			x(nx), y(ny), z(nz) {}

		/** \brief Constructor takes an aray of 3 scalar floats
		\param[in] n Array of three floats defining the X, Y, and Z axis coordinates 
		*/
		Vector3( const _TReal n[3] ) : 
			x(n[0]), y(n[1]), z(n[2]) {}

		/** \brief Copy constructor 
		*/
		Vector3( const Vector3& v ) : 
			x(v.x) , y(v.y), z(v.z) {}

		/** \brief String constructor 
		*/
		Vector3( const String& str )
		{ *this = str; }

		/** \brief Const array accessor
		\param[in] i The index from 0 to 2
		\return the value at the index
		*/
		_TReal operator []( const Index i ) const 
		{ 
			assert( i < 3 ); 
			return *(&x + i);
		}
		/** \brief Array accessor
		\param[in] i The index from 0 to 2
		\return Reference to the value at the index */
		_TReal& operator []( const Index i ) 
		{ 
			assert( i < 3 ); 
			return *(&x + i);
		}
		/** \brief Const-Cast operator to a _TReal pointer 
		*/
		const _TReal* ptr() const 
		{ return &x; }

		/** \brief Cast operator to a _TReal pointer 
		*/
		_TReal* ptr() 
		{ return &x; }

		//assignment operators
		Vector3& operator =( const Vector3& v ) 
		{ x = v.x; y = v.y; z = v.z; return (*this); }

		Vector3& operator =( const _TReal f ) 
		{ x = y = z = f; return (*this); }

		/** \brief String assignment
		*/
		Vector3& operator =( const String& str );

		//Unary operators
		Vector3 operator +( ) const { return Vector3( +x, +y, +z ); }
		Vector3 operator -( ) const { return Vector3( -x, -y, -z ); }

		//binary operators
		Vector3 operator -( const Vector3& v ) const { return Vector3( x - v.x, y - v.y, z - v.z ); }
		Vector3 operator +( const Vector3& v ) const { return Vector3( x + v.x, y + v.y, z + v.z ); }
		Vector3 operator *(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
		Vector3 operator /(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

		Vector3 operator +( _TReal f ) const { return Vector3(x + f, y + f, z + f); } 
		Vector3 operator -( _TReal f ) const { return Vector3(x - f, y - f, z - f); } 
		Vector3 operator *( _TReal f ) const { return Vector3(x * f, y * f, z * f); }
		Vector3 operator /( _TReal f ) const { return Vector3(x / f, y / f, z / f); }


		Vector3& operator +=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return (*this); }
		Vector3& operator -=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return (*this); }
		Vector3& operator *=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return (*this); }
		Vector3& operator /=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return (*this); }

		Vector3& operator +=(_TReal f) { x += f; y += f; z += f; return (*this); }
		Vector3& operator -=(_TReal f) { x -= f; y -= f; z -= f; return (*this); }
		Vector3& operator *=(_TReal f) { x *= f; y *= f; z *= f; return (*this); }
		Vector3& operator /=(_TReal f) { x /= f; y /= f; z /= f; return (*this); }

		//Matrix operations		
		Vector3 operator * ( const Matrix4< _TReal >& mat ) const;
		Vector3< _TReal >& operator *= ( const Matrix4< _TReal >& m );

		// Boolean operators.
		bool operator ==( const Vector3& v ) const { return (x == v.x) && (y == v.y) && (z == v.z); }
		bool operator ==( const _TReal f ) const { return (x == f) && (y == f) && (z == f); }
		bool operator !=( const Vector3& v ) const { return (x != v.x) || (y!= v.y) || (z != v.z); } 
		bool operator !=( const _TReal f ) const { return (x != f) || (y!= f) || (z != f); } 

		//lengths		
		_TReal length() const { return math::length( x, y, z ); } ///<Uses a more accurate+slower Sqrt
		_TReal fastLength() const { return math::fastLength( x, y, z ); } ///<Uses a FastSqrt
		_TReal lengthSquared() const { return math::lengthSquared( x, y, z ); } ///<Saves a Sqrt
		_TReal estimatedLength() const { return math::estimatedLength( x, y, z ); }

		//Normalise this vector by making its length 1 unit length
		Vector3 normalised() const
		{
			_TReal len = length();
			if(len == 0.0f) { return (*this); } //Return the zero vector 
			return Vector3( x / len, y / len, z / len );
		}

		Vector3& normalise()
		{
			_TReal len = length();
			if (len != 0.0f) { x /= len; y /= len; z /= len; }
			return (*this);
		}	

		//Dot & Cross products
		Vector3 cross( const Vector3& v ) const 
		{ return Vector3( (y * v.z)-(z * v.y), -((x * v.z)-(z * v.x)), (x * v.y)-(y * v.x) ); } 

		_TReal dot( const Vector3& v ) const
		{ return (v.x * x) + (v.y * y) + (v.z * z); }

		//Rotate the vector by an angle(Radians) about an axis 
		void rotateX( Real r )
		{
			Real c = math::cos(r); Real s = math::sin(r);
			_TReal ny = (_TReal)((y * c) - (z * s));
			z = (_TReal)((z * c) + (y * s));
			y = ny;
		}
		void rotateY( Real r )
		{
			Real c = math::cos(r); Real s = math::sin(r);
			_TReal nx = (_TReal)((x * c) + (z * s));
			z = (_TReal)((z * c) - (x * s));
			x = nx;
		}
		void rotateZ( Real r )
		{
			Real c = math::cos(r); Real s = math::sin(r);
			_TReal nx = (_TReal)((x * c) - (y * s));
			y = (_TReal)((y * c) + (x * s));
			x = nx;
		}
		
		//Stream operator
		friend OutStream& operator << ( OutStream& outStream, const Vector3& v )
		{
			outStream << v.x << _T('\t') << v.y << _T('\t') << v.z;
			return outStream;
		}
		friend InStream& operator >> ( InStream& inStream, Vector3& v )
		{
			inStream >> v.x >> v.y >> v.z;
			return inStream;
		}
	};
	
	//Typedef the string types
	typedef Vector2< Int16 > Vector2s;
	typedef Vector2< Int32 > Vector2i;
	typedef Vector3< Real > Vector2r;
	typedef Vector2< Float32 > Vector2f;
	typedef Vector2< Float64 > Vector2d;

	typedef Vector3< Int16 > Vector3s;
	typedef Vector3< Int32 > Vector3i;
	typedef Vector3< Real > Vector3r;
	typedef Vector3< Float32 > Vector3f;
	typedef Vector3< Float64 > Vector3d;

} //END: frx


#endif