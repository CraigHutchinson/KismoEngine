/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Math.inl
 * Status: current (newest copy of this file name in the archive)
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

_STD_BEGIN
#include <math.h> 
_STD_END

namespace kismo
{
	template < class T > inline int sign( T val ) 
	{ return ( val < 0? -1: (val > 0? 1: 0) ); }

	template < class T > inline T sqr( T val ) 
	{ return val * val; }

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

#if 0

	#define SIGNMASK(i) ((i)>>31) 

	// a type useful for dealing with interspersed int and float actions on the same data
	typedef union {float f; int i; } IntOrFloat;
	Float32 clamp01(Float32 fval)
	{
		int s;
		IntOrFloat val;
		val.f = fval;
		s = SIGNMASK(val.i); // all 1's if sign bit 1
		val.i &= ~s; // 0 if was negative

		// clamp to 1
		val.f = 1.0f - val.f;
		s = SIGNMASK(val.i); // all 1's if sign bit 1
		val.i &= ~s; // 0 if was negative
		val.f = 1.0f - val.f;

		assert((0 <= val.f) && (val.f <= 1.0f));
		return val.f;	
	} // Clamp01

	Float32 clampAB(Float32 fval, Float32 A, Float32 B)
	{
		assert(A < B); // todo - make asserts have message also
		fval -= A;
		fval /= (B-A);
		fval = Clamp01(fval);
		fval *= (B-A);
		fval += A;
		assert((A <= fval) && (fval <= B));
		return fval;
	} // ClampAB

	Float32 clampNonnegative(Float32 fval)
	{
		int s;
		IntOrFloat val;
		val.f = fval;
		s = SIGNMASK(val.i); // all 1's if sign bit 1
		val.i &= ~s; // 0 if was negative

		assert(0 <= val.f);
		return val.f;	
	} // Clamp01
#endif

	inline Float32 acosf( Float32 f ) 
	{ 
		if ( f >= (Float32)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float32)1.0 )
				return (Float32)std::acos( (Float64)f );
			else 
				return (Float32)0.0;
		else
			return PI;
	}

	inline Float32 asinf( Float32 f ) 
	{ 
		if ( f >= (Float32)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float32)1.0 )
				return (Float32)std::asin( (Float64)f );
			else 
				return (Float32)0.0;
		else
			return PI;
	}

	inline Float32 atanf( Float32 f )	{ return (Float32)std::atan( f ); }
	inline Float32 atan2f( Float32 x, Float32 y )  { return (Float32)std::atan2( x, y ); }
	inline Float32 cosf( Float32 f )		{ return (Float32)std::cos( f ); }
	inline Float32 expf( Float32 f )		{ return (Float32)std::exp( f ); }
	inline Float32 fabsf( Float32 f )	{ return (Float32)std::fabs( f ); }
	inline Float32 floorf( Float32 f )	{ return (Float32)std::floor( f ); }
	inline Float32 invSqrtf( Float32 f )  { return (Float32)( 1 / sqrt( f ) ); }
	inline Float32 logf( Float32 f )		{ return (Float32)std::log( f ); }
	inline Float32 powf( Float32 f, Float32 exponent )  { return (Float32)std::pow( f, exponent ); }
	inline Float32 sinf( Float32 f )		{ return (Float32)std::sin( f ); }
	inline Float32 tanf( Float32 f )		{ return (Float32)std::tan( f ); }
	inline Float32 sqrtf( Float32 f )	{ return (Float32)std::sqrt( f ); }


	inline Float64 acosd( Float64 f ) 
	{ 
		if ( f >= (Float64)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float64)1.0 )
				return (Float64)std::acos( f );
			else 
				return (Float64)0.0;
		else
			return PI;
	}

	inline Float64 asind( Float64 f ) 
	{ 
		if ( f >= (Float64)-1.0 ) //Clamp f to -1.0 otherwise acos returns an error
			if ( f <= (Float64)1.0 )
				return (Float64)std::asin( f );
			else 
				return (Float64)0.0;
		else
			return PI;
	}

	inline Float64 atand( Float64 f )	{ return (Float64)std::atan( f ); }
	inline Float64 atan2d( Float64 x, Float64 y )  { return (Float64)std::atan2( x, y ); }
	inline Float64 cosd( Float64 f )		 { return (Float64)std::cos( f ); }
	inline Float64 expd( Float64 f )		{ return (Float64)std::exp( f ); }
	inline Float64 fabsd( Float64 f )	{ return (Float64)std::fabs( f ); }
	inline Float64 floord( Float64 f )	{ return (Float64)std::floor( f ); }
	inline Float64 invSqrtd( Float64 f )  { return (Float64)( 1 / std::sqrt( f ) ); }
	inline Float64 logd( Float64 f )		{ return (Float64)std::log( f ); }
	inline Float64 powd( Float64 f, Float64 exponent )  { return (Float64)std::pow( f, exponent ); }
	inline Float64 sind( Float64 f )		{ return (Float64)std::sin( f ); }
	inline Float64 tand( Float64 f )		{ return (Float64)std::tan( f ); }
	inline Float64 sqrtd( Float64 f )	{ return (Float64)std::sqrt( f ); }

	//Fast functions
	// http://www.gamedev.net/community/forums/topic.asp?topic_id=391237
	inline Float32 fastInvSqrtf( Float32 f )
	{
		Real fHalf = (Float32)0.5 *f;
		Int32 i = *(Int32*)&f;
		i = 0x5f3759df - (i >> 1);
		f = *(Float32*)&i;
		f = f * ((Float32)1.5 - fHalf * f * f );
		return f;
	}

	inline Float64 fastInvSqrtd( Float64 f )
	{
		Float64 fHalf = (Float64)0.5 *f;
		Int64 i = *(Int64*)&f;
		i = 0x5fe6ec85e7de30da - (i >> 1);
		f = *(Float64*)&i;
		f = f * ((Float64)1.5 - fHalf * f * f );
		return f;
	}

	inline Float32 fastSqrtf( Float32 f )
	{ return (Float32)1.0 / fastInvSqrtf( f ); }

	inline Float64 fastSqrtd( Float64 f )
	{ return (Float64)1.0 / fastInvSqrtd( f ); }

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