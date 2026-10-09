/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Vector.cpp
 * Status: superseded by kismo-v0.3/Math/Vector.cpp (same name, content differs)
 */
#include "Vector.h"
#include "Framework/Matrix.h"

namespace frx
{
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::Zero( 0, 0 );
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::Unit_X( 1, 0 );
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::Unit_Y( 0, 1 );
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::Unit_SCALE( 1, 1 );
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::InvUnit_X( -1,  0 );
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::InvUnit_Y(  0, -1 );
	template< class _TReal > const Vector2<_TReal> Vector2<_TReal>::InvUnit_SCALE( -1,  -1 );

	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::Zero( 0, 0, 0 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::Unit_X( 1, 0, 0 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::Unit_Y( 0, 1, 0 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::Unit_Z( 0, 0, 1 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::Unit_SCALE( 1, 1, 1 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::InvUnit_X( -1,  0, 0 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::InvUnit_Y(  0, -1, 0 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::InvUnit_Z(  0, 0, -1 );
	template< class _TReal > const Vector3<_TReal> Vector3<_TReal>::InvUnit_SCALE( -1,  -1, -1 );

	template<>	
	Vector3<Float32>& Vector3<Float32>::operator =( const String& str ) 
	{
		_stscanf_s( str.c_str(), _T("%f %f %f"), &x, &y, &z );
		return *this;
	}

	template<>	
	Vector3<Float64>& Vector3<Float64>::operator =( const String& str ) 
	{
		Float32 tx,ty,tz;
		_stscanf_s( str.c_str(), _T("%f %f %f"), &tx, &ty, &tz );
		x = tx; y=ty;z=tz;
		return *this;
	}
	
	template<>	
	Vector3< Int16 >& Vector3<Int16>::operator =( const String& str ) 
	{
		Int32 tx,ty,tz;
		_stscanf_s( str.c_str(), _T("%d %d %d"), &tx, &ty, &tz );
		x = (Int16)tx; y = (Int16)ty; z = (Int16)tz;
		return *this;
	}
	
	template<>	
	Vector3< Int32 >& Vector3<Int32>::operator =( const String& str ) 
	{
		_stscanf_s( str.c_str(), _T("%d %d %d"), &x, &y, &z );
		return *this;
	}

	template< class _TReal > 	
	Vector3< _TReal > Vector3< _TReal >::operator * ( const Matrix4< _TReal >& mat ) const
	{		
		//Calculate the homogeneous element as if it is equal to 1
		//_TReal h = x * mat._14 + y * mat._24 + z * mat._34 + mat._44;
		
		//If zero represent the _TReal as the smallest possible value
		//if ( h == 0.0f )
		//	h = (_TReal)Real_MIN;

		return Vector3< _TReal >( 
			( (x * mat( 0, 0 )) + (y * mat( 1, 0 )) + (z * mat( 2, 0 )) + mat( 3, 0 ) )/* / h*/ , 
			( (x * mat( 0, 1 )) + (y * mat( 1, 1 )) + (z * mat( 2, 1 )) + mat( 3, 1 ) )/* / h*/, 
			( (x * mat( 0, 2 )) + (y * mat( 1, 2 )) + (z * mat( 2, 2 )) + mat( 3, 2 ) )/* / h*/ 
			 );
	}

	template< class _TReal >
	Vector3< _TReal >& Vector3< _TReal >::operator *= ( const Matrix4< _TReal >& m )
	{ return (*this = *this * m); }

	template class Vector2< Int16 >;
	template class Vector2< Int32 >;
	template class Vector2< Float32 >;
	template class Vector2< Float64 >;

	template class Vector3< Int16 >;
	template class Vector3< Int32 >;
	template class Vector3< Float32 >;
	template class Vector3< Float64 >;

} //END: frx