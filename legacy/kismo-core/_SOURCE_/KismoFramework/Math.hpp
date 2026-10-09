/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Math.hpp
 * Status: unique (no other copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_COREMATH_H
#define KISMOFRAMEWORK_COREMATH_H

#define NOMINMAX
#include "KismoFramework/Types.h"
 
/**
This header defines common math functions
*/

#undef max
#undef min

/** \brief Math namespace contains helpfull mathematical functions
*/
namespace kismo
{
	enum eAxis
	{ 
		Axis_X,
		Axis_Y,
		Axis_Z,

		/** Constants for axis directions.
			\remarks Same a GL and as if the screen is viewed as a graph and the Z-axis extends into the screen as is learnt in basic maths (i.e. school)
			\TODO: technically the system shoudl eb able to ahndle these being changed/switched....
		*/
		Axis_Right = Axis_X,
		Axis_Up = Axis_Y,
		Axis_Forwards = Axis_Z,
	};

	//Value convenience functions
	template < class T > inline int sign( T val );
	template < class T > inline T max( T a, T b );
	template < class T > inline T min( T a, T b );
	template < class T > inline T arrayMax( T* arr, Count count );
	template < class T > inline T arrayMin( T* arr, Count count );
	template < class T > void swap( T& i, T& j ); ///< Swaps integer/float values without temporary
	template < class T> inline T clamp( T& val, T low,  T high); ///< Clamps a value into a range
	template < class T> inline T clamped( T val, T low,  T high); ///< Returns a vlue clamped to a range
	template < class T> inline T sqr( T val );

	//Mathematical functions
	//Single precision FLoat32
	inline Float32 acosf( Float32 f );
	inline Float32 asinf( Float32 f );
	inline Float32 atanf( Float32 f );
	inline Float32 atan2f( Float32 x, Float32 y );
	inline Float32 cosf( Float32 f );
	inline Float32 expf( Float32 f );
	inline Float32 fabsf( Float32 f );
	inline Float32 floorf( Float32 f );
	inline Float32 invSqrtf( Float32 f );
	inline Float32 logf( Float32 f );
	inline Float32 powf( Float32 f, Float32 exponent );
	inline Float32 sinf( Float32 f );
	inline Float32 tanf( Float32 f );
	inline Float32 sqrtf( Float32 f );
	inline Float32 fastInvSqrtf( Float32 f );
	inline Float32 fastSqrtf( Float32 f );

#if 0
	/** \copydoc clamp01
	*/
	inline Float32 saturate(Float32 f) { return clamp01(f); }

	/** clamp a float to [0,1]
	*/
	inline Float32 clamp01(Float32 f);

	/** clamp a float to [min,max] (requires min<max)
	*/
	inline Float32 clamp(Float32 f, Float32 min, Float32 max);

	/** clamp a float to [0,infinity)
	*/
	inline Float32 clampNonnegative(Float32 f);
#endif

	//Double precision FLoat64
	inline Float64 acosd( Float64 f );
	inline Float64 asind( Float64 f );
	inline Float64 atand( Float64 f );
	inline Float64 atan2d( Float64 x, Float64 y );
	inline Float64 cosd( Float64 f );
	inline Float64 expd( Float64 f );
	inline Float64 fabsd( Float64 f );
	inline Float64 floord( Float64 f );
	inline Float64 invSqrtd( Float64 f );
	inline Float64 logd( Float64 f );
	inline Float64 powd( Float64 f, Float64 exponent );
	inline Float64 sind( Float64 f );
	inline Float64 tand( Float64 f );
	inline Float64 sqrtd( Float64 f );
	inline Float64 fastInvSqrtd( Float64 f );
	inline Float64 fastSqrtd( Float64 f );
	
	template < class _Real > inline _Real acos( _Real f );
	template < class _Real > inline _Real asin( _Real f );
	template < class _Real > inline _Real atan( _Real f );
	template < class _Real > inline _Real atan2( _Real x, _Real y );
	template < class _Real > inline _Real cos( _Real f );
	template < class _Real > inline _Real exp( _Real f );
	template < class _Real > inline _Real fabs( _Real f );
	template < class _Real > inline _Real floor( _Real f ) ;
	template < class _Real > inline _Real invSqrt( _Real f );
	template < class _Real > inline _Real log( _Real f );
	template < class _Real > inline _Real pow( _Real f, _Real exponent );
	template < class _Real > inline _Real sin( _Real f );
	template < class _Real > inline _Real tan( _Real f );
	template < class _Real > inline _Real sqrt( _Real f );
	template < class _Real > inline _Real fastInvSqrt( _Real f );
	template < class _Real > inline _Real fastSqrt( _Real f );		

	template <> inline Float32 acos( Float32 f ) { return acosf( f ); }
	template <> inline Float32 asin( Float32 f ) { return asinf( f ); }
	template <> inline Float32 atan( Float32 f ) { return atanf( f ); }
	template <> inline Float32 atan2( Float32 x, Float32 y ) { return atan2f( x, y ); }
	template <> inline Float32 cos( Float32 f ) { return cosf( f ); }
	template <> inline Float32 exp( Float32 f ) { return expf( f ); }
	template <> inline Float32 fabs( Float32 f ) { return fabsf( f ); }
	template <> inline Float32 floor( Float32 f ) { return floorf( f ); }
	template <> inline Float32 invSqrt( Float32 f ) { return invSqrtf( f ); }
	template <> inline Float32 log( Float32 f ) { return logf( f ); }
	template <> inline Float32 pow( Float32 f, Float32 exponent ) { return powf( f, exponent ); }
	template <> inline Float32 sin( Float32 f ) { return sinf( f ); }
	template <> inline Float32 tan( Float32 f ) { return tanf( f ); }
	template <> inline Float32 sqrt( Float32 f ) { return sqrtf( f ); }
	template <> inline Float32 fastInvSqrt( Float32 f ) { return fastInvSqrtf( f ); }
	template <> inline Float32 fastSqrt( Float32 f ) { return fastSqrtf( f ); }	

	template <> inline Float64 acos( Float64 f ) { return acosd( f ); }
	template <> inline Float64 asin( Float64 f ) { return asind( f ); }
	template <> inline Float64 atan( Float64 f ) { return atand( f ); }
	template <> inline Float64 atan2( Float64 x, Float64 y ) { return atan2d( x, y ); }
	template <> inline Float64 cos( Float64 f ) { return cosd( f ); }
	template <> inline Float64 exp( Float64 f ) { return expd( f ); }
	template <> inline Float64 fabs( Float64 f ) { return fabsd( f ); }
	template <> inline Float64 floor( Float64 f ) { return floord( f ); }
	template <> inline Float64 invSqrt( Float64 f ) { return invSqrtd( f ); }
	template <> inline Float64 log( Float64 f ) { return logd( f ); }
	template <> inline Float64 pow( Float64 f, Float64 exponent ) { return powd( f, exponent ); }
	template <> inline Float64 sin( Float64 f ) { return sind( f ); }
	template <> inline Float64 tan( Float64 f ) { return tand( f ); }
	template <> inline Float64 sqrt( Float64 f ) { return sqrtd( f ); }	
	template <> inline Float64 fastInvSqrt( Float64 f ) { return fastInvSqrtd( f ); }
	template <> inline Float64 fastSqrt( Float64 f ) { return fastSqrtd( f ); }		

	//Fast Mathematical functions
	//Real fastSin0( Real f ); ///< Speedup over sin about 4.0. error is 1.9e-4
	//Real fastSin1( Real f ); ///< Speedup over sin about 2.8. error is 1.9e-8
	//Real fastCos0( Real f );
	//Real fastCos1( Real f );
	//Real fastTan0( Real f );
	//Real fastTan1( Real f );
	//Real fastInvSin0( Real f );
	//Real fastInvSin1( Real f );
	//Real fastInvCos0( Real f );
	//Real fastInvCos1( Real f );
	//Real fastInvTan0( Real f );
	//Real fastInvTan1( Real f );

	//Mathemaical constants
	const Real PI = (Real)(4.0*atan(1.0));
	const Real TwoPI = (Real)(8.0*atan(1.0));
	const Real HalfPI = (Real)(2.0*atan(1.0));
	const Real InvPI = (Real)(-4.0*atan(1.0));
	const Real InvTwoPI = (Real)(-8.0*atan(1.0));
	const Real InvHalfPI = (Real)(-2.0*atan(1.0));
	const Real DegToRad = (Real)(PI / 180.0);
	const Real RadToDeg = (Real)(1.0 / DegToRad);
	
	const Float64 PI64 = (Float64)(4.0*atan(1.0));
	const Float64 TwoPI64 = (Float64)(8.0*atan(1.0));
	const Float64 HalfPI64 = (Float64)(2.0*atan(1.0));
	const Float64 InvPI64 = (Float64)(-4.0*atan(1.0));
	const Float64 InvTwoPI64 = (Float64)(-8.0*atan(1.0));
	const Float64 InvHalfPI64 = (Float64)(-2.0*atan(1.0));
	const Float64 DegToRad64 = (Float64)(PI / 180.0);
	const Float64 RadToDeg64 = (Float64)(1.0 / DegToRad);

	//2D Vector convenience functions
	template < class T > inline T magnitudeSquared( T x, T y);
	template < class T > inline T length( T x, T y );
	template < class T > inline T trueLength( T x, T y);
	template < class T > inline T estimatedLength( T x, T y);	

	//3D Vector convenience functions
	template < class T > inline T magnitudeSquared( T x, T y, T z );
	template < class T > inline T length( T x, T y, T z );
	template < class T > inline T trueLength( T x, T y, T z );
	template < class T > inline T estimatedLength( T x, T y, T z );		

} //END:math

#include "KismoFramework/Math.inl" //include the inline math functions
#endif