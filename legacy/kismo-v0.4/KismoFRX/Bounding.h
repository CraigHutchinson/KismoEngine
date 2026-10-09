/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Bounding.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Bounding.h (same name, content differs)
 */
#ifndef KISMOFRX_BOUNDING_H
#define KISMOFRX_BOUNDING_H

#include "KismoFRX/Common.h"
#include "KismoFRX/VectorN.h"

namespace kismo
{
	//Forward Declaration

	/** \brief 3D Axis aligned Bounding box 
	*/
	template < typename _TShape >
	class KISMOFRX_EXPORT Bounding
	{
	public:
		typedef VectorN<_TReal,3> VectorT;

	public: 

		/** \brief Constructor defalts to a zero sized box 
		*/
		Bounding();

		/**
			\brief Constructor from a shape which defines the bounds of this volume
			\remarks It is expected that if a differing shape is supplied the bounds will be expanded fully enclose that shape via a call to expand( TShape )
		*/
		template< class _TShapeB >
		explicit Bounding( _TShapeB );	
	}

} // END: math

#endif