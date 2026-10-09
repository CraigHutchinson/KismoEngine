/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\VectorN.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/VectorN.h (same name, content differs)
 */
#ifndef VECTORN_H
#define VECTORN_H

#include "KismoFRX/Types.h"
#include "KismoFRX/Math.h"
#include "KismoFRX/Vector.h"
//#include <cmath>

/**
	\brief Defines some basic types used in the rendering of ray
*/
namespace kismo
{
	
	template <typename _Real, Count _Dim >
	struct VectorNBase
	{
		_Real _vec[_Dim]; //, Data array for number fo diemsions of vector
	};

#pragma pack(push, 1)
	template <typename _Real> struct VectorNBase<_Real,1> { union { struct { _Real x; };		_Real _vec[1]; }; };
	template <typename _Real> struct VectorNBase<_Real,2> { union { struct { _Real x, y; };		_Real _vec[2]; }; };
	template <typename _Real> struct VectorNBase<_Real,3> { union { struct { _Real x, y, z; };	_Real _vec[3]; }; };
	template <typename _Real> struct VectorNBase<_Real,4> { union { struct { _Real x, y, z, w; };_Real _vec[4]; }; };
#pragma pack(pop)

	/**
		\brief N dimensional vector 
	*/
	template <typename _Real, Count _Dim, class _TVectorNBase = VectorNBase<_Real,_Dim> >
	class /*KISMOFRX_EXPORT*/ VectorN : public _TVectorNBase
	{
	public:
		typedef Vector<VectorN> Array;
	public:
		
		_Real& at( Index i )
		{ /*assert(i<_Dim);*/ return _vec[i]; }
		_Real& operator []( Index i )
		{ return at(i); }

		const _Real& at( Index i ) const
		{ /*assert(i<_Dim);*/ return _vec[i]; }		
		const _Real& operator []( Index i ) const
		{ return at(i); }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline void operator -= ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs )
		{ for ( Index i =0; i < (_Dim2>_Dim ? _Dim : _Dim2); ++i ) at(i) = _Real(at(i)-rhs.at(i)); }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline void operator += ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs )
		{ for ( Index i =0; i < (_Dim2>_Dim ? _Dim : _Dim2); ++i ) at(i) = _Real(at(i)+rhs.at(i)); }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline void operator *= ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs )
		{ for ( Index i =0; i < (_Dim2>_Dim ? _Dim : _Dim2); ++i ) at(i) = _Real(at(i)*rhs.at(i)); }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline void operator /= ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs )
		{ for ( Index i =0; i < (_Dim2>_Dim ? _Dim : _Dim2); ++i ) at(i) = _Real(at(i)/rhs.at(i)); }

		inline void operator -= ( _Real rhs )
		{ for ( Index i =0; i < _Dim; ++i ) at(i) -= rhs; }
		inline void operator += ( _Real rhs )
		{ for ( Index i =0; i < _Dim; ++i ) at(i) += rhs; }
		inline void operator *= ( _Real rhs )
		{ for ( Index i =0; i < _Dim; ++i ) at(i) *= rhs; }
		inline void operator /= ( _Real rhs )
		{ for ( Index i =0; i < _Dim; ++i ) at(i) /= rhs; }
		
		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline VectorN operator - ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs ) const
		{ VectorN t(*this); t -= rhs; return t; }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline VectorN operator + ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs ) const
		{ VectorN t(*this); t += rhs; return t; }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline VectorN operator * ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs ) const
		{ VectorN t(*this); t *= rhs; return t; }

		template <typename _Real2, Count _Dim2, class _TVectorNBase2>
		inline VectorN operator / ( const VectorN<_Real2,_Dim2,_TVectorNBase2>& rhs ) const
		{ VectorN t(*this); t /= rhs; return t; }
		
		inline VectorN operator - ( _Real rhs ) const
		{ VectorN t(*this); t -= rhs; return t; }
		inline VectorN operator + ( _Real rhs ) const
		{ VectorN t(*this); t += rhs; return t; }
		inline VectorN operator * ( _Real rhs ) const
		{ VectorN t(*this); t *= rhs; return t; }
		inline VectorN operator / ( _Real rhs ) const
		{ VectorN t(*this); t /= rhs; return t; }
		
		VectorN operator - () const
		{  VectorN t; for ( Index i =0; i < _Dim; ++i ) t.at(i) = -at(i); return t; }

		inline bool operator ==( const VectorN& v) const
		{ Index i = 0; for (; at(i) == v.at(i) && i < _Dim; ++i ){}; return i == _Dim; }
                   
		bool operator !=( const VectorN& v) const
		{ return !(*this == v); }

		bool isZero() 
		{ Index i = 0; for (; at(i) == 0.0f && i < _Dim; ++i ){}; return i == _Dim; }

		/**
			\brief Asignement from a different vector _Real type and different size
			\remarks Always assigns the number of axis upto the value of _Dim
		*/
		template<typename _Real2, Count _Dim2>
		inline VectorN& operator = ( const VectorN< _Real2, _Dim2 >& rhs )
		{
			const Index dimMin =(_Dim2>_Dim ? _Dim : _Dim2); //, maximum dim = _Dim
			for ( Index i =0; i < dimMin; ++i ) { at(i) = (_Real)rhs.at(i); } //< Copy that that exists in both vectors
			for ( Index i = dimMin; i < _Dim; ++i ) { at(i) = 0; }  //< Set remaining to 0
			return *this; 
		}

		/**
			\brief Assign the same value to all axis
		*/
		VectorN& operator = ( _Real rhs )
		{ for ( Index i =0; i < _Dim; ++i ) { at(i) =  rhs; } return *this; }


		operator const VectorN< _Real, _Dim >& () const
		{ return *this; }

		/**
			\brief Cast operator to a different _Real type with the same number of elements
		*/
		template< typename _Real2, int _Dim >
		inline operator VectorN< _Real2, _Dim > () const
		{ VectorN< _Real2, _Dim > t; t = *this; return t; }
		
		
		/**
		\brief Returns the magnitude of the vector squared
		\remarks This is the same as a dot product with itself
		*/
		_Real magnitudeSquared() const
		{ return dot(*this); }

		/**
		\brief fast suqare root versio of magnitude()
		\TODO: This may no longer be faster on current gen processors!
		*/
		//_Real magnitudeFast() const
		//{ return fastSqrt( magnitudeSquared() ); }

		/**
		\brief Gets the true vector magnitude
		*/
		_Real magnitude() const
		{ return (_Real)sqrt( (Real)magnitudeSquared() ); }

		/**
		\brief Estimated magnitude can be useful for rough approximations
		\remarks Adds all axis' i.e X + Y + z = estimated magnitude
		*/
		_Real magnitudeEstimate() const
		{ 
			_Real res = _vec[0];
			for ( Index i = 1; i < _Dim; ++i )
				res += _vec[i];
			return res;
		}
		
		/**
		\brief Modifies this vector instance and returns a reference to this
		*/
		VectorN& normalise()
		{ return setMagnitude( 1, magnitude() ); }

		VectorN& normaliseFast()
		{ return setMagnitude( 1, magnitude() ); }

		/**
		\brief Returns a normalised copy of this
		*/
		VectorN normalised() const
		{ VectorN t( *this ); return t.normalise(); }

		//VectorNBase normalisedFast() const
		//{ VectorNBase t( *this ); return t.normaliseFast(); }

		VectorN& setMagnitude( _Real newLen )
		{ return setMagnitude( newLen, magnitude() ); }

		VectorN& setMagnitude( _Real newLen, _Real currentLen )
		{			
			if( currentLen != 0.0f )
			{	
				newLen /= currentLen; 
				(*this) *= newLen; //Scale the magnitude scalar
			}
			return *this;
		}

		_Real dot( const VectorN& v) const
		{ 
			_Real res = at(0) * v.at(0); //< initialise the result
			for ( Index i = 1; i < _Dim; ++i )
				res += at(i) * v.at(i); 
			return res;		
		}

		friend std::ostream& operator << ( std::ostream& o, const VectorN& v )
		{
			Index i =0;
			for ( ; i < _Dim-1; ++i ) { o << v.at(i) << '\t'; }
			o << v.at(i); //, output last value seperately as we don't want a tab at the end
			return o;
		}

		friend std::istream& operator >> ( std::istream& o, VectorN& v )
		{
			for ( Index i =0; i < _Dim; ++i ) { o >> v.at(i); } 
			return o;
		}

	protected:
	};

	/**
		\brief N dimensional vector 
	*/
	template <typename _Real, int _Dim >
	class KISMOFRX_EXPORT VectorT : public VectorN< _Real, _Dim >
	{
	public:
		VectorT(){}
		VectorT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		operator const VectorN< _Real, _Dim >& () const { return *this; }
		operator VectorN< _Real, _Dim >& () { return *this; }
	};

	/**
		\brief 1 dimensional vector 
	*/
	template <typename _Real >
	class KISMOFRX_EXPORT VectorT< _Real, 1 > : public VectorN< _Real, 1 >
	{
	public:		
		static const VectorT Zero;
		static const VectorT Unit_X;
		static const VectorT Unit_SCALE;
		static const VectorT InvUnit_X;
		static const VectorT InvUnit_SCALE;
	public:
		VectorT(){}
		VectorT( _Real nx ) { x = nx; }
		VectorT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		/*_Real& x { return _vec[0]; }	
		const _Real& x const { return _vec[0]; }	*/

		operator const VectorN< _Real, 1 >& () const { return *this; }
		operator VectorN< _Real, 1 >& () { return *this; }
	};

	/**
		\brief 2 dimensional vector 
	*/
	template <typename _Real >
	class KISMOFRX_EXPORT VectorT< _Real, 2 > : public VectorN< _Real, 2 >
	{
	public:
		static const VectorT Zero;
		static const VectorT Unit_X;
		static const VectorT Unit_Y;
		static const VectorT Unit_SCALE;
		static const VectorT InvUnit_X;
		static const VectorT InvUnit_Y;
		static const VectorT InvUnit_SCALE;
	public:
		VectorT(){}
		VectorT( _Real nx ) { x = nx; }
		VectorT( _Real nx, _Real ny ) { x = nx; y = ny; }
		VectorT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		/*_Real& x { return _vec[0]; }	
		_Real& y { return _vec[1]; }	
		const _Real& x const { return _vec[0]; }	
		const _Real& y const { return _vec[1]; }*/

		operator const VectorN< _Real, 2 >& () const { return *this; }
		operator VectorN< _Real, 2 >& () { return *this; }

		void rotate( _Real d )
		{
			Real c = cos( (Real)d ); Real s = sin( (Real)d );
			(*this) = VectorT( _Real((x * c) - (y * s)), _Real((y * c) + (x * s)) );
		}
	};
	
	/**
		\brief 3 dimensional vector 
	*/
	template <typename _Real >
	class KISMOFRX_EXPORT VectorT< _Real, 3 > : public VectorN< _Real, 3 >
	{
	public:
		static const VectorT Zero;
		static const VectorT Unit_X;
		static const VectorT Unit_Y;
		static const VectorT Unit_Z;
		static const VectorT Unit_SCALE;
		static const VectorT InvUnit_X;
		static const VectorT InvUnit_Y;
		static const VectorT InvUnit_Z;
		static const VectorT InvUnit_SCALE;
	public:
		VectorT(){}
		VectorT( _Real nx ) { x = nx; }
		VectorT( _Real nx, _Real ny ) { x = nx; y = ny; }
		VectorT( _Real nx, _Real ny, _Real nz ) { x = nx; y = ny; z = nz; }
		VectorT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		/*_Real& x { return _vec[0]; }		
		_Real& y { return _vec[1]; }		
		_Real& z { return _vec[2]; }
		const _Real& x const { return _vec[0]; }		
		const _Real& y const { return _vec[1]; }		
		const _Real& z const { return _vec[2]; }	*/
		
		//operator const VectorN< _Real, 3 >& () const { return *this; }
		//operator VectorN< _Real, 3 >& () { return *this; }	
		
		VectorT cross( const VectorT& v) const
		{  return VectorT( (y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x) ); }
	

		void rotateX( _Real d )
		{
			Real c = cos( (Real)d ); Real s = sin( (Real)d );
			(*this) = VectorT(x,  _Real((y * c) - (z * s)), _Real((z * c) + (y * s)) );
		}

		void rotateY( _Real d )
		{
			Real c = cos( (Real)d ); Real s = sin( (Real)d );
			(*this) = VectorT( _Real((x * c) + (z * s)), y, _Real((z * c) - (x * s)) );
		}

		void rotateZ( _Real d )
		{
			Real c = cos( (Real)d ); Real s = sin( (Real)d );
			(*this) = VectorT( _Real((x * c) - (y * s)), _Real((y * c) + (x * s)), z);
		}
	};

	/**
		\brief 3 dimensional vector 
	*/
	template <typename _Real >
	class KISMOFRX_EXPORT VectorT< _Real, 4 > : public VectorN< _Real, 4 >
	{
	public:
		static const VectorT Zero;
		static const VectorT Unit_X;
		static const VectorT Unit_Y;
		static const VectorT Unit_Z;
		static const VectorT Unit_SCALE;
		static const VectorT InvUnit_X;
		static const VectorT InvUnit_Y;
		static const VectorT InvUnit_Z;
		static const VectorT InvUnit_SCALE;
	public:
		VectorT(){}
		VectorT( _Real nx ) { x = nx; }
		VectorT( _Real nx, _Real ny ) { x = nx; y = ny; }
		VectorT( _Real nx, _Real ny, _Real nz ) { x = nx; y = ny; z = nz; }
		VectorT( _Real nx, _Real ny, _Real nz, _Real nw ) { x = nx; y = ny; z = nz; w = nw; }
		VectorT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		/*_Real& x { return _vec[0]; }		
		_Real& y { return _vec[1]; }		
		_Real& z { return _vec[2]; }		
		_Real& w { return _vec[3]; }	
		const _Real& x const { return _vec[0]; }		
		const _Real& y const { return _vec[1]; }		
		const _Real& z const { return _vec[2]; }		
		const _Real& w const { return _vec[3]; }	*/

		operator const VectorN< _Real, 4 >& () const { return *this; }
		operator VectorN< _Real, 4 >& () { return *this; }
	};


	typedef VectorT< Real, 4 > Vector4;
	typedef VectorT< Float64, 4 > Vector4f;
	typedef VectorT< Float32, 4 > Vector4d;
	typedef VectorT< Int32, 4 > Vector4i;
	typedef VectorT< Int16, 4 > Vector4s;
	typedef VectorT< UInt32, 4 > Vector4ui;
	typedef VectorT< UInt16, 4 > Vector4us;

	typedef VectorT< Real, 3 > Vector3;
	typedef VectorT< Float32, 3 > Vector3f;
	typedef VectorT< Float64, 3 > Vector3d;
	typedef VectorT< Int32, 3 > Vector3i;
	typedef VectorT< Int16, 3 > Vector3s;
	typedef VectorT< UInt32, 3 > Vector3ui;
	typedef VectorT< UInt16, 3 > Vector3us;

	typedef VectorT< Real, 2 > Vector2;
	typedef VectorT< Float32, 2 > Vector2f;
	typedef VectorT< Float64, 2 > Vector2d;
	typedef VectorT< Int32, 2 > Vector2i;
	typedef VectorT< Int16, 2 > Vector2s;
	typedef VectorT< UInt32, 2 > Vector2ui;
	typedef VectorT< UInt16, 2 > Vector2us;

} //END: Kismo

#endif