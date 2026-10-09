/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoVector.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_VECTOR_H
#define	_KISMO_VECTOR_H

#include "framework/KismoMath.h"
#include "framework/KismoPlatform.h"

namespace kismo
{
	namespace frx
	{
		template< typename _T >
		class _KismoExport Vector2
		{
		public:

			// special vectors
			static const Vector2 ZERO;
			static const Vector2 UNIT_X;
			static const Vector2 UNIT_Y;
			static const Vector2 NEGATIVE_UNIT_X;
			static const Vector2 NEGATIVE_UNIT_Y;
			static const Vector2 UNIT_SCALE;

		public:    
			_T x, y; // vertex's x, y coordinates

			//Constructor - WARNING: does not initialise the vector for efficiency
			inline Vector2(){}

			//Constructor takes 3 floats defining the 3 axis scalar
			inline Vector2( _T f ) : x(f), y(f) {}
			inline Vector2( _T nx, _T ny ) : x(nx), y(ny) {}
			inline Vector2( const _T n[2] ) : x( n[0]) , y( n[1] ) {}
			
			//Copy constructor
			inline Vector2( const Vector2& v ) : x(v.x) , y(v.y) {}

			//Array accessors
			inline _T operator [] ( const k_Index i ) const 
			{ 
				assert( i < 2 ); 
				return *( &x + i ); 
			}
			inline _T& operator [] ( const k_Index i ) 
			{ 
				assert( i < 2 ); 
				return *( &x + i );
			}

			//cast operators
			inline operator const _T*() const { return &x; }
			inline operator _T*() { return &x; }

			//assignment operators
			inline Vector2& operator =( const Vector2& v ) { x = v.x; y = v.y; return (*this); }
			inline Vector2& operator =( const _T f ) { x = y = f; return (*this); }

			//Unary operators
			inline Vector2 operator +( ) const { return Vector2( +x, +y ); }
			inline Vector2 operator -( ) const { return Vector2( -x, -y ); }

			//binary operators
			inline Vector2 operator -( const Vector2& v ) const { return Vector2( x - v.x, y - v.y ); }
			inline Vector2 operator +( const Vector2& v ) const { return Vector2( x + v.x, y + v.y ); }
			inline Vector2 operator *(const Vector2& v) const { return Vector2(x * v.x, y * v.y ); }
			inline Vector2 operator /(const Vector2& v) const { return Vector2(x / v.x, y / v.y ); }

			inline Vector2 operator +( _T f ) const { return Vector2(x + f, y + f ); } 
			inline Vector2 operator -( _T f ) const { return Vector2(x - f, y - f ); } 
			inline Vector2 operator *( _T f ) const { return Vector2(x * f, y * f ); }
			inline Vector2 operator /( _T f ) const { return Vector2(x / f, y / f ); }


			inline Vector2& operator +=(const Vector2& v) { x += v.x; y += v.y; return (*this); }
			inline Vector2& operator -=(const Vector2& v) { x -= v.x; y -= v.y; return (*this); }
			inline Vector2& operator *=(const Vector2& v) { x *= v.x; y *= v.y; return (*this); }
			inline Vector2& operator /=(const Vector2& v) { x /= v.x; y /= v.y; return (*this); }

			inline Vector2& operator +=( _T f ) { x += f; y += f; return (*this); }
			inline Vector2& operator -=( _T f ) { x -= f; y -= f; return (*this); }
			inline Vector2& operator *=( _T f ) { x *= f; y *= f; return (*this); }
			inline Vector2& operator /=( _T f ) { x /= f; y /= f; return (*this); }

			// Boolean operators.
			inline bool operator ==( const Vector2& v ) const { return (x == v.x) && (y == v.y); }
			inline bool operator ==( const _T f ) const { return (x == f) && (y == f); }
			inline bool operator !=( const Vector2& v ) const { return (x != v.x) || (y!= v.y); } 
			inline bool operator !=( const _T f ) const { return (x != f) || (y!= f); } 

			//lengths		
			k_Real trueLength() const { return math::trueLength( (k_Real)x, (k_Real)y ); } //Uses a more accurate+slower Sqrt
			k_Real length() const { return math::length( (k_Real)x, (k_Real)y ); } //Uses a FastSqrt
			k_Real lengthSquared() const { return math::lengthSquared( (k_Real)x, (k_Real)y ); } //Saves a Sqrt
			k_Real estimatedLength() const { return math::estimatedLength( (k_Real)x, (k_Real)y ); }

			//Normalise this vector by making its length 1 unit length
			inline Vector2 normalised() const
			{
				k_Real len = length();
				if(len == 0.0f) { return (*this); } //Return the zero vector 
				return Vector2( (_T)(x / len), (_T)(y / len) );
			}

			inline Vector2& normalise()
			{
				k_Real len = length();
				if (len != 0.0f) 
				{ 
					x = (_T)(x / len); //TODO: Can you do a type conversion in a '/=' ?
					y = (_T)(y / len); 
				}
				return (*this);
			}	

			//Dot products
			inline _T dot( const Vector2& v) const { return (v.x * x) + (v.y * y); }

			//Rotate the vector by an angle(Radians) about an axis 
			inline void rotateZ( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				_T nx = (_T)((x * c) - (y * s));
				y = (_T)((y * c) + (x * s));
				x = nx;
			}

			//Stream operator
			inline friend k_OStream& operator << ( k_OStream& outStream, const Vector2& v )
			{
				outStream << v.x << ' ' << v.y << ' ';
				return outStream;
			}
			inline friend k_IStream& operator >> ( k_IStream& inStream, Vector2& v )
			{
				inStream >> (_T)v.x >> (_T)v.y;
				return inStream; //Allow cin >> x >> y
			}

		};

		class _KismoExport Vector3
		{
		public:

			typedef k_Vector< Vector3 > Array;
			
			// special vectors
			static const Vector3 ZERO;
			static const Vector3 UNIT_X;
			static const Vector3 UNIT_Y;
			static const Vector3 UNIT_Z;
			static const Vector3 NEGATIVE_UNIT_X;
			static const Vector3 NEGATIVE_UNIT_Y;
			static const Vector3 NEGATIVE_UNIT_Z;
			static const Vector3 UNIT_SCALE;

		public:    
			union
			{
				struct 
				{
					k_Real x, y, z; // Vectors x, y, and z coordinates
				};
				k_Real data[3]; //Array access
			};

			/** \brief Constructor
			\remarks WARNING: does not initialise the vector for efficiency */
			inline Vector3() {}
			/** \brief Constructor takes 1 float
			\param[in] f Float value assigned to all the axis values */
			inline Vector3( k_Real f ) : 
				x(f), y(f), z(f) {}
			/** \brief Constructor takes 3 floats defining the 3 axis scalar
			\param[in] x X axis coordinate
			\param[in] y Y axis coordinate
			\param[in] z Z axis coordinate */
			inline Vector3( k_Real nx, k_Real ny, k_Real nz ) : 
				x(nx), y(ny), z(nz) {}
			/** \brief Constructor takes an aray of 3 scalar floats
			\param[in] n Array of three floats defining the X, Y, and Z axis coordinates */
			inline Vector3( const k_Real n[3] ) : 
				x(n[0]), y(n[1] ), z(n[2]) {}			
			/** \brief Copy constructor */
			inline Vector3( const Vector3& v ) : 
				x(v.x) , y(v.y), z(v.z) {}
			/** \brief Const array accessor
			\param[in] i The index from 0 to 2
			\return the value at the index*/
			inline k_Real operator []( const k_Index i ) const 
			{ 
				assert( i < 3 ); 
				return data[i]; 
			}
			/** \brief Array accessor
			\param[in] i The index from 0 to 2
			\return Reference to the value at the index */
			inline k_Real& operator []( const k_Index i ) 
			{ 
				assert( i < 3 ); 
				return data[i];
			}
			/** \brief Const-Cast operator to a k_Real pointer */
			//inline operator const k_Real*() const { return data; }
			/** \brief Cast operator to a k_Real pointer */
			//inline operator k_Real*() { return data; }

			//assignment operators
			inline Vector3& operator =( const Vector3& v ) { x = v.x; y = v.y; z = v.z; return (*this); }
			inline Vector3& operator =( const k_Real f ) { x = y = z = f; return (*this); }

			//Unary operators
			inline Vector3 operator +( ) const { return Vector3( +x, +y, +z ); }
			inline Vector3 operator -( ) const { return Vector3( -x, -y, -z ); }

			//binary operators
			inline Vector3 operator -( const Vector3& v ) const { return Vector3( x - v.x, y - v.y, z - v.z ); }
			inline Vector3 operator +( const Vector3& v ) const { return Vector3( x + v.x, y + v.y, z + v.z ); }
			inline Vector3 operator *(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
			inline Vector3 operator /(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

			inline Vector3 operator +(k_Real f) const { return Vector3(x + f, y + f, z + f); } 
			inline Vector3 operator -(k_Real f) const { return Vector3(x - f, y - f, z - f); } 
			inline Vector3 operator *(k_Real f) const { return Vector3(x * f, y * f, z * f); }
			inline Vector3 operator /(k_Real f) const { return Vector3(x / f, y / f, z / f); }


			inline Vector3& operator +=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return (*this); }
			inline Vector3& operator -=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return (*this); }
			inline Vector3& operator *=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return (*this); }
			inline Vector3& operator /=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return (*this); }

			inline Vector3& operator +=(k_Real f) { x += f; y += f; z += f; return (*this); }
			inline Vector3& operator -=(k_Real f) { x -= f; y -= f; z -= f; return (*this); }
			inline Vector3& operator *=(k_Real f) { x *= f; y *= f; z *= f; return (*this); }
			inline Vector3& operator /=(k_Real f) { x /= f; y /= f; z /= f; return (*this); }

			// Boolean operators.
			inline bool operator ==( const Vector3& v ) const { return (x == v.x) && (y == v.y) && (z == v.z); }
			inline bool operator ==( const k_Real f ) const { return (x == f) && (y == f) && (z == f); }
			inline bool operator !=( const Vector3& v ) const { return (x != v.x) || (y!= v.y) || (z != v.z); } 
			inline bool operator !=( const k_Real f ) const { return (x != f) || (y!= f) || (z != f); } 

			//lengths		
			k_Real trueLength() const { return math::trueLength( x, y, z ); } //Uses a more accurate+slower Sqrt
			k_Real length() const { return math::length( x, y, z ); } //Uses a FastSqrt
			k_Real lengthSquared() const { return math::lengthSquared( x, y, z ); } //Saves a Sqrt
			k_Real estimatedLength() const { return math::estimatedLength( x, y, z ); }

			//Normalise this vector by making its length 1 unit length
			inline Vector3 normalised() const
			{
				k_Real len = length();
				if(len == 0.0f) { return (*this); } //Return the zero vector 
				return Vector3( x / len, y / len, z / len );
			}

			inline Vector3& normalise()
			{
				k_Real len = length();
				if (len != 0.0f) { x /= len; y /= len; z /= len; }
				return (*this);
			}	

			//Dot & Cross products
			inline Vector3 cross( const Vector3& v) const { return Vector3( (y * v.z)-(z * v.y), (z * v.x)-(x * v.z), (x * v.y)-(y * v.x) ); } 
			inline k_Real dot( const Vector3& v) const { return (v.x * x) + (v.y * y) + (v.z * z); }

			//Rotate the vector by an angle(Radians) about an axis 
			inline void rotateX( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				k_Real ny = (y * c) - (z * s);
				z = (z * c) + (y * s);
				y = ny;
			}
			inline void rotateY( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				k_Real nx = (x * c) + (z * s);
				z = (z * c) - (x * s);
				x = nx;
			}
			inline void rotateZ( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				k_Real nx = (x * c) - (y * s);
				y = (y * c) + (x * s);
				x = nx;
			}

			//Stream operator
			inline friend k_OStream& operator << ( k_OStream& outStream, const Vector3& v )
			{
				outStream << v.x << " " << v.y << ' ' << v.z << ' ';
				return outStream;
			}
			inline friend k_IStream& operator >> ( k_IStream& inStream, Vector3& v )
			{
				inStream >> (k_Real)v.x >> (k_Real)v.y >> (k_Real)v.z;
				return inStream; //Allow cin >> x >> y >> z
			}

		};

		class _KismoExport Vector4
		{
		public:
			
			//Special vectors
			static const Vector4 ZERO;
			static const Vector4 UNIT_X;
			static const Vector4 UNIT_Y;
			static const Vector4 UNIT_Z;
			static const Vector4 NEGATIVE_UNIT_X;
			static const Vector4 NEGATIVE_UNIT_Y;
			static const Vector4 NEGATIVE_UNIT_Z;
			static const Vector4 UNIT_SCALE;

		public:    

			k_Real x, y, z, w; // vertex's x, y, z, and homogeneous coordinates

			//Constructor - WARNING: does not initialise the vector for efficiency
			inline Vector4(){}

			//Constructor takes 4 floats defining the 4 axis scalar
			inline Vector4( k_Real nx, k_Real ny, k_Real nz, k_Real nw = 1 ) : x(nx), y(ny), z(nz), w(nw) {}
			inline Vector4( const k_Real n[4] ) : x( n[0]) , y( n[1] ), z( n[2] ), w( n[3] ) {}
			
			//Copy constructor
			inline Vector4( const Vector4& v ) : x(v.x) , y(v.y), z(v.z), w(v.w) {}

			//Array accessors
			inline k_Real operator [] ( const k_Index i ) const 
			{ 
				assert( i < 4 ); 
				return *( &x + i ); 
			}
			inline k_Real& operator [] ( const k_Index i ) 
			{ 
				assert( i < 4 ); 
				return *( &x + i );
			}

			//cast operators
			inline operator const k_Real*() const { return &x; }
			inline operator k_Real*() { return &x; }
			
			//assignment operators
			inline Vector4& operator =( const Vector4& v ) { x = v.x; y = v.y; z = v.z; w = v.w; return (*this); }
			inline Vector4& operator =( const k_Real f ) { x = y = z = w = f; return (*this); }
			inline Vector4& operator = ( const Vector3& rhs )
			{
				x = rhs.x; y = rhs.y; z = rhs.z; w = 1.0f;
				return (*this);
			}

			//Unary operators
			inline Vector4 operator +( ) const { return Vector4( +x, +y, +z, +w ); }
			inline Vector4 operator -( ) const { return Vector4( -x, -y, -z, -w ); }

			//binary operators
			inline Vector4 operator -( const Vector4& v ) const { return Vector4( x - v.x, y - v.y, z - v.z, w - v.w ); }
			inline Vector4 operator +( const Vector4& v ) const { return Vector4( x + v.x, y + v.y, z + v.z, w + v.w  ); }
			inline Vector4 operator *(const Vector4& v) const { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w ); }
			inline Vector4 operator /(const Vector4& v) const { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w ); }

			inline Vector4 operator +(k_Real f) const { return Vector4(x + f, y + f, z + f, w + f); } 
			inline Vector4 operator -(k_Real f) const { return Vector4(x - f, y - f, z - f, w - f); } 
			inline Vector4 operator *(k_Real f) const { return Vector4(x * f, y * f, z * f, w * f); }
			inline Vector4 operator /(k_Real f) const { return Vector4(x / f, y / f, z / f, w / f); }

			inline Vector4& operator +=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return (*this); }
			inline Vector4& operator -=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return (*this); }
			inline Vector4& operator *=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return (*this); }
			inline Vector4& operator /=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return (*this); }

			inline Vector4& operator +=(k_Real f) { x += f; y += f; z += f; w += f; return (*this); }
			inline Vector4& operator -=(k_Real f) { x -= f; y -= f; z -= f; w -= f; return (*this); }
			inline Vector4& operator *=(k_Real f) { x *= f; y *= f; z *= f; w *= f; return (*this); }
			inline Vector4& operator /=(k_Real f) { x /= f; y /= f; z /= f; w /= f; return (*this); }

			// Boolean operators.
			inline bool operator ==( const Vector4& v ) const { return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)); }
			inline bool operator ==( const k_Real f ) const { return ((x == f) && (y == f) && (z == f) && (w == f)); }
			inline bool operator !=( const Vector4& v ) const { return ((x != v.x) || (y!= v.y) || (z != v.z) || (w != v.w)); } 
			inline bool operator !=( const k_Real f ) const { return ((x != f) || (y!= f) || (z != f) || (w != f)); } 

			//Dot product
			inline k_Real dot( const Vector4& v) const { return (v.x * x) + (v.y * y) + (v.z * z) + (v.w * w); }

			//Rotate the vector by an angle(Radians) about an axis 
			inline void rotateX( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				k_Real ny = (y * c) - (z * s);
				z = (z * c) + (y * s);
				y = ny;
			}
			inline void rotateY( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				k_Real nx = (x * c) + (z * s);
				z = (z * c) - (x * s);
				x = nx;
			}
			inline void rotateZ( k_Real r )
			{
				k_Real c = cos(r); k_Real s = sin(r);
				k_Real nx = (x * c) - (y * s);
				y = (y * c) + (x * s);
				x = nx;
			}

			//Stream operator
			inline friend k_OStream& operator << ( k_OStream& outStream, const Vector4& v )
			{
				outStream << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ' ';
				return outStream;
			}
			inline friend k_IStream& operator >> ( k_IStream& inStream, Vector4& v )
			{
				inStream >> (k_Real)v.x >> (k_Real)v.y >> (k_Real)v.z >> (k_Real)v.w;
				return inStream; //Allow cin >> x >> y >> z
			}
		};

		//void operator *=( const CMatrix4& m )
		//{
		//	*this = m * *this;
		//}

		//Vector3 operator *( const CMatrix4& m) const
		//{
		//	//Calculate the homogeneous element as if = 1
		//	k_Real h = x * m._14 + y * m._24 + z * m._34 + m._44;
		//	
		//	//If zero represent the k_Real as the smallest possible value
		//	if ( h == 0.0f )
		//		h = FLT_MIN;

		//	return Vector3( 
		//		( (x * m._11) + (y * m._21) + (z * m._31) + m._41 ) / h , 
		//		( (y * m._12) + (y * m._22) + (z * m._32) + m._42 ) / h , 
		//		( (x * m._13) + (y * m._23) + (z * m._33) + m._43 ) / h 
		//		);
		//}

		//Vector3 operator *( const CMatrix4& m, const Vector3& v )
		//{
		//	//Calculate the homogeneous element as if = 1
		//	k_Real h = v.x * m._41 + v.y * m._42 + v.z * m._43 + m._44;
		//	
		//	//If zero represent the k_Real as the smallest possible value
		//	if ( h == 0.0f )
		//		h = FLT_MIN;

		//	return Vector3( 
		//		( (v.x * m._11) + (v.y * m._12) + (v.z * m._13) + m._14 ) / h , 
		//		( (v.y * m._21) + (v.y * m._22) + (v.z * m._23) + m._24 ) / h , 
		//		( (v.x * m._31) + (v.y * m._32) + (v.z * m._33) + m._34 ) / h 
		//		);
		//}

		//Instanciate common vector types
		template class Vector2< k_Float32 >;
		template class Vector2< k_Float64 >;
		template class Vector2< k_Int32 >;

		//Typedef more common rectangle types
		typedef Vector2< k_Float32 > Vector2f; ///< A 2dimensional vector of float type
		typedef Vector2< k_Float64 > Vector2d; ///< A 2dimensional vector of double type
		typedef REAL_TYPE_SWITCH( Vector2f, Vector2d ) Vector2r; ///< a 2Dimensional vector of Real type
		typedef Vector2< k_Int32 > Vector2i; ///< A 2dimensional vector of integer type

	} // END: frx
} // END: kismo

#endif
