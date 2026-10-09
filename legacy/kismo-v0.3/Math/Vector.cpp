/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Math\Vector.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Vector.h"
//#include "Math/Matrix.h"

namespace math
{
	template< class _TReal > const VectorN<_TReal,1> VectorN<_TReal,1>::Zero( 0 );
	template< class _TReal > const VectorN<_TReal,1> VectorN<_TReal,1>::Unit_X( 1 );
	template< class _TReal > const VectorN<_TReal,1> VectorN<_TReal,1>::Unit_SCALE( 1 );
	template< class _TReal > const VectorN<_TReal,1> VectorN<_TReal,1>::InvUnit_X( -1 );
	template< class _TReal > const VectorN<_TReal,1> VectorN<_TReal,1>::InvUnit_SCALE( -1 );

	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::Zero( 0, 0 );
	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::Unit_X( 1, 0 );
	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::Unit_Y( 0, 1 );
	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::Unit_SCALE( 1, 1 );
	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::InvUnit_X( -1,  0 );
	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::InvUnit_Y(  0, -1 );
	template< class _TReal > const VectorN<_TReal,2> VectorN<_TReal,2>::InvUnit_SCALE( -1,  -1 );

	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::Zero( 0, 0, 0 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::Unit_X( 1, 0, 0 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::Unit_Y( 0, 1, 0 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::Unit_Z( 0, 0, 1 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::Unit_SCALE( 1, 1, 1 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::InvUnit_X( -1,  0, 0 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::InvUnit_Y(  0, -1, 0 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::InvUnit_Z(  0, 0, -1 );
	template< class _TReal > const VectorN<_TReal,3> VectorN<_TReal,3>::InvUnit_SCALE( -1,  -1, -1 );
	
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::Zero( 0, 0, 0, 0 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::Unit_X( 1, 0, 0, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::Unit_Y( 0, 1, 0, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::Unit_Z( 0, 0, 1, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::Unit_SCALE( 1, 1, 1, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::InvUnit_X( -1,  0, 0, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::InvUnit_Y(  0, -1, 0, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::InvUnit_Z(  0, 0, -1, 1 );
	template< class _TReal > const VectorN<_TReal,4> VectorN<_TReal,4>::InvUnit_SCALE( -1,  -1, -1, 1 );

	//template< class _TReal > 	
	//VectorN<_TReal,3 > VectorN<_TReal,3 >::operator * ( const Matrix4< _TReal >& mat ) const
	//{		
	//	//Calculate the homogeneous element as if it is equal to 1
	//	//_TReal h = x * mat._14 + y * mat._24 + z * mat._34 + mat._44;
	//	
	//	//If zero represent the _TReal as the smallest possible value
	//	//if ( h == 0.0f )
	//	//	h = (_TReal)Real_MIN;

	//	return VectorN<_TReal,3 >( 
	//		( (x * mat( 0, 0 )) + (y * mat( 1, 0 )) + (z * mat( 2, 0 )) + mat( 3, 0 ) )/* / h*/ , 
	//		( (x * mat( 0, 1 )) + (y * mat( 1, 1 )) + (z * mat( 2, 1 )) + mat( 3, 1 ) )/* / h*/, 
	//		( (x * mat( 0, 2 )) + (y * mat( 1, 2 )) + (z * mat( 2, 2 )) + mat( 3, 2 ) )/* / h*/ 
	//		 );
	//}

	//template< class _TReal >
	//VectorN<_TReal,3 >& VectorN<_TReal,3 >::operator *= ( const Matrix4< _TReal >& m )
	//{ return (*this = *this * m); }

	template class VectorN< Real, 4 >;
	template class VectorN< Float64, 4 >;
	template class VectorN< Float64, 4 >;
	template class VectorN< Int64, 4 >;
	template class VectorN< Int16, 4 >;
	template class VectorN< UInt64, 4 >;
	template class VectorN< UInt16, 4 >;

	template class VectorN< Real, 3 >;
	template class VectorN< Float32, 3 >;
	template class VectorN< Float64, 3 >;
	template class VectorN< Int32, 3 >;
	template class VectorN< Int16, 3 >;
	template class VectorN< UInt32, 3 >;
	template class VectorN< UInt16, 3 >;

	template class VectorN< Real, 2 >;
	template class VectorN< Float32, 2 >;
	template class VectorN< Float64, 2 >;
	template class VectorN< Int32, 2 >;
	template class VectorN< Int16, 2 >;
	template class VectorN< UInt32, 2 >;
	template class VectorN< UInt16, 2 >;

} //END: math