/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\VectorN.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/VectorN.cpp (same name, content differs)
 */
#include "VectorN.h"
//#include "KismoFrxMatrix.h"

namespace kismo	
{
	template< class _TReal > const VectorT<_TReal,1> VectorT<_TReal,1>::Zero( 0 );
	template< class _TReal > const VectorT<_TReal,1> VectorT<_TReal,1>::Unit_X( 1 );
	template< class _TReal > const VectorT<_TReal,1> VectorT<_TReal,1>::Unit_SCALE( 1 );
	template< class _TReal > const VectorT<_TReal,1> VectorT<_TReal,1>::InvUnit_X( -1 );
	template< class _TReal > const VectorT<_TReal,1> VectorT<_TReal,1>::InvUnit_SCALE( -1 );

	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::Zero( 0, 0 );
	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::Unit_X( 1, 0 );
	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::Unit_Y( 0, 1 );
	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::Unit_SCALE( 1, 1 );
	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::InvUnit_X( -1,  0 );
	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::InvUnit_Y(  0, -1 );
	template< class _TReal > const VectorT<_TReal,2> VectorT<_TReal,2>::InvUnit_SCALE( -1,  -1 );

	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::Zero( 0, 0, 0 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::Unit_X( 1, 0, 0 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::Unit_Y( 0, 1, 0 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::Unit_Z( 0, 0, 1 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::Unit_SCALE( 1, 1, 1 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::InvUnit_X( -1,  0, 0 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::InvUnit_Y(  0, -1, 0 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::InvUnit_Z(  0, 0, -1 );
	template< class _TReal > const VectorT<_TReal,3> VectorT<_TReal,3>::InvUnit_SCALE( -1,  -1, -1 );
	
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::Zero( 0, 0, 0, 0 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::Unit_X( 1, 0, 0, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::Unit_Y( 0, 1, 0, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::Unit_Z( 0, 0, 1, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::Unit_SCALE( 1, 1, 1, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::InvUnit_X( -1,  0, 0, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::InvUnit_Y(  0, -1, 0, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::InvUnit_Z(  0, 0, -1, 1 );
	template< class _TReal > const VectorT<_TReal,4> VectorT<_TReal,4>::InvUnit_SCALE( -1,  -1, -1, 1 );

	//template< class _TReal > 	
	//VectorT<_TReal,3 > VectorT<_TReal,3 >::operator * ( const Matrix4< _TReal >& mat ) const
	//{		
	//	//Calculate the homogeneous element as if it is equal to 1
	//	//_TReal h = x * mat._14 + y * mat._24 + z * mat._34 + mat._44;
	//	
	//	//If zero represent the _TReal as the smallest possible value
	//	//if ( h == 0.0f )
	//	//	h = (_TReal)Real_MIN;

	//	return VectorT<_TReal,3 >( 
	//		( (x * mat( 0, 0 )) + (y * mat( 1, 0 )) + (z * mat( 2, 0 )) + mat( 3, 0 ) )/* / h*/ , 
	//		( (x * mat( 0, 1 )) + (y * mat( 1, 1 )) + (z * mat( 2, 1 )) + mat( 3, 1 ) )/* / h*/, 
	//		( (x * mat( 0, 2 )) + (y * mat( 1, 2 )) + (z * mat( 2, 2 )) + mat( 3, 2 ) )/* / h*/ 
	//		 );
	//}

	//template< class _TReal >
	//VectorT<_TReal,3 >& VectorT<_TReal,3 >::operator *= ( const Matrix4< _TReal >& m )
	//{ return (*this = *this * m); }

	template class VectorT< Real, 4 >;
	template class VectorT< Float64, 4 >;
	template class VectorT< Float32, 4 >;
	template class VectorT< Int32, 4 >;
	template class VectorT< Int16, 4 >;
	template class VectorT< UInt32, 4 >;
	template class VectorT< UInt16, 4 >;

	template class VectorT< Real, 3 >;
	template class VectorT< Float32, 3 >;
	template class VectorT< Float64, 3 >;
	template class VectorT< Int32, 3 >;
	template class VectorT< Int16, 3 >;
	template class VectorT< UInt32, 3 >;
	template class VectorT< UInt16, 3 >;

	template class VectorT< Real, 2 >;
	template class VectorT< Float32, 2 >;
	template class VectorT< Float64, 2 >;
	template class VectorT< Int32, 2 >;
	template class VectorT< Int16, 2 >;
	template class VectorT< UInt32, 2 >;
	template class VectorT< UInt16, 2 >;

	template class VectorT< Real, 1 >;
	template class VectorT< Float32, 1 >;
	template class VectorT< Float64, 1 >;
	template class VectorT< Int32, 1 >;
	template class VectorT< Int16, 1 >;
	template class VectorT< UInt32, 1 >;
	template class VectorT< UInt16, 1 >;

} //END: kismo