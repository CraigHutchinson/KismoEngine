/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Math.inl
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Math.inl (same name, content differs)
 */
_STD_BEGIN
#include <math.h> 
_STD_END

namespace kismo
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

	inline Float32 acos( Float32 f ) 
	{ 
		if ( f >= (Float32)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float32)1.0 )
				return (Float32)std::acos( (Float64)f );
			else 
				return (Float32)0.0;
		else
			return PI;
	}

	inline Float32 asin( Float32 f ) 
	{ 
		if ( f >= (Float32)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float32)1.0 )
				return (Float32)std::asin( (Float64)f );
			else 
				return (Float32)0.0;
		else
			return PI;
	}

	inline Float32 atan( Float32 f )	{ return (Float32)std::atan( f ); }
	inline Float32 atan2( Float32 x, Float32 y )  { return (Float32)std::atan2( x, y ); }
	inline Float32 cos( Float32 f )		{ return (Float32)std::cos( f ); }
	inline Float32 exp( Float32 f )		{ return (Float32)std::exp( f ); }
	inline Float32 fabs( Float32 f )	{ return (Float32)std::fabs( f ); }
	inline Float32 floor( Float32 f )	{ return (Float32)std::floor( f ); }
	inline Float32 invSqrt( Float32 f )  { return (Float32)( 1 / sqrt( f ) ); }
	inline Float32 log( Float32 f )		{ return (Float32)std::log( f ); }
	inline Float32 pow( Float32 f, Float32 exponent )  { return (Float32)std::pow( f, exponent ); }
	inline Float32 sin( Float32 f )		{ return (Float32)std::sin( f ); }
	inline Float32 tan( Float32 f )		{ return (Float32)std::tan( f ); }
	inline Float32 sqr( Float32 f )		{ return (Float32)(f*f); }
	inline Float32 sqrt( Float32 f )	{ return (Float32)std::sqrt( f ); }


	inline Float64 acos( Float64 f ) 
	{ 
		if ( f >= (Float64)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float64)1.0 )
				return (Float64)std::acos( f );
			else 
				return (Float64)0.0;
		else
			return PI;
	}

	inline Float64 asin( Float64 f ) 
	{ 
		if ( f >= (Float64)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float64)1.0 )
				return (Float64)std::asin( f );
			else 
				return (Float64)0.0;
		else
			return PI;
	}

	inline Float64 atan( Float64 f )	{ return (Float64)std::atan( f ); }
	inline Float64 atan2( Float64 x, Float64 y )  { return (Float64)std::atan2( x, y ); }
	inline Float64 cos( Float64 f )		 { return (Float64)std::cos( f ); }
	inline Float64 exp( Float64 f )		{ return (Float64)std::exp( f ); }
	inline Float64 fabs( Float64 f )	{ return (Float64)std::fabs( f ); }
	inline Float64 floor( Float64 f )	{ return (Float64)std::floor( f ); }
	inline Float64 invSqrt( Float64 f )  { return (Float64)( 1 / sqrt( f ) ); }
	inline Float64 log( Float64 f )		{ return (Float64)std::log( f ); }
	inline Float64 pow( Float64 f, Float64 exponent )  { return (Float64)std::pow( f, exponent ); }
	inline Float64 sin( Float64 f )		{ return (Float64)std::sin( f ); }
	inline Float64 tan( Float64 f )		{ return (Float64)std::tan( f ); }
	inline Float64 sqr( Float64 f )		{ return (Float64)(f*f); }
	inline Float64 sqrt( Float64 f )	{ return (Float64)std::sqrt( f ); }

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
	template < class T > inline T magnitudeSquared( T x, T y ) 
	{
		return sqr(x) + sqr(y);
	}
	template < class T > inline T fastMagnitude( T x, T y ) 
	{
		return fastSqrt( magnitudeSquared( x, y ) );
	}
	template < class T > inline T magnitude( T x, T y ) 
	{ 
		return sqrt( magnitudeSquared( x, y ) ); 
	}
	template < class T > inline T estimatedLength( T x, T y) 
	{ 
		return ( x + y );
	}	

	//3D Vector convenience functions
	template < class T > inline T magnitudeSquared( T x, T y, T z ) 
	{ 
		return sqr(x) + sqr(y) + sqr(z); 
	}
	template < class T > inline T fastMagnitude( T x, T y, T z ) 
	{ 
		return fastSqrt( magnitudeSquared( x, y, z ) ); 
	}
	template < class T > inline T magnitude( T x, T y, T z ) 
	{ 
		return sqrt( magnitudeSquared( x, y, z ) );
	}
	template < class T > inline T estimatedMagnitude( T x, T y, T z ) 
	{ 
		return ( x + y + z );
	}

} //END: kismo