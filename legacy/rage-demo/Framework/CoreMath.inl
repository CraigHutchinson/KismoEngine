/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\CoreMath.inl
 * Status: superseded by kismo-v0.3/Math/CoreMath.inl (same name, content differs)
 */
#include <cmath> 

namespace math
{
	template < class T > inline int sign( T val ) 
	{ return ( val < 0? -1: (val > 0? 1: 0) ); }

#ifndef max
	template < class T > inline T max( T a, T b ) 
	{ return ( a > b ) ? a : b; }
#endif
#ifndef min
	template < class T > inline T min( T a, T b ) 
	{ return ( a > b ) ? b : a; }
#endif

	template < class T > inline T arrayMax( T* arr, Count count ) 
	{ 
		T tmp = arr[0];
		for ( T* curr = arr + count; --curr > arr; )
			if ( (*curr) > tmp ) 
				tmp = (*curr);
		return tmp;
	}

	template < class T > inline T arrayMin( T* arr, Count count ) 
	{ 
		T tmp = arr[0];
		for ( T* curr = arr + count; --curr > arr; )
			if ( (*curr) < tmp ) 
				tmp = (*curr);
		return tmp;
	}

	template < class T > void swap( T& i, T& j ) 
	{
		i -= j;
		j += i; // j gets the original value of i
		i = (j - i); // i gets the original value of j
	}

	template < class T > inline T clamp( T& val, T low,  T high )
	{ return ( val < low ? (val = low) : ( val > high ? (val = high) : val ) ); }
	template < class T > inline T clamped( T val, T low,  T high )
	{ return ( val < low ? low : ( val > high ? high : val) ); }

	inline Real acos( Real f ) 
	{ 
		if ( f >= (Real)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Real)1.0 )
				return (Real)::acos( (Float64)f );
			else 
				return (Real)0.0;
		else
			return PI;
	}

	inline Real asin( Real f ) 
	{ 
		if ( f >= (Real)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Real)1.0 )
				return (Real)::asin( (Float64)f );
			else 
				return (Real)0.0;
		else
			return PI;
	}

	inline Real atan( Real f ) 
	{ return (Real)::atan( (Float64)f ); }

	inline Real atan2( Real x, Real y ) 
	{ return (Real)::atan2( (Float64)x, (Float64)y ); }

	inline Real cos( Real f ) 
	{ return (Real)::cos( (Float64)f ); }

	inline Real exp( Real f ) 
	{ return (Real)::exp( (Float64)f ); }

	inline Real fabs( Real f ) 
	{ return (Real)::fabs( (Float64)f ); }

	inline Real floor( Real f ) 
	{ return (Real)::floor( (Float64)f ); }

	inline Real invSqrt( Real f ) 
	{ return (Real)( 1 / sqrt( f ) ); }

	inline Real log( Real f ) 
	{ return (Real)::log( (Float64)f ); }

	inline Real pow( Real f, Real exponent ) 
	{ return (Real)::pow( (Float64)f, (Float64)exponent ); }

	inline Real sin( Real f ) 
	{ return (Real)::sin( (Float64)f ); }

	inline Real tan( Real f ) 
	{ return (Real)::tan( (Float64)f ); }

	template < class T > inline T sqr( T f ) 
	{ return (T)(f*f); }

	template < class T > inline T sqrt( T f ) 
	{ return (T)::sqrt( (Float64)f ); }

	//Fast functions
	// http://www.gamedev.net/community/forums/topic.asp?topic_id=391237
	inline Float32 fastInvSqrt( Float32 f )
	{
		Real fHalf = (Float32)0.5 *f;
		Int32 i = *(Int32*)&f;
		i = 0x5f3759df - (i >> 1);
		f = *(Float32*)&i;
		f = f * ((Float32)1.5 - fHalf * f * f );
		return f;
	}

	inline Float64 fastInvSqrt( Float64 f )
	{
		Float64 fHalf = (Float64)0.5 *f;
		Int64 i = *(Int64*)&f;
		i = 0x5fe6ec85e7de30da - (i >> 1);
		f = *(Float64*)&i;
		f = f * ((Float64)1.5 - fHalf * f * f );
		return f;
	}

	template <>
	inline Float32 fastSqrt( Float32 f )
	{ return (Float32)1.0 / fastInvSqrt( f ); }

	template <>
	inline Float64 fastSqrt( Float64 f )
	{ return (Float64)1.0 / fastInvSqrt( f ); }

	template < class T >
	inline T fastSqrt( T f )
	{ return (T)fastSqrt( (Float32)f ); }

	//2D Vector convenience functions
	template < class T > inline T lengthSquared( T x, T y ) 
	{
		return sqr(x) + sqr(y);
	}
	template < class T > inline T fastLength( T x, T y ) 
	{
		return fastSqrt( lengthSquared( x, y ) );
	}
	template < class T > inline T length( T x, T y ) 
	{ 
		return sqrt( lengthSquared( x, y ) ); 
	}
	template < class T > inline T estimatedLength( T x, T y) 
	{ 
		return ( x + y );
	}	

	//3D Vector convenience functions
	template < class T > inline T lengthSquared( T x, T y, T z ) 
	{ 
		return sqr(x) + sqr(y) + sqr(z); 
	}
	template < class T > inline T fastLength( T x, T y, T z ) 
	{ 
		return fastSqrt( lengthSquared( x, y, z ) ); 
	}
	template < class T > inline T length( T x, T y, T z ) 
	{ 
		return sqrt( lengthSquared( x, y, z ) );
	}
	template < class T > inline T estimatedLength( T x, T y, T z ) 
	{ 
		return ( x + y + z );
	}

} //END: math