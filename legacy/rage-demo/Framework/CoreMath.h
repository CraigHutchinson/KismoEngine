/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\CoreMath.h
 * Status: superseded by kismo-v0.3/Math/CoreMath.h (same name, content differs)
 */
#ifndef COREMATH_H
#define COREMATH_H

#define NOMINMAX
#include "Types.h"
 
/**
This header defines common math functions
*/

#undef max
#undef min

/** \brief Math namespace contains helpfull mathematical functions
*/
namespace math
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
	Real acos( Real f );
	Real asin( Real f );
	Real atan( Real f );
	Real atan2( Real x, Real y );
	Real cos( Real f );
	Real exp( Real f );
	Real fabs( Real f );
	Real floor( Real f );
	Real invSqrt( Real f );
	Real log( Real f );
	Real pow( Real f, Real expponent );
	Real sin( Real f );
	Real tan( Real f );
	template < class T > T sqr( T f );
	template < class T > T sqrt( T f );

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
	template < class T > inline T lengthSquared( T x, T y);
	template < class T > inline T length( T x, T y );
	template < class T > inline T trueLength( T x, T y);
	template < class T > inline T estimatedLength( T x, T y);	

	//3D Vector convenience functions
	template < class T > inline T lengthSquared( T x, T y, T z );
	template < class T > inline T length( T x, T y, T z );
	template < class T > inline T trueLength( T x, T y, T z );
	template < class T > inline T estimatedLength( T x, T y, T z );		

} //END:math

#include "coreMath.inl" //include the inline math functions
#endif