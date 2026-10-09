/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Transform.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Transform.cpp (same name, content differs)
 */
#include "Transform.h"

namespace gfx
{
	template< class _TReal > const Transform<_TReal> Transform<_TReal>::Identity = Transform<_TReal>();

	template class Transform< Float32 >;
	template class Transform< Float64 >;

	
	template< class _TReal >
	Transform< _TReal >::Transform() :
		m_matrix( Matrix_T::Identity ),
		m_translate( Vector_T::Zero ),
		m_scale( Vector_T::Unit_SCALE ),
		m_pivot( Vector_T::Zero ),
		m_flags( 1 << TransformFlag_Identity )
	{}
		
	template< class _TReal >
	void Transform< _TReal >::setRotate( const Matrix_T& rotate )
	{
		m_matrix = rotate;
		m_flags[ TransformFlag_Identity ] = false;
		m_flags[ TransformFlag_RSMatrix ] = true;
		//m_flags[ TransformFlag_UserMatrix ] = false;
	}

	//template< class _TReal >
	//void Transform< _TReal >::setUserMatrix( const Matrix_T& matrix )
	//{
	//	m_matrix = matrix;
	//	m_flags[ TransformFlag_Identity ] = false;
	//	m_flags[ TransformFlag_RSMatrix ] = false;
	//	m_flags[ TransformFlag_UniformScale ] = false;
	//	m_flags[ TransformFlag_UserMatrix ] = true;
	//}

	template< class _TReal >
	void Transform< _TReal >::setTranslate( const Vector_T& translate )
	{
		m_translate = translate;
		m_flags[ TransformFlag_Identity ] = false;
	}

	template< class _TReal >
	void Transform< _TReal >::setScale( const Vector_T& scale )
	{
		m_scale = scale;
		m_flags[ TransformFlag_Identity ] = false;
		m_flags[ TransformFlag_UniformScale ] = false;
	}

	template< class _TReal >
	void Transform< _TReal >::setUniformScale( Real_T scale )
	{
		m_scale = scale;
		m_flags[ TransformFlag_Identity ] = false;
		m_flags[ TransformFlag_UniformScale ] = true;
	}

	template< class _TReal >
	typename Transform< _TReal >::Vector_T Transform< _TReal >::applyForward( const Vector_T& v ) const
	{
		if ( m_flags[ TransformFlag_Identity ] )
			return v; ///< Do nothing!

		if ( m_flags[ TransformFlag_RSMatrix ] )
		{
			// Y = R*S*X + T
			Vector_T tvec( m_scale.x * v.x,  m_scale.y * v.y,  m_scale.z * v.z );
			tvec = m_matrix * tvec;
			tvec += m_translate; 
			return tvec;
		}

		// Y = M*X + t
		Vector_T tvec( m_matrix * v );
		tvec += m_translate; 
		return tvec;
	}

	template< class _TReal >
	typename Transform< _TReal >::Vector_T Transform< _TReal >::applyInverse( const Vector_T& v ) const
	{
		if ( m_flags[ TransformFlag_Identity ] )
			return v; ///< Do nothing!

		//Invert translate
		Vector_T tvec( v.x - m_translate.x,  v.y - m_translate.y,  v.z - m_translate.z );
		if ( m_flags[ TransformFlag_RSMatrix ] )
		{
			// Y = S^(-1)*R^T*(Y - T)
			tvec = tvec * m_matrix;
			if ( m_flags[ TransformFlag_UniformScale ] )
			{
				tvec /= getUniformScale();
			}
			else
			{
				Real_T sxy = m_scale.x * m_scale.y;
				Real_T sxyz = sxy * m_scale.z;
				Real_T invSxyz = (Real_T)1.0 / sxyz;
				Real_T invSxy = invSxyz * m_scale.z;
				tvec.x *= invSxy * m_scale.y;
				tvec.y *= invSxy * m_scale.x;
				tvec.z *= invSxyz * sxy;
			}
		}
		else
		{
			// x = M^{-1}*(Y - T)
			tvec = m_matrix.inverse() * tvec;
		}

		return tvec;
	}

	template< class _TReal >
	void Transform< _TReal >::inverse( const Transform& t )
	{
		if ( t.isIdentity() )
		{
			*this = t;
			return;
		}

		m_translate = -t.m_translate;
		m_matrix = t.m_matrix.inverse();
		//TODO: Scale?
	}

	template< class _TReal >
	void Transform< _TReal >::product( const Transform& a, const Transform& b )
	{
		if ( a.isIdentity() ) //If a transform is identity it makes things easy!
		{
			*this = b;
			return;
		}

		if ( b.isIdentity() ) //If a transform is identity it makes things easy!
		{
			*this = a; ///We assign the left hand to this :P
			return;
		}

		//Not an identity as both transforms aren't!
		m_flags[ TransformFlag_Identity ] = false;

		//Combine the rotation matrices
		if ( !a.isRSMatrix() )
		{
			m_matrix = b.m_matrix;
			m_flags[ TransformFlag_RSMatrix ] = b.m_flags[ TransformFlag_RSMatrix ];
		}
		else
		if ( !b.isRSMatrix() )
		{
			m_matrix = a.m_matrix;
			m_flags[ TransformFlag_RSMatrix ] = a.m_flags[ TransformFlag_RSMatrix ];
		}
		else
		{
			m_matrix = a.m_matrix * b.m_matrix;
			m_flags[ TransformFlag_RSMatrix ] = true;
		}

		//Combine the translations
		//if ( a.isRSMatrix() || b.isRSMatrix() )
			m_translate = a.m_translate 
			+ (a.m_matrix * (b.m_translate + b.m_pivot))
			- (m_matrix * b.m_pivot);
		//else
			//m_translate = a.m_translate + b.m_translate + a.m_pivot;

		//m_pivot = b.m_pivot;
		
		//TODO: scale
	}

	
	template< class _TReal >
	void Transform< _TReal >::setPivot( const Vector_T& pivot )
	{
		m_pivot = pivot;
		m_flags[ TransformFlag_OffsetPivot ] = true;
	}

	template< class _TReal >
	void Transform< _TReal >::translate( const Vector_T& translate, eSpace space )
	{
		switch ( space )
		{
		case eSpace_Local:
				setTranslate( m_translate + translate * m_matrix ); break;
		case eSpace_World:
				setTranslate( m_translate + translate ); break;
		case eSpace_InverseLocal:
				setTranslate( m_translate + translate * m_matrix.inverse() ); break;
		case eSpace_InverseWorld:
				setTranslate( m_translate - translate ); break;
		}
	}

	template< class _TReal >
	void Transform< _TReal >::rotate( const Matrix_T& matrix, eSpace space  )
	{
		switch ( space )
		{
		case eSpace_Local:
				setRotate( matrix * m_matrix ); break;
		case eSpace_World:
				setRotate( m_matrix * matrix ); break;
		case eSpace_InverseLocal:
				setRotate( matrix * m_matrix.inverse() ); break;
		case eSpace_InverseWorld:
				setRotate( m_matrix.inverse() * matrix ); break;
		}
	}

	template< class _TReal >
	void Transform< _TReal >::rotateX( _TReal deg, eSpace space  )
	{ rotate( Matrix_T::rotationX( deg ), space ); }

	template< class _TReal >
	void Transform< _TReal >::rotateY( _TReal deg, eSpace space )
	{ rotate( Matrix_T::rotationY( deg ), space ); }

	template< class _TReal >
	void Transform< _TReal >::rotateZ( _TReal deg, eSpace space  )
	{ rotate( Matrix_T::rotationZ( deg ), space ); }

} //END: frx