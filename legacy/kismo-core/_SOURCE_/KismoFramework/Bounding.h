/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Bounding.h
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

#ifndef KISMOFRAMEWORK_BOUNDING_H
#define KISMOFRAMEWORK_BOUNDING_H

#include "KismoFramework\Common.h"
#include "KismoFramework/VectorN.h"

namespace kismo
{
	//Forward Declaration

	/** \brief 3D Axis aligned Bounding box 
	*/
	template < typename _TShape >
	class Bounding
	{
	public:
		typedef VectorN<_Real,3> VectorT;

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