/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Transform.cpp
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


#include "Transform.h"

namespace kismo
{
	template< class _Real > const Transform<_Real> Transform<_Real>::Identity( Transform<_Real>::_Matrix43::Identity );
	
	template< class _Real >
	Transform< _Real >::Transform()
	{}

	template< class _Real >
	Transform< _Real >::Transform( eTransformFlag transformType )
	{
		switch( transformType )
		{
		case TransformFlag_Identity: m_matrix = Identity.matrix(); break;
		default: AssertEx( false, "Invalid transform type when constructing Transform!" ); break;
		}
	}

	template< class _Real >
	Transform< _Real >::Transform( const _Matrix43& matrix ) :
		m_matrix(matrix)
	{}
		
	template< class _Real >
	Transform< _Real >::Transform( const _Matrix33& rotation ) :
		m_matrix(rotation)
	{
		Assert( translation() == _Vector::Zero ); //, Sanity check as not sure the conversion from 3x3 to 4x3 sets the last row
	}
		
	template< class _Real >
	const typename Transform< _Real >::_Vector& Transform< _Real >::translate( const _Vector& translate, eSpace space )
	{
		switch ( space )
		{
		case Space_World:
			return setTranslation( translation() + translate );
		case Space_InverseLocal: ///< It has some purposes as it is different from '-translate'
			AssertEx(false, "TODO: Matrix inversion for Space_InverseLocal!" );
			return translation();
			//return setTranslation( translation() + translate * m_matrix.inverse() );
		case Space_InverseWorld:
			return setTranslation( translation() - translate );
		default:
			AssertEx(false, "Invalid translate() coordinate space suplied. Defaultign to 'Local'" );
		case Space_Local:
			return setTranslation( translation() + translate * m_matrix );
		}
	}

	//template< class _Real >
	//void Transform< _Real >::setRotate( const _Matrix& rotate )
	//{
	//	m_matrix = rotate;
	//	m_flags[ TransformFlag_Identity ] = false;
	//	m_flags[ TransformFlag_RSMatrix ] = true;
	//	//m_flags[ TransformFlag_UserMatrix ] = false;
	//}

	////template< class _Real >
	////void Transform< _Real >::setUserMatrix( const _Matrix& matrix )
	////{
	////	m_matrix = matrix;
	////	m_flags[ TransformFlag_Identity ] = false;
	////	m_flags[ TransformFlag_RSMatrix ] = false;
	////	m_flags[ TransformFlag_UniformScale ] = false;
	////	m_flags[ TransformFlag_UserMatrix ] = true;
	////}

	//template< class _Real >
	//void Transform< _Real >::setTranslate( const _Vector& translate )
	//{
	//	m_translate = translate;
	//	m_flags[ TransformFlag_Identity ] = false;
	//}

	//template< class _Real >
	//void Transform< _Real >::setScale( const _Vector& scale )
	//{
	//	m_scale = scale;
	//	m_flags[ TransformFlag_Identity ] = false;
	//	m_flags[ TransformFlag_UniformScale ] = false;
	//}

	//template< class _Real >
	//void Transform< _Real >::setUniformScale( Real_T scale )
	//{
	//	m_scale = scale;
	//	m_flags[ TransformFlag_Identity ] = false;
	//	m_flags[ TransformFlag_UniformScale ] = true;
	//}

	//template< class _Real >
	//typename Transform< _Real >::_Vector Transform< _Real >::applyForward( const _Vector& v ) const
	//{
	//	if ( m_flags[ TransformFlag_Identity ] )
	//		return v; ///< Do nothing!

	//	if ( m_flags[ TransformFlag_RSMatrix ] )
	//	{
	//		// Y = R*S*X + T
	//		_Vector tvec( m_scale.x * v.x,  m_scale.y * v.y,  m_scale.z * v.z );
	//		tvec = m_matrix * tvec;
	//		tvec += m_translate; 
	//		return tvec;
	//	}

	//	// Y = M*X + t
	//	_Vector tvec( m_matrix * v );
	//	tvec += m_translate; 
	//	return tvec;
	//}

	//template< class _Real >
	//typename Transform< _Real >::_Vector Transform< _Real >::applyInverse( const _Vector& v ) const
	//{
	//	if ( m_flags[ TransformFlag_Identity ] )
	//		return v; ///< Do nothing!

	//	//Invert translate
	//	_Vector tvec( v.x - m_translate.x,  v.y - m_translate.y,  v.z - m_translate.z );
	//	if ( m_flags[ TransformFlag_RSMatrix ] )
	//	{
	//		// Y = S^(-1)*R^T*(Y - T)
	//		tvec = tvec * m_matrix;
	//		if ( m_flags[ TransformFlag_UniformScale ] )
	//		{
	//			tvec /= getUniformScale();
	//		}
	//		else
	//		{
	//			Real_T sxy = m_scale.x * m_scale.y;
	//			Real_T sxyz = sxy * m_scale.z;
	//			Real_T invSxyz = (Real_T)1.0 / sxyz;
	//			Real_T invSxy = invSxyz * m_scale.z;
	//			tvec.x *= invSxy * m_scale.y;
	//			tvec.y *= invSxy * m_scale.x;
	//			tvec.z *= invSxyz * sxy;
	//		}
	//	}
	//	else
	//	{
	//		// x = M^{-1}*(Y - T)
	//		tvec = m_matrix.inverse() * tvec;
	//	}

	//	return tvec;
	//}

	//template< class _Real >
	//void Transform< _Real >::inverse( const Transform& t )
	//{
	//	if ( t.isIdentity() )
	//	{
	//		*this = t;
	//		return;
	//	}

	//	m_translate = -t.m_translate;
	//	m_matrix = t.m_matrix.inverse();
	//	//TODO: Scale?
	//}

	//template< class _Real >
	//void Transform< _Real >::product( const Transform& a, const Transform& b )
	//{
	//	if ( a.isIdentity() ) //If a transform is identity it makes things easy!
	//	{
	//		*this = b;
	//		return;
	//	}

	//	if ( b.isIdentity() ) //If a transform is identity it makes things easy!
	//	{
	//		*this = a; ///We assign the left hand to this :P
	//		return;
	//	}

	//	//Not an identity as both transforms aren't!
	//	m_flags[ TransformFlag_Identity ] = false;

	//	//Combine the rotation matrices
	//	if ( !a.isRSMatrix() )
	//	{
	//		m_matrix = b.m_matrix;
	//		m_flags[ TransformFlag_RSMatrix ] = b.m_flags[ TransformFlag_RSMatrix ];
	//	}
	//	else
	//	if ( !b.isRSMatrix() )
	//	{
	//		m_matrix = a.m_matrix;
	//		m_flags[ TransformFlag_RSMatrix ] = a.m_flags[ TransformFlag_RSMatrix ];
	//	}
	//	else
	//	{
	//		m_matrix = a.m_matrix * b.m_matrix;
	//		m_flags[ TransformFlag_RSMatrix ] = true;
	//	}

	//	//Combine the translations
	//	//if ( a.isRSMatrix() || b.isRSMatrix() )
	//		m_translate = a.m_translate 
	//		+ (a.m_matrix * (b.m_translate + b.m_pivot))
	//		- (m_matrix * b.m_pivot);
	//	//else
	//		//m_translate = a.m_translate + b.m_translate + a.m_pivot;

	//	//m_pivot = b.m_pivot;
	//	
	//	//TODO: scale
	//}

	//
	//template< class _Real >
	//void Transform< _Real >::setPivot( const _Vector& pivot )
	//{
	//	m_pivot = pivot;
	//	m_flags[ TransformFlag_OffsetPivot ] = true;
	//}


	//template< class _Real >
	//void Transform< _Real >::rotate( const _Matrix& matrix, eSpace space  )
	//{
	//	switch ( space )
	//	{
	//	case Space_Local:
	//			setRotate( matrix * m_matrix ); break;
	//	case Space_World:
	//			setRotate( m_matrix * matrix ); break;
	//	case Space_InverseLocal:
	//			setRotate( matrix * m_matrix.inverse() ); break;
	//	case Space_InverseWorld:
	//			setRotate( m_matrix.inverse() * matrix ); break;
	//	}
	//}

	//template< class _Real >
	//void Transform< _Real >::rotateX( _Real deg, eSpace space  )
	//{ rotate( _Matrix::rotationX( deg ), space ); }

	//template< class _Real >
	//void Transform< _Real >::rotateY( _Real deg, eSpace space )
	//{ rotate( _Matrix::rotationY( deg ), space ); }

	//template< class _Real >
	//void Transform< _Real >::rotateZ( _Real deg, eSpace space  )
	//{ rotate( _Matrix::rotationZ( deg ), space ); }

	template class Transform< Float32 >;
	template class Transform< Float64 >;

} //END: kismo