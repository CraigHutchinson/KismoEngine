/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Math\Vector.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Vector.h (same name, content differs)
 */
#ifndef VECTORN_H
#define VECTORN_H

#include "Platform/Platform.h"
#include "Framework/Types.h"
#include "Math/CoreMath.h"

/**
\brief Defines some basic types used in the rendering of ray
*/
namespace math
{
	template <typename _TReal, Count _Dim >
	class VectorNData
	{
	public:
		_TReal _vec[_Dim]; //, Data array for number fo diemsions of vector
	};

	//#pragma pack(1)  //< Must be byte packed for the arry/float union to work
	template <typename _TReal> class VectorNData<_TReal,1> { public: union{ _TReal _vec[1]; struct{ _TReal x; }; }; };
	template <typename _TReal> class VectorNData<_TReal,2> { public: union{ _TReal _vec[2]; struct{ _TReal x, y; }; }; }; 
	template <typename _TReal> class VectorNData<_TReal,3> { public: union{ _TReal _vec[3]; struct{ _TReal x, y, z; }; }; }; 
	template <typename _TReal> class VectorNData<_TReal,4> { public: union{ _TReal _vec[4]; struct{ _TReal x, y, z, w; }; }; }; 
	//#pragma pack()

	/**
	\brief N dimensional vector 
	*/
	template <typename _TReal, Count _Dim >
	class VectorNBase : public VectorNData<_TReal, _Dim>
	{
	public:

		//typedef Vector< VectorNBase > Array;

	public:

		_TReal& at( Index i )
		{ /*assert(i<_Dim);*/ return _vec[i]; }
		_TReal& operator []( Index i )
		{ return _vec[i]; }

		const _TReal& at( Index i ) const
		{ /*assert(i<_Dim);*/ return _vec[i]; }		
		const _TReal& operator []( Index i ) const
		{ return _vec[i]; }

		void operator -= ( const VectorNBase& rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] -= rhs._vec[i]; }
		void operator += ( const VectorNBase& rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] += rhs._vec[i]; }
		void operator *= ( const VectorNBase& rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] *= rhs._vec[i]; }
		void operator /= ( const VectorNBase& rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] /= rhs._vec[i]; }

		void operator -= ( _TReal rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] -= rhs; }
		void operator += ( _TReal rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] += rhs; }
		void operator *= ( _TReal rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] *= rhs; }
		void operator /= ( _TReal rhs )
		{ for ( Index i =0; i < _Dim; ++i ) _vec[i] /= rhs; }

		VectorNBase operator - ( const VectorNBase& rhs ) const
		{ VectorNBase t(*this); t -= rhs; return t; }
		VectorNBase operator + ( const VectorNBase& rhs ) const
		{ VectorNBase t(*this); t += rhs; return t; }
		VectorNBase operator * ( const VectorNBase& rhs ) const
		{ VectorNBase t(*this); t *= rhs; return t; }
		VectorNBase operator / ( const VectorNBase& rhs ) const
		{ VectorNBase t(*this); t /= rhs; return t; }

		VectorNBase operator - ( _TReal rhs ) const
		{ VectorNBase t(*this); t -= rhs; return t; }
		VectorNBase operator + ( _TReal rhs ) const
		{ VectorNBase t(*this); t += rhs; return t; }
		VectorNBase operator * ( _TReal rhs ) const
		{ VectorNBase t(*this); t *= rhs; return t; }
		VectorNBase operator / ( _TReal rhs ) const
		{ VectorNBase t(*this); t /= rhs; return t; }

		VectorNBase operator - () const
		{  VectorNBase t; for ( Index i =0; i < _Dim; ++i ) t._vec[i] = -_vec[i]; return t; }

		bool operator ==( const VectorNBase& v) const
		{ Index i = 0; for (; _vec[i] == v._vec[i] && i < _Dim; ++i ){}; return i == _Dim; }

		bool operator !=( const VectorNBase& v) const
		{ return !(*this == v); }

		bool isZero() 
		{ Index i = 0; for (; _vec[i] == 0.0f && i < _Dim; ++i ){}; return i == _Dim; }

		/**
		\brief Asignement from a different vector _TReal type and different size
		\remarks Always assigns the number of axis upto the value of _Dim
		*/
		template<typename _Real2, Count _Dim2>
		VectorNBase& operator = ( const VectorNBase< _Real2, _Dim2 >& rhs )
		{
			const Index dim =(_Dim2>_Dim ? _Dim : _Dim2); //, maximum dim = _Dim
			for ( Index i =0; i < dim; ++i ) { _vec[i] = (_TReal)rhs._vec[i]; } return *this; }

		/**
		\brief Assign the same value to all axis
		*/
		VectorNBase& operator = ( _TReal rhs )
		{ for ( Index i =0; i < _Dim; ++i ) { _vec[i] =  rhs; } return *this; }


		/**
		\brief Cast operator to a different _TReal type with the same numbe rof elements
		*/
		template< typename _Real2, int _Dim >
		operator VectorNBase< _Real2, _Dim > () const
		{ VectorNBase< _Real2, _Dim > t; t = *this; return t; }

		/**
		\brief Modifies this vector instance and returns a reference to this
		*/
		VectorNBase& normalise()
		{ return setMagnitude( 1, magnitude() ); }

		VectorNBase& normaliseFast()
		{ return setMagnitude( 1, magnitudeFast() ); }

		/**
		\brief Returns a normalised copy of this
		*/
		VectorNBase normalised() const
		{ VectorNBase t( *this ); return t.normalise(); }

		VectorNBase normalisedFast() const
		{ VectorNBase t( *this ); return t.normaliseFast(); }

		VectorNBase& setMagnitude( _TReal newLen )
		{ return setMagnitude( newLen, magnitude() ); }

		VectorNBase& setMagnitude( _TReal newLen, _TReal currentLen )
		{			
			if( currentLen != 0.0f )
			{	
				newLen /= currentLen; 
				(*this) *= newLen; //Scale the magnitude scalar
			}
			return *this;
		}

		_TReal dot( const VectorNBase& v) const
		{ 
			_TReal res = _vec[0] * v._vec[0]; //< initialise the result
			for ( Index i = 1; i < _Dim; ++i )
				res += _vec[i] * v._vec[i]; 
			return res;		
		}

		/**
		\brief Returns the magnitude of the vector squared
		\remarks This is the same as a dot product with itself
		*/
		_TReal magnitudeSquared() const
		{ return dot(*this); }

		/**
		\brief fast suqare root versio of magnitude()
		\TODO: This may no longer be faster on current gen processors!
		*/
		_TReal magnitudeFast() const
		{ return fastSqrt( magnitudeSquared() ); }

		/**
		\brief Gets the true vector magnitude
		*/
		_TReal magnitude() const
		{ return sqrt( magnitudeSquared() ); }

		/**
		\brief Estimated magnitude can be useful for rough approximations
		\remarks Adds all axis' i.e X + Y + z = estimated magnitude
		*/
		_TReal magnitudeEstimate() const
		{ 
			_TReal res = _vec[0];
			for ( Index i = 1; i < _Dim; ++i )
				res += _vec[i];
			return res;
		}

		friend std::ostream& operator << ( std::ostream& o, const VectorNBase& v )
		{
			Index i =0;
			for ( ; i < _Dim-1; ++i ) { o << v._vec[i] << '\t'; }
			o << v._vec[i]; //, output last value seperately as we don't want a tab at the end
			return o;
		}

		friend std::istream& operator >> ( std::istream& o, VectorNBase& v )
		{
			for ( Index i =0; i < _Dim; ++i ) { o >> v._vec[i]; } 
			return o;
		}
	};

	/**
	\brief N dimensional vector 
	*/
	template <typename _TReal, int _Dim >
	class VectorN : public VectorNBase< _TReal, _Dim >
	{
	public:		
		typedef Vector< VectorN<_TReal,_Dim> > Array;
	public:
		VectorN(){}
		template <typename _Real2 > VectorN( const VectorNBase< _Real2, _Dim >& rhs ) { *this = rhs; }

		operator const VectorNBase< _TReal, _Dim >& () const { return *this; }
		operator VectorNBase< _TReal, _Dim >& () { return *this; }
	};

	/**
	\brief 1 dimensional vector 
	*/
	template <typename _TReal >
	class VectorN< _TReal, 1 > : public VectorNBase< _TReal, 1 >
	{
	public:		
		typedef Vector< VectorN<_TReal,1> > Array;

		// special vectors
		static const VectorN Zero;
		static const VectorN Unit_SCALE;
		static const VectorN InvUnit_SCALE;
		static const VectorN Unit_X;
		static const VectorN InvUnit_X;

	public:
		VectorN(){}
		VectorN( _TReal nx ) { x = nx; }
		template <typename _Real2 > VectorN( const VectorNBase< _Real2, 1 >& rhs ) { ((VectorNBase< _TReal, _Dim >&)*this) = rhs; }

		operator const VectorNBase< _TReal, 1 >& () const { return *this; }
		operator VectorNBase< _TReal, 1 >& () { return *this; }
	};

	/**
	\brief 2 dimensional vector 
	*/
	template <typename _TReal >
	class VectorN< _TReal, 2 > : public VectorNBase< _TReal, 2 >
	{
	public:		
		typedef Vector< VectorN<_TReal,2> > Array;

		// special vectors
		static const VectorN Zero;
		static const VectorN Unit_SCALE;
		static const VectorN InvUnit_SCALE;
		static const VectorN Unit_X;
		static const VectorN Unit_Y;
		static const VectorN InvUnit_X;
		static const VectorN InvUnit_Y;

	public:
		VectorN(){}
		VectorN( _TReal nx ) { x = nx; }
		VectorN( _TReal nx, _TReal ny ) { x = nx; y = ny; }
		template <typename _Real2 > VectorN( const VectorNBase< _Real2, 2 >& rhs ) { ((VectorNBase< _TReal, 2 >&)*this) = rhs; }

		operator const VectorNBase< _TReal, 2 >& () const { return *this; }
		operator VectorNBase< _TReal, 2 >& () { return *this; }

		void rotate( _TReal d )
		{
			_TReal c = cos(d); _TReal s = sin(d);
			(*this) = VectorN( (x * c) - (y * s), (y * c) + (x * s) );
		}
	};

	/**
	\brief 3 dimensional vector 
	*/
	template <typename _TReal >
	class VectorN< _TReal, 3 > : public VectorNBase< _TReal, 3 >
	{

	public:		
		typedef Vector< VectorN<_TReal,3> > Array;

		// special vectors
		static const VectorN Zero;
		static const VectorN Unit_SCALE;
		static const VectorN InvUnit_SCALE;
		static const VectorN Unit_X;
		static const VectorN Unit_Y;
		static const VectorN Unit_Z;
		static const VectorN InvUnit_X;
		static const VectorN InvUnit_Y;
		static const VectorN InvUnit_Z;

	public:
		VectorN(){}
		VectorN( _TReal nx ) { x = nx; }
		VectorN( _TReal nx, _TReal ny ) { x = nx; y = ny; }
		VectorN( _TReal nx, _TReal ny, _TReal nz ) { x = nx; y = ny; z = nz; }
		template <typename _Real2 > VectorN( const VectorNBase< _Real2, 3 >& rhs ) { ((VectorNBase< _TReal, 3 >&)*this) = rhs; }

		//operator const VectorNBase< _TReal, 3 >& () const { return *this; }
		//operator VectorNBase< _TReal, 3 >& () { return *this; }	

		VectorN cross( const VectorN& v) const
		{  return VectorN( (y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x) ); }


		void rotateX( _TReal d )
		{
			Real c = (Real)cos(d); Real s = (Real)sin(d);
			(*this) = VectorN( Real(x), Real((y * c) - (z * s)), Real((z * c) + (y * s)) );
		}

		void rotateY( _TReal d )
		{
			Real c = cos(d); Real s = sin(d);
			(*this) = VectorN( Real((x * c) + (z * s)), Real(y), Real((z * c) - (x * s)) );
		}

		void rotateZ( _TReal d )
		{
			Real c = cos(d); Real s = sin(d);
			(*this) = VectorN( Real((x * c) - (y * s)), Real((y * c) + (x * s)), Real(z) );
		}
	};

	/**
	\brief 3 dimensional vector 
	*/
	template <typename _TReal >
	class VectorN< _TReal, 4 > : public VectorNBase< _TReal, 4 >
	{

	public:		
		typedef Vector< VectorN<_TReal,4> > Array;

		// special vectors
		static const VectorN Zero;
		static const VectorN Unit_SCALE;
		static const VectorN InvUnit_SCALE;
		static const VectorN Unit_X;
		static const VectorN Unit_Y;
		static const VectorN Unit_Z;
		static const VectorN InvUnit_X;
		static const VectorN InvUnit_Y;
		static const VectorN InvUnit_Z;

	public:
		VectorN(){}
		VectorN( _TReal nx ) { x = nx; }
		VectorN( _TReal nx, _TReal ny ) { x = nx; y = ny; }
		VectorN( _TReal nx, _TReal ny, _TReal nz ) { x = nx; y = ny; z = nz; }
		VectorN( _TReal nx, _TReal ny, _TReal nz, _TReal nw ) { x = nx; y = ny; w = nw; }
		template <typename _Real2 > VectorN( const VectorNBase< _Real2, 4 >& rhs ) { ((VectorNBase< _TReal, 4 >&)*this) = rhs; }

		operator const VectorNBase< _TReal, 4 >& () const { return *this; }
		operator VectorNBase< _TReal, 4 >& () { return *this; }
	};

	typedef VectorN< Real, 4 > Vector4;
	typedef VectorN< Float64, 4 > Vector4f;
	typedef VectorN< Float64, 4 > Vector4d;
	typedef VectorN< Int64, 4 > Vector4i;
	typedef VectorN< Int16, 4 > Vector4s;
	typedef VectorN< UInt64, 4 > Vector4ui;
	typedef VectorN< UInt16, 4 > Vector4us;

	typedef VectorN< Real, 3 > Vector3;
	typedef VectorN< Float32, 3 > Vector3f;
	typedef VectorN< Float64, 3 > Vector3d;
	typedef VectorN< Int32, 3 > Vector3i;
	typedef VectorN< Int16, 3 > Vector3s;
	typedef VectorN< UInt32, 3 > Vector3ui;
	typedef VectorN< UInt16, 3 > Vector3us;

	typedef VectorN< Real, 2 > Vector2;
	typedef VectorN< Float32, 2 > Vector2f;
	typedef VectorN< Float64, 2 > Vector2d;
	typedef VectorN< Int32, 2 > Vector2i;
	typedef VectorN< Int16, 2 > Vector2s;
	typedef VectorN< UInt32, 2 > Vector2ui;
	typedef VectorN< UInt16, 2 > Vector2us;

} //END: math

#endif