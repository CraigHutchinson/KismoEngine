/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoVector.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoVector.h"

namespace kismo
{
	namespace frx
	{	
		template< class T > const Vector2<T> Vector2<T>::ZERO( 0, 0 );
		template< class T > const Vector2<T> Vector2<T>::UNIT_X( 1, 0 );
		template< class T > const Vector2<T> Vector2<T>::UNIT_Y( 0, 1 );
		template< class T > const Vector2<T> Vector2<T>::NEGATIVE_UNIT_X( -1,  0 );
		template< class T > const Vector2<T> Vector2<T>::NEGATIVE_UNIT_Y(  0, -1 );
		template< class T > const Vector2<T> Vector2<T>::UNIT_SCALE( 1, 1 );

		const Vector3 Vector3::ZERO( 0, 0, 0 );
		const Vector3 Vector3::UNIT_X( 1, 0, 0 );
		const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
		const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
		const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
		const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
		const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
		const Vector3 Vector3::UNIT_SCALE( 1, 1, 1 );

		const Vector4 Vector4::ZERO( 0, 0, 0, 1 );
		const Vector4 Vector4::UNIT_X( 1, 0, 0, 1 );
		const Vector4 Vector4::UNIT_Y( 0, 1, 0, 1 );
		const Vector4 Vector4::UNIT_Z( 0, 0, 1, 1 );
		const Vector4 Vector4::NEGATIVE_UNIT_X( -1,  0,  0, 1 );
		const Vector4 Vector4::NEGATIVE_UNIT_Y(  0, -1,  0, 1 );
		const Vector4 Vector4::NEGATIVE_UNIT_Z(  0,  0, -1, 1 );
		const Vector4 Vector4::UNIT_SCALE( 1, 1, 1, 1 );
	}
}