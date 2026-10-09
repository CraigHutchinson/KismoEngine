/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Math.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Math.h (same name, content differs)
 */
#ifndef COREMATH_H
#define COREMATH_H

#define NOMINMAX
#include "KismoFRX/Types.h"
 
/**
This header defines common math functions
*/

#undef max
#undef min

/** \brief Math namespace contains helpfull mathematical functions
*/
namespace kismo
{

	//Value convenience functions
	template < class T > inline int sign( T val );
	template < class T > inline T max( T a, T b );
	template < class T > inline T min( T a, T b );
	template < class T > inline T arrayMax( T* arr, Count count );
	template < class T > inline T arrayMin( T* arr, Count count );
	template < class T > void swap( T& i, T& j ); ///< Swaps integer/float values without temporary
	template < class T> inline T clamp( T& val, T low,  T high); ///< Clamps a value into a range
	template < class T> inline T clamped( T val, T low,  T high); ///< Returns a vlue clamped to a range

	//Mathematical functions
	Float32 acos( Float32 f );
	Float32 asin( Float32 f );
	Float32 atan( Float32 f );
	Float32 atan2( Float32 x, Float32 y );
	Float32 cos( Float32 f );
	Float32 exp( Float32 f );
	Float32 fabs( Float32 f );
	Float32 floor( Float32 f );
	Float32 invSqrt( Float32 f );
	Float32 log( Float32 f );
	Float32 pow( Float32 f, Float32 expponent );
	Float32 sin( Float32 f );
	Float32 tan( Float32 f );
	Float32 sqr( Float32 f );
	Float32 sqrt( Float32 f );
	
	Float64 acos( Float64 f );
	Float64 asin( Float64 f );
	Float64 atan( Float64 f );
	Float64 atan2( Float64 x, Float64 y );
	Float64 cos( Float64 f );
	Float64 exp( Float64 f );
	Float64 fabs( Float64 f );
	Float64 floor( Float64 f );
	Float64 invSqrt( Float64 f );
	Float64 log( Float64 f );
	Float64 pow( Float64 f, Float64 expponent );
	Float64 sin( Float64 f );
	Float64 tan( Float64 f );
	Float64 sqr( Float64 f );
	Float64 sqrt( Float64 f );

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
	template < class T > T fastInvSqrt( T f );
	template < class T > T fastSqrt( T f );

	//Mathemaical constants
	const Real PI = (Real)(4.0*atan(1.0));
	const Real TwoPI = (Real)(8.0*atan(1.0));
	const Real HalfPI = (Real)(2.0*atan(1.0));
	const Real InvPI = (Real)(-4.0*atan(1.0));
	const Real InvTwoPI = (Real)(-8.0*atan(1.0));
	const Real InvHalfPI = (Real)(-2.0*atan(1.0));
	const Real DegToRad = (Real)(PI / 180.0);
	const Real RadToDeg = (Real)(1.0 / DegToRad);

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

#include "KismoFRX/Math.inl" //include the inline math functions
#endif